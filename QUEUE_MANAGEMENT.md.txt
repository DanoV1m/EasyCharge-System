# Queue Management System

This document covers the FIFO (First In, First Out) queue implementation used to manage waiting vehicles at charging stations. The queue system ensures fair processing by maintaining the order in which vehicles arrive when all charging ports at a station are occupied.

---

## Queue Data Structure Architecture
The system implements a standard linked list-based FIFO queue. It uses a `CarQueue` struct as the main container and `QueueNode` structs for the individual elements holding `Car` pointers.

![Queue Data Structure](Architecture%20Documentation/Queue%20Management%20System/queue-data-structure.png)

---

## Core Queue Operations
The queue system provides five fundamental operations that maintain FIFO ordering with O(1) complexity for enqueue/dequeue and O(n) for cleanup.

![Queue Operations Flowchart](Architecture%20Documentation/Queue%20Management%20System/queue-operations-flowchart.png)

| Operation | Time Complexity | Description |
| :--- | :--- | :--- |
| `initQueue` | O(1) | Sets `front` and `rear` pointers to NULL. |
| `isQueueEmpty`| O(1) | Checks if the `front` pointer is NULL. |
| `enqueue` | O(1) | Adds a new node to the `rear` of the queue. |
| `dequeue` | O(1) | Removes a node from the `front` of the queue. |
| `freeQueue` | O(n) | Traverses all nodes to free memory. |

---

## Enqueue Operation
The `enqueue` function adds a vehicle to the end of the queue, handling both empty and non-empty queue cases.

![Enqueue Operation Logic](Architecture%20Documentation/Queue%20Management%20System/enqueue-operation-logic.png)

---

## Dequeue Operation
The `dequeue` function removes and returns a vehicle from the front of the queue. It correctly handles the case where the queue becomes empty after the operation.

![Dequeue Operation Logic](Architecture%20Documentation/Queue%20Management%20System/dequeue-operation-logic.png)

---

## Integration with EasyCharge System
Each station maintains its own `CarQueue` instance. The queue ensures that vehicles are processed in a strict first-come, first-served order when ports are unavailable.

![Queue System Integration](Architecture%20Documentation/Queue%20Management%20System/queue-system-integration.png)