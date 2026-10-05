#ifndef USER_H
#define USER_H

#include <string>
#include "Vehicle.h"

using namespace std;

class User
{
private:
    int userID;
    string userName;
    string phoneNumber;
    Vehicle vehicle;

public:
    User(int id, string name, string phone, Vehicle v);

    void setUserID(int id);
    void setUserName(string name);
    void setPhoneNumber(string phone);
    void setVehicle(Vehicle v);

    int getUserID();
    string getUserName();
    string getPhoneNumber();
    Vehicle getVehicle();

    void displayUser();
};

#endif