# System Architecture

This document details the overall system design, data structure strategy, and architectural patterns used in the EasyCharge electric vehicle charging station management system. It covers the high-level organization of modules, the strategic use of data structures for performance optimization, and the file-based persistence approach.

---

## Architecture Overview
The EasyCharge system follows a layered architecture with clear separation between user interface, business logic, data management, and persistence layers.

![System Architecture Diagram](Architecture%20Documentation/System%20Architecture/system-architecture-diagram.png)

---

## Data Structure Strategy
The system employs three primary data structures, each chosen for specific performance characteristics and use cases.

#### Core Data Structures and Their Roles
| Data Structure | Purpose | Key Operations | Performance |
| :--- | :--- | :--- | :--- |
| **Binary Search Tree** | Station lookup by ID, Customer lookup by license plate | Insert, Search, Delete | O(log n) average |
| **FIFO Queue** | Fair vehicle waiting management | Enqueue, Dequeue | O(1) |
| **Linked List** | Dynamic port management per station | Insert, Delete, Traverse | O(1) insert/delete |

#### Data Structure Implementation Map
![Data Structure Map](Architecture%20Documentation/System%20Architecture/data-structure-map.png)

---

## Module Organization and Dependencies
The system is organized into distinct modules with well-defined responsibilities and minimal coupling.

![Module Dependency Diagram](Architecture%20Documentation/System%20Architecture/module-dependency-diagram.png)

---

## Data Persistence Architecture
The system implements a file-based persistence strategy that loads all data into memory at startup and saves changes at shutdown or on demand.

![Persistence Layer Design](Architecture%20Documentation/System%20Architecture/persistence-layer-design.png)

---

## Build and Compilation Architecture
The project uses a standard C build system with `make` and `gcc`, organizing output into dedicated directories.

#### Build System Flow
![Build System Flow](Architecture%20Documentation/System%20Architecture/build-system-flow.png)

#### Compilation Command Structure
| Build Phase | Command | Output Location |
| :--- | :--- | :--- |
| **Object Compilation** | `gcc -c src/*.c -Iinclude` | `build/*.o` |
| **Linking** | `gcc build/*.o -o bin/easycharge` | `bin/easycharge` |
| **Execution** | `./bin/easycharge` | Runtime |

---

## System Performance Characteristics
The architectural choices provide specific performance benefits for the charging station management domain.

| Operation Category | Data Structure | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Station Lookup** | BST | O(log n) | O(n) |
| **Customer Lookup** | BST | O(log n) | O(n) |
| **Port Assignment** | Linked List | O(k) where k = ports per station | O(k) |
| **Queue Operations** | FIFO Queue | O(1) | O(m) where m = waiting cars |
| **Nearest Station** | Linear Search | O(n) | O(1) |

The system optimizes for the most frequent operations (station and customer lookups) while maintaining efficient queue processing for fairness in vehicle assignment.