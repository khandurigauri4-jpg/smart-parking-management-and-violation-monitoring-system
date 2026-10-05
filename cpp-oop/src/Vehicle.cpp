#include "../include/Vehicle.h"
#include <iostream>

using namespace std;


// Default constructor
Vehicle::Vehicle()
{
    vehicleNumber = "";
    vehicleType = "";
    ownerName = "";
}


// Parameterized constructor
Vehicle::Vehicle(string number, string type, string owner)
{
    vehicleNumber = number;
    vehicleType = type;
    ownerName = owner;
}


// Destructor
Vehicle::~Vehicle()
{
}


// Setters
void Vehicle::setVehicleNumber(string number)
{
    vehicleNumber = number;
}

void Vehicle::setVehicleType(string type)
{
    vehicleType = type;
}

void Vehicle::setOwnerName(string owner)
{
    ownerName = owner;
}


// Getters
string Vehicle::getVehicleNumber()
{
    return vehicleNumber;
}

string Vehicle::getVehicleType()
{
    return vehicleType;
}

string Vehicle::getOwnerName()
{
    return ownerName;
}


// Display vehicle
void Vehicle::displayVehicle()
{
    cout << "Vehicle Number: " << vehicleNumber << endl;
    cout << "Vehicle Type: " << vehicleType << endl;
    cout << "Owner Name: " << ownerName << endl;
}


// Car
Car::Car(string number, string owner)
    : Vehicle(number, "Car", owner)
{
}

Car::~Car()
{
}

void Car::displayVehicle()
{
    cout << "Vehicle Number: " << getVehicleNumber() << endl;
    cout << "Vehicle Type: Car" << endl;
    cout << "Owner Name: " << getOwnerName() << endl;
}


// Bike
Bike::Bike(string number, string owner)
    : Vehicle(number, "Bike", owner)
{
}

Bike::~Bike()
{
}

void Bike::displayVehicle()
{
    cout << "Vehicle Number: " << getVehicleNumber() << endl;
    cout << "Vehicle Type: Bike" << endl;
    cout << "Owner Name: " << getOwnerName() << endl;
}