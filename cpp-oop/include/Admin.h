#ifndef ADMIN_H
#define ADMIN_H

#include <string>

using namespace std;

class Admin
{
private:
    string adminID;
    string adminName;

public:
    // Constructor
    Admin(string id, string name);

    // Setters
    void setAdminID(string id);
    void setAdminName(string name);

    // Getters
    string getAdminID();
    string getAdminName();

    // Display admin details
    void displayAdmin();
};

#endif