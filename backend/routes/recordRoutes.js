const express = require("express");
const bcrypt = require("bcryptjs");

const Reservation = require("../models/Reservation");
const Violation = require("../models/Violation");
const Vehicle = require("../models/Vehicle");
const User = require("../models/User");
const ParkingSlot = require("../models/ParkingSlot");

const router = express.Router();

// VEHICLES

router.get("/vehicles", async (req, res) => {
    try {
        res.json(await Vehicle.find().sort({ vehicleNumber: 1 }));
    } catch (error) {
        res.status(500).json({ message: "Could not fetch vehicles." });
    }
});

router.post("/vehicles", async (req, res) => {
    try {
        const {
            vehicleNumber,
            ownerName = "",
            ownerContact = "",
            blacklisted = false,
            blacklistReason = ""
        } = req.body;

        if (
            typeof vehicleNumber !== "string" ||
            !vehicleNumber.trim()
        ) {
            return res.status(400).json({
                message: "Vehicle number is required."
            });
        }

        const vehicle = await Vehicle.create({
            vehicleNumber: vehicleNumber.trim().toUpperCase(),
            ownerName,
            ownerContact,
            blacklisted,
            blacklistReason
        });

        res.status(201).json(vehicle);
    } catch (error) {
        res.status(error.code === 11000 ? 409 : 400).json({
            message: error.code === 11000
                ? "Vehicle is already registered."
                : "Could not register vehicle."
        });
    }
});

// RESERVATIONS

router.get("/reservations", async (req, res) => {
    try {
        res.json(
            await Reservation.find().sort({ reservedFrom: 1 })
        );
    } catch (error) {
        res.status(500).json({
            message: "Could not fetch reservations."
        });
    }
});

router.post("/reservations", async (req, res) => {
    try {
        const { customerName, customerEmail, customerPhone, vehicleNumber, slotId, reservedFrom, reservedUntil } = req.body;
        const start = new Date(reservedFrom);
        const end = new Date(reservedUntil);

        if (
            typeof customerName !== "string" || !customerName.trim() || customerName.trim().length > 80 ||
            typeof customerEmail !== "string" || !/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(customerEmail.trim()) || customerEmail.trim().length > 120 ||
            typeof customerPhone !== "string" || !/^[0-9+() -]{7,20}$/.test(customerPhone.trim()) ||
            typeof vehicleNumber !== "string" ||
            !vehicleNumber.trim() ||
            !Number.isFinite(start.getTime()) ||
            !Number.isFinite(end.getTime()) ||
            start <= new Date() ||
            end <= start
        ) {
            return res.status(400).json({
                message: "Provide a vehicle number and a future start/end time; the end must be after the start."
            });
        }

        const vehicle = vehicleNumber.trim().toUpperCase();
        const registeredVehicle = await Vehicle.findOne({ vehicleNumber: vehicle });

        if (registeredVehicle?.blacklisted) {
            return res.status(403).json({
                message: "Blacklisted vehicles cannot make reservations."
            });
        }

        const requestedSlot = Number(slotId);
        if (!Number.isInteger(requestedSlot) || requestedSlot < 1 || requestedSlot > 10) {
            return res.status(400).json({
                message: "Choose a valid parking slot from 1 to 10."
            });
        }

        const slotExists = await ParkingSlot.exists({ slotId: requestedSlot });
        if (!slotExists) {
            return res.status(404).json({ message: "Requested parking slot does not exist." });
        }

        const occupiedSlot = await ParkingSlot.exists({
            slotId: requestedSlot,
            occupied: true
        });
        if (occupiedSlot) {
            return res.status(409).json({
                message: "This slot is currently occupied. Choose another slot."
            });
        }

        // Reservations may not overlap another booking for the same slot.
        const conflict = await Reservation.exists({
            slotId: requestedSlot,
            status: "BOOKED",
            reservedFrom: { $lt: end },
            reservedUntil: { $gt: start }
        });

        if (conflict) {
            return res.status(409).json({
                message: "This slot already has an overlapping reservation."
            });
        }

        const reservation = await Reservation.create({
            customerName: customerName.trim(),
            customerEmail: customerEmail.trim().toLowerCase(),
            customerPhone: customerPhone.trim(),
            vehicleNumber: vehicle,
            slotId: requestedSlot,
            reservedFrom: start,
            reservedUntil: end
        });

        res.status(201).json({
            message: "Reservation created successfully.",
            reservation
        });
    } catch (error) {
        console.error("Create reservation error:", error.message);
        res.status(400).json({ message: "Could not create reservation." });
    }
});

router.patch("/reservations/:id/cancel", async (req, res) => {
    try {
        const reservation = await Reservation.findById(req.params.id);

        if (!reservation) {
            return res.status(404).json({
                message: "Reservation not found."
            });
        }

        if (reservation.status !== "BOOKED") {
            return res.status(409).json({
                message: "Only booked reservations can be cancelled."
            });
        }

        reservation.status = "CANCELLED";
        await reservation.save();

        res.json({
            message: "Reservation cancelled.",
            reservation
        });
    } catch (error) {
        res.status(400).json({
            message: "Invalid reservation ID."
        });
    }
});

// VIOLATIONS

router.get("/violations", async (req, res) => {
    try {
        res.json(await Violation.find().sort({ detectedAt: -1 }));
    } catch (error) {
        res.status(500).json({
            message: "Could not fetch violations."
        });
    }
});

router.post("/violations", async (req, res) => {
    try {
        const {
            vehicleNumber,
            slotId = null,
            violationType,
            description = "",
            detectedAt,
            penaltyAmount = 0
        } = req.body;

        if (
            typeof customerName !== "string" || !customerName.trim() || customerName.trim().length > 80 ||
            typeof customerEmail !== "string" || !/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(customerEmail.trim()) || customerEmail.trim().length > 120 ||
            typeof customerPhone !== "string" || !/^[0-9+() -]{7,20}$/.test(customerPhone.trim()) ||
            typeof vehicleNumber !== "string" ||
            !vehicleNumber.trim() ||
            typeof violationType !== "string" ||
            !violationType.trim() ||
            !Number.isFinite(Number(penaltyAmount)) ||
            Number(penaltyAmount) < 0
        ) {
            return res.status(400).json({
                message: "Provide valid vehicle, violation type and penalty details."
            });
        }

        const violation = await Violation.create({
            vehicleNumber: vehicleNumber.trim().toUpperCase(),
            slotId: slotId == null ? null : Number(slotId),
            violationType: violationType.trim(),
            description,
            detectedAt: detectedAt || new Date(),
            penaltyAmount: Number(penaltyAmount)
        });

        res.status(201).json({
            message: "Violation recorded.",
            violation
        });
    } catch (error) {
        res.status(400).json({
            message: "Could not record violation."
        });
    }
});

router.patch("/violations/:id/status", async (req, res) => {
    try {
        const { status } = req.body;

        if (!["PENDING", "PAID", "DISMISSED"].includes(status)) {
            return res.status(400).json({
                message: "Invalid violation status."
            });
        }

        const violation = await Violation.findByIdAndUpdate(
            req.params.id,
            { $set: { status } },
            { new: true, runValidators: true }
        );

        if (!violation) {
            return res.status(404).json({
                message: "Violation not found."
            });
        }

        res.json(violation);
    } catch (error) {
        res.status(400).json({
            message: "Could not update violation."
        });
    }
});

// USERS
// Passwords are hashed. Authentication and authorization still need to
// be added before exposing user/admin operations publicly.

router.post("/users", async (req, res) => {
    try {
        const { name, email, password, role = "USER" } = req.body;

        if (
            typeof name !== "string" ||
            !name.trim() ||
            typeof email !== "string" ||
            !email.trim() ||
            typeof password !== "string" ||
            password.length < 8 ||
            !["USER", "ADMIN"].includes(role)
        ) {
            return res.status(400).json({
                message: "Provide a name, valid email, password of at least 8 characters, and valid role."
            });
        }

        const passwordHash = await bcrypt.hash(password, 12);

        const user = await User.create({
            name: name.trim(),
            email: email.trim().toLowerCase(),
            passwordHash,
            role
        });

        res.status(201).json({
            message: "User created.",
            user: {
                id: user._id,
                name: user.name,
                email: user.email,
                role: user.role
            }
        });
    } catch (error) {
        res.status(error.code === 11000 ? 409 : 400).json({
            message: error.code === 11000
                ? "An account with this email already exists."
                : "Could not create user."
        });
    }
});

module.exports = router;