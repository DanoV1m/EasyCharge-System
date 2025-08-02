#define _CRT_SECURE_NO_WARNINGS
#include "Queue.h"

// sets a queue to a safe, empty state. must be called before using a new queue

/* this function initializes a Car Queue struct to a empty state
 input: CarQueue* q - a pointer to the queue to be initialized */

void initQueue(CarQueue* q) {
    q->front = NULL;
    q->rear = NULL;
}

/* this function checks if a given queue is empty
 input: const Car Queue* q - a pointer to the queue to check
output: int - 1 (true) if the queue is empty, else 0 */

int isQueueEmpty(const CarQueue* q) {
    return (q->front == NULL);
}

/* adds a new car pointer to the end of the queue
 input: CarQueue* q - a pointer to the queue to be modified
 input: struct Car* car - the car to be added to the queue */

void enqueue(CarQueue* q, struct Car* car) {
    // create a new node to hold the car pointer
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));

    node->p2car = car;
    node->next = NULL;

    // if the queue was empty ,the new node is both the first and the last
    if (isQueueEmpty(q)) {
        q->front = node;
        q->rear = node;
    }
    // otherwise ,link the previous last node to the new one ,and update the rear
    else {
        q->rear->next = node;
        q->rear = node;
    }
}

// removes the car pointer from the front of the queue and returns it
struct Car* dequeue(CarQueue* q) {
    if (isQueueEmpty(q)) {
        return NULL; // if queue is empty cant be removed
    }

    // temporarily holds the front node to free it later
    QueueNode* temp = q->front;
    struct Car* car = temp->p2car;

    // the new front is the next node in the list
    q->front = q->front->next;

    // if the queue is now empty, the rear must also be NULL
    if (q->front == NULL) {
        q->rear = NULL;
    }

    free(temp); // free the nodes memory 
    return car;
}

// empties the queue and frees the memory of all nodes
void freeQueue(CarQueue* queue) {
    // a more efficient way instead of just calling dequeue
    QueueNode* curr = queue->front;
    QueueNode* next;
    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }
    // reset the queue pointers
    queue->front = NULL;
    queue->rear = NULL;
}
