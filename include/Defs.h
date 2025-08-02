#ifndef DEFS_H
#define DEFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// defines the charging speed
typedef enum PortType {
    PORT_TYPE_FASR,
    PORT_TYPE_MID,
    PORT_TYPE_SLOW
}PortType;

 // struct that is used for keeping date and time
// used by Port to know when a car started charging
typedef struct Date {
    int year;
    int month;
    int day;
    int hour;
    int min;
} Date;

// forward declarations - used here mainly to let structs know about other structs that they depend on
struct Car;
struct Port;
struct Station;

#endif
