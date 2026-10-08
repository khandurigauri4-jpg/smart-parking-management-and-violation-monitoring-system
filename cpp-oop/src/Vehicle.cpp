#include "../include/Vehicle.h"
#include <iostream>

using namespace std;


// =================================
// Vehicle Base Class
// =================================

// Default constructor
Vehicle::Vehicle()
{
    vehicleNumber = "";
    vehicleCategory = "";
    vehicleType = "";
    ownerName = "";
}


// Parameterized constructor
Vehicle::Vehicle(
    string number,
    string category,
    string type,
    string owner
)
{
    vehicleNumber = number;
    vehicleCategory = category;
    vehicleType = type;
    ownerName = owner;
}


// Destructor
Vehicle::~Vehicle()
{
}


// =================================
// Setters
// =================================

void Vehicle::setVehicleNumber(string number)
{
    vehicleNumber = number;
}


void Vehicle::setVehicleCategory(string category)
{
    vehicleCategory = category;
}


void Vehicle::setVehicleType(string type)
{
    vehicleType = type;
}


void Vehicle::setOwnerName(string owner)
{
    ownerName = owner;
}


// =================================
// Getters
// =================================

string Vehicle::getVehicleNumber()
{
    return vehicleNumber;
}


string Vehicle::getVehicleCategory()
{
    return vehicleCategory;
}


string Vehicle::getVehicleType()
{
    return vehicleType;
}


string Vehicle::getOwnerName()
{
    return ownerName;
}


// =================================
// Display Vehicle
// =================================

void Vehicle::displayVehicle()
{
    cout << "Vehicle Number: "
         << vehicleNumber
         << endl;

    cout << "Vehicle Category: "
         << vehicleCategory
         << endl;

    cout << "Vehicle Type: "
         << vehicleType
         << endl;

    cout << "Owner Name: "
         << ownerName
         << endl;
}


// =================================
// Bike
// =================================

Bike::Bike(
    string number,
    string owner
)
    : Vehicle(
        number,
        "Two Wheeler",
        "Bike",
        owner
    )
{
}


Bike::~Bike()
{
}


void Bike::displayVehicle()
{
    cout << "Vehicle Number: "
         << getVehicleNumber()
         << endl;

    cout << "Vehicle Category: Two Wheeler"
         << endl;

    cout << "Vehicle Type: Bike"
         << endl;

    cout << "Owner Name: "
         << getOwnerName()
         << endl;
}


// =================================
// Scooty
// =================================

Scooty::Scooty(
    string number,
    string owner
)
    : Vehicle(
        number,
        "Two Wheeler",
        "Scooty",
        owner
    )
{
}


Scooty::~Scooty()
{
}


void Scooty::displayVehicle()
{
    cout << "Vehicle Number: "
         << getVehicleNumber()
         << endl;

    cout << "Vehicle Category: Two Wheeler"
         << endl;

    cout << "Vehicle Type: Scooty"
         << endl;

    cout << "Owner Name: "
         << getOwnerName()
         << endl;
}


// =================================
// Car
// =================================

Car::Car(
    string number,
    string owner
)
    : Vehicle(
        number,
        "Four Wheeler",
        "Car",
        owner
    )
{
}


Car::~Car()
{
}


void Car::displayVehicle()
{
    cout << "Vehicle Number: "
         << getVehicleNumber()
         << endl;

    cout << "Vehicle Category: Four Wheeler"
         << endl;

    cout << "Vehicle Type: Car"
         << endl;

    cout << "Owner Name: "
         << getOwnerName()
         << endl;
}


// =================================
// Truck
// =================================

Truck::Truck(
    string number,
    string owner
)
    : Vehicle(
        number,
        "Heavy Vehicle",
        "Truck",
        owner
    )
{
}


Truck::~Truck()
{
}


void Truck::displayVehicle()
{
    cout << "Vehicle Number: "
         << getVehicleNumber()
         << endl;

    cout << "Vehicle Category: Heavy Vehicle"
         << endl;

    cout << "Vehicle Type: Truck"
         << endl;

    cout << "Owner Name: "
         << getOwnerName()
         << endl;
}