const express = require("express");
const bcrypt = require("bcryptjs");
const User = require("../models/User");

const router = express.Router();

router.post("/signup", async (req, res) => {
    try {
        const { name, email, password, role } = req.body;
        const normalizedEmail = String(email || "").trim().toLowerCase();
        const normalizedName = String(name || "").trim();
        const accountRole = String(role || "").toUpperCase();

        if (!normalizedName || !normalizedEmail || !password) {
            return res.status(400).json({ message: "Name, email and password are required." });
        }
        if (!/^\S+@\S+\.\S+$/.test(normalizedEmail)) {
            return res.status(400).json({ message: "Enter a valid email address." });
        }
        if (String(password).length < 6) {
            return res.status(400).json({ message: "Password must contain at least 6 characters." });
        }
        if (!["USER", "ADMIN"].includes(accountRole)) {
            return res.status(400).json({ message: "Choose a valid account type." });
        }
        const existing = await User.findOne({ email: normalizedEmail });
        if (existing) return res.status(409).json({ message: "An account with this email already exists." });

        const passwordHash = await bcrypt.hash(String(password), 10);
        const user = await User.create({ name: normalizedName, email: normalizedEmail, passwordHash, role: accountRole });
        return res.status(201).json({ message: "Account created successfully. You can now log in.", role: user.role });
    } catch (error) {
        if (error.code === 11000) return res.status(409).json({ message: "An account with this email already exists." });
        console.error("Signup error:", error.message);
        return res.status(500).json({ message: "Could not create the account." });
    }
});

router.post("/login", async (req, res) => {
    try {
        const email = String(req.body.email || "").trim().toLowerCase();
        const password = String(req.body.password || "");
        const requestedRole = String(req.body.role || "").toUpperCase();
        if (!email || !password || !["USER", "ADMIN"].includes(requestedRole)) {
            return res.status(400).json({ message: "Email, password and account type are required." });
        }
        const user = await User.findOne({ email });
        if (!user || user.role !== requestedRole || !(await bcrypt.compare(password, user.passwordHash))) {
            return res.status(401).json({ message: "Invalid email, password or account type." });
        }
        return res.json({ message: "Login successful.", user: { id: user._id, name: user.name, email: user.email, role: user.role } });
    } catch (error) {
        console.error("Login error:", error.message);
        return res.status(500).json({ message: "Could not log in. Please try again." });
    }
});

module.exports = router;
