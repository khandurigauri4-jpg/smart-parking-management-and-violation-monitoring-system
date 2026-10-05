#include "../include/ParkingSlot.h"
#include <iostream>

using namespace std;

ParkingSlot::ParkingSlot()
{
    slotID = 0;
    slotType = "";
    available = true;
}

ParkingSlot::ParkingSlot(int id, string type)
{
    slotID = id;
    slotType = type;
    available = true;
}

void ParkingSlot::setSlotID(int id)
{
    slotID = id;
}

void ParkingSlot::setSlotType(string type)
{
    slotType = type;
}

void ParkingSlot::setAvailability(bool status)
{
    available = status;
}

int ParkingSlot::getSlotID()
{
    return slotID;
}

string ParkingSlot::getSlotType()
{
    return slotType;
}

bool ParkingSlot::getAvailability()
{
    return available;
}

void ParkingSlot::displaySlot()
{
    cout << "Slot ID: " << slotID << endl;
    cout << "Slot Type: " << slotType << endl;

    if (available)
    {
        cout << "Status: Available" << endl;
    }
    else
    {
        cout << "Status: Occupied" << endl;
    }
}