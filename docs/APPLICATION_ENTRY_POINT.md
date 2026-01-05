# Application Entry Point and Menu System

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `src/Main.c` | 6-61 | Contains the `main()` function, which orchestrates the entire application lifecycle. |
| `src/System.c`| 20-87 | Implements `loadFiles()` for system initialization. |
| `src/System.c`| 181-199| Implements `displayMenu()` for showing user options. |
| `src/System.c`| 204-211| Implements `exitSys()` for graceful shutdown. |

## Purpose and Scope

This document covers the EasyCharge application's main entry point, interactive menu system, and core program control flow. It details the `main()` function, menu display logic, user input handling, command dispatch mechanism, and integration with the data persistence layer.

For the specific business logic functions called from the menu, see `Implementation Details`. For data file formats, see the individual data format documents.

## Application Lifecycle Overview

The EasyCharge application follows a structured lifecycle managed by the `main()` function. The application initializes system data from files, enters an interactive menu loop, processes user commands, and ensures data is saved before shutdown.

### Application Startup Flow

![Flowchart of the application's startup and main loop](Assets/application_lifecycle_flow.png)

## System Initialization Process

Application startup involves loading persistent data from four text files into memory. The `loadFiles()` function coordinates this process in a specific order to maintain data integrity:

| Loading Phase | File | Data Structure | Function Called |
| :--- | :--- | :--- | :--- |
| 1 | `Stations.txt` | Station BST | `addStationToBST()` |
| 2 | `Cars.txt` | Car BST | `addCarToBST()` |
| 3 | `Ports.txt` | Port linked lists| `addPortToList()` |
| 4 | `LineOfCars.txt`| Station queues | `enqueue()` |

## Interactive Menu System

The menu system provides a text-based interface for all operations. The `displayMenu()` function presents 13 operational commands plus an exit option.

### Menu Structure and Command Dispatch

![Diagram of the menu structure and command dispatch flow](Assets/menu_command_dispatch.png)

### Command Categories and Data Impact

| Category | Commands | `needs_update` Flag | Description |
| :--- | :--- | :--- | :--- |
| **Query Operations** | 1, 3, 5, 6, 7, 8 | Set to `0` | Read-only operations that display information. |
| **Modification Ops**| 2, 4, 9, 10, 11, 12, 13 | Remains `1` | Operations that modify system state. |
| **System Control** | 0 | Set to `0` | Application exit with data save. |

## Input Validation and Error Handling

The application implements robust input validation to prevent crashes and infinite loops caused by non-numeric or out-of-range user input.

### Input Processing Flow

![Flowchart of the user input validation process](Assets/input_validation_flow.png)

## Data Persistence Integration

The application ensures data consistency by tracking whether the current operation modifies system data via the `needs_update` flag.

### Persistence Control Flow
1.  **Initialization**: `needs_update` is set to `1` at the start of each command cycle.
2.  **Query Operations**: The flag is explicitly reset to `0` for read-only commands.
3.  **File Update**: `updateFiles()` is called at the end of the cycle only if `needs_update` is still `1`.

This mechanism ensures that data is saved to all four `.txt` files (`Stations.txt`, `Cars.txt`, `Ports.txt`, `LineOfCars.txt`) if and only if a modification occurred.

## System Shutdown Process

The `exitSys()` function handles graceful application termination, ensuring complete memory cleanup and final data persistence.

### Shutdown Sequence

![Flowchart of the system shutdown sequence](Assets/system_shutdown_flow.png)

### Memory Management Strategy
The application uses a centralized cleanup approach:
-   **BST Structures**: Freed by dedicated functions (`freeStationsBST()`, `freeCarsBST()`).
-   **Linked Lists**: Port and queue lists are freed as part of the station cleanup process.
-   **File Handles**: All file pointers are properly closed after each I/O operation.