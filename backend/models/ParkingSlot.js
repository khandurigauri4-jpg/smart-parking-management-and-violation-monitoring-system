const mongoose = require("mongoose");

const parkingSlotSchema = new mongoose.Schema(
    {
        slotId: {
            type: Number,
            required: true,
            unique: true,
            min: 1
        },
        occupied: {
            type: Boolean,
            default: false
        },
        vehicleNumber: {
            type: String,
            default: "",
            uppercase: true,
            trim: true
        }
    },
    { timestamps: true }
);

module.exports = mongoose.model("ParkingSlot", parkingSlotSchema);