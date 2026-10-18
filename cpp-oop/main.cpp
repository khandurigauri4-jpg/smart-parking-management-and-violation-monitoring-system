#include <iostream>
#include <iomanip>
#include <string>
#include <cstdlib>

#include "include/Vehicle.h"
#include "include/User.h"
#include "include/ParkingSlot.h"
#include "include/Ticket.h"
#include "include/Violation.h"

using namespace std;

// Input helpers (written at the bottom of this file)
string readText(string prompt);
string readTime(string prompt);
int readVehicleCategory(string prompt);
int readAllowedHours(string prompt);

int main()
{
    cout << fixed << setprecision(2);   // money as 80.00

    cout << "===== PARKWISE SMART PARKING =====\n";

    // ---- Step 1: vehicle and owner ----
    // Full system: number comes from OpenCV + OCR, other details from the database
    string vehicleNumber = readText("Enter vehicle number: ");
    string ownerName = readText("Enter owner name : ");
    int vehicleCategory = readVehicleCategory("Enter vehicle type (2 = 2 wheeler, 4 = 4 wheeler): ");
    string vehicleName = readText("Enter your vehicle name (e.g. Car/Truck/Van or Bike/Scooter): ");

    // A Vehicle pointer can point to a TwoWheeler or a FourWheeler
    Vehicle* vehicle;

    if (vehicleCategory == 2)
        vehicle = new TwoWheeler(vehicleNumber, vehicleName);
    else
        vehicle = new FourWheeler(vehicleNumber, vehicleName);

    cout << "\nVehicle Details:\n";
    vehicle->displayDetails();

    User user(ownerName, vehicle);

    cout << "\nUser Details:\n";
    user.displayUser();

    // ---- Step 2: slot and ticket ----
    // Full system: slot ID comes from the C DSA slot allocation
    string slotID = readText("\nEnter allocated slot ID: ");
    ParkingSlot slot(slotID);

    string entryTime = readTime("\nEnter entry time (HH:MM, 24-hour format): ");
    int allowedHours = readAllowedHours("Enter allowed parking hours (whole hours): ");

    // Creating the ticket occupies the slot
    // Full system: ticket ID is given by the database
    Ticket ticket("T001", &user, &slot, entryTime, allowedHours);

    cout << "\nSlot " << slot.getSlotID() << " is now occupied.\n";
    cout << "Parking ticket created successfully!\n";
    ticket.displayTicket();

    // ---- Step 3: exit and billing ----
    string exitTime = readTime("\nEnter actual exit time (HH:MM, 24-hour format): ");

    // Exiting calculates the bill and releases the slot
    ticket.exitParking(exitTime);
    ticket.displayTicket();

    if (ticket.hasOverstay())
    {
        Violation violation(vehicle, "Overstay", ticket.getPenalty());
        violation.displayViolation();
    }
    else
    {
        cout << "\nNo parking violation.\n";
    }

    if (!slot.isOccupied())
        cout << "\nSlot " << slot.getSlotID() << " is now available.\n";

    cout << "\nThank you for using ParkWise!\n";

    delete vehicle;

    return 0;
}

// ================= INPUT HELPERS =================

// getline() reads the whole line, so names with spaces work
string readText(string prompt)
{
    string text;

    while (true)
    {
        cout << prompt;

        if (!getline(cin, text))   // input ended (Ctrl+Z)
        {
            cout << "\nNo more input. Exiting.\n";
            exit(1);
        }

        if (text != "")
            return text;

        cout << "Input cannot be empty. Please try again.\n";
    }
}

bool isAllDigits(string text)
{
    if (text == "")
        return false;

    for (char c : text)
    {
        if (c < '0' || c > '9')
            return false;
    }

    return true;
}

// Checks for a real 24-hour time like "09:30"
bool isValidTime(string text)
{
    if (text.length() != 5 || text[2] != ':')
        return false;

    string hh = text.substr(0, 2);
    string mm = text.substr(3, 2);

    if (!isAllDigits(hh) || !isAllDigits(mm))
        return false;

    return stoi(hh) <= 23 && stoi(mm) <= 59;
}

string readTime(string prompt)
{
    while (true)
    {
        string text = readText(prompt);

        if (isValidTime(text))
            return text;

        cout << "Please use HH:MM format, e.g. 09:30 or 17:05.\n";
    }
}

int readVehicleCategory(string prompt)
{
    while (true)
    {
        string text = readText(prompt);

        if (text == "2" || text == "4")
            return stoi(text);

        cout << "Invalid category. Please enter 2 or 4.\n";
    }
}

// Max 24 hours, because we only store HH:MM and not the date
int readAllowedHours(string prompt)
{
    while (true)
    {
        string text = readText(prompt);

        if (isAllDigits(text) && text.length() <= 2)
        {
            int hours = stoi(text);

            if (hours >= 1 && hours <= 24)
                return hours;
        }

        cout << "Parking hours must be a whole number from 1 to 24.\n";
    }
}
