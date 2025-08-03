# Data Files and Formats

This document comprehensively covers the data file formats used by the EasyCharge system for persistent storage. The system uses four CSV-formatted text files to maintain state across application sessions: station configurations, customer records, port assignments, and vehicle queues.

For information about the data structures these files populate in memory, see Data Structures and Algorithms. For details about the data loading and saving implementation, see Data Persistence Strategy.

---

## Data Persistence Architecture
The EasyCharge system implements a file-based persistence strategy using CSV-formatted text files. During startup, the `loadFiles()` function reads all data files and constructs the in-memory data structures. During shutdown, the `updateFiles()` function writes the current system state back to the files.

![Data Lifecycle Overview](Architecture%20Documentation/Data%20Files%20and%20Formats/data-lifecycle-overview.png)

---

## File Format Overview
The system uses four primary data files. The relationships between them ensure data integrity, with `Stations.txt` and `Cars.txt` serving as primary data sources.

![File Relationship Diagram](Architecture%20Documentation/Data%20Files%20and%20Formats/file-relationship-diagram.png)

| File | Purpose | Key Fields |
| :--- | :--- | :--- |
| **Stations.txt** | Station configurations | ID, Name, NumPorts, Coordinates |
| **Cars.txt** | Customer and vehicle data | License, Type, TotalPaid, Status |
| **Ports.txt** | Port assignments and status | StationID, PortNum, Type, Status |
| **LineOfCars.txt**| Vehicle waiting queues | CarLicense, StationID |

---

## File Specifications

### 1. Station Configuration Data (`Stations.txt`)
Contains the master configuration data for all charging stations in the system. Each record defines a station's identity, capacity, and geographic location.
```csv
ID,Name,NumPorts,CoordX,CoordY
205,Haifa Port,3,32.79,34.99
307,Beer Sheva North,5,31.25,34.79
412,Jerusalem East,6,31.77,35.21

2. Vehicle and Customer Data (Cars.txt)
License,Type,TotalPaid,IsCharging,IsInQueue,DummyValue
ABC123,FAST,45.50,1,0,0
XYZ789,MID,23.75,0,1,0
DEF456,SLOW,12.00,0,0,0

3. Port Configuration and Status (Ports.txt)
StationID,PortNum,Type,Status,Year,Month,Day,Hour,Min,CarLicense
205,1,FAST,1,2024,3,15,14,30,ABC123
205,2,MID,0,0,0,0,0,0,NULL

4. Queue State Management (LineOfCars.txt)
CarLicense,StationID
XYZ789,205
GHI012,307

Queue Restoration Process
During system startup, the queue loading process reads each entry, locates the corresponding station and car records, and adds the vehicle to the correct station's queue to rebuild the state.
![alt text](Architecture%20Documentation/Data%20Files%20and%20Formats/queue-restoration-process