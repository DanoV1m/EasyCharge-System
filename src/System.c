#define _CRT_SECURE_NO_WARNINGS
#include "System.h"
#define MAX_LINE_LEN 256  // defines the maximum length for a line buffer when reading lines from files

/* this is a helper function to convert a port type string to its enum equivalent
   input: const char* str - the string to convert
   output: PortType - the corresponding enum value */
static PortType stringToPortType(const char* str) {
    if (strcmp(str, "FAST") == 0)
        return PORT_TYPE_FASR;
    if (strcmp(str, "MID") == 0)
        return PORT_TYPE_MID;
    return PORT_TYPE_SLOW;
}

// loads all data from txt files into the systems memory
void loadFiles(Station** stationsRoot, tCar** carsRoot) {
    FILE* fp = NULL;
    char line_buffer[MAX_LINE_LEN];

    // load Stations from data folder
    if ((fp = fopen("data/Stations.txt", "r")) == NULL) { 
        perror("Error opening data/Stations.txt"); 
        return; 
    }
    fgets(line_buffer, sizeof(line_buffer), fp); // skip header
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
        int id, numPorts; char name[100]; double x, y;
        sscanf(line_buffer, "%d,%[^,],%d,%lf,%lf", &id, name, &numPorts, &x, &y);
        *stationsRoot = addStationToBST(*stationsRoot, createStation(id, name, numPorts, x, y));
    }
    fclose(fp);

    // load Cars from data folder
    if ((fp = fopen("data/Cars.txt", "r")) == NULL) { 
        perror("Error opening data/Cars.txt"); 
        return; 
    }
    fgets(line_buffer, sizeof(line_buffer), fp); // skip header
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
        char license[10], portTypeStr[10]; double totalPayed; int inQueue;
        sscanf(line_buffer, "%[^,],%[^,],%lf,%*d,%*d,%d", license, portTypeStr, &totalPayed, &inQueue);
        Car* newCar = createCar(license, stringToPortType(portTypeStr));
        if (newCar) { // safety check
            newCar->totalPayed = totalPayed;
            newCar->inqueue = inQueue;
            *carsRoot = addCarToBST(*carsRoot, newCar);
        }
    }
    fclose(fp);

    // load Ports from data folder
    if ((fp = fopen("data/Ports.txt", "r")) == NULL) { 
        perror("Error opening data/Ports.txt"); 
        return; 
    }
    fgets(line_buffer, sizeof(line_buffer), fp); // skip header
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
        int stationId, portNum, status, year, month, day, hour, min;
        char portTypeStr[10], carLicense[20] = "NULL";
        sscanf(line_buffer, "%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%s", &stationId, &portNum, portTypeStr, &status, &year, &month, &day, &hour, &min, carLicense);
        Station* targetStation = searchStationByID(*stationsRoot, stationId);
        if (targetStation) {
            Port* newPort = createPort(portNum, stringToPortType(portTypeStr));
            if (newPort) { // safety check
                newPort->status = (PortStatus)status;
                addPortToList(&targetStation->port_list_head, newPort);
                if (newPort->status == OCCUPIED && strcmp(carLicense, "NULL") != 0) {
                    Car* targetCar = searchCar(*carsRoot, carLicense);
                    if (targetCar) {
                        assignCar2port(targetCar, newPort);
                        newPort->chargeStartTime = (Date){ year, month, day, hour, min };
                    }
                }
            }
        }
    }
    fclose(fp);

    // load Car Queues from data folder
    if ((fp = fopen("data/LineOfCars.txt", "r")) == NULL) { 
        perror("Error opening data/LineOfCars.txt"); 
        return; 
    }
    fgets(line_buffer, sizeof(line_buffer), fp); // skip header
    while (fgets(line_buffer, sizeof(line_buffer), fp)) {
        int stationId; char carLicense[20];
        sscanf(line_buffer, "%[^,],%d", carLicense, &stationId);
        Station* targetStation = searchStationByID(*stationsRoot, stationId);
        Car* targetCar = searchCar(*carsRoot, carLicense);
        if (targetStation && targetCar) {
            enqueue(&targetStation->carQueue, targetCar);
            targetStation->nCars++;
            targetCar->inqueue = 1; 
        }
    }
    fclose(fp);
}

static const char* portTypeToString(PortType type) {
    if (type == PORT_TYPE_FASR) return "FAST";
    if (type == PORT_TYPE_MID) return "MID";
    return "SLOW";
}

static void saveCarsRecursive(FILE* fp, tCar* root) {
    if (!root) return;
    saveCarsRecursive(fp, root->left);
    Car* car = root->p2car;
    fprintf(fp, "%s,%s,%.2f,%d,%d,0\n", car->nLicense, portTypeToString(car->portType), car->totalPayed, (car->pPort != NULL), car->inqueue);
    saveCarsRecursive(fp, root->right);
}

static void saveStationsRecursive(FILE* stations_fp, FILE* ports_fp, FILE* queues_fp, Station* root) {
    if (!root) return;
    saveStationsRecursive(stations_fp, ports_fp, queues_fp, root->left);
    fprintf(stations_fp, "%d,%s,%d,%.2f,%.2f\n", root->id, root->name, root->num_Ports, root->coord.x, root->coord.y);
    Port* p = root->port_list_head;
    while (p) {
        Date* dt = &p->chargeStartTime;
        char* carLicense = (p->p2car) ? p->p2car->nLicense : "NULL";
        fprintf(ports_fp, "%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n", root->id, p->num, portTypeToString(p->portType), p->status, dt->year, dt->month, dt->day, dt->hour, dt->min, carLicense);
        p = p->next;
    }
    QueueNode* qn = root->carQueue.front;
    while (qn) {
        fprintf(queues_fp, "%s,%d\n", qn->p2car->nLicense, root->id);
        qn = qn->next;
    }
    saveStationsRecursive(stations_fp, ports_fp, queues_fp, root->right);
}

void updateFiles(Station* stationsRoot, tCar* carsRoot) {
    FILE* fp = NULL;

    // save Cars to data folder
    if ((fp = fopen("data/Cars.txt", "w")) == NULL) { 
        perror("Error opening data/Cars.txt for writing"); 
        return; 
    }
    fprintf(fp, "License,Type,TotalPaid,IsCharging,IsInQueue,DummyValue\n");
    saveCarsRecursive(fp, carsRoot);
    fclose(fp);

    FILE* stations_fp = fopen("data/Stations.txt", "w");
    FILE* ports_fp = fopen("data/Ports.txt", "w");
    FILE* queues_fp = fopen("data/LineOfCars.txt", "w");

    if (!stations_fp || !ports_fp || !queues_fp) {
        perror("Error opening data files for writing");
        if (stations_fp) fclose(stations_fp);
        if (ports_fp) fclose(ports_fp);
        if (queues_fp) fclose(queues_fp);
        return;
    }

    fprintf(stations_fp, "ID,Name,NumPorts,CoordX,CoordY\n");
    fprintf(ports_fp, "StationID,PortNum,Type,Status,Year,Month,Day,Hour,Min,CarLicense\n");
    fprintf(queues_fp, "CarLicense,StationID\n");

    saveStationsRecursive(stations_fp, ports_fp, queues_fp, stationsRoot);

    fclose(stations_fp);
    fclose(ports_fp);
    fclose(queues_fp);

    printf("System state saved to data folder.\n");
}

void displayMenu() {
    printf("\n\n--- EasyCharge System Menu ---\n");
    printf("1. Locate nearest station\n");
    printf("2. Charge car\n");
    printf("3. Check car status\n");
    printf("4. Stop charging\n");
    printf("5. Display all stations\n");
    printf("6. Display cars at a station\n");
    printf("7. Station statistics report\n");
    printf("8. Display top 5 customers\n");
    printf("9. Add new port\n");
    printf("10. Release long-charging ports\n");
    printf("11. Remove out-of-order port\n");
    printf("12. Remove customer\n");
    printf("13. Close station\n");
    printf("0. Exit System\n");
    printf("--------------------------------\n");
    printf("Enter your choice: ");
}

void exitSys(Station* stationsRoot, tCar* carsRoot) {
    printf("\nSystem shutting down...\n");
    updateFiles(stationsRoot, carsRoot);
    printf("Freeing all system memory...\n");
    freeStationsBST(stationsRoot);
    freeCarsBST(carsRoot);
    printf("System shut down successfully. see you later!\n");
}