require("dotenv").config();

const express = require("express");
const cors = require("cors");
const mongoose = require("mongoose");

const ParkingSlot = require("./models/ParkingSlot");
const parkingRoutes = require("./routes/parkingRoutes");
const recordsRoutes = require("./routes/recordRoutes");
const { compileParkingDSA } = require("./services/parkingDsa");

const app = express();

const PORT = process.env.PORT || 3000;
const TOTAL_SLOTS = 10;

app.use(cors());
app.use(express.json({ limit: "100kb" }));

app.get("/", (req, res) => {
    res.json({
        message: "Smart Parking Backend is running!",
        database: mongoose.connection.readyState === 1
            ? "connected"
            : "disconnected"
    });
});

app.get("/api/health", (req, res) => {
    res.json({
        server: "running",
        database: mongoose.connection.readyState === 1
            ? "connected"
            : "disconnected"
    });
});

app.use("/api/parking", parkingRoutes);
app.use("/api", recordsRoutes);

app.use((req, res) => {
    res.status(404).json({
        message: "Route not found."
    });
});

app.use((error, req, res, next) => {
    console.error("Request error:", error.message);

    res.status(500).json({
        message: "An unexpected server error occurred."
    });
});

// Create missing slots only; do not reset existing data.
async function initializeParkingSlots() {
    const operations = [];

    for (let slotId = 1; slotId <= TOTAL_SLOTS; slotId++) {
        operations.push({
            updateOne: {
                filter: { slotId },
                update: {
                    $setOnInsert: {
                        slotId,
                        occupied: false,
                        vehicleNumber: ""
                    }
                },
                upsert: true
            }
        });
    }

    await ParkingSlot.bulkWrite(operations);
}

async function startServer() {
    try {
        if (!process.env.MONGODB_URI) {
            throw new Error(
                "MONGODB_URI is missing from backend/.env."
            );
        }

        compileParkingDSA();
        console.log("C parking and graph engine compiled.");

        await mongoose.connect(process.env.MONGODB_URI);

        console.log("Connected to MongoDB successfully.");

        await initializeParkingSlots();

        console.log("Parking slots initialized.");

        app.listen(PORT, () => {
            console.log(
                `Smart Parking Backend running at http://localhost:${PORT}`
            );
        });
    } catch (error) {
        console.error(
            "Failed to start the backend:",
            error.message
        );

        process.exit(1);
    }
}

startServer();