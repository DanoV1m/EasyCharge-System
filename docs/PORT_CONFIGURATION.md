# Port Configuration and Status

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `Ports.txt` | 1-24 | Master data file for all port configurations and live status. |
| `src/System.c` | 11-17 | Defines `stringToPortType()` for converting string to enum. |
| `src/System.c` | 48-70 | Implements the data loading logic for ports. |
| `src/System.c` | 92-96 | Defines `portTypeToString()` for converting enum to string. |
| `src/System.c` | 112-137 | Implements the recursive saving logic for stations and their ports. |
| `src/System.c` | 143-177 | Handles file opening and header writing for `Ports.txt`. |

## Purpose and Scope

This document describes the `Ports.txt` file format used by the EasyCharge system to persist charging port configuration and operational status data. The file maintains the complete state of all charging ports across all stations, including their types, availability status, current assignments, and charging session timestamps.

For implementation details of port operations, see `Charging Port Operations`. For station-level configuration, see `Station Configuration Data`. For vehicle data, see `Vehicle and Customer Data`.

## File Format Overview

The `Ports.txt` file uses a comma-separated values (CSV) format with a header row. Each subsequent row represents an individual charging port with its complete configuration and current state.

```csv
StationID,PortNum,Type,Status,Year,Month,Day,Hour,Min,CarLicense
205,3,SLOW,2,0,0,0,0,0,NULL
205,2,MID,2,0,0,0,0,0,NULL
307,3,SLOW,3,0,0,0,0,0,NULL
412,1,FAST,2,0,0,0,0,0,NULL