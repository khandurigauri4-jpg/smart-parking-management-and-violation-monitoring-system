#pragma once
#include <string>

// One parking space, which is either free or occupied.
class ParkingSlot
{
private:
    std::string slotID;
    bool occupied;

public:
    ParkingSlot(std::string id);

    std::string getSlotID() const;
    bool isOccupied() const;

    void occupySlot();
    void releaseSlot();
};
