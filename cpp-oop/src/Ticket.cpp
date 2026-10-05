#include "../include/Ticket.h"
#include <iostream>

using namespace std;

Ticket::Ticket(int id, Reservation r, string entry, string exit, double fee, string ticketStatus)
    : reservation(r)
{
    ticketID = id;
    entryTime = entry;
    exitTime = exit;
    parkingFee = fee;
    status = ticketStatus;
}

void Ticket::setTicketID(int id)
{
    ticketID = id;
}

void Ticket::setReservation(Reservation r)
{
    reservation = r;
}

void Ticket::setEntryTime(string entry)
{
    entryTime = entry;
}

void Ticket::setExitTime(string exit)
{
    exitTime = exit;
}

void Ticket::setParkingFee(double fee)
{
    parkingFee = fee;
}

void Ticket::setStatus(string ticketStatus)
{
    status = ticketStatus;
}

int Ticket::getTicketID()
{
    return ticketID;
}

Reservation Ticket::getReservation()
{
    return reservation;
}

string Ticket::getEntryTime()
{
    return entryTime;
}

string Ticket::getExitTime()
{
    return exitTime;
}

double Ticket::getParkingFee()
{
    return parkingFee;
}

string Ticket::getStatus()
{
    return status;
}


// Function overloading - normal parking fee
double Ticket::calculateFee(int hours)
{
    return hours * 50;
}


// Function overloading - parking fee with extra charge
double Ticket::calculateFee(int hours, double extraCharge)
{
    return (hours * 50) + extraCharge;
}


void Ticket::displayTicket()
{
    cout << "Ticket ID: " << ticketID << endl;
    cout << "Entry Time: " << entryTime << endl;
    cout << "Exit Time: " << exitTime << endl;
    cout << "Parking Fee: Rs. " << parkingFee << endl;
    cout << "Status: " << status << endl;

    cout << "Reservation ID: "
         << reservation.getReservationID() << endl;
}