#include "../include/User.h"
#include <iostream>

using namespace std;

User::User(int id, string name, string phone, Vehicle v)
    : vehicle(v)
{
    userID = id;
    userName = name;
    phoneNumber = phone;
}

void User::setUserID(int id)
{
    userID = id;
}

void User::setUserName(string name)
{
    userName = name;
}

void User::setPhoneNumber(string phone)
{
    phoneNumber = phone;
}

void User::setVehicle(Vehicle v)
{
    vehicle = v;
}

int User::getUserID()
{
    return userID;
}

string User::getUserName()
{
    return userName;
}

string User::getPhoneNumber()
{
    return phoneNumber;
}

Vehicle User::getVehicle()
{
    return vehicle;
}

void User::displayUser()
{
    cout << "User ID: " << userID << endl;
    cout << "User Name: " << userName << endl;
    cout << "Phone Number: " << phoneNumber << endl;

    cout << "Vehicle Details:" << endl;
    vehicle.displayVehicle();
}