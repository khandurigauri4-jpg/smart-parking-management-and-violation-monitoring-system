#ifndef VIOLATION_H
#define VIOLATION_H

#include <string>

using namespace std;

class Violation
{
private:
    int violationID;
    string vehicleNumber;
    string violationType;

    double fineAmount;

    int violationScore;
    bool blacklisted;

public:
    // Constructors
    Violation();

    Violation(
        int id,
        string vehicle,
        string type,
        double fine
    );

    // Setters
    void setViolationID(int id);
    void setVehicleNumber(string vehicle);
    void setViolationType(string type);
    void setFineAmount(double fine);

    // Getters
    int getViolationID();
    string getVehicleNumber();
    string getViolationType();
    double getFineAmount();

    int getViolationScore();
    bool isBlacklisted();

    // Violation scoring
    void addViolationScore(int points);

    // Blacklisting
    void blacklistVehicle();

    // Display
    void displayViolation();
};

#endif