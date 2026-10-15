#include "../include/Violation.h"
#include <iostream>
using namespace std;

Violation::Violation(Vehicle* v, string type, double amount)
{
    vehicle = v;
    violationType = type;
    penalty = amount;
}

void Violation::displayViolation() const
{
    cout << "\n========== VIOLATION RECORD ==========\n";

    cout << "Vehicle Number: " << vehicle->getVehicleNumber() << endl;
    cout << "Violation Type: " << violationType << endl;
    cout << "Penalty: Rs. " << penalty << endl;

    cout << "======================================\n";
}
