const path = require("path");
const { execFileSync } = require("child_process");
const dir = path.resolve(__dirname, "../../cpp-oop");
const executable = path.join(dir, "parking_cpp_bridge");
function compileCppParking() {
  const sources = [path.join(dir, "bridge.cpp"), ...["ParkingSlot.cpp", "Ticket.cpp", "User.cpp", "Vehicle.cpp", "Violation.cpp"].map(name => path.join(dir, "src", name))];
  execFileSync("g++", ["-std=c++17", "-Wall", "-Wextra", ...sources, "-o", executable], { stdio: "pipe" });
  return executable;
}
function calculateCppFee({ vehicleNumber, ownerName, category, vehicleName, slotId, entryTime, exitTime, allowedHours }) {
  const output = execFileSync(executable, [vehicleNumber, ownerName || "Guest", String(category || 4), vehicleName || "Vehicle", String(slotId), entryTime, exitTime, String(allowedHours || 2)], { encoding: "utf8", timeout: 3000 });
  return JSON.parse(output);
}
module.exports = { compileCppParking, calculateCppFee };
