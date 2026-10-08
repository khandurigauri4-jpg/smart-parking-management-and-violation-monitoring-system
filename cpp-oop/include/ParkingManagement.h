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
    Ticket parkingTickets[MAX_SLOTS];
    Violation violations[MAX_SLOTS];

    string violationVehicles[MAX_SLOTS];
    int vehicleViolationScores[MAX_SLOTS];
    bool blacklistedVehicles[MAX_SLOTS];

    int totalSlots;
    int violationCount;
    int trackedVehicleCount;

    bool slotOccupied[MAX_SLOTS];
    bool ticketActive[MAX_SLOTS];

public:
    ParkingManagement(int numberOfSlots);

    // Vehicle entry
    void processVehicleEntry(
        Vehicle vehicle,
        int allocatedSlotID,
        int allowedHours,
        string entryTime
    );

    // Vehicle exit
    void processVehicleExit(
        string vehicleNumber,
        int actualHours,
        string exitTime
    );

    // Parking status
    void displayParkingStatus();

    // Ticket management
    void displayTicket(string vehicleNumber);

    // Violation management
    void displayVehicleViolation(string vehicleNumber);

    // Check blacklist
    bool isVehicleBlacklisted(string vehicleNumber);

    // Existing project-related functions
    void addUser(User user);
    void addReservation(Reservation reservation);
    void addTicket(Ticket ticket);
    void addViolation(Violation violation);

    void displayUser(User user);
    void displayReservation(Reservation reservation);
    void displayTicketDetails(Ticket ticket);
    void displayViolation(Violation violation);
};

#endif