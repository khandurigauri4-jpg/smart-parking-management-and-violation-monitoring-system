/* =====================================================
   DIGITAL TICKETS
   ===================================================== */

let tickets =
    JSON.parse(
        localStorage.getItem("tickets")
    ) || [];


/* =====================================================
   SAVE
   ===================================================== */

function saveTickets() {

    localStorage.setItem(
        "tickets",
        JSON.stringify(tickets)
    );

}


/* =====================================================
   GENERATE TICKET
   ===================================================== */

function generateTicket(
    vehicleNumber,
    slotId,
    entryTime
) {

    const ticket = {

        id:
            "TKT-" +
            Date.now(),

        vehicleNumber,

        slot: slotId,

        entryTime,

        status: "Active"

    };


    tickets.unshift(ticket);

    saveTickets();

    renderTickets();

}


/* =====================================================
   DISPLAY TICKETS
   ===================================================== */

function renderTickets() {

    const container =
        document.getElementById(
            "ticketGrid"
        );


    if (!container) return;


    container.innerHTML = "";


    if (tickets.length === 0) {

        container.innerHTML = `

            <div class="panel empty-state">
                No parking tickets generated yet.
            </div>

        `;

        return;
    }


    tickets.forEach(ticket => {

        const card =
            document.createElement("div");


        card.className =
            "ticket-card";


        card.innerHTML = `

            <div class="ticket-header">

                <div>

                    <h3>
                        Parking Ticket
                    </h3>

                    <span class="ticket-id">
                        ${ticket.id}
                    </span>

                </div>

                <span class="status-badge active">
                    ${ticket.status}
                </span>

            </div>


            <div class="ticket-details">

                <div class="ticket-detail">

                    <span>Vehicle</span>

                    <strong>
                        ${ticket.vehicleNumber}
                    </strong>

                </div>


                <div class="ticket-detail">

                    <span>Slot</span>

                    <strong>
                        ${ticket.slot}
                    </strong>

                </div>


                <div class="ticket-detail">

                    <span>Entry Time</span>

                    <strong>
                        ${ticket.entryTime}
                    </strong>

                </div>


                <div class="ticket-detail">

                    <span>Payment</span>

                    <strong>
                        Pending
                    </strong>

                </div>

            </div>

        `;


        container.appendChild(card);

    });

}


/* =====================================================
   INITIALIZE
   ===================================================== */

document.addEventListener(
    "DOMContentLoaded",
    renderTickets
);