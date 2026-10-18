const express = require("express");
const path = require("path");
const fs = require("fs");
const { execFileSync } = require("child_process");
const router = express.Router();
const projectRoot = path.resolve(__dirname, "../..");
const script = path.join(projectRoot, "python", "ocr", "recognize_plate.py");
const pythonCandidates = [
  path.join(projectRoot, ".venv", "bin", "python3"),
  path.join(projectRoot, ".venv", "bin", "python"),
  "python3"
];
function pythonExecutable() {
  return pythonCandidates.find(candidate => candidate.includes(path.sep) ? fs.existsSync(candidate) : true) || "python3";
}
router.get("/health", (_req, res) => {
  const python = pythonExecutable();
  try {
    const output = execFileSync(python, ["-c", "import cv2, numpy, pytesseract; print('python-ok')"], { encoding: "utf8", timeout: 5000 });
    res.json({ available: output.includes("python-ok"), python, tesseractPath: (() => { try { return execFileSync("tesseract", ["--version"], { encoding: "utf8", timeout: 3000 }).split(/\r?\n/)[0]; } catch { return null; } })() });
  } catch (error) {
    res.status(503).json({ available: false, python, message: "OCR dependencies are missing. Run the setup commands in INTEGRATION-README.md." });
  }
});
router.post("/recognize", (req, res) => {
  const imageBase64 = req.body?.imageBase64;
  if (typeof imageBase64 !== "string" || !imageBase64.trim()) return res.status(400).json({ message: "Choose a plate image first." });
  if (imageBase64.length > 8 * 1024 * 1024) return res.status(413).json({ message: "Image is too large. Maximum encoded size is 8 MB." });
  try {
    const python = pythonExecutable();
    const output = execFileSync(python, [script], { input: JSON.stringify({ imageBase64 }), encoding: "utf8", timeout: 25000, cwd: path.dirname(script), maxBuffer: 1024 * 1024 });
    const result = JSON.parse(output);
    if (result.error) return res.status(422).json({ message: `Could not recognize the plate: ${result.error}` });
    if (!result.plateNumber || result.plateNumber.length < 8 || result.plateNumber.length > 10) return res.status(422).json({ message: "Could not read a complete registration number. Try a clearer, front-facing plate image.", ...result });
    return res.json({ message: "Number plate recognized.", ...result });
  } catch (error) {
    console.error("OCR recognition failed:", error.stderr?.toString?.() || error.message);
    return res.status(503).json({ message: "OCR service is not ready. Run the Python/Tesseract setup commands, then restart the backend." });
  }
});
module.exports = router;
