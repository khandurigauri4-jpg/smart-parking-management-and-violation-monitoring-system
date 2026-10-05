#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include "Vehicle.h"
#include "ParkingSlot.h"

using namespace std;

class Reservation
{
private:
    int reservationID;
    Vehicle vehicle;
    ParkingSlot slot;
    string dateTime;
    string status;

public:
    Reservation(int id, Vehicle v, ParkingSlot s, string date, string reservationStatus);

    void setReservationID(int id);
    void setVehicle(Vehicle v);
    void setSlot(ParkingSlot s);
    void setDateTime(string date);
    void setStatus(string reservationStatus);

    int getReservationID();
    Vehicle getVehicle();
    ParkingSlot getSlot();
    string getDateTime();
    string getStatus();

    void displayReservation();
};

#endif