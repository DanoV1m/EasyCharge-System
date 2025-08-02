#ifndef PORT_H
#define PORT_H

#include "Defs.h"

// defines the states of a charging port
typedef enum { OCCUPIED = 1, FREE, OUT_OF_ORDER } PortStatus;

// a port struct. this will be a node in a stations linked list of ports
typedef struct Port {
    int num;             // the number of the port in the station
    PortType portType;   // the speed of this port
    PortStatus status;   // the current status of the port

    struct Car* p2car;    // pointer to a Car if in use, otherwise NULL
    Date chargeStartTime; // the date and time for when charging started 

    struct Port* next;    // pointer to the next port in the linked list
} Port;

// function signatures
Port* createPort(int num, PortType type);
void addPortToList(Port** head, Port* newPort);
void printPortsList(const Port* head); // a helper function for debugging
void freePortsList(Port* head);

#endif 
