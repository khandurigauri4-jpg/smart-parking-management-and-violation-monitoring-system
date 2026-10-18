const mongoose = require("mongoose");

const violationSchema = new mongoose.Schema(
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
        violationType: {
            type: String,
            required: true,
            trim: true
        },
        description: {
            type: String,
            default: ""
        },
        detectedAt: {
            type: Date,
            default: Date.now
        },
        penaltyAmount: {
            type: Number,
            default: 0,
            min: 0
        },
        status: {
            type: String,
            enum: ["PENDING", "PAID", "DISMISSED"],
            default: "PENDING"
        }
    },
    { timestamps: true }
);

module.exports = mongoose.model("Violation", violationSchema);