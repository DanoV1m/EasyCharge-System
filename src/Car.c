#define _CRT_SECURE_NO_WARNINGS
#include "Car.h"
#include "Station.h" 
#include "Queue.h"  

/* this function creates a new Car struct in dynamic memory and initializes its fields
  input: const char* license - the car's license plate number
  input: PortType type - the car's preferred charging type
 output: Car* - a pointer to the newly allocated Car struct, or NULL on failure */
Car* createCar(const char* license, PortType type) {
    Car* pCar = (Car*)malloc(sizeof(Car));
    if (!pCar) {
        return NULL;
    }
    strcpy(pCar->nLicense, license);
    pCar->portType = type;
    pCar->totalPayed = 0;
    pCar->pPort = NULL;
    pCar->inqueue = 0;
    return pCar;
}

/* this function recursively adds a new car to the car Binary Search Tree, sorted by license number
  input: tCar* root - the root of the BST to add the car to
  input: Car* car - a pointer to the Car data to be added
 output: tCar* - the new root of the BST */

tCar* addCarToBST(tCar* root, Car* car) {
    if (root == NULL) {
        tCar* node = (tCar*)malloc(sizeof(tCar));
        if (!node) {
            fprintf(stderr, "ERROR: Memory allocation failed for new car tree node\n");
            return NULL;
        }
        node->p2car = car;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    int diff = strcmp(car->nLicense, root->p2car->nLicense);
    if (diff < 0) {
        root->left = addCarToBST(root->left, car);
    }
    else if (diff > 0) {
        root->right = addCarToBST(root->right, car);
    }
    return root;
}

/* this function searches for a car in the BST by its license number
  input: tCar* root - the root of the car BST to search
  input: const char* licenseNum - the license plate to search for
 output: Carr* - a pointer to the found Cars data, or NULL if not found */

Car* searchCar(tCar* root, const char* licenseNum) {
    tCar* p = root;
    while (p) {
        int diff = strcmp(licenseNum, p->p2car->nLicense);
        if (diff == 0) return p->p2car;
        if (diff < 0) p = p->left;
        else p = p->right;
    }
    return NULL;
}

// recursively frees the entire car BST using post-order traversal
void freeCarsBST(tCar* root) {
    if (!root) return;
    freeCarsBST(root->left);
    freeCarsBST(root->right);
    free(root->p2car);
    free(root);
}


// functions for displaying top customers

static int countCars(tCar* root) {
    if (root == NULL) return 0;
    return 1 + countCars(root->left) + countCars(root->right);
}


/* recursively traverses a binary search tree of cars using an in-order
    input: tCar* root - the root of the car binary tree / subtree to traverse
    input: Car** carArray - the dynamic array that is being populated with pointers to cars
    input: int* index - a pointer to the current insertion index in the carArray */
static void populateCarArray(tCar* root, Car** carArray, int* index) {
    if (root == NULL) return;
    populateCarArray(root->left, carArray, index);
    carArray[*index] = root->p2car;
    (*index)++;
    populateCarArray(root->right, carArray, index);
}

/* a comparison function for qsort to sort customers in descending order of payment
  input: const void* a - first element to compare
  input: const void* b - second element to compare
 output: int - negative, zero, or positive */
static int compareCarsByPayment(const void* a, const void* b) {
    Car* carA = *(Car**)a;
    Car* carB = *(Car**)b;
    if (carB->totalPayed > carA->totalPayed) return 1;
    if (carB->totalPayed < carA->totalPayed) return -1;
    return 0;
}

/* displays the top 5 customers based on thier stats
    input: tCar* root - the root of the car data structure to be analyzed */
void dispTopCustomers(tCar* root) {
    printf("\n--- Top 5 Customers by Total Payment ---\n");
    if (!root) {
        printf("No customer data available.\n");
        return;
    }
    int totalCars = countCars(root);
    if (totalCars == 0) {
        printf("No customer data available.\n");
        return;
    }
    Car** carArray = (Car**)malloc(totalCars * sizeof(Car*));
    if (!carArray) {
        fprintf(stderr, "ERROR: Memory allocation failed for sorting customers.\n");
        return;
    }
    int index = 0;
    populateCarArray(root, carArray, &index);
    qsort(carArray, totalCars, sizeof(Car*), compareCarsByPayment);
    int count = (totalCars < 5) ? totalCars : 5;
    printf("Displaying top %d customers:\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d. License: %s, Total Paid: %.2f\n", i + 1, carArray[i]->nLicense, carArray[i]->totalPayed);
    }
    free(carArray);
}

// helper function to find the minimum value node in a tCar BST
static tCar* findMin(tCar* node) {
    tCar* current = node;
    while (current && current->left != NULL) {
        current = current->left;
    }
    return current;
}

// this function frees and deletes the tCar node and the Car data it points to
static tCar* deleteCarNode(tCar* root, const char* license) {
    if (root == NULL) {
        return NULL;
    }

    int diff = strcmp(license, root->p2car->nLicense);

    if (diff < 0) {
        root->left = deleteCarNode(root->left, license);
    }
    else if (diff > 0) {
        root->right = deleteCarNode(root->right, license);
    }
    else { // found the node to be deleted
          //  node with only one child or no child
        if (root->left == NULL) {
            tCar* temp = root->right;
            free(root->p2car); // free the car data
            free(root);       // free the BST node
            return temp;
        }
        else if (root->right == NULL) {
            tCar* temp = root->left;
            free(root->p2car); // free the car data
            free(root);       // free the BST node
            return temp;
        }

        // find the inorder successor
        tCar* temp = findMin(root->right); 

        free(root->p2car);
        root->p2car = temp->p2car;

         // now, the original root node has the correct data
        // find the right node 
        tCar* successor = findMin(root->right);

        // copy the car data pointer to the current node
        Car* carToFree = root->p2car;
        root->p2car = successor->p2car;
        successor->p2car = carToFree; // temporarily give the old data to the successor
        // so it can be found and freed correctly

        // recursively delete the successor, which now holds the data to be freed
        root->right = deleteCarNode(root->right, carToFree->nLicense);
    }
    return root;
}


 // removes a specific car from a single stations queue
// returns 1 if found and removed, 0 otherwise
static int removeCarFromSingleQueue(CarQueue* q, const char* license, int* nCars) {
    if (isQueueEmpty(q)) return 0;

    QueueNode* prev = NULL;
    QueueNode* current = q->front;

    while (current != NULL) {
        if (strcmp(current->p2car->nLicense, license) == 0) {
            // found the car. unlink the QueueNode from the list
            if (prev == NULL) q->front = current->next; // it was the first node
            else prev->next = current->next; // it was in the middle/end

            if (q->rear == current) q->rear = prev; // update rear if it was the last node

            free(current); // free the QueueNode memory
            (*nCars)--;    
            return 1;    
        }
        prev = current;
        current = current->next;
    }
    return 0; // car not found in this queue
}

// traverses all stations to find and remove a car from any queue it might be in
static void findAndRemoveCarFromAllQueues(Station* stationsRoot, const char* license) {
    if (stationsRoot == NULL) return;

    findAndRemoveCarFromAllQueues(stationsRoot->left, license);
        // process current stations queue
    if (removeCarFromSingleQueue(&stationsRoot->carQueue, license, &stationsRoot->nCars)) {
        // we can stop searching if we found and removed it, but in case the data comes wrong in the files we keep searching
    }
    findAndRemoveCarFromAllQueues(stationsRoot->right, license);
}

// the main function to remove a customer from the entire system
tCar* remCustomer(tCar* root, Station* stationsRoot) {
    char licenseNum[10];
    printf("\n--- Remove Customer ---\n");
    printf("Enter license number of the car to remove: ");
    if (scanf("%9s", licenseNum) != 1) {
        while (getchar() != '\n');
        printf("Invalid input.\n");
        return root;
    }

    Car* carToRemove = searchCar(root, licenseNum);
    if (!carToRemove) {
        printf("ERROR: Customer with license %s not found.\n", licenseNum);
        return root;
    }

    // a customer cant be removed if their car is charging
    if (carToRemove->pPort != NULL) {
        printf("ERROR: Cannot remove customer. Car %s is currently charging.\n", licenseNum);
        return root;
    }

    // if the car is in a queue, remove it from there first
    if (carToRemove->inqueue) {
        findAndRemoveCarFromAllQueues(stationsRoot, licenseNum);
        printf("Car %s was found in a waiting queue and has been removed from it.\n", licenseNum);
    }

     // now that the car is idle, remove it from the main BST
    // this will also free the memory of the Car object 
    root = deleteCarNode(root, licenseNum);
    printf("Customer with license %s has been successfully removed from the system.\n", licenseNum);

    return root;
}