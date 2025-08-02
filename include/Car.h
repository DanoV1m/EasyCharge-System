#ifndef CAR_H
#define CAR_H

#include "Defs.h"

// defines the Car struct itself. This holds customer data.
typedef struct Car {
    char nLicense[10];     // license number 
    PortType portType;     // charging type
    double totalPayed;     // total amount customer has paid.
    struct Port* pPort;    // a pointer to a Port if charging, otherwise NULL.
    int inqueue;           // a signal that gives 1 if the car is in a queue, 0 otherwise.
} Car;

 // defines a node for the Car BST
// it holds a pointer to a Car, not the Car itself, to avoid data duplication.
typedef struct tCar {
    Car* p2car;
    struct tCar* left;
    struct tCar* right;
} tCar;

 // function signatures 
// functions for creating and managing cars.

Car* createCar(const char* license, PortType type);
tCar* addCarToBST(tCar* root, Car* car);
Car* searchCar(tCar* root, const char* licenseNum);
void freeCarsBST(tCar* root);

// menu functions that mainly operate on the car database.

void dispTopCustomers(tCar* root);
tCar* remCustomer(tCar* root, struct Station* stationsRoot);

#endif
