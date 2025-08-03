# Charging Port Operations

This document covers the implementation of charging port operations, including port creation, linked list management, status tracking, and vehicle assignment mechanisms.

---

## Port Data Structure
The system models charging ports using a `Port` struct. Each port maintains its own state, type, and assignment information.

![Port Data Structure](Architecture%20Documentation/Charging%20Port%20Operations/port-data-structure.png)

#### Port Components
| Field | Type | Purpose |
| :--- | :--- | :--- |
| `num` | `int` | Port number within the station |
| `portType` | `PortType` | Charging speed classification (FAST, MID, SLOW) |
| `status` | `PortStatus` | Current operational status (FREE, OCCUPIED, OUT_OF_ORDER) |
| `p2car` | `Car*` | Pointer to the assigned vehicle (NULL if free) |
| `chargeStartTime`| `Date` | Timestamp of when the charging session began |
| `next` | `Port*` | Pointer to the next port in the station's linked list |

---

## Port Creation and Initialization
The `createPort` function handles dynamic memory allocation and initialization of new port instances.

![Port Creation Process](Architecture%20Documentation/Charging%20Port%20Operations/port-creation-process.png)

---

## Linked List Management
Ports within each station are organized as a singly-linked list. New ports are added at the head for O(1) insertion performance.

![Port Linked List Operation](Architecture%20Documentation/Charging%20Port%20Operations/port-linked-list-operation.png)

The `freePortsList` function uses an iterative approach to deallocate all nodes, preventing stack overflow with long lists.

---

## Port State Tracking
The system maintains port status through direct field updates and pointer assignments.

![Port State Machine](Architecture%20Documentation/Charging%20Port%20Operations/port-state-machine.png)

---

## Port Information Display
The `printPortsList` function provides status information by traversing the linked list and displaying each port's details.

![Port Display Logic](Architecture%20Documentation/Charging%20Port%20Operations/port-display-logic.png)

---

## Integration with Station and Vehicle Systems
Charging ports serve as the connection point between stations and vehicles.

![Port Integration Diagram](Architecture%20Documentation/Charging%20Port%20Operations/port-integration-diagram.png)

Key integration points include the `Station`'s head pointer to the port list and the `Port`'s `p2car` pointer to the assigned vehicle.