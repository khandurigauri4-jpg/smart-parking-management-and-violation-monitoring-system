#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

using namespace std;

class Reservation
{
private:
    int reservationID;
    string vehicleNumber;
    int slotID;
    string reservationDate;
    string status;

public:
    // Constructors
    Reservation();
    Reservation(
        int id,
        string vehicle,
        int slot,
        string date
    );

    // Setters
    void setReservationID(int id);
    void setVehicleNumber(string vehicle);
    void setSlotID(int slot);
    void setReservationDate(string date);

    // Getters
    int getReservationID();
    string getVehicleNumber();
    int getSlotID();
    string getReservationDate();
    string getStatus();

    // Reservation behaviour
    void confirmReservation();
    void completeReservation();
    void cancelReservation();

    // Display
    void displayReservation();
};

#endif