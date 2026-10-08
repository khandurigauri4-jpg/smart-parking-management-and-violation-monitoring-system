#include "../include/ParkingManagement.h"
#include <iostream>

using namespace std;


// Constructor
ParkingManagement::ParkingManagement(int numberOfSlots)
{
    totalSlots = numberOfSlots;

    violationCount = 0;
    trackedVehicleCount = 0;

    for (int i = 0; i < totalSlots; i++)
    {
        slots[i] = ParkingSlot(
            i + 1,
            "Parking Slot"
        );

        slotOccupied[i] = false;
        ticketActive[i] = false;
    }

    for (int i = 0; i < MAX_SLOTS; i++)
    {
        violationVehicles[i] = "";
        vehicleViolationScores[i] = 0;
        blacklistedVehicles[i] = false;
    }
}


// Check whether vehicle is blacklisted
bool ParkingManagement::isVehicleBlacklisted(
    string vehicleNumber
)
{
    for (int i = 0; i < trackedVehicleCount; i++)
    {
        if (violationVehicles[i] == vehicleNumber)
        {
            return blacklistedVehicles[i];
        }
    }

    return false;
}


// Process vehicle entry
void ParkingManagement::processVehicleEntry(
    Vehicle vehicle,
    int allocatedSlotID,
    int allowedHours,
    string entryTime
)
{
    // Check blacklist before allowing entry
    if (isVehicleBlacklisted(
            vehicle.getVehicleNumber()
        ))
    {
        cout << endl;
        cout << "Vehicle is blacklisted."
             << endl;

        cout << "Parking entry denied."
             << endl;

        return;
    }


    int index = allocatedSlotID - 1;

    if (index < 0 || index >= totalSlots)
    {
        cout << "Invalid parking slot."
             << endl;

        return;
    }


    if (slotOccupied[index])
    {
        cout << "The allocated parking slot is already occupied."
             << endl;

        return;
    }


    // Store vehicle
    parkedVehicles[index] = vehicle;


    // Mark slot occupied
    slotOccupied[index] = true;
    slots[index].setAvailability(false);


    // Create ticket
    parkingTickets[index] = Ticket(
        index + 1,
        vehicle.getVehicleNumber(),
        allocatedSlotID
    );


    parkingTickets[index].setEntryTime(entryTime);
    parkingTickets[index].setAllowedHours(allowedHours);

    parkingTickets[index].activateTicket();

    ticketActive[index] = true;


    cout << endl;
    cout << "Vehicle entry processed successfully."
         << endl;

    cout << "Vehicle Number: "
         << vehicle.getVehicleNumber()
         << endl;

    cout << "Owner: "
         << vehicle.getOwnerName()
         << endl;

    cout << "Vehicle Type: "
         << vehicle.getVehicleType()
         << endl;

    cout << "Allocated Slot: "
         << allocatedSlotID
         << endl;

    cout << "Ticket ID: "
         << parkingTickets[index].getTicketID()
         << endl;

    cout << "Allowed Parking Time: "
         << allowedHours
         << " hour(s)"
         << endl;

    cout << "Ticket Status: Active"
         << endl;
}


// Process vehicle exit
void ParkingManagement::processVehicleExit(
    string vehicleNumber,
    int actualHours,
    string exitTime
)
{
    for (int i = 0; i < totalSlots; i++)
    {
        if (
            slotOccupied[i] &&
            parkedVehicles[i].getVehicleNumber()
                == vehicleNumber
        )
        {
            parkingTickets[i].setActualHours(actualHours);
            parkingTickets[i].setExitTime(exitTime);


            int allowedHours =
                parkingTickets[i].getAllowedHours();


            double fee = 0.0;


            // Normal parking
            if (actualHours <= allowedHours)
            {
                fee =
                    parkingTickets[i].calculateFee(
                        actualHours
                    );

                cout << endl;
                cout << "Vehicle exited within allowed time."
                     << endl;
            }


            // Overstay
            else
            {
                int extraHours =
                    actualHours - allowedHours;

                double penalty =
                    extraHours * 20.0;


                fee =
                    parkingTickets[i].calculateFee(
                        actualHours,
                        penalty
                    );


                cout << endl;
                cout << "Vehicle overstayed."
                     << endl;

                cout << "Extra Hours: "
                     << extraHours
                     << endl;

                cout << "Penalty: Rs. "
                     << penalty
                     << endl;


                // -----------------------------
                // Find existing violation record
                // -----------------------------

                int vehicleIndex = -1;

                for (int j = 0;
                     j < trackedVehicleCount;
                     j++)
                {
                    if (
                        violationVehicles[j]
                            == vehicleNumber
                    )
                    {
                        vehicleIndex = j;
                        break;
                    }
                }


                // First violation for vehicle
                if (vehicleIndex == -1)
                {
                    vehicleIndex =
                        trackedVehicleCount;

                    violationVehicles[
                        vehicleIndex
                    ] = vehicleNumber;

                    vehicleViolationScores[
                        vehicleIndex
                    ] = 0;

                    blacklistedVehicles[
                        vehicleIndex
                    ] = false;

                    trackedVehicleCount++;
                }


                // Add 3 points
                vehicleViolationScores[
                    vehicleIndex
                ] += 3;


                // Create violation object
                Violation newViolation(
                    violationCount + 1,
                    vehicleNumber,
                    "Overstay",
                    penalty
                );


                newViolation.addViolationScore(3);


                violations[
                    violationCount
                ] = newViolation;

                violationCount++;


                cout << "Violation recorded."
                     << endl;

                cout << "Violation Score Added: 3"
                     << endl;

                cout << "Total Violation Score: "
                     << vehicleViolationScores[
                            vehicleIndex
                        ]
                     << endl;


                // Blacklist at 10 points
                if (
                    vehicleViolationScores[
                        vehicleIndex
                    ] >= 10
                )
                {
                    blacklistedVehicles[
                        vehicleIndex
                    ] = true;

                    cout << "Vehicle has been BLACKLISTED."
                         << endl;
                }
                else
                {
                    cout << "Vehicle is not blacklisted."
                         << endl;
                }
            }


            // Complete ticket
            parkingTickets[i].completeTicket();

            ticketActive[i] = false;


            // Release slot
            slotOccupied[i] = false;
            slots[i].setAvailability(true);


            cout << endl;
            cout << "Vehicle exit processed successfully."
                 << endl;

            cout << "Vehicle Number: "
                 << vehicleNumber
                 << endl;

            cout << "Slot Released: "
                 << slots[i].getSlotID()
                 << endl;

            cout << "Total Parking Fee: Rs. "
                 << fee
                 << endl;

            cout << "Ticket Status: Completed"
                 << endl;

            return;
        }
    }


    cout << "Vehicle not found in parking."
         << endl;
}


// Display parking status
void ParkingManagement::displayParkingStatus()
{
    int availableSlots = 0;
    int occupiedSlots = 0;


    for (int i = 0; i < totalSlots; i++)
    {
        if (slotOccupied[i])
        {
            occupiedSlots++;
        }
        else
        {
            availableSlots++;
        }
    }


    cout << endl;
    cout << "----- Parking Status -----"
         << endl;

    cout << "Total Slots: "
         << totalSlots
         << endl;

    cout << "Occupied Slots: "
         << occupiedSlots
         << endl;

    cout << "Available Slots: "
         << availableSlots
         << endl;

    cout << endl;
}


// Display ticket for a vehicle
void ParkingManagement::displayTicket(
    string vehicleNumber
)
{
    for (int i = 0; i < totalSlots; i++)
    {
        if (
            parkedVehicles[i].getVehicleNumber()
                == vehicleNumber
        )
        {
            parkingTickets[i].displayTicket();

            return;
        }
    }


    cout << "No active ticket found."
         << endl;
}


// Display violation for a vehicle
void ParkingManagement::displayVehicleViolation(
    string vehicleNumber
)
{
    bool found = false;


    for (int i = 0; i < violationCount; i++)
    {
        if (
            violations[i].getVehicleNumber()
                == vehicleNumber
        )
        {
            violations[i].displayViolation();

            found = true;
        }
    }


    if (!found)
    {
        cout << "No violation found for this vehicle."
             << endl;
    }
}


// Add user
void ParkingManagement::addUser(User user)
{
    cout << "User added: "
         << user.getUserName()
         << endl;
}


// Add reservation
void ParkingManagement::addReservation(
    Reservation reservation
)
{
    cout << "Reservation added: "
         << reservation.getReservationID()
         << endl;
}


// Add ticket
void ParkingManagement::addTicket(Ticket ticket)
{
    cout << "Ticket added: "
         << ticket.getTicketID()
         << endl;
}


// Add violation
void ParkingManagement::addViolation(
    Violation violation
)
{
    cout << "Violation added: "
         << violation.getViolationID()
         << endl;
}


// Display user
void ParkingManagement::displayUser(User user)
{
    user.displayUser();
}


// Display reservation
void ParkingManagement::displayReservation(
    Reservation reservation
)
{
    reservation.displayReservation();
}


// Display ticket details
void ParkingManagement::displayTicketDetails(
    Ticket ticket
)
{
    ticket.displayTicket();
}


// Display violation
void ParkingManagement::displayViolation(
    Violation violation
)
{
    violation.displayViolation();
}