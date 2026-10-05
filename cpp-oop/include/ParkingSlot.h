#ifndef PARKING_SLOT_H
#define PARKING_SLOT_H

#include <string>

using namespace std;

class ParkingSlot
{
private:
    int slotID;
    string slotType;
    bool available;

public:
    ParkingSlot();
    ParkingSlot(int id, string type);

    void setSlotID(int id);
    void setSlotType(string type);
    void setAvailability(bool status);

    int getSlotID();
    string getSlotType();
    bool getAvailability();

    void displaySlot();
};

#endif