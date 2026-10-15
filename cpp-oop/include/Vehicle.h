#pragma once
#include <string>

// Base class: stores the details common to every vehicle.
class Vehicle
{
private:
    std::string vehicleNumber;
    std::string vehicleCategory;
    std::string vehicleName;

public:
    Vehicle(std::string number, std::string category, std::string name);

    std::string getVehicleNumber() const;
    std::string getVehicleCategory() const;
    std::string getVehicleName() const;

    // Rs. per 30 minutes; each vehicle type gives its own rate
    virtual double getRatePerBlock() const = 0;

    virtual void displayDetails() const;

    virtual ~Vehicle() {}
};

// A TwoWheeler is a Vehicle whose category is always "2 Wheeler".
class TwoWheeler : public Vehicle
{
public:
    TwoWheeler(std::string number, std::string name);

    double getRatePerBlock() const override;
};

// A FourWheeler is a Vehicle whose category is always "4 Wheeler".
class FourWheeler : public Vehicle
{
public:
    FourWheeler(std::string number, std::string name);

    double getRatePerBlock() const override;
};
