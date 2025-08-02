#define _CRT_SECURE_NO_WARNINGS
#include "Port.h"
#include "Car.h" // needed to access car members in printPortsList


/* this function creates a new port struct in dynamic memory and initializes it
 input: int num - the ports  number at its station 
 input: PortType type - the charging speed of the port
output: Port* - a pointer to the newly allocated Port, or NULL on failure */

Port* createPort(int num, PortType type) {
    Port* p = (Port*)malloc(sizeof(Port));
    if (!p) {
        perror("ERROR: Memory allocation failed for new port");
        return NULL;
    }
    p->num = num;
    p->portType = type;
    p->status = FREE; // new ports are free by default
    p->p2car = NULL;  // no car connected
    p->next = NULL;   // this is the last port in the list until linked
    return p;
}

// adds a new port to the beginning of a linked list
void addPortToList(Port** head, Port* newPort) {
    if (!newPort) {
        return; // do nothing if new new port is NULL 
    }

    newPort->next = *head;   // the new ports points to the old head of the list
    *head = newPort;        // update the head of the list to be the new port 
}

// a helper function to print the details of all ports in a list
void printPortsList(const Port* head) {
    const Port* curr = head;
    if (!curr) {
        printf("  (This station has no ports.)\n");
        return;
    }

    printf("  --- Port Details ---\n");
    while (curr) {
        printf("  Port #%d | Type: %d | Status: %d",
            curr->num, curr->portType, curr->status);
        if (curr->p2car != NULL) {
            // if car is connected print its license plate 
            printf(" | Connected Car: %s\n", curr->p2car->nLicense);
        }
        else {
            printf("\n");
        }
        curr = curr->next;
    }
    printf("  --------------------\n");
}

// recursively frees all nodes in the ports linked list
void freePortsList(Port* head) {
    Port* current = head;
    Port* next_node;

    while (current != NULL) {
        next_node = current->next; // save next before freeing current
        free(current);
        current = next_node; // move to the next node 
    }
}
