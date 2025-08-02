#define _CRT_SECURE_NO_WARNINGS
#include "Station.h"
#include <math.h>  
#include <float.h> 

static Station* findStationWithCar(Station* root, const char* license);
static Car* findAndDequeueMatchingCar(CarQueue* q, PortType type, int* nCars);

// function implementations

/* this function creates a new Station struct in dynamic memory
 input: int id - the ID of the station
 input: const char* name - the name of the station
 input: int nPorts - the number of ports the station has
 input: double x - the X coordinate of the stationn
 input: double y - the Y coordinate of the station 
output: Station* - a pointer to the allocated Station, or NULL on failure */

Station* createStation(int id, const char* name, int nPorts, double x, double y) {
    Station* st = (Station*)malloc(sizeof(Station));
    if (st) {
        st->name = (char*)malloc(strlen(name) + 1);
    }
    if (!st || !st->name) {
        fprintf(stderr, "ERR: failed to create station %d\n", id);
        if (st) free(st);
        return NULL;
    }
    st->id = id;
    strcpy(st->name, name);
    st->coord.x = x;
    st->coord.y = y;
    st->num_Ports = nPorts;
    st->port_list_head = NULL;
    st->nCars = 0;
    initQueue(&st->carQueue);
    st->left = NULL;
    st->right = NULL;
    return st;
}

// this function recursively adds a new station to the stations BST, sorted by ID
Station* addStationToBST(Station* root, Station* newStation) {
    if (root == NULL) return newStation;
    if (newStation->id < root->id)  root->left = addStationToBST(root->left, newStation);
    else if (newStation->id > root->id) root->right = addStationToBST(root->right, newStation);
    return root;
}

// recursively searches for a station by its ID
Station* searchStationByID(Station* root, int id) {
    if (root == NULL || root->id == id) return root;
    if (id < root->id) return searchStationByID(root->left, id);
    else return searchStationByID(root->right, id);
}

// frees the entire stations BST and all its data
void freeStationsBST(Station* root) {
    if (root == NULL) return;
    freeStationsBST(root->left);
    freeStationsBST(root->right);
    free(root->name);
    freePortsList(root->port_list_head);
    freeQueue(&root->carQueue);
    free(root);
}

/* this function makes a both way link between a car and a port
 input: Car* car - the car to be connected
 input: Port* port - the port to connect to */
void assignCar2port(Car* car, Port* port) {
    if (!car || !port) return;
    port->p2car = car;
    port->status = OCCUPIED;
    car->pPort = port;
    car->inqueue = 0;

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    port->chargeStartTime.year = t->tm_year + 1900;
    port->chargeStartTime.month = t->tm_mon + 1;
    port->chargeStartTime.day = t->tm_mday;
    port->chargeStartTime.hour = t->tm_hour;
    port->chargeStartTime.min = t->tm_min;
}

// menu functions

static double calculateDistance(double x1, double y1, double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2)); // uses pythagorean theorem
}


/* this function recursively finds the closest station to a given user coordinate by using a binary tree of stations
    input: Station* curr - a pointer to the current station node in the tree
    input: double userX - the user's X coordinate
    input: double userY - the user's Y coordinate
    input: Station** closest_st - a pointer to the pointer holding the closest station found so far
    input: double* closest_dist - a pointer to the variable holding the minimum distance  */
static void find_closest_recursive(Station* curr, double userX, double userY, Station** closest_st, double* closest_dist) {
    if (!curr) return;
    double dist = calculateDistance(userX, userY, curr->coord.x, curr->coord.y);
    if (dist < *closest_dist) {
        *closest_dist = dist;
        *closest_st = curr;
    }
    find_closest_recursive(curr->left, userX, userY, closest_st, closest_dist);
    find_closest_recursive(curr->right, userX, userY, closest_st, closest_dist);
}


/* prompts the user for their coordinates, finds the closest station in the system,
   and prints its detailed information to the console
    input: Station* stationsRoot - the root of the station's binary tree */
void locNearSt(Station* stationsRoot) {
    double userX, userY;
    printf("\n--- Locate Nearest Station ---\n");
    printf("Enter your X coordinate: ");
    if (scanf("%lf", &userX) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return; }
    printf("Enter your Y coordinate: ");
    if (scanf("%lf", &userY) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return; }
    if (stationsRoot == NULL) { printf("System Error: There are no stations loaded.\n"); return; }
    Station* nearestStation = NULL;
    double minDistance = DBL_MAX;
    find_closest_recursive(stationsRoot, userX, userY, &nearestStation, &minDistance);
    if (nearestStation) {
        printf("\n--- Nearest Station Found ---\n");
        printf("Station ID: %d\n", nearestStation->id);
        printf("Station Name: %s\n", nearestStation->name);
        printf("Distance: %.2f units\n", minDistance);
        int activePorts = 0, occupiedPorts = 0;
        Port* p = nearestStation->port_list_head;
        while (p) {
            if (p->status != OUT_OF_ORDER) activePorts++;
            if (p->status == OCCUPIED) occupiedPorts++;
            p = p->next;
        }
        printf("Active (non-broken) Ports: %d\n", activePorts);
        printf("Currently Occupied Ports: %d\n", occupiedPorts);
        printf("Cars Waiting in Queue: %d\n", nearestStation->nCars);
    }
}

/* starts the process of charging a car. it starts by prompting the
   user for a station ID and checks that the station exists in the system
    input: Station* stationsRoot - the root of the stations data structure, used to search for the station
    input: tCar** carsRoot - pointer to the root of the cars data structure */
void chargeCar(Station* stationsRoot, tCar** carsRoot) {
    int stationId;
    char licenseNum[10];
    printf("\n--- Charge Car ---\n");
    printf("Enter Station ID: ");
    if (scanf("%d", &stationId) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }

    Station* station = searchStationByID(stationsRoot, stationId);
    if (!station) {
        printf("ERROR: Station with ID %d not found.\n", stationId);
        return;
    }

    printf("Enter Car License Number: ");
    scanf("%9s", licenseNum); // use %9s to prevent errors while user is giving input

    Car* car = searchCar(*carsRoot, licenseNum);
    // if the car does not exist, create it
    if (!car) {
        int typeChoice;
        printf("Car not found in database. Let's register it.\n");
        printf("Select charging type (0: FAST, 1: MID, 2: SLOW): ");
        if (scanf("%d", &typeChoice) != 1 || typeChoice < 0 || typeChoice > 2) {
            while (getchar() != '\n');
            printf("Invalid choice.\n");
            return;
        }
        car = createCar(licenseNum, (PortType)typeChoice);
        if (!car) {
            fprintf(stderr, "ERROR: Failed to create a new car.\n");
            return;
        }
        *carsRoot = addCarToBST(*carsRoot, car);
        printf("New car %s registered.\n", licenseNum);
    }

    
    // check if the car is already active in the system 
    if (car->pPort != NULL) {
        printf("ERROR: Car %s is already charging.\n", car->nLicense);
        return;
    }
    if (car->inqueue == 1) {
        printf("ERROR: Car %s is already in a waiting queue.\n", car->nLicense);
        return;
    }
   

    // try to find a suitable and free port
    Port* suitablePort = NULL;
    Port* p = station->port_list_head;
    while (p) {
        if (p->status == FREE && p->portType == car->portType) {
            suitablePort = p;
            break;
        }
        p = p->next;
    }

    if (suitablePort) {
        // a suitable port is available, assign the car to it
        assignCar2port(car, suitablePort);
        printf("Car %s is now charging at station %d, port %d.\n", car->nLicense, station->id, suitablePort->num);
    }
    else {
        // no suitable port is free, offer to wait in the queue
        char choice;
        printf("No suitable and free port available.\n");
        printf("Do you want to wait in the queue? (y/n): ");
        scanf(" %c", &choice);
        if (choice == 'y' || choice == 'Y') {
            enqueue(&station->carQueue, car);
            station->nCars++;
            car->inqueue = 1; //  update the cars state to reflect its in a queue
            printf("Car %s has been added to the waiting queue at station %d.\n", car->nLicense, station->id);
        }
        else {
            printf("Car %s will not wait. Returning to menu.\n", car->nLicense);
        }
    }
}

/* looks up a car by its license number and prints its current status and its location details
    input: Station* stationsRoot - the root of the station data structure, used to find the cars station
    input: tCar* carsRoot - the root of the car data structure, used to find the car by license plate */
void checkCarStatus(Station* stationsRoot, tCar* carsRoot) {
    char licenseNum[10];
    printf("\n--- Check Car Status ---\n");
    printf("Enter Car License Number: ");
    scanf("%9s", licenseNum);
    Car* car = searchCar(carsRoot, licenseNum);
    if (!car) { printf("Car with license %s not found in the system.\n", licenseNum); return; }
    if (car->pPort) {
        Station* station = findStationWithCar(stationsRoot, licenseNum);
        if (station) {
            printf("Car %s is currently CHARGING.\n", licenseNum);
            printf("  - Station: %s (ID: %d)\n", station->name, station->id);
            printf("  - Port Number: %d\n", car->pPort->num);
            printf("  - Started at: %02d/%02d/%d %02d:%02d\n",
                car->pPort->chargeStartTime.day, car->pPort->chargeStartTime.month, car->pPort->chargeStartTime.year,
                car->pPort->chargeStartTime.hour, car->pPort->chargeStartTime.min);
        }
        else {
            printf("Error: Car is linked to a port but station not found.\n");
        }
    }
    else if (car->inqueue) {
        Station* station = findStationWithCar(stationsRoot, licenseNum);
        if (station) {
            printf("Car %s is currently WAITING in a queue.\n", licenseNum);
            printf("  - Station: %s (ID: %d)\n", station->name, station->id);
        }
        else {
            printf("Error: Car is in queue but station not found.\n");
        }
    }
    else {
        printf("Car %s is IDLE (not charging or waiting).\n", licenseNum);
    }
}

/* searches a car queue for the first car that requires a specific port type
   if a match is found, it removes the car from the queue and returns it
    input: CarQueue* q - a pointer to the car queue to be searched
    input: PortType type - the type of charging port the car must be compatible with
    input: int* nCars - a pointer to the station's car counter
   output: Car* (returns a pointer to the dequeued car, or NULL if no match is found) */
static Car* findAndDequeueMatchingCar(CarQueue* q, PortType type, int* nCars) {
    if (isQueueEmpty(q)) return NULL;
    QueueNode* prev = NULL;
    QueueNode* current = q->front;
    while (current != NULL) {
        if (current->p2car->portType == type) {
            Car* matchingCar = current->p2car;
            if (prev == NULL) q->front = current->next;
            else prev->next = current->next;
            if (q->rear == current) q->rear = prev;
            free(current);
            (*nCars)--;
            return matchingCar;
        }
        prev = current;
        current = current->next;
    }
    return NULL;
}

/* stops an active charging session for a car specified by its license number
   it calculates the session time and cost, updates the car and port status,
   and then assign the newly freed port to the right car from the stations queue
    input: Station* stationsRoot - root of the station data structure, used to find the cars location
    input: tCar* carsRoot - root of the car data structure, used to find the car by its license number */
void stopCharge(Station* stationsRoot, tCar* carsRoot) {
    char licenseNum[10];
    printf("\n--- Stop Charging ---\n");
    printf("Enter Car License Number: ");
    if (scanf("%9s", licenseNum) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return;
    }
    Car* car = searchCar(carsRoot, licenseNum);
    if (!car || !car->pPort) {
        printf("ERROR: Car %s is not currently charging.\n", licenseNum);
        return;
    }
    Port* port = car->pPort;
    Station* station = findStationWithCar(stationsRoot, licenseNum);
    if (!station) {
        printf("CRITICAL ERROR: Car is linked to a port, but its station could not be found.\n");
        car->pPort = NULL;
        return;
    }
    double rates[] = { 1.5, 1.0, 0.5 };
    time_t now_t = time(NULL);
    struct tm start_tm = { 0 };
    start_tm.tm_year = port->chargeStartTime.year - 1900;
    start_tm.tm_mon = port->chargeStartTime.month - 1;
    start_tm.tm_mday = port->chargeStartTime.day;
    start_tm.tm_hour = port->chargeStartTime.hour;
    start_tm.tm_min = port->chargeStartTime.min;
    time_t start_t = mktime(&start_tm);
    double duration_min = difftime(now_t, start_t) / 60.0;
    if (duration_min < 1) duration_min = 1;
    double cost = duration_min * rates[port->portType];
    car->totalPayed += cost;
    printf("Charging stopped for car %s at station %s.\n", car->nLicense, station->name);
    printf("Duration: %.2f minutes.\n", duration_min);
    printf("Cost for this session: %.2f. Total paid by customer: %.2f\n", cost, car->totalPayed);
    port->p2car = NULL;
    port->status = FREE;
    car->pPort = NULL;
    printf("Searching for a suitable car in the queue for the freed %s port...\n",
        port->portType == PORT_TYPE_FASR ? "FAST" : (port->portType == PORT_TYPE_MID ? "MID" : "SLOW"));
    Car* nextCar = findAndDequeueMatchingCar(&station->carQueue, port->portType, &station->nCars);
    if (nextCar) {
        assignCar2port(nextCar, port);
        printf("------------------------------------------------------------------\n");
        printf("    Found matching car! %s is now charging at port %d.   \n", nextCar->nLicense, port->num);
        printf("------------------------------------------------------------------\n");
    }
    else {
        printf("No suitable car found in the queue for this specific port type.\n");
    }
}



/* prints detailed information for a single station, including a breakdown of its charging ports by status 
    input: Station* station - a pointer to the station whose details will be printed */
static void printStationInfo(Station* station) {
    if (!station) return;

    int occupied = 0, available = 0, out_of_order = 0;
    Port* p = station->port_list_head;
    while (p) {
        if (p->status == OCCUPIED) occupied++;
        else if (p->status == FREE) available++;
        else if (p->status == OUT_OF_ORDER) out_of_order++;
        p = p->next;
    }
    int total_ports_from_list = occupied + available + out_of_order; // get the total number of ports from the list
    printf("ID: %-4d | Name: %-20s | Ports: %d (O: %d, F: %d, X: %d) | Queue: %d\n",
        station->id,
        station->name,
       total_ports_from_list,// use the total 
        occupied,
        available,
        out_of_order,// display the out-of-order count
        station->nCars);
}

/* starts the binary tree of stations using an in-order algorithm,
   calling a helper function to print the details of each station in sorted order
    input: Station* root - the root of the station binary tree to be displayed */
static void displayStationsInOrder(Station* root) {
    if (root == NULL) return;
    displayStationsInOrder(root->left);
    printStationInfo(root);
    displayStationsInOrder(root->right);
}


// display all stations in the system 
void dispAllSt(Station* stationsRoot) {
    printf("\n--- All Charging Stations ---\n");
    if (!stationsRoot) { printf("No stations in the system.\n"); return; }
    displayStationsInOrder(stationsRoot);
    printf("-----------------------------\n");
}


/* prompts the user for a station ID and then displays a detailed list of cars
   associated with that station, separated into cars currently charging and cars waiting in the queue
    input: Station* stationsRoot - the root of the station data structure, used to search for the station by ID */
void dispCarsAtSt(Station* stationsRoot) {
    int stationId;
    printf("\n--- Display Cars at Station ---\n");
    printf("Enter Station ID: ");
    if (scanf("%d", &stationId) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return; }
    Station* station = searchStationByID(stationsRoot, stationId);
    if (!station) { printf("ERROR: Station with ID %d not found.\n", stationId); return; }
    printf("\n--- Status for Station: %s (ID: %d) ---\n", station->name, station->id);
    printf("Cars Currently Charging:\n");
    int chargingCount = 0;
    Port* p = station->port_list_head;
    while (p) {
        if (p->status == OCCUPIED && p->p2car) {
            printf("  - Port %d: Car %s\n", p->num, p->p2car->nLicense);
            chargingCount++;
        }
        p = p->next;
    }
    if (chargingCount == 0) printf("  (None)\n");
    printf("\nCars in Waiting Queue (%d):\n", station->nCars);
    if (isQueueEmpty(&station->carQueue)) {
        printf("  (Queue is empty)\n");
    }
    else {
        QueueNode* current = station->carQueue.front;
        int i = 1;
        while (current) {
            printf("  %d. Car %s\n", i++, current->p2car->nLicense);
            current = current->next;
        }
    }
    printf("----------------------------------------\n");
}


/* initiates the station statistics report process by prompting the user for a station ID and validating its existence before printing a report 
    input: Station* stationsRoot - the root of the station data structure, used to find the station */
void reportStStat(Station* stationsRoot) {
    int stationId;
    printf("\n--- Station Statistics Report ---\n");
    printf("Enter Station ID: ");
    if (scanf("%d", &stationId) != 1){
        while (getchar() != '\n');
        printf("Invalid input.\n"); 
        return; 
    }

    Station* station = searchStationByID(stationsRoot, stationId);
    if (!station) {
        printf("ERROR: Station with ID %d not found.\n", stationId); 
        return;
    }
    printf("\n--- Statistics for Station: %s (ID: %d) ---\n", station->name, station->id);

  
    // count totals first to display it
    int total = 0, occupied = 0, out_of_order = 0, free_ports = 0;
    Port* p_counter = station->port_list_head;
    while (p_counter) {
        total++;
        if (p_counter->status == OCCUPIED) occupied++;
        else if (p_counter->status == OUT_OF_ORDER) out_of_order++;
        else free_ports++;
        p_counter = p_counter->next;
    }
    printf("Total Ports: %d\n", total);

    // print details for each category by iterating again
    Port* p_printer = station->port_list_head;

    // print occupied ports
    printf("  - Occupied Ports (%d): ", occupied);
    if (occupied == 0) {
        printf("None\n");
    }
    else {
        int first = 1;
        while (p_printer) {
            if (p_printer->status == OCCUPIED) {
                if (!first) printf(", ");
                printf("Port #%d (Car: %s)", p_printer->num, p_printer->p2car->nLicense);
                first = 0;
            }
            p_printer = p_printer->next;
        }
        printf("\n");
    }

    // print Out-of-Order ports
    p_printer = station->port_list_head; // reset iterator
    printf("  - Out-of-Order Ports (%d): ", out_of_order);
    if (out_of_order == 0) {
        printf("None\n");
    }
    else {
        int first = 1;
        while (p_printer) {
            if (p_printer->status == OUT_OF_ORDER) {
                if (!first) printf(", ");
                printf("Port #%d", p_printer->num);
                first = 0;
            }
            p_printer = p_printer->next;
        }
        printf("\n");
    }

    // print free Ports with type breakdown
    p_printer = station->port_list_head; // reset iterator
    printf("  - Free Ports (%d):\n", free_ports);
    if (free_ports == 0) {
        printf("    None\n");
    }
    else {
        const char* typeStr[] = { "FAST", "MID", "SLOW" };
        int printed_for_type[3] = { 0,0,0 };

        // print FAST ports
        printf("    - FAST: ");
        while (p_printer) {
            if (p_printer->status == FREE && p_printer->portType == PORT_TYPE_FASR) {
                if (printed_for_type[0]++ > 0) printf(", ");
                printf("#%d", p_printer->num);
            }
            p_printer = p_printer->next;
        }
        if (printed_for_type[0] == 0) printf("None");
        printf("\n");

        // print MID ports
        p_printer = station->port_list_head; // reset
        printf("    - MID:  ");
        while (p_printer) {
            if (p_printer->status == FREE && p_printer->portType == PORT_TYPE_MID) {
                if (printed_for_type[1]++ > 0) printf(", ");
                printf("#%d", p_printer->num);
            }
            p_printer = p_printer->next;
        }
        if (printed_for_type[1] == 0) printf("None");
        printf("\n");

        // print SLOW ports
        p_printer = station->port_list_head; // reset
        printf("    - SLOW: ");
        while (p_printer) {
            if (p_printer->status == FREE && p_printer->portType == PORT_TYPE_SLOW) {
                if (printed_for_type[2]++ > 0) printf(", ");
                printf("#%d", p_printer->num);
            }
            p_printer = p_printer->next;
        }
        if (printed_for_type[2] == 0) printf("None");
        printf("\n");
    }

    printf("Cars waiting in queue: %d\n", station->nCars);
    printf("--------------------------------------------\n");
}
   

/* adds a new charging port to a specified station this function prompts the user for a station ID and a port type 
it then automatically assigns a new, unique port number, creates the port, and links it to the station
   input: Station* stationsRoot - the root of the station data structure, used to find the target station */
void addNewPort(Station* stationsRoot) {
    int stationId;
    printf("\n--- Add New Port ---\n");
    printf("Enter Station ID to add a port to: ");
    if (scanf("%d", &stationId) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return; }
    Station* station = searchStationByID(stationsRoot, stationId);
    if (!station) { printf("ERROR: Station with ID %d not found.\n", stationId); return; }
    int typeChoice;
    printf("Select new port type (0: FAST, 1: MID, 2: SLOW): ");
    if (scanf("%d", &typeChoice) != 1 || typeChoice < 0 || typeChoice > 2) { while (getchar() != '\n'); printf("Invalid choice.\n"); return; }
    PortType newType = (PortType)typeChoice;
    int maxPortNum = 0;
    Port* p = station->port_list_head;
    while (p) {
        if (p->num > maxPortNum) maxPortNum = p->num;
        p = p->next;
    }
    Port* newPort = createPort(maxPortNum + 1, newType);
    if (!newPort) { fprintf(stderr, "ERROR: Failed to create a new port.\n"); return; }
    addPortToList(&station->port_list_head, newPort);
    station->num_Ports++;
    printf("Successfully added new %s port (ID: %d) to station %s.\n",
        newType == PORT_TYPE_FASR ? "FAST" : (newType == PORT_TYPE_MID ? "MID" : "SLOW"),
        newPort->num, station->name);
}


/* recursively traverses a tree of stations to free all associated ports,
   counting the total number of ports freed during the process
    input: Station* station - a pointer to the current station node in the tree
    input: int* releasedCount - a pointer to a counter that tracks the total number of freed ports */
static void releasePortsRecursive(Station* station, int* releasedCount) {
    if (station == NULL) {
        return;
    }

    // current station 
    Port* port = station->port_list_head;
    const double MAX_CHARGE_MINUTES = 120.0; // set a limit for 2 hours

    while (port != NULL) {
        if (port->status == OCCUPIED && port->p2car != NULL) {
            // calculate charging duration
            time_t now_t = time(NULL);
            struct tm start_tm = { 0 };
            start_tm.tm_year = port->chargeStartTime.year - 1900;
            start_tm.tm_mon = port->chargeStartTime.month - 1;
            start_tm.tm_mday = port->chargeStartTime.day;
            start_tm.tm_hour = port->chargeStartTime.hour;
            start_tm.tm_min = port->chargeStartTime.min;
            time_t start_t = mktime(&start_tm);

            double duration_min = difftime(now_t, start_t) / 60.0;

            // check if the duration exceeds the limit
            if (duration_min > MAX_CHARGE_MINUTES) {
                printf("Port %d at station %d: Car %s has been charging for %.0f minutes. Releasing port.\n",
                    port->num, station->id, port->p2car->nLicense, duration_min);

                // release the port 
                Car* car_to_release = port->p2car;

                // set a cost
                double cost = duration_min * 0.5; // cost calculation
                car_to_release->totalPayed += cost;

                // free up the port and the car
                port->p2car = NULL;
                port->status = FREE;
                car_to_release->pPort = NULL;

                (*releasedCount)++; // up the counter of released cars

                // check if a car from the queue can now charge
                Car* nextCar = findAndDequeueMatchingCar(&station->carQueue, port->portType, &station->nCars);
                if (nextCar) {
                    assignCar2port(nextCar, port);
                    printf("  -> AUTO-ASSIGN: Car %s from the queue is now charging at this port.\n", nextCar->nLicense);
                }
            }
        }
        port = port->next;
    }

    releasePortsRecursive(station->left, releasedCount);
    releasePortsRecursive(station->right, releasedCount);
}


// the main function called from the menu
void releasePorts(Station* stationsRoot) {
    printf("\n--- Releasing Long-Charging Ports (Threshold: 120 minutes) ---\n");
    if (stationsRoot == NULL) {
        printf("No stations in the system.\n");
        return;
    }

    int releasedCount = 0;
    releasePortsRecursive(stationsRoot, &releasedCount);

    if (releasedCount == 0) {
        printf("No ports exceeded the charging time limit.\n");
    }
    else {
        printf("\nFinished. A total of %d ports were released.\n", releasedCount);
    }
}


/* removes a specific charging port from a station, but only if that port is marked as out-of-order
   it prompts the user for the station and port IDs, validates the port's status, and then unlinks it from the list if the criteria are met
    input: Station* stationsRoot - the root of the station data structure, used to find the target stationn */
void remOutOrderPort(Station* stationsRoot) {
    int stationId, portNum;
    printf("\n--- Remove Out-of-Order Port ---\n");
    printf("Enter Station ID: ");
    if (scanf("%d", &stationId) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return; }
    Station* station = searchStationByID(stationsRoot, stationId);
    if (!station) { printf("ERROR: Station with ID %d not found.\n", stationId); return; }
    printf("Enter number of the port to remove: ");
    if (scanf("%d", &portNum) != 1) { while (getchar() != '\n'); printf("Invalid input.\n"); return; }
    Port** pp = &station->port_list_head;
    while (*pp) {
        Port* entry = *pp;
        if (entry->num == portNum) {
            if (entry->status == OUT_OF_ORDER) {
                *pp = entry->next;
                free(entry);
                station->num_Ports--;
                printf("Port %d at station %d has been removed.\n", portNum, stationId);
                return;
            }
            else {
                printf("ERROR: Port %d is not out-of-order. It is currently %s.\n", portNum,
                    entry->status == FREE ? "FREE" : "OCCUPIED");
                return;
            }
        }
        pp = &(*pp)->next;
    }
    printf("ERROR: Port %d not found at station %d.\n", portNum, stationId);
}


/* finds the node with the minimum value in a Binary Search Tree this helper function starts at a given node and repeatedly
traverses to the left child, as the leftmost node in a BST is always the one with the minimum value
    input: Station* node - the root of the tree or subtree to be searched */
static Station* findMinStation(Station* node) {
    Station* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

/* recursively searches for and deletes a station node from a Binary Search Tree 
    input: Station* root - the root of the tree (or subtree) to search within
    input: int id - the ID of the station to delete 
   output: Station* (returns the new root of the modified subtree after deletion) */

static Station* deleteStationNode(Station* root, int id) {
    if (root == NULL) return root;
    if (id < root->id) {
        root->left = deleteStationNode(root->left, id);
    }
    else if (id > root->id) {
        root->right = deleteStationNode(root->right, id);
    }
    else {
        if (root->nCars > 0) {
            printf("ERROR: Cannot close station. %d cars are in the queue.\n", root->nCars);
            return root;
        }
        Port* p = root->port_list_head;
        while (p) {
            if (p->status == OCCUPIED) {
                printf("ERROR: Cannot close station. Car %s is charging at port %d.\n", p->p2car->nLicense, p->num);
                return root;
            }
            p = p->next;
        }
        printf("Closing station %s (ID: %d). All data will be freed.\n", root->name, root->id);

        if (root->left == NULL) {
            Station* temp = root->right;
            free(root->name);
            freePortsList(root->port_list_head);
            freeQueue(&root->carQueue);
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Station* temp = root->left;
            free(root->name);
            freePortsList(root->port_list_head);
            freeQueue(&root->carQueue);
            free(root);
            return temp;
        }

        Station* temp = findMinStation(root->right);
        root->id = temp->id;
        free(root->name);
        root->name = (char*)malloc(strlen(temp->name) + 1);
        strcpy(root->name, temp->name);
        root->right = deleteStationNode(root->right, temp->id);
    }
    return root;
}


/* manages the user-facing process of closing a station. it prompts the user for a
   station ID, performs safety checks to ensure no cars are waiting or charging,
   and if the checks pass, calls the recursive helper function to delete the station from the tree
    input: Station* stationsRoot - the root of the station binary search tree
   output: Station* (returns the new root of the tree, which may change if the original root is deleted) */
Station* closeSt(Station* stationsRoot) {
    int stationId;
    printf("\n--- Close Station ---\n");
    printf("Enter ID of the station to close: ");
    if (scanf("%d", &stationId) != 1) {
        while (getchar() != '\n');
        return stationsRoot;
    }
    Station* stationToClose = searchStationByID(stationsRoot, stationId);
    if (!stationToClose) { printf("ERROR: Station with ID %d not found.\n", stationId); return stationsRoot; }
    if (stationToClose->nCars > 0) { printf("ERROR: Cannot close station %d. There are %d cars waiting in the queue.\n", stationId, stationToClose->nCars); return stationsRoot; }
    Port* p = stationToClose->port_list_head;
    while (p) {
        if (p->status == OCCUPIED) {
            printf("ERROR: Cannot close station %d. A car is charging at port %d.\n", stationId, p->num);
            return stationsRoot;
        }
        p = p->next;
    }
    printf("Proceeding to close station %d. This will remove it from the system.\n", stationId);
    return deleteStationNode(stationsRoot, stationId);
}

/* recursively searches the entire station tree to find the station where a
   specific car is either charging or waiting in the queue
    input: Station* root - the root of the station tree to search
    input: const char* license - the license plate number of the car to find
   output: Station* (returns a pointer to the station where the car is located, or NULL if not found) */
static Station* findStationWithCar(Station* root, const char* license) {
    if (!root) return NULL;
    Port* p = root->port_list_head;
    while (p) {
        if (p->p2car && strcmp(p->p2car->nLicense, license) == 0) {
            return root;
        }
        p = p->next;
    }
    QueueNode* qNode = root->carQueue.front;
    while (qNode) {
        if (strcmp(qNode->p2car->nLicense, license) == 0) {
            return root;
        }
        qNode = qNode->next;
    }
    Station* found = findStationWithCar(root->left, license);
    if (found) return found;
    return findStationWithCar(root->right, license);
}