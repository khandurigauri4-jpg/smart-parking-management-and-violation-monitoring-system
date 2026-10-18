const express = require("express");
const crypto = require("crypto");

const ParkingSlot = require("../models/ParkingSlot");
const ParkingSession = require("../models/ParkingSession");
const Reservation = require("../models/Reservation");
const Vehicle = require("../models/Vehicle");
const Violation = require("../models/Violation");
const GateAction = require("../models/GateAction");
const { findNearestSlot, findRoute } = require("../services/parkingDsa");
const { searchActiveVehicle, listActiveVehicles, buildReservationQueue, buildActionStack } = require("../services/teamDsa");
const { calculateCppFee } = require("../services/cppParking");

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

// Mradul's C linked list: active vehicle search and active occupancy index.
router.get("/dsa/active-search", async (req, res) => {
    const vehicleNumber = String(req.query.vehicleNumber || "").trim().toUpperCase();
    if (!vehicleNumber) return res.status(400).json({ message: "Provide vehicleNumber." });
    try {
        const slots = await ParkingSlot.find({ occupied: true }).sort({ slotId: 1 });
        res.json(searchActiveVehicle(slots, vehicleNumber));
    } catch (error) {
        console.error("Linked-list search failed:", error.message);
        res.status(500).json({ message: "Could not search active vehicles." });
    }
});

router.get("/dsa/active-vehicles", async (req, res) => {
    try {
        const slots = await ParkingSlot.find({ occupied: true }).sort({ slotId: 1 });
        res.json(listActiveVehicles(slots));
    } catch (error) {
        res.status(500).json({ message: "Could not build active vehicle index." });
    }
});

// Mradul's circular queue is rebuilt from MongoDB's booked reservations each request.
router.get("/dsa/reservation-queue", async (req, res) => {
    try {
        const reservations = await Reservation.find({ status: "BOOKED", reservedUntil: { $gt: new Date() } }).sort({ reservedFrom: 1 }).limit(51);
        if (reservations.length > 50) return res.status(409).json({ message: "C queue capacity is 50. Showing the database reservation list is still available at /api/reservations." });
        res.json(buildReservationQueue(reservations));
    } catch (error) {
        console.error("Reservation queue failed:", error.message);
        res.status(500).json({ message: "Could not build reservation queue." });
    }
});

// Mradul's LIFO stack: recent gate actions persisted in MongoDB and loaded into the C stack.
router.get("/dsa/action-stack", async (req, res) => {
    try {
        const actions = await GateAction.find().sort({ createdAt: -1 }).limit(50);
        res.json(buildActionStack(actions));
    } catch (error) {
        console.error("Action stack failed:", error.message);
        res.status(500).json({ message: "Could not build gate action stack." });
    }
});

router.get("/sessions/ticket/:ticketId", async (req, res) => {
    try {
        const ticketId = String(req.params.ticketId || "").trim();
        const session = await ParkingSession.findOne({ ticketId });
        if (!session) return res.status(404).json({ message: "Ticket not found." });
        res.json(session);
    } catch (error) {
        res.status(500).json({ message: "Could not fetch ticket details." });
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
        const allowedHours = vehicleReservation
            ? Math.max(1, Math.min(24, Math.ceil((vehicleReservation.reservedUntil - vehicleReservation.reservedFrom) / 3600000)))
            : 2;
        session = await ParkingSession.create({
            ticketId,
            vehicleNumber: vehicle,
            slotId: slot.slotId,
            entryTime: new Date(),
            allowedHours,
            status: "ACTIVE"
        });
        GateAction.create({ actionType: "ENTRY", vehicleNumber: vehicle, slotId: slot.slotId }).catch(err => console.error("Could not persist ENTRY action:", err.message));

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
        const registeredVehicle = await Vehicle.findOne({ vehicleNumber: session.vehicleNumber });
        const toHHMM = date => `${String(date.getHours()).padStart(2, "0")}:${String(date.getMinutes()).padStart(2, "0")}`;
        const cppBill = calculateCppFee({
            vehicleNumber: session.vehicleNumber,
            ownerName: registeredVehicle?.ownerName || "Guest",
            category: registeredVehicle?.vehicleCategory || 4,
            vehicleName: registeredVehicle?.vehicleName || "Vehicle",
            slotId,
            entryTime: toHHMM(session.entryTime),
            exitTime: toHHMM(exitTime),
            allowedHours: session.allowedHours || 2
        });
        const durationMinutes = Math.max(1, Math.ceil((exitTime.getTime() - session.entryTime.getTime()) / 60000));
        const fee = cppBill.total;

        session.exitTime = exitTime;
        session.durationMinutes = durationMinutes;
        session.fee = fee;
        session.status = "COMPLETED";
        await session.save();

        slot.occupied = false;
        slot.vehicleNumber = "";
        await slot.save();
        GateAction.create({ actionType: "EXIT", vehicleNumber: session.vehicleNumber, slotId }).catch(err => console.error("Could not persist EXIT action:", err.message));
        if (cppBill.hasOverstay) {
            Violation.create({ vehicleNumber: session.vehicleNumber, slotId, violationType: "OVERSTAY", description: `Overstayed by ${cppBill.overstayMinutes} minutes.`, detectedAt: exitTime, penaltyAmount: cppBill.penalty }).catch(err => console.error("Could not persist overstay violation:", err.message));
        }

        res.json({
            message: "Vehicle released successfully.",
            ticketId: session.ticketId,
            slotId,
            vehicleNumber: session.vehicleNumber,
            entryTime: session.entryTime,
            exitTime,
            durationMinutes,
            fee,
            parkingFee: cppBill.parkingFee,
            penalty: cppBill.penalty,
            hasOverstay: cppBill.hasOverstay,
            session
        });
    } catch (error) {
        console.error("Release vehicle error:", error.message);
        res.status(500).json({ message: "Could not release the vehicle." });
    }
});

module.exports = router;
