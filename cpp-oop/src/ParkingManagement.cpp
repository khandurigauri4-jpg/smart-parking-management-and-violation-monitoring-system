#include "../include/ParkingManagement.h"
#include <iostream>

using namespace std;

ParkingManagement::ParkingManagement(
    int twoWheelerSlots,
    int fourWheelerSlots
)
{
    totalTwoWheelerSlots = twoWheelerSlots;
    totalFourWheelerSlots = fourWheelerSlots;

    int slotNumber = 1;

    // Create two-wheeler slots
    for (int i = 0; i < totalTwoWheelerSlots; i++)
    {
        slots[i] = ParkingSlot(slotNumber, "Two Wheeler");
        slotOccupied[i] = false;
        slotNumber++;
    }

    // Create four-wheeler slots
    for (int i = 0; i < totalFourWheelerSlots; i++)
    {
        int index = totalTwoWheelerSlots + i;

        slots[index] = ParkingSlot(slotNumber, "Four Wheeler");
        slotOccupied[index] = false;
        slotNumber++;
    }
}


// Park a vehicle
void ParkingManagement::parkVehicle(Vehicle vehicle)
{
    string vehicleType = vehicle.getVehicleType();

    int startIndex = 0;
    int endIndex = totalTwoWheelerSlots;

    if (vehicleType == "Car")
    {
        startIndex = totalTwoWheelerSlots;
        endIndex = totalTwoWheelerSlots + totalFourWheelerSlots;
    }

    for (int i = startIndex; i < endIndex; i++)
    {
        if (!slotOccupied[i])
        {
            parkedVehicles[i] = vehicle;
            slotOccupied[i] = true;
            slots[i].setAvailability(false);

            cout << "Vehicle successfully parked." << endl;
            cout << "Vehicle Number: "
                 << vehicle.getVehicleNumber() << endl;
            cout << "Owner: "
                 << vehicle.getOwnerName() << endl;
            cout << "Slot Assigned: "
                 << slots[i].getSlotID() << endl;

            return;
        }
    }

    cout << "Sorry, no suitable parking slot is available." << endl;
}


// Remove a vehicle
void ParkingManagement::removeVehicle(string vehicleNumber)
{
    for (int i = 0; i < totalTwoWheelerSlots + totalFourWheelerSlots; i++)
    {
        if (slotOccupied[i] &&
            parkedVehicles[i].getVehicleNumber() == vehicleNumber)
        {
            slotOccupied[i] = false;
            slots[i].setAvailability(true);

            cout << "Vehicle exited successfully." << endl;
            cout << "Vehicle Number: "
                 << vehicleNumber << endl;
            cout << "Slot Released: "
                 << slots[i].getSlotID() << endl;

            return;
        }
    }

    cout << "Vehicle not found in parking." << endl;
}


// Display parking status
void ParkingManagement::displayParkingStatus()
{
    int totalSlots =
        totalTwoWheelerSlots + totalFourWheelerSlots;

    int availableTwoWheelers = 0;
    int availableFourWheelers = 0;

    for (int i = 0; i < totalTwoWheelerSlots; i++)
    {
        if (!slotOccupied[i])
        {
            availableTwoWheelers++;
        }
    }

    for (int i = totalTwoWheelerSlots; i < totalSlots; i++)
    {
        if (!slotOccupied[i])
        {
            availableFourWheelers++;
        }
    }

    cout << endl;
    cout << "----- Parking Availability -----" << endl;

    cout << "Two Wheeler: "
         << availableTwoWheelers
         << " / "
         << totalTwoWheelerSlots
         << " available" << endl;

    cout << "Four Wheeler: "
         << availableFourWheelers
         << " / "
         << totalFourWheelerSlots
         << " available" << endl;

    cout << endl;
}


// Existing project-related functions
void ParkingManagement::addUser(User user)
{
    cout << "User added: "
         << user.getUserName() << endl;
}

void ParkingManagement::addReservation(Reservation reservation)
{
    cout << "Reservation added: "
         << reservation.getReservationID() << endl;
}

void ParkingManagement::addTicket(Ticket ticket)
{
    cout << "Ticket added: "
         << ticket.getTicketID() << endl;
}

void ParkingManagement::addViolation(Violation violation)
{
    cout << "Violation added: "
         << violation.getViolationID() << endl;
}


void ParkingManagement::displayUser(User user)
{
    user.displayUser();
}

void ParkingManagement::displayReservation(Reservation reservation)
{
    reservation.displayReservation();
}

void ParkingManagement::displayTicket(Ticket ticket)
{
    ticket.displayTicket();
}

void ParkingManagement::displayViolation(Violation violation)
{
    violation.displayViolation();
}