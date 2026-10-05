#include <iostream>
#include <string>

#include "include/Vehicle.h"
#include "include/ParkingManagement.h"

using namespace std;

int main()
{
    cout << "========================================" << endl;
    cout << "     PARKWISE SMART PARKING SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << endl;



    // Parking setup


    int twoWheelerSlots;
    int fourWheelerSlots;

    cout << "Enter number of Two Wheeler slots: ";
    cin >> twoWheelerSlots;

    cout << "Enter number of Four Wheeler slots: ";
    cin >> fourWheelerSlots;

    ParkingManagement parking(
        twoWheelerSlots,
        fourWheelerSlots
    );

    cout << endl;
    cout << "Parking system ready." << endl;



    // Main parking menu


    int choice;

    do
    {
        cout << endl;
        cout << "------------- MENU -------------" << endl;
        cout << "1. Park Vehicle" << endl;
        cout << "2. Remove Vehicle" << endl;
        cout << "3. Check Parking Availability" << endl;
        cout << "4. Exit" << endl;
        cout << "--------------------------------" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string vehicleNumber;
            string ownerName;
            int vehicleChoice;

            cout << endl;
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            cout << "Enter Owner Name: ";
            cin >> ownerName;

            cout << "Select Vehicle Type:" << endl;
            cout << "1. Two Wheeler" << endl;
            cout << "2. Four Wheeler" << endl;
            cout << "Enter choice: ";
            cin >> vehicleChoice;

            if (vehicleChoice == 1)
            {
                Bike bike(vehicleNumber, ownerName);
                parking.parkVehicle(bike);
            }
            else if (vehicleChoice == 2)
            {
                Car car(vehicleNumber, ownerName);
                parking.parkVehicle(car);
            }
            else
            {
                cout << "Invalid vehicle type." << endl;
            }
        }

        else if (choice == 2)
        {
            string vehicleNumber;

            cout << endl;
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            parking.removeVehicle(vehicleNumber);
        }

        else if (choice == 3)
        {
            parking.displayParkingStatus();
        }

        else if (choice == 4)
        {
            cout << endl;
            cout << "Thank you for using ParkWise." << endl;
        }

        else
        {
            cout << "Invalid choice." << endl;
        }

    } while (choice != 4);


    return 0;
}