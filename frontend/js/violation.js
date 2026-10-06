/* =====================================================
   VIOLATION MANAGEMENT
   ===================================================== */

let violations =
    JSON.parse(
        localStorage.getItem("violations")
    ) || [];


/* =====================================================
   SAVE
   ===================================================== */

function saveViolations() {

    localStorage.setItem(
        "violations",
        JSON.stringify(violations)
    );

}


/* =====================================================
   CREATE VIOLATION
   ===================================================== */

function createViolation(
    vehicleNumber,
    type,
    fine
) {

    const violation = {

        vehicleNumber:
            vehicleNumber
                .trim()
                .toUpperCase(),

        type,

        fine:

            Number(fine),

        date:
            new Date()
                .toLocaleDateString(),

        status: "Pending"

    };


    violations.unshift(
        violation
    );


    saveViolations();

    renderViolations();


    showToast(
        "Violation reported successfully."
    );

}


/* =====================================================
   DISPLAY VIOLATIONS
   ===================================================== */

function renderViolations() {

    const table =
        document.getElementById(
            "violationTable"
        );


    if (!table) return;


    table.innerHTML = "";


    if (violations.length === 0) {

        table.innerHTML = `

            <tr>

                <td colspan="5">

                    <div class="empty-state">
                        No violations recorded.
                    </div>

                </td>

            </tr>

        `;

    }


    violations.forEach(violation => {

        const row =
            document.createElement("tr");


        row.innerHTML = `

            <td>
                <strong>
                    ${violation.vehicleNumber}
                </strong>
            </td>

            <td>
                ${violation.type}
            </td>

            <td>
                ₹${violation.fine}
            </td>

            <td>
                ${violation.date}
            </td>

            <td>

                <span class="status-badge pending">
                    ${violation.status}
                </span>

            </td>

        `;


        table.appendChild(row);

    });


    updateViolationStats();

}


/* =====================================================
   STATISTICS
   ===================================================== */

function updateViolationStats() {

    const total =
        violations.length;


    const pending =
        violations.filter(
            violation =>
                violation.status === "Pending"
        );


    const resolved =
        violations.filter(
            violation =>
                violation.status === "Resolved"
        );


    const pendingFine =
        pending.reduce(
            (sum, violation) =>
                sum + Number(violation.fine),
            0
        );


    setText(
        "totalViolations",
        total
    );

    setText(
        "pendingFines",
        `₹${pendingFine}`
    );

    setText(
        "resolvedViolations",
        resolved.length
    );

}


/* =====================================================
   INITIALIZE
   ===================================================== */

document.addEventListener(
    "DOMContentLoaded",
    renderViolations
);