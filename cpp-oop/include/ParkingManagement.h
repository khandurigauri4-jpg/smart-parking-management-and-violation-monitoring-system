#ifndef PARKING_MANAGEMENT_H
#define PARKING_MANAGEMENT_H

#include "Vehicle.h"
#include "ParkingSlot.h"
#include "User.h"
#include "Reservation.h"
#include "Ticket.h"
#include "Violation.h"

using namespace std;

class ParkingManagement
{
private:
    static const int MAX_SLOTS = 50;

    ParkingSlot slots[MAX_SLOTS];
    Vehicle parkedVehicles[MAX_SLOTS];

    int totalTwoWheelerSlots;
    int totalFourWheelerSlots;

    bool slotOccupied[MAX_SLOTS];

public:
    ParkingManagement(int twoWheelerSlots, int fourWheelerSlots);

    void parkVehicle(Vehicle vehicle);
    void removeVehicle(string vehicleNumber);

    void displayParkingStatus();

    void addUser(User user);
    void addReservation(Reservation reservation);
    void addTicket(Ticket ticket);
    void addViolation(Violation violation);

    void displayUser(User user);
    void displayReservation(Reservation reservation);
    void displayTicket(Ticket ticket);
    void displayViolation(Violation violation);
};

#endif