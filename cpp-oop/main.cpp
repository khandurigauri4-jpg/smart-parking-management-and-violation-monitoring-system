#include <iostream>
#include <string>

#include "include/Vehicle.h"
#include "include/ParkingManagement.h"

using namespace std;

int main()
{

    
    cout << "       PARKWISE C++ OOP ENGINE" << endl;

    
    cout << endl;


    int totalSlots;

    cout << "Enter total number of parking slots: ";
    cin >> totalSlots;

    ParkingManagement parking(totalSlots);

    cout << endl;
    cout << "C++ OOP parking engine ready."
         << endl;


    int choice = 0;


    while (choice != 7)
    {
        cout << endl;
    
        
        cout << "              PARKWISE MENU" << endl;
    
        

        cout << "1. Vehicle Entry" << endl;
        cout << "2. Vehicle Exit" << endl;
        cout << "3. Display Parking Status" << endl;
        cout << "4. Display Active Ticket" << endl;
        cout << "5. Display Vehicle Violation" << endl;
        cout << "6. Check Vehicle Blacklist" << endl;
        cout << "7. Exit" << endl;

        cout << endl;
        cout << "Enter your choice: ";
        cin >> choice;



        // 1. Vehicle Entry


        if (choice == 1)
        {
            string vehicleNumber;
            string ownerName;
            string entryTime;

            int vehicleChoice;
            int allocatedSlotID;
            int allowedHours;


            cout << endl;
            cout << "--------- VEHICLE ENTRY ---------"
                 << endl;

            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            cout << "Enter Owner Name: ";
            cin >> ownerName;


            cout << endl;
            cout << "Select Vehicle Type:" << endl;

            cout << "1. Bike" << endl;
            cout << "2. Scooty" << endl;
            cout << "3. Car" << endl;
            cout << "4. Truck" << endl;

            cout << "Enter choice: ";
            cin >> vehicleChoice;


            cout << endl;

            cout << "Enter Allocated Slot ID: ";
            cin >> allocatedSlotID;

            cout << "Enter Allowed Parking Time (hours): ";
            cin >> allowedHours;

            cout << "Enter Entry Time: ";
            cin >> entryTime;


    
            // Create correct vehicle object
    

            if (vehicleChoice == 1)
            {
                Bike bike(
                    vehicleNumber,
                    ownerName
                );

                parking.processVehicleEntry(
                    bike,
                    allocatedSlotID,
                    allowedHours,
                    entryTime
                );
            }

            else if (vehicleChoice == 2)
            {
                Scooty scooty(
                    vehicleNumber,
                    ownerName
                );

                parking.processVehicleEntry(
                    scooty,
                    allocatedSlotID,
                    allowedHours,
                    entryTime
                );
            }

            else if (vehicleChoice == 3)
            {
                Car car(
                    vehicleNumber,
                    ownerName
                );

                parking.processVehicleEntry(
                    car,
                    allocatedSlotID,
                    allowedHours,
                    entryTime
                );
            }

            else if (vehicleChoice == 4)
            {
                Truck truck(
                    vehicleNumber,
                    ownerName
                );

                parking.processVehicleEntry(
                    truck,
                    allocatedSlotID,
                    allowedHours,
                    entryTime
                );
            }

            else
            {
                cout << "Invalid vehicle type."
                     << endl;
            }
        }



        // 2. Vehicle Exit


        else if (choice == 2)
        {
            string vehicleNumber;
            string exitTime;

            int actualHours;


            cout << endl;
            cout << "--------- VEHICLE EXIT ---------"
                 << endl;

            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            cout << "Enter Actual Parking Time (hours): ";
            cin >> actualHours;

            cout << "Enter Exit Time: ";
            cin >> exitTime;


            parking.processVehicleExit(
                vehicleNumber,
                actualHours,
                exitTime
            );
        }



        // 3. Parking Status


        else if (choice == 3)
        {
            parking.displayParkingStatus();
        }



        // 4. Active Ticket


        else if (choice == 4)
        {
            string vehicleNumber;

            cout << endl;
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            parking.displayTicket(
                vehicleNumber
            );
        }



        // 5. Vehicle Violation


        else if (choice == 5)
        {
            string vehicleNumber;

            cout << endl;
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;

            parking.displayVehicleViolation(
                vehicleNumber
            );
        }



        // 6. Check Blacklist


        else if (choice == 6)
        {
            string vehicleNumber;

            cout << endl;
            cout << "Enter Vehicle Number: ";
            cin >> vehicleNumber;


            if (
                parking.isVehicleBlacklisted(
                    vehicleNumber
                )
            )
            {
                cout << endl;
                cout << "Vehicle Status: BLACKLISTED"
                     << endl;

                cout << "Parking entry will be denied."
                     << endl;
            }
            else
            {
                cout << endl;
                cout << "Vehicle Status: NOT BLACKLISTED"
                     << endl;
            }
        }



        // 7. Exit


        else if (choice == 7)
        {
            cout << endl;
            cout << "Thank you for using ParkWise."
                 << endl;
        }



        // Invalid choice


        else
        {
            cout << endl;
            cout << "Invalid choice."
                 << endl;
        }
    }


    return 0;
}