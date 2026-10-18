#pragma once
#include <string>
#include "Vehicle.h"

// A record created when a vehicle breaks a parking rule.
class Violation
{
private:
    Vehicle* vehicle;
    std::string violationType;
    double penalty;

public:
    Violation(Vehicle* v, std::string type, double amount);

    void displayViolation() const;
};
