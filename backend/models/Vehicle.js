const mongoose = require("mongoose");

const vehicleSchema = new mongoose.Schema(
    {
        vehicleNumber: {
            type: String,
            required: true,
            unique: true,
            uppercase: true,
            trim: true
        },
        ownerName: {
            type: String,
            default: ""
        },
        ownerContact: {
            type: String,
            default: ""
        },
        blacklisted: {
            type: Boolean,
            default: false
        },
        blacklistReason: {
            type: String,
            default: ""
        }
    },
    { timestamps: true }
);

module.exports = mongoose.model("Vehicle", vehicleSchema);