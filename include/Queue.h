#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>

// forward declaration of the Car struct
struct Car;

// definition of the QueueNode struct
typedef struct QueueNode {
    struct Car* p2car; // pointer to a Car
    struct QueueNode* next; // pointer to the next node in the queue
} QueueNode;

// definition of the CarQueue struct
typedef struct {
    QueueNode* front;  // pointer to the front of the queue
    QueueNode* rear;  // pointer to the rear of the queue
} CarQueue;

// function prototypes
void initQueue(CarQueue* q);                    // prepares a new queue to be used 
int isQueueEmpty(const CarQueue* q);            // checks if theres anything in the queue
void enqueue(CarQueue* q, struct Car* car);     // adds an item to the end of the queue
struct Car* dequeue(CarQueue* q);               // removes and returns the item from the front
void freeQueue(CarQueue* q);                    // cleans up all memory used by the queues nodes

#endif 
