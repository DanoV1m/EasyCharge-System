# Data Persistence Strategy

This document explains the file-based data persistence approach used in the EasyCharge system, covering data loading mechanisms, storage formats, memory management, and the lifecycle of system state. The strategy ensures data durability across application sessions while maintaining efficient in-memory operations during runtime.

---

## Persistence Architecture Overview
The EasyCharge system implements a file-based persistence strategy that separates data storage from in-memory operations. The system loads all data from text files into efficient memory structures at startup, operates entirely in memory, and saves the complete system state back to files during shutdown.

![File-Based Storage Architecture](Architecture%20Documentation/Data%20Persistence%20Strategy/persistence-architecture-overview.png)

---

## Data Loading Process
The `loadFiles()` function orchestrates the complete data loading sequence, parsing each file and reconstructing the in-memory data structures with proper relationships. The loading process follows a strict dependency order to ensure referential integrity.

![Loading Sequence and Dependencies](Architecture%20Documentation/Data%20Persistence%20Strategy/data-loading-process.png)

### File Format Parsing
Each file uses a CSV-like format with headers. The system employs `sscanf()` for structured parsing:

| File | Key Fields |
| :--- | :--- |
| **Stations.txt** | ID, Name, NumPorts, CoordX, CoordY |
| **Cars.txt** | License, Type, TotalPaid, InQueue |
| **Ports.txt** | StationID, PortNum, Type, Status, DateTime, CarLicense |
| **LineOfCars.txt** | CarLicense, StationID |

---

## Data Saving Process
The `updateFiles()` function implements a complete system state serialization using recursive BST traversal and linked structure iteration. `saveStationsRecursive()` efficiently coordinates writing to three files simultaneously during a single traversal to minimize overhead.

![Saving Strategy and File Generation](Architecture%20Documentation/Data%20Persistence%20Strategy/data-saving-process.png)

---

## Data Consistency and Memory Management

### Atomicity and Error Handling
The system uses file handle validation and coordinated file operations to maintain consistency. On partial failure (e.g., one file fails to open), all successfully opened files are closed to prevent partial writes.

```c
// Atomic file opening with validation
FILE* stations_fp = fopen("Stations.txt", "w");
FILE* ports_fp = fopen("Ports.txt", "w");
FILE* queues_fp = fopen("LineOfCars.txt", "w");

if (!stations_fp || !ports_fp || !queues_fp) {
    // Cleanup on partial failure
    if (stations_fp) fclose(stations_fp);
    if (ports_fp) fclose(ports_fp);
    if (queues_fp) fclose(queues_fp);
    return;
}```

### Memory Lifecycle Management
The system implements explicit memory management tied to the persistence lifecycle.

| Phase | Operation | Memory State | Key Functions |
| :--- | :--- | :--- | :--- |
| **Startup** | Load from files | Allocate all structures | `loadFiles()`, `create*()` |
| **Runtime** | In-memory operations | Maintain references | BST/Queue/List ops |
| **Shutdown** | Save and cleanup | Deallocate all structures | `exitSys()`, `updateFiles()`, `free*()` |

The `exitSys()` function ensures a proper shutdown sequence: saving state, then freeing all allocated memory to prevent leaks.