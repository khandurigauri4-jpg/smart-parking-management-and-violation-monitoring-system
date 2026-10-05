#include "../include/Reservation.h"
#include <iostream>

using namespace std;

Reservation::Reservation(int id, Vehicle v, ParkingSlot s, string date, string reservationStatus)
    : vehicle(v), slot(s)
{
    reservationID = id;
    dateTime = date;
    status = reservationStatus;
}

void Reservation::setReservationID(int id)
{
    reservationID = id;
}

void Reservation::setVehicle(Vehicle v)
{
    vehicle = v;
}

void Reservation::setSlot(ParkingSlot s)
{
    slot = s;
}

void Reservation::setDateTime(string date)
{
    dateTime = date;
}

void Reservation::setStatus(string reservationStatus)
{
    status = reservationStatus;
}

int Reservation::getReservationID()
{
    return reservationID;
}

Vehicle Reservation::getVehicle()
{
    return vehicle;
}

ParkingSlot Reservation::getSlot()
{
    return slot;
}

string Reservation::getDateTime()
{
    return dateTime;
}

string Reservation::getStatus()
{
    return status;
}

void Reservation::displayReservation()
{
    cout << "Reservation ID: " << reservationID << endl;
    cout << "Date/Time: " << dateTime << endl;
    cout << "Status: " << status << endl;

    cout << "Vehicle: "
         << vehicle.getVehicleNumber() << endl;

    cout << "Parking Slot: "
         << slot.getSlotID() << endl;
}