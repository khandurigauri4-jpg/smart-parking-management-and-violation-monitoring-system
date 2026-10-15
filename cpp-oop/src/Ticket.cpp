#include "../include/Ticket.h"
#include <iostream>
using namespace std;

const int BLOCK_MINUTES = 30;   // time is charged in 30-minute blocks

Ticket::Ticket(string id, User* u, ParkingSlot* s, string entry, int allowedHours)
{
    ticketID = id;
    user = u;
    slot = s;

    entryTime = convertTime(entry);
    exitTime = -1;

    allowedMinutes = allowedHours * 60;
    actualMinutes = 0;
    overstayMinutes = 0;

    parkingFee = 0;
    penalty = 0;

    slot->occupySlot();
}

// "HH:MM" -> minutes after midnight, e.g. "10:30" -> 630
int Ticket::convertTime(string time) const
{
    int hours = stoi(time.substr(0, 2));
    int minutes = stoi(time.substr(3, 2));

    return hours * 60 + minutes;
}

// Minutes after midnight -> "HH:MM", e.g. 630 -> "10:30"
string Ticket::formatTime(int minutes) const
{
    int hours = (minutes / 60) % 24;
    int mins = minutes % 60;

    string result = "";

    if (hours < 10)
        result += "0";
    result += to_string(hours) + ":";

    if (mins < 10)
        result += "0";
    result += to_string(mins);

    return result;
}

// Every started block is charged in full: 1-30 min = 1 block, 31-60 min = 2 blocks
double Ticket::calculateFee(int minutes, double ratePerBlock) const
{
    int blocks = minutes / BLOCK_MINUTES;

    if (minutes % BLOCK_MINUTES != 0)
        blocks++;

    return blocks * ratePerBlock;
}

// Uses this vehicle's own rate, with a minimum charge of one block
double Ticket::calculateFee(int minutes) const
{
    if (minutes < BLOCK_MINUTES)
        minutes = BLOCK_MINUTES;

    return calculateFee(minutes, user->getVehicle()->getRatePerBlock());
}

void Ticket::exitParking(string exit)
{
    exitTime = convertTime(exit);

    // Exit earlier than entry means the vehicle left the next day
    if (exitTime < entryTime)
        exitTime += 24 * 60;

    actualMinutes = exitTime - entryTime;
    parkingFee = calculateFee(actualMinutes);

    if (actualMinutes > allowedMinutes)
    {
        // Overstay time is charged again as a penalty, so it costs double
        overstayMinutes = actualMinutes - allowedMinutes;
        penalty = calculateFee(overstayMinutes);
    }
    else
    {
        overstayMinutes = 0;
        penalty = 0;
    }

    slot->releaseSlot();
}

bool Ticket::hasOverstay() const
{
    return overstayMinutes > 0;
}

double Ticket::getPenalty() const
{
    return penalty;
}

void Ticket::displayTicket() const
{
    Vehicle* vehicle = user->getVehicle();

    cout << "\n========== PARKING TICKET ==========\n";

    cout << "Ticket ID: " << ticketID << endl;
    cout << "Vehicle Number: " << vehicle->getVehicleNumber() << endl;
    cout << "Vehicle: " << vehicle->getVehicleName()
         << " (" << vehicle->getVehicleCategory() << ")" << endl;
    cout << "Owner Name: " << user->getName() << endl;
    cout << "Slot ID: " << slot->getSlotID() << endl;
    cout << "Parking Rate: Rs. " << vehicle->getRatePerBlock() << " per 30 minutes" << endl;
    cout << "Entry Time: " << formatTime(entryTime) << endl;

    int allowedHours = allowedMinutes / 60;
    cout << "Allowed Duration: " << allowedHours
         << (allowedHours == 1 ? " hour" : " hours") << endl;
    cout << "Expected Exit Time: " << formatTime(entryTime + allowedMinutes) << endl;

    if (exitTime == -1)
    {
        cout << "Status: ACTIVE" << endl;
    }
    else
    {
        cout << "Actual Exit Time: " << formatTime(exitTime) << endl;

        int hours = actualMinutes / 60;
        cout << "Actual Duration: " << hours
             << (hours == 1 ? " hour " : " hours ")
             << actualMinutes % 60 << " minutes" << endl;

        cout << "Parking Fee: Rs. " << parkingFee << endl;
        cout << "Overstay Penalty: Rs. " << penalty << endl;
        cout << "Total Amount: Rs. " << parkingFee + penalty << endl;

        if (hasOverstay())
            cout << "Status: OVERSTAY - VIOLATION" << endl;
        else
            cout << "Status: COMPLETED" << endl;
    }

    cout << "====================================\n";
}
