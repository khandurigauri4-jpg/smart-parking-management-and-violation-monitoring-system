#pragma once
#include <string>
#include "Vehicle.h"

// A user (owner) has a vehicle.
class User
{
private:
    std::string name;
    Vehicle* vehicle;

public:
    User(std::string n, Vehicle* v);

    std::string getName() const;
    Vehicle* getVehicle() const;

    void displayUser() const;
};
