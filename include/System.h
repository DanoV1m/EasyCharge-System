#ifndef SYSTEM_H
#define SYSTEM_H

#include "Station.h"

 // main system functions
// loads all stations and cars from their files into memory

void loadFiles(Station** stationsRoot, tCar** carsRoot);// uses double pointers to the linked lists
void updateFiles(Station* stationsRoot, tCar* carsRoot);// saves the current state back to the files
void displayMenu();
void exitSys(Station* stationsRoot, tCar* carsRoot);    // frees all memory and prevent leaks


#endif 