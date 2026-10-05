#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

using namespace std;

class Vehicle
{
private:
    string vehicleNumber;
    string vehicleType;
    string ownerName;

public:
    Vehicle();
    Vehicle(string number, string type, string owner);
    virtual ~Vehicle();

    void setVehicleNumber(string number);
    void setVehicleType(string type);
    void setOwnerName(string owner);

    string getVehicleNumber();
    string getVehicleType();
    string getOwnerName();

    virtual void displayVehicle();
};


// Derived class: Car
class Car : public Vehicle
{
public:
    Car(string number, string owner);
    ~Car();

    void displayVehicle() override;
};


// Derived class: Bike
class Bike : public Vehicle
{
public:
    Bike(string number, string owner);
    ~Bike();

    void displayVehicle() override;
};

#endif