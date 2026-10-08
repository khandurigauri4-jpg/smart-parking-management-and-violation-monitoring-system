#include "../include/Admin.h"
#include <iostream>

using namespace std;


// Constructor
Admin::Admin(string id, string name)
{
    adminID = id;
    adminName = name;
}


// Setters
void Admin::setAdminID(string id)
{
    adminID = id;
}

void Admin::setAdminName(string name)
{
    adminName = name;
}


// Getters
string Admin::getAdminID()
{
    return adminID;
}

string Admin::getAdminName()
{
    return adminName;
}


// Display admin details
void Admin::displayAdmin()
{
    cout << "Admin ID: " << adminID << endl;
    cout << "Admin Name: " << adminName << endl;
}