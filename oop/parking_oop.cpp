#include <iostream>
#include <string>
using namespace std;

class ParkingSlot
{
private:
    int slotNumber;
    string vehicleNumber;
    bool available;

public:
    ParkingSlot(int number)
    {
        slotNumber = number;
        vehicleNumber = "";
        available = true;
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

    virtual void showType()
    {
        cout << "Parking Type: General" << endl;
    }

    // Function Overloading
    void searchVehicle(string vehicle)
    {
        if (vehicleNumber == vehicle)
            cout << "Vehicle found in Slot " << slotNumber << endl;
        else
            cout << "Vehicle not found in this slot." << endl;
    }

    void searchVehicle(int number)
    {
        if (slotNumber == number)
            cout << "Slot found: " << slotNumber << endl;
        else
            cout << "Slot not found." << endl;
    }
};

class RegularParking : public ParkingSlot
{
public:
    RegularParking(int number) : ParkingSlot(number)
    {
    }

    void showType() override
    {
        cout << "Parking Type: Regular" << endl;
    }
};

class ReservedParking : public ParkingSlot
{
public:
    ReservedParking(int number) : ParkingSlot(number)
    {
    }

    void showType() override
    {
        cout << "Parking Type: Reserved" << endl;
    }
};

int main()
{
    cout << "========================================" << endl;
    cout << "        SMART PARKING SYSTEM" << endl;
    cout << "========================================" << endl;

    RegularParking regularSlot(1);
    ReservedParking reservedSlot(2);

    // Polymorphism
    ParkingSlot* slot1 = &regularSlot;
    ParkingSlot* slot2 = &reservedSlot;

    cout << "\n[REGULAR PARKING]" << endl;
    cout << "----------------------------------------" << endl;
    slot1->displaySlot();
    slot1->showType();

    cout << "\n[RESERVED PARKING]" << endl;
    cout << "----------------------------------------" << endl;
    slot2->displaySlot();
    slot2->showType();

    string vehicle;

    cout << "\nEnter vehicle number: ";
    cin >> vehicle;

    regularSlot.parkVehicle(vehicle);

    cout << "\n[AFTER PARKING]" << endl;
    cout << "----------------------------------------" << endl;
    regularSlot.displaySlot();

    cout << "\n[SEARCH]" << endl;
    cout << "----------------------------------------" << endl;

    cout << "Searching by vehicle number..." << endl;
    regularSlot.searchVehicle(vehicle);

    cout << "Searching by slot number..." << endl;
    regularSlot.searchVehicle(1);

    cout << "\n[VEHICLE EXIT]" << endl;
    cout << "----------------------------------------" << endl;

    regularSlot.removeVehicle();

    cout << "\n[FINAL SLOT STATUS]" << endl;
    cout << "----------------------------------------" << endl;
    regularSlot.displaySlot();

    cout << "\n========================================" << endl;
    cout << "          PROGRAM COMPLETED" << endl;
    cout << "========================================" << endl;

    return 0;
}
