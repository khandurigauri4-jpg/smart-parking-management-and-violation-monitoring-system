#ifndef TICKET_H
#define TICKET_H

#include <string>

using namespace std;

class Ticket
{
private:
    int ticketID;
    string vehicleNumber;
    int slotID;

    string entryTime;
    string exitTime;

    int allowedHours;
    int actualHours;

    double fee;

    string status;

public:
    // Constructors
    Ticket();

    Ticket(
        int id,
        string vehicle,
        int slot
    );

    // Setters
    void setTicketID(int id);
    void setVehicleNumber(string vehicle);
    void setSlotID(int slot);

    void setEntryTime(string time);
    void setExitTime(string time);

    void setAllowedHours(int hours);
    void setActualHours(int hours);

    void setFee(double amount);

    // Getters
    int getTicketID();
    string getVehicleNumber();
    int getSlotID();

    string getEntryTime();
    string getExitTime();

    int getAllowedHours();
    int getActualHours();

    double getFee();
    string getStatus();

    // Parking fee calculation
    double calculateFee(int hours);

    // Function overloading
    double calculateFee(
        int hours,
        double extraCharge
    );

    // Ticket lifecycle
    void activateTicket();
    void completeTicket();

    // Display ticket
    void displayTicket();
};

#endif