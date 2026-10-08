#include "../include/Ticket.h"
#include <iostream>

using namespace std;


// Default constructor
Ticket::Ticket()
{
    ticketID = 0;
    vehicleNumber = "";
    slotID = 0;

    entryTime = "";
    exitTime = "";

    allowedHours = 0;
    actualHours = 0;

    fee = 0.0;

    status = "Created";
}


// Parameterized constructor
Ticket::Ticket(
    int id,
    string vehicle,
    int slot
)
{
    ticketID = id;
    vehicleNumber = vehicle;
    slotID = slot;

    entryTime = "";
    exitTime = "";

    allowedHours = 0;
    actualHours = 0;

    fee = 0.0;

    status = "Created";
}


// Setters
void Ticket::setTicketID(int id)
{
    ticketID = id;
}

void Ticket::setVehicleNumber(string vehicle)
{
    vehicleNumber = vehicle;
}

void Ticket::setSlotID(int slot)
{
    slotID = slot;
}

void Ticket::setEntryTime(string time)
{
    entryTime = time;
}

void Ticket::setExitTime(string time)
{
    exitTime = time;
}

void Ticket::setAllowedHours(int hours)
{
    allowedHours = hours;
}

void Ticket::setActualHours(int hours)
{
    actualHours = hours;
}

void Ticket::setFee(double amount)
{
    fee = amount;
}


// Getters
int Ticket::getTicketID()
{
    return ticketID;
}

string Ticket::getVehicleNumber()
{
    return vehicleNumber;
}

int Ticket::getSlotID()
{
    return slotID;
}

string Ticket::getEntryTime()
{
    return entryTime;
}

string Ticket::getExitTime()
{
    return exitTime;
}

int Ticket::getAllowedHours()
{
    return allowedHours;
}

int Ticket::getActualHours()
{
    return actualHours;
}

double Ticket::getFee()
{
    return fee;
}

string Ticket::getStatus()
{
    return status;
}


// Calculate parking fee
// Basic parking rate: Rs. 20 per hour
double Ticket::calculateFee(int hours)
{
    fee = hours * 20.0;

    return fee;
}


// Calculate parking fee with extra charge
// This demonstrates function overloading
double Ticket::calculateFee(
    int hours,
    double extraCharge
)
{
    fee = (hours * 20.0) + extraCharge;

    return fee;
}


// Activate ticket
void Ticket::activateTicket()
{
    status = "Active";
}


// Complete ticket
void Ticket::completeTicket()
{
    status = "Completed";
}


// Display ticket
void Ticket::displayTicket()
{
    cout << "Ticket ID: "
         << ticketID
         << endl;

    cout << "Vehicle Number: "
         << vehicleNumber
         << endl;

    cout << "Slot ID: "
         << slotID
         << endl;

    cout << "Entry Time: "
         << entryTime
         << endl;

    cout << "Exit Time: "
         << exitTime
         << endl;

    cout << "Allowed Parking Hours: "
         << allowedHours
         << endl;

    cout << "Actual Parking Hours: "
         << actualHours
         << endl;

    cout << "Parking Fee: Rs. "
         << fee
         << endl;

    cout << "Status: "
         << status
         << endl;
}