#include "../include/Vehicle.h"
#include <iostream>
using namespace std;

Vehicle::Vehicle(string number, string category, string name)
{
    vehicleNumber = number;
    vehicleCategory = category;
    vehicleName = name;
}

string Vehicle::getVehicleNumber() const
{
    return vehicleNumber;
}

string Vehicle::getVehicleCategory() const
{
    return vehicleCategory;
}

string Vehicle::getVehicleName() const
{
    return vehicleName;
}

void Vehicle::displayDetails() const
{
    cout << "Vehicle Number: " << vehicleNumber << endl;
    cout << "Vehicle Category: " << vehicleCategory << endl;
    cout << "Vehicle: " << vehicleName << endl;
    cout << "Parking Rate: Rs. " << getRatePerBlock() << " per 30 minutes" << endl;
}

TwoWheeler::TwoWheeler(string number, string name)
    : Vehicle(number, "2 Wheeler", name)
{
}

// Half the four wheeler rate
double TwoWheeler::getRatePerBlock() const
{
    return 10;
}

FourWheeler::FourWheeler(string number, string name)
    : Vehicle(number, "4 Wheeler", name)
{
}

double FourWheeler::getRatePerBlock() const
{
    return 20;
}
