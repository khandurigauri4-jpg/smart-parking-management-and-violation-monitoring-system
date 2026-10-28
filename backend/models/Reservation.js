const mongoose = require("mongoose");

const reservationSchema = new mongoose.Schema(
    {
        customerName: { type: String, required: true, trim: true, maxlength: 80 },
        customerEmail: { type: String, required: true, lowercase: true, trim: true, maxlength: 120 },
        customerPhone: { type: String, required: true, trim: true, maxlength: 20 },
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
        reservationFee: { type: Number, default: 20, min: 0 },
        paymentStatus: { type: String, enum: ["PENDING", "PAID"], default: "PENDING" },
        paymentReference: { type: String, default: "" },
        paidAt: { type: Date, default: null },
        status: {
            type: String,
            enum: ["BOOKED", "COMPLETED", "CANCELLED"],
            default: "BOOKED"
        }
    },
    { timestamps: true }
);

module.exports = mongoose.model("Reservation", reservationSchema);