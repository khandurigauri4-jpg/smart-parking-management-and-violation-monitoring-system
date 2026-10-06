/* =====================================================
   PARKING MANAGEMENT
   ===================================================== */

const TOTAL_SLOTS = 20;

let parkingSlots =
    JSON.parse(localStorage.getItem("parkingSlots")) ||
    createInitialSlots();


function createInitialSlots() {

    const slots = [];

    for (let i = 1; i <= TOTAL_SLOTS; i++) {

        slots.push({
            id: i,
            status: "available",
            vehicleNumber: "",
            vehicleType: "",
            entryTime: ""
        });

    }

    return slots;
}


/* =====================================================
   SAVE DATA
   ===================================================== */

function saveParkingData() {

    localStorage.setItem(
        "parkingSlots",
        JSON.stringify(parkingSlots)
    );

}


/* =====================================================
   DISPLAY PARKING
   ===================================================== */

function renderParking() {

    const parkingGrid =
        document.getElementById("parkingGrid");

    const dashboardGrid =
        document.getElementById("dashboardParkingGrid");

    if (!parkingGrid || !dashboardGrid) return;


    parkingGrid.innerHTML = "";
    dashboardGrid.innerHTML = "";


    parkingSlots.forEach(slot => {

        /* Full parking page */

        const slotElement =
            document.createElement("div");

        slotElement.className =
            `parking-slot ${slot.status}`;

        slotElement.innerHTML = `

            <span class="slot-number">
                ${slot.id}
            </span>

            <span class="slot-status">
                ${slot.status}
            </span>

            ${
                slot.vehicleNumber
                ?
                `<span class="vehicle-label">
                    ${slot.vehicleNumber}
                </span>`
                :
                ""
            }

        `;

        slotElement.onclick = () => {

            if (slot.status === "occupied") {

                const confirmRelease =
                    confirm(
                        `Release vehicle ${slot.vehicleNumber} from Slot ${slot.id}?`
                    );

                if (confirmRelease) {

                    releaseVehicle(slot.id);

                }

            } else {

                showToast(
                    `Slot ${slot.id} is ${slot.status}.`
                );

            }

        };

        parkingGrid.appendChild(slotElement);


        /* Dashboard mini slot */

        const miniSlot =
            document.createElement("div");

        miniSlot.className =
            `parking-mini-slot ${slot.status}`;

        miniSlot.textContent =
            slot.id;

        miniSlot.title =
            slot.vehicleNumber ||
            `Slot ${slot.id} - ${slot.status}`;

        dashboardGrid.appendChild(miniSlot);

    });


    updateParkingStats();

}


/* =====================================================
   PARK VEHICLE
   ===================================================== */

function parkVehicle(vehicleNumber, vehicleType) {

    vehicleNumber =
        vehicleNumber.trim().toUpperCase();


    if (!vehicleNumber) {

        showToast("Please enter vehicle number.");

        return false;

    }


    /* Check duplicate vehicle */

    const existingVehicle =
        parkingSlots.find(
            slot =>
                slot.vehicleNumber === vehicleNumber &&
                slot.status === "occupied"
        );


    if (existingVehicle) {

        showToast(
            `Vehicle is already parked in Slot ${existingVehicle.id}.`
        );

        return false;

    }


    /* Find first available slot */

    const availableSlot =
        parkingSlots.find(
            slot => slot.status === "available"
        );


    if (!availableSlot) {

        showToast("No parking slots available.");

        return false;

    }


    availableSlot.status = "occupied";

    availableSlot.vehicleNumber =
        vehicleNumber;

    availableSlot.vehicleType =
        vehicleType;

    availableSlot.entryTime =
        new Date().toLocaleTimeString(
            [],
            {
                hour: "2-digit",
                minute: "2-digit"
            }
        );


    saveParkingData();

    addParkingHistory(
        vehicleNumber,
        availableSlot.id,
        availableSlot.entryTime
    );


    renderParking();

    updateDashboard();

    generateTicket(
        vehicleNumber,
        availableSlot.id,
        availableSlot.entryTime
    );


    showToast(
        `Vehicle parked in Slot ${availableSlot.id}.`
    );


    return true;
}


/* =====================================================
   RELEASE VEHICLE
   ===================================================== */

function releaseVehicle(slotId) {

    const slot =
        parkingSlots.find(
            slot => slot.id === slotId
        );


    if (!slot ||
        slot.status !== "occupied") {

        return;

    }


    const vehicleNumber =
        slot.vehicleNumber;


    slot.status = "available";

    slot.vehicleNumber = "";

    slot.vehicleType = "";

    slot.entryTime = "";


    saveParkingData();

    renderParking();

    updateDashboard();


    showToast(
        `${vehicleNumber} released from Slot ${slotId}.`
    );

}


/* =====================================================
   SEARCH VEHICLE
   ===================================================== */

function findVehicle(vehicleNumber) {

    vehicleNumber =
        vehicleNumber.trim().toUpperCase();


    return parkingSlots.find(
        slot =>
            slot.vehicleNumber === vehicleNumber &&
            slot.status === "occupied"
    );

}


/* =====================================================
   PARKING STATISTICS
   ===================================================== */

function updateParkingStats() {

    const available =
        parkingSlots.filter(
            slot => slot.status === "available"
        ).length;


    const occupied =
        parkingSlots.filter(
            slot => slot.status === "occupied"
        ).length;


    const reserved =
        parkingSlots.filter(
            slot => slot.status === "reserved"
        ).length;


    setText(
        "parkingAvailable",
        available
    );

    setText(
        "parkingOccupied",
        occupied
    );

    setText(
        "parkingReserved",
        reserved
    );


    setText(
        "totalSlotsStat",
        TOTAL_SLOTS
    );

    setText(
        "availableSlotsStat",
        available
    );

    setText(
        "occupiedSlotsStat",
        occupied
    );

}


/* =====================================================
   INITIALIZE
   ===================================================== */

document.addEventListener(
    "DOMContentLoaded",
    renderParking
);