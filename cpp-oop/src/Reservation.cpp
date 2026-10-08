#include "../include/Reservation.h"
#include <iostream>

using namespace std;


// Default constructor
Reservation::Reservation()
{
    reservationID = 0;
    vehicleNumber = "";
    slotID = 0;
    reservationDate = "";
    status = "Pending";
}


// Parameterized constructor
Reservation::Reservation(
    int id,
    string vehicle,
    int slot,
    string date
)
{
    reservationID = id;
    vehicleNumber = vehicle;
    slotID = slot;
    reservationDate = date;
    status = "Pending";
}


// Setters
void Reservation::setReservationID(int id)
{
    reservationID = id;
}

void Reservation::setVehicleNumber(string vehicle)
{
    vehicleNumber = vehicle;
}

void Reservation::setSlotID(int slot)
{
    slotID = slot;
}

void Reservation::setReservationDate(string date)
{
    reservationDate = date;
}


// Getters
int Reservation::getReservationID()
{
    return reservationID;
}

string Reservation::getVehicleNumber()
{
    return vehicleNumber;
}

int Reservation::getSlotID()
{
    return slotID;
}

string Reservation::getReservationDate()
{
    return reservationDate;
}

string Reservation::getStatus()
{
    return status;
}


// Confirm reservation
void Reservation::confirmReservation()
{
    status = "Confirmed";
}


// Complete reservation
void Reservation::completeReservation()
{
    status = "Completed";
}


// Cancel reservation
void Reservation::cancelReservation()
{
    status = "Cancelled";
}


// Display reservation
void Reservation::displayReservation()
{
    cout << "Reservation ID: " << reservationID << endl;
    cout << "Vehicle Number: " << vehicleNumber << endl;
    cout << "Slot ID: " << slotID << endl;
    cout << "Reservation Date: " << reservationDate << endl;
    cout << "Status: " << status << endl;
}