# C++ OOP – Parking Ticket & Overstay

My part of the Smart Parking project: the parking ticket, fee and overstay penalty, written in C++ using OOP.

## Flow

1. Vehicle number and owner name
2. Vehicle type (2 or 4 wheeler) and vehicle name
3. Slot ID, entry time and allowed hours → ticket is created, slot is occupied
4. Exit time → fee and penalty are calculated, slot is released
5. If the vehicle overstayed, a violation record is shown

## Build and run

From inside the `cpp-oop` folder:

```
g++ main.cpp src/*.cpp -o parking.exe
.\parking.exe
```

## Charges

| Vehicle | Rate per 30 min | Overstay penalty per 30 min |
|---|---|---|
| 2 wheeler | Rs. 10 | Rs. 10 |
| 4 wheeler | Rs. 20 | Rs. 20 |

Every started 30-minute block is charged in full. Overstay time is charged again as the penalty.

## Classes

| Class | What it does |
|---|---|
| `Vehicle` | Base class: number, category, name. Abstract (`getRatePerBlock()` is pure virtual) |
| `TwoWheeler`, `FourWheeler` | Inherit from `Vehicle`, each gives its own rate |
| `User` | Owner name, has a `Vehicle` |
| `ParkingSlot` | Slot ID, occupied or free |
| `Ticket` | Links a `User` to a `ParkingSlot`, calculates fee and penalty |
| `Violation` | Overstay record for a `Vehicle` |

OOP concepts used: encapsulation, inheritance, polymorphism, abstraction, function overloading (`calculateFee`), constructors/destructor, and class relationships.

## Inputs from other parts (when merged)

Right now all inputs are typed by the user. In the full system:

| Input | Comes from |
|---|---|
| Vehicle number | OpenCV + OCR |
| Owner name, vehicle type | Database |
| Slot ID | C DSA slot allocation |
| Ticket ID | Database |
