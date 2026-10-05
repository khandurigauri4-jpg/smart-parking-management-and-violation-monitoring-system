#include "../include/Violation.h"
#include <iostream>

using namespace std;

Violation::Violation(int id, Vehicle v, string type, double fine, string date, string violationStatus)
    : vehicle(v)
{
    violationID = id;
    violationType = type;
    fineAmount = fine;
    dateTime = date;
    status = violationStatus;
}

void Violation::setViolationID(int id)
{
    violationID = id;
}

void Violation::setVehicle(Vehicle v)
{
    vehicle = v;
}

void Violation::setViolationType(string type)
{
    violationType = type;
}

void Violation::setFineAmount(double fine)
{
    fineAmount = fine;
}

void Violation::setDateTime(string date)
{
    dateTime = date;
}

void Violation::setStatus(string violationStatus)
{
    status = violationStatus;
}

int Violation::getViolationID()
{
    return violationID;
}

Vehicle Violation::getVehicle()
{
    return vehicle;
}

string Violation::getViolationType()
{
    return violationType;
}

double Violation::getFineAmount()
{
    return fineAmount;
}

string Violation::getDateTime()
{
    return dateTime;
}

string Violation::getStatus()
{
    return status;
}

void Violation::displayViolation()
{
    cout << "Violation ID: " << violationID << endl;
    cout << "Vehicle Number: "
         << vehicle.getVehicleNumber() << endl;

    cout << "Violation Type: " << violationType << endl;
    cout << "Fine Amount: Rs. " << fineAmount << endl;
    cout << "Date/Time: " << dateTime << endl;
    cout << "Status: " << status << endl;
}