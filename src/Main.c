#define _CRT_SECURE_NO_WARNINGS
#include "System.h"
#include "Station.h"
#include "Car.h"

int main() {
    Station* stationsRoot = NULL;
    tCar* carsRoot = NULL;
    int choice;
    int needs_update = 0; // signal to determine if files need to be saved

    printf("Loading system data from files...\n");
    loadFiles(&stationsRoot, &carsRoot);
    printf("Data loaded.\n");

        // this is the main program loop. it continues until the user chooses to exit
    do {
        displayMenu();

        // get and check user input
        if (scanf("%d", &choice) != 1) {
            printf("\nERROR: Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // clear the input buffer to prevent a loop
            choice = -1; // set to an invalid choice to force the loop to repeat
            continue;
        }

        // assume most actions will modify data. display-only functiond will reset this
        needs_update = 1;

        // call the write function based on the user choice
        switch (choice) {
        case 1: locNearSt(stationsRoot); needs_update = 0; break;
        case 2: chargeCar(stationsRoot, &carsRoot); break;
        case 3: checkCarStatus(stationsRoot, carsRoot); needs_update = 0; break;
        case 4: stopCharge(stationsRoot, carsRoot); break;
        case 5: dispAllSt(stationsRoot); needs_update = 0; break;
        case 6: dispCarsAtSt(stationsRoot); needs_update = 0; break;
        case 7: reportStStat(stationsRoot); needs_update = 0; break;
        case 8: dispTopCustomers(carsRoot); needs_update = 0; break;
        case 9: addNewPort(stationsRoot); break;
        case 10: releasePorts(stationsRoot); break;
        case 11: remOutOrderPort(stationsRoot); break;
        case 12: carsRoot = remCustomer(carsRoot, stationsRoot); break;
        case 13: stationsRoot = closeSt(stationsRoot); break;
        case 0: exitSys(stationsRoot, carsRoot); needs_update = 0; break;
        default:
            printf("\nERROR: Invalid choice. Please try again.\n");
            needs_update = 0;
            break;
        }

        // update files after every change made by user
        if (needs_update) {
            updateFiles(stationsRoot, carsRoot);
        }

    } while (choice != 0);

    return 0;
}