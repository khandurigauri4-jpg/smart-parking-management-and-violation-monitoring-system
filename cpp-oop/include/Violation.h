#ifndef VIOLATION_H
#define VIOLATION_H

#include <string>
#include "Vehicle.h"

using namespace std;

class Violation
{
private:
    int violationID;
    Vehicle vehicle;
    string violationType;
    double fineAmount;
    string dateTime;
    string status;

public:
    Violation(int id, Vehicle v, string type, double fine, string date, string violationStatus);

    void setViolationID(int id);
    void setVehicle(Vehicle v);
    void setViolationType(string type);
    void setFineAmount(double fine);
    void setDateTime(string date);
    void setStatus(string violationStatus);

    int getViolationID();
    Vehicle getVehicle();
    string getViolationType();
    double getFineAmount();
    string getDateTime();
    string getStatus();

    void displayViolation();
};

#endif