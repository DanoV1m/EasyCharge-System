# Station Management

The Station Management module handles all operations related to charging station entities. This includes station creation, organization in a Binary Search Tree (BST), nearest station location algorithms, port management integration, queue management, and station lifecycle operations.

---

## Station Data Structure and BST Organization
The `Station` struct is the core entity for stations. Each station is organized in a BST sorted by station ID for efficient O(log n) lookup performance.

![Station BST Structure](Architecture%20Documentation/Station%20Management/station-bst-structure.png)

---

## Station Creation and Lifecycle
Station creation involves dynamic memory allocation and initialization of all associated data structures.

| Function | Purpose |
| :--- | :--- |
| `createStation()` | Allocate and initialize a new station object. |
| `addStationToBST()`| Insert a station into the BST, maintaining ID order. |
| `searchStationByID()`| Find a station by its ID with O(log n) performance. |
| `closeSt()` | Safely remove an active station after validation checks. |
| `freeStationsBST()`| Clean up all memory associated with the stations BST. |

The module ensures careful memory management, handling allocation failures to prevent leaks.

---

## Nearest Station Location Algorithm
The system implements a location service using Euclidean distance calculations across the entire station BST. The algorithm performs an exhaustive search to find the closest station to a given coordinate.

![Nearest Station Location Algorithm](Architecture%20Documentation/Station%20Management/nearest-station-algorithm.png)

---

## Charging Operations Integration
Station management is tightly integrated with charging operations, handling vehicle assignment, port allocation, and queue management when a charging request is made.

![Charging Operations Flow](Architecture%20Documentation/Station%20Management/charging-operations-flow.png)

---

## Station Display and Reporting
The system provides comprehensive station information display capabilities with detailed port and queue statistics.

![Station Display Operations](Architecture%20Documentation/Station%20Management/station-display-operations.png)

---

## Station Closure and Cleanup
Station closure involves safety checks (e.g., no cars charging or queued), BST node deletion, and complete memory cleanup of all associated resources (ports, queue, etc.). The process correctly handles all three cases of BST node deletion to maintain the tree's integrity.

![Station Closure Process](Architecture%20Documentation/Station%20Management/station-closure-process.png)