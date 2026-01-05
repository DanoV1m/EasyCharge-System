# Data Structures and Algorithms

This document details the strategic data structure choices and algorithmic approaches employed in the EasyCharge system for efficient electric vehicle charging station management. It covers the implementation rationale, performance characteristics, and integration patterns of the core data structures: Binary Search Trees for entity lookups, FIFO queues for fair vehicle processing, and linked lists for dynamic port management.

---

## Data Structure Strategy Overview
The EasyCharge system employs three primary data structures, each chosen for specific performance and operational requirements. The system prioritizes scalability through logarithmic lookup times, fairness through FIFO processing, and flexibility through dynamic memory allocation.

| Data Structure | Primary Use Case | Key Operations | Time Complexity |
| :--- | :--- | :--- | :--- |
| **Binary Search Tree** | Station and customer lookups | Search, insert, delete | O(log n) average |
| **FIFO Queue** | Vehicle waiting management | Enqueue, dequeue | O(1) |
| **Linked List** | Port management per station | Insert, delete, traverse | O(1) for insert/delete |

### Performance Optimization Strategy
![Performance Optimization Flow](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/performance-optimization-flow.png)

---

## Binary Search Tree (BST) Implementation
The system uses BSTs for two critical entity types, optimizing frequent lookup operations that scale with system growth.

![BST Structure Diagram](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/bst-structure-diagram.png)

The station BST uses integer station IDs as keys, while the customer BST uses license plate strings. This supports both rapid customer lookup and efficient station selection.

#### Search Performance
| Operation | Station BST | Customer BST | Lookup Key |
| :--- | :--- | :--- | :--- |
| Find nearest station | O(n) + O(log n) | - | Geographic calculation + ID |
| Customer registration check| - | O(log n) | License plate string |
| Payment history update | - | O(log n) | License plate string |
| Station port access | O(log n) | - | Station ID integer |

---

## FIFO Queue System for Vehicle Management
The queue implementation ensures fair processing of waiting vehicles when charging ports are unavailable.

### Queue Node Structure and Operations
![Queue Structure and Operations](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/queue-structure-and-operations.png)

The `CarQueue` structure maintains separate `front` and `rear` pointers, enabling constant-time O(1) enqueue and dequeue operations.

### Fair Processing Algorithm
![Queue Processing Algorithm](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/queue-processing-algorithm.png)

---

## Linked List Management for Charging Ports
Each station maintains a dynamic linked list of charging ports, enabling flexible port management.

![Port Linked List Structure](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/port-linked-list-structure.png)

#### Port Management Operations
| Operation | Time Complexity | Description |
| :--- | :--- | :--- |
| Add new port | O(1) | Insert at head of linked list |
| Remove malfunctioning port | O(n) | Traverse list to find and remove |
| Find available port by type | O(n) | Linear search through port list |
| Update port status | O(1) | Direct pointer access after location |

---

## Algorithmic Complexity and Memory Management

### System-Wide Performance Characteristics
![Scalability Factors Diagram](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/scalability-factors-diagram.png)

### Data Structure Integration and Memory Layout
![System Memory Layout](Architecture%20Documentation/Data%20Structures%20and%20Algorithms/system-memory-layout.png)

The system maintains referential integrity through careful pointer management, ensuring that station nodes reference their associated port lists and waiting queues, while customer nodes remain independent for efficient lookup operations.