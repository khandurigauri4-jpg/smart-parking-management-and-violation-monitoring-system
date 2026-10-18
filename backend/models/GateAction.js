const mongoose = require("mongoose");

const gateActionSchema = new mongoose.Schema({
  actionType: { type: String, enum: ["ENTRY", "EXIT", "OVERRIDE"], required: true },
  vehicleNumber: { type: String, required: true, uppercase: true, trim: true },
  slotId: { type: Number, required: true, min: 1, max: 10 },
  createdAt: { type: Date, default: Date.now }
}, { timestamps: false });

module.exports = mongoose.model("GateAction", gateActionSchema);
