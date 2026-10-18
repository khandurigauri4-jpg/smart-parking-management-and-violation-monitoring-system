const path = require("path");
const { execFileSync } = require("child_process");
const dir = path.resolve(__dirname, "../../dsa/mradul");
const executable = path.join(dir, "dsa_bridge");
function compileTeamDSA() {
  execFileSync("make", ["-C", dir, "dsa_bridge"], { stdio: "pipe" });
  return executable;
}
function run(command, records, extraArgs = []) {
  const input = records.map(record => record.join("\t")).join("\n") + (records.length ? "\n" : "");
  const output = execFileSync(executable, [command, ...extraArgs], { input, encoding: "utf8", timeout: 3000 });
  return JSON.parse(output);
}
function searchActiveVehicle(slots, vehicleNumber) {
  return run("search", slots.filter(s => s.occupied).map(s => [s.vehicleNumber, s.slotId]), [vehicleNumber]);
}
function listActiveVehicles(slots) {
  return run("list", slots.filter(s => s.occupied).map(s => [s.vehicleNumber, s.slotId]));
}
function buildReservationQueue(reservations) {
  return run("queue", reservations.map(r => [r.vehicleNumber, new Date(r.reservedFrom).toISOString(), 1]));
}
function buildActionStack(actions) {
  return run("stack", actions.slice().reverse().map(a => [a.actionType, a.vehicleNumber, a.slotId]));
}
module.exports = { compileTeamDSA, searchActiveVehicle, listActiveVehicles, buildReservationQueue, buildActionStack };
