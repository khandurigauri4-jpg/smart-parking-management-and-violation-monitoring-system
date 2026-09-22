#include <iostream>
#include <string>
using namespace std;

// Base Class
class Vehicle
{
private:
    string vehicleNumber;

public:
    Vehicle(string number)
    {
        vehicleNumber = number;
    }

    string getVehicleNumber()
    {
        return vehicleNumber;
    }

    virtual void displayVehicleType()
    {
        cout << "Vehicle Type: General Vehicle" << endl;
    }
};


// Derived Class
class TwoWheeler : public Vehicle
{
public:
    TwoWheeler(string number) : Vehicle(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Category: Two Wheeler" << endl;
    }
};


// Derived Class
class FourWheeler : public Vehicle
{
public:
    FourWheeler(string number) : Vehicle(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Category: Four Wheeler" << endl;
    }
};


// Derived Class
class Bike : public TwoWheeler
{
public:
    Bike(string number) : TwoWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Bike" << endl;
        cout << "Category: Two Wheeler" << endl;
    }
};


// Derived Class
class Scooty : public TwoWheeler
{
public:
    Scooty(string number) : TwoWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Scooty" << endl;
        cout << "Category: Two Wheeler" << endl;
    }
};


// Derived Class
class Car : public FourWheeler
{
public:
    Car(string number) : FourWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Car" << endl;
        cout << "Category: Four Wheeler" << endl;
    }
};


// Derived Class
class Truck : public FourWheeler
{
public:
    Truck(string number) : FourWheeler(number)
    {
    }

    void displayVehicleType() override
    {
        cout << "Vehicle Type: Truck" << endl;
        cout << "Category: Four Wheeler" << endl;
    }
};


// Parking Slot Class
class ParkingSlot
{
private:
    int slotNumber;
    bool available;
    string vehicleNumber;

public:
    ParkingSlot(int number)
    {
        slotNumber = number;
        available = true;
        vehicleNumber = "";
    }

    void parkVehicle(string vehicle)
    {
        if (available)
        {
            vehicleNumber = vehicle;
            available = false;

            cout << "Vehicle parked successfully." << endl;
        }
        else
        {
            cout << "Slot is already occupied." << endl;
        }
    }

    void removeVehicle()
    {
        if (!available)
        {
            vehicleNumber = "";
            available = true;

            cout << "Vehicle removed successfully." << endl;
        }
        else
        {
            cout << "Slot is already available." << endl;
        }
    }

    void displaySlot()
    {
        cout << "\nSlot Number : " << slotNumber << endl;

        if (available)
        {
            cout << "Status      : Available" << endl;
        }
        else
        {
            cout << "Status      : Occupied" << endl;
            cout << "Vehicle No. : " << vehicleNumber << endl;
        }
    }

    // Function Overloading
    void searchVehicle(string vehicle)
    {
        if (vehicleNumber == vehicle)
        {
            cout << "Vehicle found in Slot " << slotNumber << endl;
        }
        else
        {
            cout << "Vehicle not found in this slot." << endl;
        }
    }

    void searchVehicle(int number)
    {
        if (slotNumber == number)
        {
            cout << "Slot found: " << slotNumber << endl;
        }
        else
        {
            cout << "Slot not found." << endl;
        }
    }
};


int main()
{
    cout << "========================================" << endl;
    cout << "        SMART PARKING SYSTEM" << endl;
    cout << "========================================" << endl;

    string vehicleNumber;
    int choice;

    cout << "\nEnter vehicle number: ";
    cin >> vehicleNumber;

    cout << "\nSelect Vehicle Type:" << endl;
    cout << "1. Bike" << endl;
    cout << "2. Scooty" << endl;
    cout << "3. Car" << endl;
    cout << "4. Truck" << endl;

    cout << "\nEnter your choice: ";
    cin >> choice;

    Vehicle* vehicle = nullptr;

    if (choice == 1)
    {
        vehicle = new Bike(vehicleNumber);
    }
    else if (choice == 2)
    {
        vehicle = new Scooty(vehicleNumber);
    }
    else if (choice == 3)
    {
        vehicle = new Car(vehicleNumber);
    }
    else if (choice == 4)
    {
        vehicle = new Truck(vehicleNumber);
    }
    else
    {
        cout << "Invalid choice." << endl;
        return 0;
    }

    cout << "\n----------------------------------------" << endl;
    cout << "          VEHICLE DETAILS" << endl;
    cout << "----------------------------------------" << endl;

    cout << "Vehicle Number: " << vehicle->getVehicleNumber() << endl;

    // Runtime Polymorphism
    vehicle->displayVehicleType();

    ParkingSlot slot1(1);

    cout << "\n----------------------------------------" << endl;
    cout << "          PARKING SLOT" << endl;
    cout << "----------------------------------------" << endl;

    slot1.displaySlot();

    cout << "\nParking vehicle..." << endl;
    slot1.parkVehicle(vehicleNumber);

    slot1.displaySlot();

    cout << "\n----------------------------------------" << endl;
    cout << "             SEARCH" << endl;
    cout << "----------------------------------------" << endl;

    cout << "Searching by vehicle number..." << endl;
    slot1.searchVehicle(vehicleNumber);

    cout << "Searching by slot number..." << endl;
    slot1.searchVehicle(1);

    cout << "\n----------------------------------------" << endl;
    cout << "          VEHICLE EXIT" << endl;
    cout << "----------------------------------------" << endl;

    slot1.removeVehicle();

    slot1.displaySlot();

    delete vehicle;

    cout << "\n========================================" << endl;
    cout << "          PROGRAM COMPLETED" << endl;
    cout << "========================================" << endl;

    return 0;
}