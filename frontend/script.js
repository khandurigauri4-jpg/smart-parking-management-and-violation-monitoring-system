
const API_BASE = "http://localhost:3000/api/parking";
const RECORDS_API = "http://localhost:3000/api";

function showParking() {
    document.getElementById("parking").scrollIntoView({
        behavior: "smooth"
    });
}

async function apiRequest(url, options = {}) {
    const response = await fetch(url, options);
    const data = await response.json();

    if (!response.ok) {
        throw new Error(data.message || "Request failed.");
    }

    return data;
}

async function loadParkingStatus() {
    try {
        const data = await apiRequest(API_BASE);

        data.slots.forEach(function (slotData) {
            const slot = document.getElementById("slot" + slotData.slotId);
            if (!slot) return;

            slot.classList.toggle("occupied", slotData.occupied);
            slot.classList.toggle("available", !slotData.occupied);

            slot.querySelector("p").innerText = slotData.occupied
                ? slotData.vehicleNumber || "Occupied"
                : "Available";
        });
    } catch (error) {
        console.error("Parking status error:", error);
        showMessage("Could not load parking status. Check the backend.");
    }
}

async function parkVehicle() {
    const input = document.getElementById("vehicleNumber");
    const vehicleNumber = input.value.trim().toUpperCase();

    if (!vehicleNumber) {
        showMessage("Please enter vehicle number.");
        return;
    }

    try {
        const data = await apiRequest(`${API_BASE}/park`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ vehicleNumber })
        });

        input.value = "";
        await loadParkingStatus();

        showMessage(
            `${data.vehicleNumber} parked in Slot ${data.slotId}. Ticket: ${data.ticketId}`
        );
    } catch (error) {
        showMessage(error.message);
    }
}

async function releaseVehicle() {
    const input = document.getElementById("slotNumber");
    const slotId = Number(input.value);

    if (!input.value.trim()) {
        showMessage("Please enter slot number.");
        return;
    }

    if (!Number.isInteger(slotId) || slotId < 1 || slotId > 10) {
        showMessage("Enter a slot number from 1 to 10.");
        return;
    }

    try {
        const data = await apiRequest(`${API_BASE}/release`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({ slotId })
        });

        input.value = "";
        await loadParkingStatus();

        showMessage(
            `Slot ${slotId} released. Duration: ${data.durationMinutes} minutes. Fee: ₹${data.fee}.`
        );
    } catch (error) {
        showMessage(error.message);
    }
}

// CREATE A RESERVATION
async function createReservation(event) {
    event.preventDefault();

    const vehicleNumber = document.getElementById("reservationVehicle")
        .value.trim().toUpperCase();
    const slotId = Number(document.getElementById("reservationSlot").value);
    const reservedFrom = document.getElementById("reservedFrom").value;
    const reservedUntil = document.getElementById("reservedUntil").value;

    const start = new Date(reservedFrom);
    const end = new Date(reservedUntil);

    if (!vehicleNumber || !reservedFrom || !reservedUntil) {
        showMessage("Please complete all reservation fields.");
        return;
    }

    if (!Number.isFinite(start.getTime()) ||
        !Number.isFinite(end.getTime()) ||
        end <= start) {
        showMessage("Reservation end time must be after its start time.");
        return;
    }

    if (start <= new Date()) {
        showMessage("Choose a future reservation start time.");
        return;
    }

    try {
        const data = await apiRequest(`${RECORDS_API}/reservations`, {
            method: "POST",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify({
                vehicleNumber,
                slotId,
                reservedFrom: start.toISOString(),
                reservedUntil: end.toISOString()
            })
        });

        document.getElementById("reservationForm").reset();
        showMessage(data.message || "Reservation created successfully.");
        await loadReservations();
    } catch (error) {
        showMessage(error.message);
    }
}

function formatDate(value) {
    return new Date(value).toLocaleString();
}

// LOAD SAVED RESERVATIONS
async function loadReservations() {
    const container = document.getElementById("reservationList");
    container.replaceChildren();

    const loading = document.createElement("p");
    loading.textContent = "Loading reservations...";
    container.appendChild(loading);

    try {
        const reservations = await apiRequest(`${RECORDS_API}/reservations`);
        container.replaceChildren();

        if (!reservations.length) {
            const empty = document.createElement("p");
            empty.textContent = "No reservations yet.";
            container.appendChild(empty);
            return;
        }

        reservations.forEach(function (reservation) {
            const card = document.createElement("article");
            card.className = "reservation-item";

            const title = document.createElement("h4");
            title.textContent = reservation.vehicleNumber;

            const details = document.createElement("p");
            details.textContent =
                `Slot: ${reservation.slotId ?? "Unassigned"} | ` +
                `From: ${formatDate(reservation.reservedFrom)} | ` +
                `Until: ${formatDate(reservation.reservedUntil)}`;

            const status = document.createElement("p");
            status.textContent = `Status: ${reservation.status}`;
            status.className = "reservation-status";

            card.append(title, details, status);

            if (reservation.status === "BOOKED") {
                const cancelButton = document.createElement("button");
                cancelButton.type = "button";
                cancelButton.textContent = "Cancel Reservation";
                cancelButton.addEventListener("click", function () {
                    cancelReservation(reservation._id);
                });
                card.appendChild(cancelButton);
            }

            container.appendChild(card);
        });
    } catch (error) {
        container.replaceChildren();
        const errorText = document.createElement("p");
        errorText.textContent = error.message;
        container.appendChild(errorText);
    }
}

// CANCEL A RESERVATION
async function cancelReservation(id) {
    if (!confirm("Are you sure you want to cancel this reservation?")) {
        return;
    }

    try {
        const data = await apiRequest(
            `${RECORDS_API}/reservations/${encodeURIComponent(id)}/cancel`,
            { method: "PATCH" }
        );

        showMessage(data.message || "Reservation cancelled.");
        await loadReservations();
    } catch (error) {
        showMessage(error.message);
    }
}


// DSA GRAPH: nearest available slot calculated by the C Dijkstra engine.
async function findNearestParkingSlot() {
    const resultBox = document.getElementById("navigationResult");
    resultBox.textContent = "Calculating nearest available slot...";

    try {
        const result = await apiRequest(`${API_BASE}/nearest`);
        resultBox.textContent =
            `Nearest available: Slot ${result.slotId}. ` +
            `Distance: ${result.distance} units. Route: ${result.pathLabels.join(" → ")}.`;
        document.getElementById("routeSlotId").value = String(result.slotId);
    } catch (error) {
        resultBox.textContent = error.message;
    }
}

// DSA GRAPH: shortest path from entrance to selected slot.
async function findRouteToSlot() {
    const slotId = Number(document.getElementById("routeSlotId").value);
    const resultBox = document.getElementById("navigationResult");
    resultBox.textContent = "Calculating shortest route...";

    try {
        const result = await apiRequest(`${API_BASE}/route/${slotId}`);
        resultBox.textContent =
            `Route to Slot ${result.slotId}: ${result.pathLabels.join(" → ")}. ` +
            `Total distance: ${result.distance} units.`;
    } catch (error) {
        resultBox.textContent = error.message;
    }
}

function showMessage(message) {
    const messageBox = document.getElementById("message");
    messageBox.textContent = message;
    messageBox.style.display = "block";

    clearTimeout(window.messageTimeout);
    window.messageTimeout = setTimeout(function () {
        messageBox.style.display = "none";
    }, 6000);
}

document.addEventListener("DOMContentLoaded", function () {
    document.getElementById("reservationForm").addEventListener(
        "submit",
        createReservation
    );

    loadParkingStatus();
    loadReservations();
});
