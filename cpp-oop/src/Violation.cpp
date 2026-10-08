#include "../include/Violation.h"
#include <iostream>

using namespace std;


// Default constructor
Violation::Violation()
{
    violationID = 0;
    vehicleNumber = "";
    violationType = "";

    fineAmount = 0.0;

    violationScore = 0;
    blacklisted = false;
}


// Parameterized constructor
Violation::Violation(
    int id,
    string vehicle,
    string type,
    double fine
)
{
    violationID = id;
    vehicleNumber = vehicle;
    violationType = type;

    fineAmount = fine;

    violationScore = 0;
    blacklisted = false;
}


// Setters
void Violation::setViolationID(int id)
{
    violationID = id;
}

void Violation::setVehicleNumber(string vehicle)
{
    vehicleNumber = vehicle;
}

void Violation::setViolationType(string type)
{
    violationType = type;
}

void Violation::setFineAmount(double fine)
{
    fineAmount = fine;
}


// Getters
int Violation::getViolationID()
{
    return violationID;
}

string Violation::getVehicleNumber()
{
    return vehicleNumber;
}

string Violation::getViolationType()
{
    return violationType;
}

double Violation::getFineAmount()
{
    return fineAmount;
}

int Violation::getViolationScore()
{
    return violationScore;
}

bool Violation::isBlacklisted()
{
    return blacklisted;
}


// Add violation score
void Violation::addViolationScore(int points)
{
    violationScore += points;

    if (violationScore >= 10)
    {
        blacklisted = true;
    }
}


// Blacklist vehicle
void Violation::blacklistVehicle()
{
    blacklisted = true;
}


// Display violation
void Violation::displayViolation()
{
    cout << endl;
    cout << "----- Violation Details -----"
         << endl;

    cout << "Violation ID: "
         << violationID
         << endl;

    cout << "Vehicle Number: "
         << vehicleNumber
         << endl;

    cout << "Violation Type: "
         << violationType
         << endl;

    cout << "Fine Amount: Rs. "
         << fineAmount
         << endl;

    cout << "Violation Score: "
         << violationScore
         << endl;

    if (blacklisted)
    {
        cout << "Blacklist Status: Blacklisted"
             << endl;
    }
    else
    {
        cout << "Blacklist Status: Not Blacklisted"
             << endl;
    }
}