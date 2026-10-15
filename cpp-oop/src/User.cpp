#include "../include/User.h"
#include <iostream>
using namespace std;

User::User(string n, Vehicle* v)
{
    name = n;
    vehicle = v;
}

string User::getName() const
{
    return name;
}

Vehicle* User::getVehicle() const
{
    return vehicle;
}

void User::displayUser() const
{
    cout << "Owner Name: " << name << endl;
    cout << "Vehicle Number: " << vehicle->getVehicleNumber() << endl;
}
