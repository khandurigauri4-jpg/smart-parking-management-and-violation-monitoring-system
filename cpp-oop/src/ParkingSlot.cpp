#include "../include/ParkingSlot.h"
using namespace std;

ParkingSlot::ParkingSlot(string id)
{
    slotID = id;
    occupied = false;   // every new slot starts empty
}

string ParkingSlot::getSlotID() const
{
    return slotID;
}

bool ParkingSlot::isOccupied() const
{
    return occupied;
}

void ParkingSlot::occupySlot()
{
    occupied = true;
}

void ParkingSlot::releaseSlot()
{
    occupied = false;
}
