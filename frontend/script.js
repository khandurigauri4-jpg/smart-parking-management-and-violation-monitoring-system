function showParking()
{
    document.getElementById("parking").scrollIntoView();
}


function parkVehicle()
{
    let vehicleNumber =
        document.getElementById("vehicleNumber").value;

    if (vehicleNumber === "")
    {
        showMessage("Please enter vehicle number.");
        return;
    }


    for (let i = 1; i <= 10; i++)
    {
        let slot =
            document.getElementById("slot" + i);

        if (slot.classList.contains("available"))
        {
            slot.classList.remove("available");

            slot.classList.add("occupied");

            slot.querySelector("p").innerText =
                vehicleNumber;

            showMessage(
                vehicleNumber +
                " parked in Slot " +
                i
            );

            document.getElementById(
                "vehicleNumber"
            ).value = "";

            return;
        }
    }


    showMessage("Sorry! No parking slot available.");
}


function releaseVehicle()
{
    let slotNumber =
        document.getElementById("slotNumber").value;


    if (slotNumber === "")
    {
        showMessage("Please enter slot number.");
        return;
    }


    let slot =
        document.getElementById(
            "slot" + slotNumber
        );


    if (!slot)
    {
        showMessage("Invalid slot number.");
        return;
    }


    if (slot.classList.contains("available"))
    {
        showMessage("This slot is already available.");
        return;
    }


    slot.classList.remove("occupied");

    slot.classList.add("available");

    slot.querySelector("p").innerText =
        "Available";


    showMessage(
        "Slot " + slotNumber + " released."
    );


    document.getElementById(
        "slotNumber"
    ).value = "";
}


function showMessage(message)
{
    let messageBox =
        document.getElementById("message");


    messageBox.innerText = message;

    messageBox.style.display = "block";


    setTimeout(function()
    {
        messageBox.style.display = "none";
    }, 2500);
}