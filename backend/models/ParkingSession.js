const mongoose = require("mongoose");

const parkingSessionSchema = new mongoose.Schema(
    {
        ticketId: {
            type: String,
            required: true,
            unique: true
        },
        vehicleNumber: {
            type: String,
            required: true,
            uppercase: true,
            trim: true
        },
        slotId: {
            type: Number,
            required: true
        },
        entryTime: {
            type: Date,
            default: Date.now,
            required: true
        },
        exitTime: {
            type: Date,
            default: null
        },
        allowedHours: { type: Number, default: 2, min: 1, max: 24 },
        durationMinutes: {
            type: Number,
            default: 0
        },
        fee: {
            type: Number,
            default: 0,
            min: 0
        },
        status: {
            type: String,
            enum: ["ACTIVE", "COMPLETED"],
            default: "ACTIVE"
        }
    },
    { timestamps: true }
);

module.exports = mongoose.model("ParkingSession", parkingSessionSchema);