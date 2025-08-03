# Vehicle and Customer Data

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `Cars.txt` | 1-27 | Master data file for vehicle and customer information. |
| `src/System.c` | 34-46 | Implements the data loading logic for vehicles. |
| `src/System.c` | 92-96 | Implements data consistency checks during state saving. |
| `src/System.c` | 98-107 | Defines the recursive saving function for vehicle data. |
| `src/System.c` | 143-150 | Handles file writing operations for the vehicle BST. |

## Purpose and Scope

This document covers the `Cars.txt` file format and its role in persisting vehicle and customer information within the EasyCharge system. The file stores customer registration data, payment history, vehicle charging preferences, and current system state for all vehicles that have used the charging network.

For information about other data files, see `Station Configuration Data`, `Port Configuration and Status`, and `Queue State Management`. For implementation details of customer management operations, see `Customer and Vehicle Management`.

## File Format Overview

The `Cars.txt` file uses a comma-separated values (CSV) format with a header row defining the data structure. Each subsequent row represents a single vehicle/customer record in the system.

```csv
License,Type,TotalPaid,IsCharging,IsInQueue,DummyValue
11223344,FAST,22519.87,0,0,0
12345678,FAST,127.00,0,0,0
20053042,SLOW,17.11,0,0,0

### File Format Structure

![Diagram showing the structure of a record in Cars.txt](Assets/car_file_format_structure.png)

### Field Definitions

| Field | Type | Description | Valid Values |
| :--- | :--- | :--- | :--- |
| **License** | String | Vehicle license plate number (primary key) | Alphanumeric, up to 10 characters |
| **Type** | String | Vehicle charging port type preference | `FAST`, `MID`, `SLOW` |
| **TotalPaid** | Float | Cumulative amount paid by customer | Non-negative decimal value |
| **IsCharging** | Integer | Current charging status flag | `0` (not charging), `1` (currently charging) |
| **IsInQueue** | Integer | Queue status flag | `0` (not queued), `1` (waiting in queue) |
| **DummyValue** | Integer | Reserved field for future use | Always `0` |

### Data Type Mapping

![Diagram mapping file fields to memory structure data types](Assets/car_data_type_mapping.png)

## Data Loading Process

The system loads vehicle data during startup through the `loadFiles()` function, which parses each line and constructs in-memory data structures.

#### Loading Workflow

![Flowchart of the vehicle data loading process](Assets/car_data_loading_flow.png)

The parsing logic extracts specific fields using the format string `"%[^,],%[^,],%lf,%*d,%*d,%d"`, which reads the license plate, type string, and total paid amount, while skipping fields that are recalculated at runtime.

## Data Saving Process

The system persists vehicle data during shutdown or explicit save operations through the `updateFiles()` function and its helper `saveCarsRecursive()`.

#### Saving Workflow

![Flowchart of the vehicle data saving process](Assets/car_data_saving_flow.png)

The save operation uses in-order BST traversal to write records sorted by license plate. The `IsCharging` field is dynamically calculated based on whether the car's `pPort` pointer is non-null, ensuring data consistency.

## Integration with Memory Structures

The file data integrates with the system's Binary Search Tree (BST) for efficient customer lookup operations.

#### BST Organization and File Mapping

![Diagram showing the mapping from the Cars.txt file to the in-memory BST structure](Assets/car_file_to_bst_mapping.png)

The BST maintains vehicles sorted by license plate, enabling O(log n) lookup performance. The `searchCar()` function uses string comparison of license plates to navigate the tree.

## Data Consistency and State Management

The file format maintains consistency between persistent and runtime states through coordinated updates across multiple data files.

| Field | Runtime Source | File Purpose |
| :--- | :--- | :--- |
| **License** | `Car` struct `nLicense` | Primary key for customer identification |
| **Type** | `Car` struct `portType` | Vehicle charging preference |
| **TotalPaid**| `Car` struct `totalPayed` | Billing and payment history |
| **IsCharging**| Calculated from `pPort != NULL` | Current charging session status |
| **IsInQueue** | `Car` struct `inqueue` | Queue membership status |

The system automatically recalculates the `IsCharging` field during save operations, ensuring the file reflects actual port assignments rather than stale status flags.

