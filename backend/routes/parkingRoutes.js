const express = require("express");
const crypto = require("crypto");

const ParkingSlot = require("../models/ParkingSlot");
const ParkingSession = require("../models/ParkingSession");
const Reservation = require("../models/Reservation");
const Vehicle = require("../models/Vehicle");
const { findNearestSlot, findRoute } = require("../services/parkingDsa");

const router = express.Router();

router.get("/", async (req, res) => {
    try {
        const slots = await ParkingSlot.find().sort({ slotId: 1 });
        const availableSlots = slots.filter(slot => !slot.occupied).length;

        res.json({
            totalSlots: slots.length,
            availableSlots,
            occupiedSlots: slots.length - availableSlots,
            slots
        });
    } catch (error) {
        res.status(500).json({ message: "Could not fetch parking status." });
    }
});

// DSA: nearest currently available slot using C + Dijkstra.
router.get("/nearest", async (req, res) => {
    try {
        const now = new Date();
        const slots = await ParkingSlot.find().sort({ slotId: 1 });
        const activeReservations = await Reservation.find({
            status: "BOOKED",
            reservedUntil: { $gt: now },
            slotId: { $ne: null }
        });

        const reservedSlotIds = new Set(activeReservations.map(item => item.slotId));
        const dsaSlots = slots.map(slot => ({
            slotId: slot.slotId,
            occupied: slot.occupied || reservedSlotIds.has(slot.slotId)
        }));

        const result = findNearestSlot(dsaSlots);
        if (!result.slotId) {
            return res.status(409).json({ message: "No available slot found." });
        }

        res.json({
            ...result,
            pathLabels: result.path.map(vertex => vertex === 0 ? "Entrance" : `Slot ${vertex}`)
        });
    } catch (error) {
        console.error("Nearest-slot DSA error:", error.message);
        res.status(500).json({ message: "Could not calculate nearest slot." });
    }
});

// DSA: shortest route from entrance to a selected slot.
router.get("/route/:slotId", async (req, res) => {
    const slotId = Number(req.params.slotId);
    if (!Number.isInteger(slotId) || slotId < 1 || slotId > 10) {
        return res.status(400).json({ message: "Slot must be between 1 and 10." });
    }

    try {
        const result = findRoute(slotId);
        res.json({
            ...result,
            pathLabels: result.path.map(vertex => vertex === 0 ? "Entrance" : `Slot ${vertex}`)
        });
    } catch (error) {
        console.error("Shortest-path DSA error:", error.message);
        res.status(500).json({ message: "Could not calculate shortest route." });
    }
});

router.get("/sessions", async (req, res) => {
    try {
        const sessions = await ParkingSession.find().sort({ entryTime: -1 });
        res.json(sessions);
    } catch (error) {
        res.status(500).json({ message: "Could not fetch parking sessions." });
    }
});

router.post("/park", async (req, res) => {
    let slot = null;
    let session = null;

    try {
        const { vehicleNumber } = req.body;
        if (typeof vehicleNumber !== "string" || !vehicleNumber.trim()) {
            return res.status(400).json({ message: "Please enter a valid vehicle number." });
        }

        const vehicle = vehicleNumber.trim().toUpperCase();
        const registeredVehicle = await Vehicle.findOne({ vehicleNumber: vehicle });

        if (registeredVehicle?.blacklisted) {
            return res.status(403).json({ message: "This vehicle is blacklisted and cannot park." });
        }

        const alreadyParked = await ParkingSlot.findOne({ occupied: true, vehicleNumber: vehicle });
        if (alreadyParked) {
            return res.status(409).json({
                message: "This vehicle is already parked.",
                slotId: alreadyParked.slotId
            });
        }

        const now = new Date();
        const vehicleReservation = await Reservation.findOne({
            vehicleNumber: vehicle,
            status: "BOOKED",
            reservedFrom: { $lte: now },
            reservedUntil: { $gt: now }
        }).sort({ reservedFrom: 1 });

        const slots = await ParkingSlot.find().sort({ slotId: 1 });
        const activeReservations = await Reservation.find({
            status: "BOOKED",
            reservedUntil: { $gt: now },
            slotId: { $ne: null }
        });

        const reservedSlotIds = new Set(activeReservations.map(item => item.slotId));

        // A vehicle may use its assigned slot only while its own reservation is active.
        if (vehicleReservation && vehicleReservation.slotId !== null) {
            reservedSlotIds.delete(vehicleReservation.slotId);
        }

        let chosenSlotId;
        if (vehicleReservation && vehicleReservation.slotId !== null) {
            chosenSlotId = vehicleReservation.slotId;
            if (reservedSlotIds.has(chosenSlotId)) {
                return res.status(409).json({
                    message: "Your reserved slot is currently unavailable."
                });
            }
        } else {
            const dsaSlots = slots.map(item => ({
                slotId: item.slotId,
                occupied: item.occupied || reservedSlotIds.has(item.slotId)
            }));
            const nearest = findNearestSlot(dsaSlots);
            chosenSlotId = nearest.slotId;

            if (!chosenSlotId) {
                return res.status(409).json({ message: "Sorry! No parking slot is available." });
            }
        }

        slot = await ParkingSlot.findOneAndUpdate(
            { slotId: chosenSlotId, occupied: false },
            { $set: { occupied: true, vehicleNumber: vehicle } },
            { new: true }
        );

        if (!slot) {
            return res.status(409).json({
                message: "The selected slot was just taken. Refresh and try again."
            });
        }

        const ticketId = "PK-" + crypto.randomUUID().slice(0, 8).toUpperCase();
        session = await ParkingSession.create({
            ticketId,
            vehicleNumber: vehicle,
            slotId: slot.slotId,
            entryTime: new Date(),
            status: "ACTIVE"
        });

        if (vehicleReservation) {
            vehicleReservation.status = "COMPLETED";
            await vehicleReservation.save();
        }

        res.status(201).json({
            message: "Vehicle parked successfully.",
            ticketId,
            slotId: slot.slotId,
            vehicleNumber: vehicle,
            entryTime: session.entryTime,
            session
        });
    } catch (error) {
        if (slot && !session) {
            await ParkingSlot.updateOne(
                { _id: slot._id, occupied: true, vehicleNumber: slot.vehicleNumber },
                { $set: { occupied: false, vehicleNumber: "" } }
            ).catch(() => {});
        }
        console.error("Park vehicle error:", error.message);
        res.status(500).json({ message: "Could not park the vehicle." });
    }
});

router.post("/release", async (req, res) => {
    try {
        const slotId = Number(req.body.slotId);
        if (!Number.isInteger(slotId) || slotId < 1 || slotId > 10) {
            return res.status(400).json({ message: "Please enter a slot number from 1 to 10." });
        }

        const slot = await ParkingSlot.findOne({ slotId });
        if (!slot) {
            return res.status(404).json({ message: "Parking slot not found." });
        }
        if (!slot.occupied) {
            return res.status(409).json({ message: "This slot is already available." });
        }

        const session = await ParkingSession.findOne({
            slotId, vehicleNumber: slot.vehicleNumber, status: "ACTIVE"
        }).sort({ entryTime: -1 });

        if (!session) {
            return res.status(409).json({ message: "No active parking ticket was found for this slot." });
        }

        const exitTime = new Date();
        const durationMinutes = Math.max(
            1,
            Math.ceil((exitTime.getTime() - session.entryTime.getTime()) / 60000)
        );
        const fee = Math.ceil(durationMinutes / 60) * 20;

        session.exitTime = exitTime;
        session.durationMinutes = durationMinutes;
        session.fee = fee;
        session.status = "COMPLETED";
        await session.save();

        slot.occupied = false;
        slot.vehicleNumber = "";
        await slot.save();

        res.json({
            message: "Vehicle released successfully.",
            ticketId: session.ticketId,
            slotId,
            vehicleNumber: session.vehicleNumber,
            entryTime: session.entryTime,
            exitTime,
            durationMinutes,
            fee,
            session
        });
    } catch (error) {
        console.error("Release vehicle error:", error.message);
        res.status(500).json({ message: "Could not release the vehicle." });
    }
});

module.exports = router;
