/* =====================================================
   SMARTPARK MAIN SCRIPT
   ===================================================== */


/* =====================================================
   DOM READY
   ===================================================== */

document.addEventListener(
    "DOMContentLoaded",
    () => {

        setupNavigation();

        setupForms();

        updateDate();

        updateDashboard();

    }
);


/* =====================================================
   NAVIGATION
   ===================================================== */

function setupNavigation() {

    const navItems =
        document.querySelectorAll(
            ".nav-item"
        );


    navItems.forEach(item => {

        item.addEventListener(
            "click",
            () => {

                const section =
                    item.dataset.section;

                showSection(section);

            }
        );

    });

}


function showSection(sectionId) {

    const sections =
        document.querySelectorAll(
            ".page-section"
        );


    sections.forEach(section => {

        section.classList.remove(
            "active-section"
        );

    });


    const target =
        document.getElementById(
            sectionId
        );


    if (target) {

        target.classList.add(
            "active-section"
        );

    }


    const navItems =
        document.querySelectorAll(
            ".nav-item"
        );


    navItems.forEach(item => {

        item.classList.remove(
            "active"
        );


        if (
            item.dataset.section ===
            sectionId
        ) {

            item.classList.add(
                "active"
            );

        }

    });


    window.scrollTo({
        top: 0,
        behavior: "smooth"
    });

}


/* =====================================================
   DATE
   ===================================================== */

function updateDate() {

    const dateElement =
        document.getElementById(
            "pageDate"
        );


    if (!dateElement) return;


    const today =
        new Date();


    dateElement.textContent =
        today.toLocaleDateString(
            "en-IN",
            {
                weekday: "long",
                day: "numeric",
                month: "long",
                year: "numeric"
            }
        );

}


/* =====================================================
   FORMS
   ===================================================== */

function setupForms() {


    /* Park vehicle */

    const parkForm =
        document.getElementById(
            "parkForm"
        );


    parkForm.addEventListener(
        "submit",
        event => {

            event.preventDefault();


            const vehicleNumber =
                document
                    .getElementById(
                        "vehicleNumber"
                    )
                    .value;


            const vehicleType =
                document
                    .getElementById(
                        "vehicleType"
                    )
                    .value;


            const success =
                parkVehicle(
                    vehicleNumber,
                    vehicleType
                );


            if (success) {

                parkForm.reset();

                closeModal(
                    "parkModal"
                );

            }

        }
    );


    /* Search vehicle */

    const searchForm =
        document.getElementById(
            "searchForm"
        );


    searchForm.addEventListener(
        "submit",
        event => {

            event.preventDefault();


            const vehicleNumber =
                document
                    .getElementById(
                        "searchVehicleNumber"
                    )
                    .value;


            const vehicle =
                findVehicle(
                    vehicleNumber
                );


            const result =
                document.getElementById(
                    "searchResult"
                );


            if (vehicle) {

                result.innerHTML = `

                    <div class="panel"
                         style="margin-bottom:0;">

                        <strong>
                            Vehicle Found
                        </strong>

                        <p style="
                            margin-top:10px;
                            font-size:12px;
                            color:#6b7280;
                        ">

                            Vehicle:
                            <strong>
                                ${vehicle.vehicleNumber}
                            </strong>

                            <br><br>

                            Parking Slot:
                            <strong>
                                ${vehicle.id}
                            </strong>

                            <br><br>

                            Entry Time:
                            <strong>
                                ${vehicle.entryTime}
                            </strong>

                        </p>

                    </div>

                `;

            } else {

                result.innerHTML = `

                    <div class="panel"
                         style="
                            margin-bottom:0;
                            color:#dc2626;
                         ">

                        Vehicle not found
                        in the parking area.

                    </div>

                `;

            }

        }
    );


    /* Reservation */

    const reservationForm =
        document.getElementById(
            "reservationForm"
        );


    reservationForm.addEventListener(
        "submit",
        event => {

            event.preventDefault();


            const vehicle =
                document
                    .getElementById(
                        "reservationVehicle"
                    )
                    .value;


            const date =
                document
                    .getElementById(
                        "reservationDate"
                    )
                    .value;


            const time =
                document
                    .getElementById(
                        "reservationTime"
                    )
                    .value;


            const success =
                createReservation(
                    vehicle,
                    date,
                    time
                );


            if (success) {

                reservationForm.reset();

                closeModal(
                    "reservationModal"
                );

            }

        }
    );


    /* Violation */

    const violationForm =
        document.getElementById(
            "violationForm"
        );


    violationForm.addEventListener(
        "submit",
        event => {

            event.preventDefault();


            const vehicle =
                document
                    .getElementById(
                        "violationVehicle"
                    )
                    .value;


            const type =
                document
                    .getElementById(
                        "violationType"
                    )
                    .value;


            const fine =
                document
                    .getElementById(
                        "violationFine"
                    )
                    .value;


            createViolation(
                vehicle,
                type,
                fine
            );


            violationForm.reset();

            closeModal(
                "violationModal"
            );

        }
    );

}


/* =====================================================
   MODALS
   ===================================================== */

function openParkModal() {

    document
        .getElementById("parkModal")
        .classList.add("active");

}


function openSearchModal() {

    document
        .getElementById("searchModal")
        .classList.add("active");

}


function openReservationModal() {

    document
        .getElementById(
            "reservationModal"
        )
        .classList.add("active");

}


function openViolationModal() {

    document
        .getElementById(
            "violationModal"
        )
        .classList.add("active");

}


function closeModal(id) {

    document
        .getElementById(id)
        .classList.remove("active");

}


/* =====================================================
   CLICK OUTSIDE MODAL
   ===================================================== */

document.addEventListener(
    "click",
    event => {

        if (
            event.target.classList
                .contains("modal-overlay")
        ) {

            event.target.classList
                .remove("active");

        }

    }
);


/* =====================================================
   DASHBOARD
   ===================================================== */

function updateDashboard() {

    if (
        typeof renderParking ===
        "function"
    ) {

        renderParking();

    }


    if (
        typeof renderReservations ===
        "function"
    ) {

        renderReservations();

    }


    if (
        typeof renderViolations ===
        "function"
    ) {

        renderViolations();

    }


    if (
        typeof renderTickets ===
        "function"
    ) {

        renderTickets();

    }


    renderRecentActivity();

}


/* =====================================================
   RECENT ACTIVITY
   ===================================================== */

function addParkingHistory(
    vehicleNumber,
    slotId,
    entryTime
) {

    let history =
        JSON.parse(
            localStorage.getItem(
                "parkingHistory"
            )
        ) || [];


    history.unshift({

        vehicleNumber,

        slot: slotId,

        entry: entryTime,

        exit: "-",

        duration: "-",

        status: "Active"

    });


    localStorage.setItem(
        "parkingHistory",
        JSON.stringify(history)
    );


    renderRecentActivity();

    renderHistory();

}


function renderRecentActivity() {

    const table =
        document.getElementById(
            "recentActivityTable"
        );


    if (!table) return;


    const history =
        JSON.parse(
            localStorage.getItem(
                "parkingHistory"
            )
        ) || [];


    table.innerHTML = "";


    if (history.length === 0) {

        table.innerHTML = `

            <tr>
                <td colspan="4">
                    <div class="empty-state">
                        No parking activity yet.
                    </div>
                </td>
            </tr>

        `;

        return;

    }


    history
        .slice(0, 5)
        .forEach(record => {

            const row =
                document.createElement(
                    "tr"
                );


            row.innerHTML = `

                <td>
                    <strong>
                        ${record.vehicleNumber}
                    </strong>
                </td>

                <td>
                    Slot ${record.slot}
                </td>

                <td>
                    ${record.entry}
                </td>

                <td>

                    <span class="status-badge active">
                        ${record.status}
                    </span>

                </td>

            `;


            table.appendChild(row);

        });

}


/* =====================================================
   HISTORY
   ===================================================== */

function renderHistory() {

    const table =
        document.getElementById(
            "historyTable"
        );


    if (!table) return;


    const history =
        JSON.parse(
            localStorage.getItem(
                "parkingHistory"
            )
        ) || [];


    table.innerHTML = "";


    if (history.length === 0) {

        table.innerHTML = `

            <tr>
                <td colspan="6">

                    <div class="empty-state">
                        No parking history available.
                    </div>

                </td>
            </tr>

        `;

        return;

    }


    history.forEach(record => {

        const row =
            document.createElement(
                "tr"
            );


        row.innerHTML = `

            <td>
                <strong>
                    ${record.vehicleNumber}
                </strong>
            </td>

            <td>
                Slot ${record.slot}
            </td>

            <td>
                ${record.entry}
            </td>

            <td>
                ${record.exit}
            </td>

            <td>
                ${record.duration}
            </td>

            <td>

                <span class="status-badge active">
                    ${record.status}
                </span>

            </td>

        `;


        table.appendChild(row);

    });

}


/* =====================================================
   UTILITY
   ===================================================== */

function setText(
    elementId,
    value
) {

    const element =
        document.getElementById(
            elementId
        );


    if (element) {

        element.textContent =
            value;

    }

}


/* =====================================================
   TOAST
   ===================================================== */

function showToast(message) {

    const toast =
        document.getElementById(
            "toast"
        );


    if (!toast) return;


    toast.textContent =
        message;


    toast.classList.add(
        "show"
    );


    setTimeout(
        () => {

            toast.classList.remove(
                "show"
            );

        },
        2500
    );

}


/* =====================================================
   INITIAL HISTORY
   ===================================================== */

document.addEventListener(
    "DOMContentLoaded",
    () => {

        renderHistory();

        renderRecentActivity();

    }
);