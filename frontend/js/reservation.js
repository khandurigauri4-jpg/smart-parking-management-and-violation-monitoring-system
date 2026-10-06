/* =====================================================
   RESERVATION MANAGEMENT
   ===================================================== */

let reservations =
    JSON.parse(
        localStorage.getItem("reservations")
    ) || [];


/* =====================================================
   SAVE
   ===================================================== */

function saveReservations() {

    localStorage.setItem(
        "reservations",
        JSON.stringify(reservations)
    );

}


/* =====================================================
   CREATE RESERVATION
   ===================================================== */

function createReservation(
    vehicleNumber,
    date,
    time
) {

    vehicleNumber =
        vehicleNumber.trim().toUpperCase();


    const availableSlot =
        parkingSlots.find(
            slot => slot.status === "available"
        );


    if (!availableSlot) {

        showToast(
            "No slots available for reservation."
        );

        return false;

    }


    const reservation = {

        id:
            "RES-" +
            String(
                reservations.length + 1
            ).padStart(3, "0"),

        vehicleNumber,

        slot:
            availableSlot.id,

        date,

        time,

        status: "Confirmed"

    };


    reservations.push(reservation);

    saveReservations();

    renderReservations();

    updateDashboard();


    showToast(
        `Slot ${availableSlot.id} reserved successfully.`
    );


    return true;
}


/* =====================================================
   DISPLAY RESERVATIONS
   ===================================================== */

function renderReservations() {

    const table =
        document.getElementById(
            "reservationTable"
        );


    if (!table) return;


    table.innerHTML = "";


    if (reservations.length === 0) {

        table.innerHTML = `

            <tr>
                <td colspan="6">
                    <div class="empty-state">
                        No reservations found.
                    </div>
                </td>
            </tr>

        `;

        setText(
            "reservationStat",
            0
        );

        return;
    }


    reservations.forEach(reservation => {

        const row =
            document.createElement("tr");


        row.innerHTML = `

            <td>
                <strong>
                    ${reservation.id}
                </strong>
            </td>

            <td>
                ${reservation.vehicleNumber}
            </td>

            <td>
                Slot ${reservation.slot}
            </td>

            <td>
                ${reservation.date}
            </td>

            <td>
                ${reservation.time}
            </td>

            <td>
                <span class="status-badge active">
                    ${reservation.status}
                </span>
            </td>

        `;


        table.appendChild(row);

    });


    setText(
        "reservationStat",
        reservations.length
    );

}


/* =====================================================
   INITIALIZE
   ===================================================== */

document.addEventListener(
    "DOMContentLoaded",
    renderReservations
);