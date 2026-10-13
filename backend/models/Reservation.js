const mongoose = require("mongoose");

const reservationSchema = new mongoose.Schema(
    {
        vehicleNumber: {
            type: String,
            required: true,
            uppercase: true,
            trim: true
        },
        slotId: {
            type: Number,
            default: null
        },
        reservedFrom: {
            type: Date,
            required: true
        },
        reservedUntil: {
            type: Date,
            required: true
        },
        status: {
            type: String,
            enum: ["BOOKED", "COMPLETED", "CANCELLED"],
            default: "BOOKED"
        }
    },
    { timestamps: true }
);

module.exports = mongoose.model("Reservation", reservationSchema);