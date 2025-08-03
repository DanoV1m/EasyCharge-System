# Core Entity Relationships

This document explains the relationships between the core entities in the EasyCharge system: Stations, Cars/Customers, Ports, and charging sessions. It covers how these entities are organized in memory, their interconnections, and lifecycle management patterns.

---

## Entity Schema and Primary Relationships
The EasyCharge system is built around four primary entities that work together to manage the charging station network.

![Entity Relationship Schema](Architecture%20Documentation/Core%20Entity%20Relationships/entity-relationship-schema.png)

*   **Station-Port Relationship:** Each `Station` maintains a linked list of `Port` entities.
*   **Car-Port Assignment:** When a car is charging, a bidirectional relationship is created between the `Car` and `Port` entities.
*   **Station Queue Management:** Each station maintains its own FIFO queue (`CarQueue`) for waiting cars.

---

## In-Memory Organization and Lookup
The system uses Binary Search Trees to organize entities for efficient O(log n) lookup operations.

![In-Memory BST Organization](Architecture%20Documentation/Core%20Entity%20Relationships/in-memory-bst-organization.png)

*   **Station BST:** Organized by `id` for fast lookups.
*   **Car BST:** Organized by `nLicense` (license plate) for quick customer identification.
*   **Port Linked Lists:** Each station's ports are in a singly-linked list for dynamic management.

---

## State Management and Entity Lifecycle
The system manages complex state transitions as cars move through the charging process, from arrival to departure.

![Car Lifecycle State Diagram](Architecture%20Documentation/Core%20Entity%20Relationships/car-lifecycle-statediagram.png)

*   **Idle State:** `Car.pPort == NULL && Car.inqueue == 0`
*   **Charging State:** `Car.pPort != NULL`
*   **Queued State:** `Car.inqueue == 1`

---

## Entity Interaction Patterns
Key functions manage the creation of entities and the establishment of relationships between them.

![Entity Function Interactions](Architecture%20Documentation/Core%20Entity%20Relationships/entity-function-interactions.png)

*   **Entity Creation:** `create*()` functions instantiate entities, which are then added to data structures (e.g., `addStationToBST()`).
*   **Relationship Establishment:** `assignCar2port()` is a central function that creates the car-port link.
*   **Queue Integration:** `enqueue()` and `dequeue()` manage the flow of waiting cars.

---

## Memory Management and Cleanup
Careful memory management is required to prevent leaks and maintain data integrity.

| Entity Type | Cleanup Function | Relationship Cleanup |
| :--- | :--- | :--- |
| **Station BST** | `freeStationsBST()` | Recursively frees ports and queue. |
| **Car BST** | `freeCarsBST()` | Breaks car-port references. |
| **Port Lists** | `freePortsList()` | Iterates through linked list. |
| **Queues** | Handled by station cleanup | Deallocates FIFO nodes. |

A cascading cleanup approach ensures that when a station is removed, all its associated ports and waiting cars are also freed correctly.