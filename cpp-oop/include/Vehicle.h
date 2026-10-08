#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

using namespace std;


// Base Vehicle class
class Vehicle
{
private:
    string vehicleNumber;
    string vehicleCategory;
    string vehicleType;
    string ownerName;

public:
    // Constructors
    Vehicle();

    Vehicle(
        string number,
        string category,
        string type,
        string owner
    );

    virtual ~Vehicle();

    // Setters
    void setVehicleNumber(string number);
    void setVehicleCategory(string category);
    void setVehicleType(string type);
    void setOwnerName(string owner);

    // Getters
    string getVehicleNumber();
    string getVehicleCategory();
    string getVehicleType();
    string getOwnerName();

    // Polymorphic function
    virtual void displayVehicle();
};


// ---------------------------------
// Bike
// ---------------------------------

class Bike : public Vehicle
{
public:
    Bike(
        string number,
        string owner
    );

    ~Bike();

    void displayVehicle() override;
};


// ---------------------------------
// Scooty
// ---------------------------------

class Scooty : public Vehicle
{
public:
    Scooty(
        string number,
        string owner
    );

    ~Scooty();

    void displayVehicle() override;
};


// ---------------------------------
// Car
// ---------------------------------

class Car : public Vehicle
{
public:
    Car(
        string number,
        string owner
    );

    ~Car();

    void displayVehicle() override;
};


// ---------------------------------
// Truck
// ---------------------------------

class Truck : public Vehicle
{
public:
    Truck(
        string number,
        string owner
    );

    ~Truck();

    void displayVehicle() override;
};

#endif