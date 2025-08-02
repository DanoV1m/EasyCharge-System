#ifndef STATION_H
#define STATION_H

#include "Defs.h"
#include "Port.h"
#include "Queue.h"
#include "Car.h"

// defines a Station item
typedef struct Station {
    int id;
    char* name;                     // station name 
    struct { double x, y; } coord; // coordinates of the station

    int num_Ports;                  // number of ports at this station
    Port* port_list_head;          // the head of the linked list 

    int nCars;                      // the number of cars currently waiting in the queue
    CarQueue carQueue;             // the queue manager for waiting cars

    struct Station* left;
    struct Station* right;
} Station;

//  function signetures 
Station* createStation(int id, const char* name, int nPorts, double x, double y);
Station* addStationToBST(Station* root, Station* newStation);
void freeStationsBST(Station* root);
Station* searchStationByID(Station* root, int id);
void assignCar2port(Car* car, Port* port);

// menu function signetures 
void locNearSt(Station* stationsRoot);
void chargeCar(Station* stationsRoot, tCar** carsRoot);
void checkCarStatus(Station* stationsRoot, tCar* carsRoot);
void stopCharge(Station* stationsRoot, tCar* carsRoot);
void dispAllSt(Station* stationsRoot);
void dispCarsAtSt(Station* stationsRoot);
void reportStStat(Station* stationsRoot);
void addNewPort(Station* stationsRoot);
void releasePorts(Station* stationsRoot);
void remOutOrderPort(Station* stationsRoot);
Station* closeSt(Station* stationsRoot);

#endif 