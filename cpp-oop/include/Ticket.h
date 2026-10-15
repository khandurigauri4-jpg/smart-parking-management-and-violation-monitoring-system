#pragma once
#include <string>
#include "User.h"
#include "ParkingSlot.h"

// A parking ticket links a user (and their vehicle) to a parking slot.
// Times are stored as minutes after midnight, e.g. 10:30 -> 630.
class Ticket
{
private:
    std::string ticketID;
    User* user;
    ParkingSlot* slot;

    int entryTime;
    int exitTime;          // -1 means the vehicle has not left yet

    int allowedMinutes;
    int actualMinutes;
    int overstayMinutes;

    double parkingFee;
    double penalty;

    int convertTime(std::string time) const;
    std::string formatTime(int minutes) const;

    // Function overloading
    double calculateFee(int minutes) const;
    double calculateFee(int minutes, double ratePerBlock) const;

public:
    Ticket(std::string id, User* u, ParkingSlot* s, std::string entry, int allowedHours);

    void exitParking(std::string exit);

    bool hasOverstay() const;
    double getPenalty() const;

    void displayTicket() const;
};
