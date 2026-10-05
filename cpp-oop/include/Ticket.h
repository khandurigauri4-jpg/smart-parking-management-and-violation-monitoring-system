#ifndef TICKET_H
#define TICKET_H

#include <string>
#include "Reservation.h"

using namespace std;

class Ticket
{
private:
    int ticketID;
    Reservation reservation;
    string entryTime;
    string exitTime;
    double parkingFee;
    string status;

public:
    Ticket(int id, Reservation r, string entry, string exit, double fee, string ticketStatus);

    void setTicketID(int id);
    void setReservation(Reservation r);
    void setEntryTime(string entry);
    void setExitTime(string exit);
    void setParkingFee(double fee);
    void setStatus(string ticketStatus);

    int getTicketID();
    Reservation getReservation();
    string getEntryTime();
    string getExitTime();
    double getParkingFee();
    string getStatus();

    // Function overloading
    double calculateFee(int hours);
    double calculateFee(int hours, double extraCharge);

    void displayTicket();
};

#endif