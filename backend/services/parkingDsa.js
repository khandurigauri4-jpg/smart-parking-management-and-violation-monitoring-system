const path = require("path");
const { execFileSync } = require("child_process");

const dsaDirectory = path.resolve(__dirname, "../../dsa/parking");
const executable = path.join(dsaDirectory, "parking_bridge");

function compileParkingDSA() {
    const sourceFiles = [
        path.join(dsaDirectory, "bridge.c"),
        path.join(dsaDirectory, "parking.c"),
        path.join(dsaDirectory, "graph.c")
    ];

    execFileSync("cc", [
        "-std=c11",
        "-Wall",
        "-Wextra",
        ...sourceFiles,
        "-o",
        executable
    ], { stdio: "pipe" });

    return executable;
}

function runBridge(args) {
    try {
        const output = execFileSync(executable, args.map(String), {
            encoding: "utf8",
            timeout: 3000
        });
        return JSON.parse(output);
    } catch (error) {
        const detail = error.stderr
            ? error.stderr.toString().trim()
            : error.message;
        throw new Error(`C parking engine failed: ${detail}`);
    }
}

function findNearestSlot(slots) {
    const occupiedFlags = Array.from({ length: 10 }, (_, index) => {
        const slot = slots.find(item => item.slotId === index + 1);
        return slot && slot.occupied ? 1 : 0;
    });

    return runBridge(["nearest", ...occupiedFlags]);
}

function findRoute(slotId) {
    return runBridge(["path", slotId]);
}

module.exports = { compileParkingDSA, findNearestSlot, findRoute };
