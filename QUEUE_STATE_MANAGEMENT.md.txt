# Queue State Management

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `LineOfCars.txt`| 1-2 | Persists vehicle waiting queues. |
| `src/System.c` | 72-86 | Implements queue data loading and restoration logic. |
| `src/System.c` | 129-134| Implements queue data serialization during save operations. |
| `src/System.c` | 112-137| Defines the recursive saving function that processes queues. |

## Purpose and Scope

This document covers the `LineOfCars.txt` file format and the queue state persistence mechanisms used to maintain vehicle waiting queues across system restarts. This documentation focuses specifically on how FIFO queues at charging stations are serialized to and restored from persistent storage.

For information about in-memory queue operations, see `Queue Management System`. For details about other data files, see `Station Configuration Data`, `Vehicle and Customer Data`, and `Port Configuration and Status`.

## File Format Specification

The queue state is persisted in the `LineOfCars.txt` file using a simple CSV format that maintains the FIFO order of vehicles waiting at each charging station.

### File Structure
```csv
CarLicense,StationID
ABC123,1
XYZ789,1
DEF456,2