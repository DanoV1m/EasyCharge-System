# Station Configuration Data

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `Stations.txt` | 1-6 | Master data file for all station configurations. |
| `src/System.c` | 28-30 | Implements the data loading logic from the text file. |

## Purpose and Scope

This document describes the `Stations.txt` file format and its role in persisting charging station configuration data for the EasyCharge system. This file stores the fundamental properties of each charging station including location coordinates, capacity information, and identification details.

For information about port-specific configuration and status, see `Port Configuration and Status`. For details about the station management operations that use this data, see `Station Management`.

## File Format Specification

The `Stations.txt` file uses a comma-separated values (CSV) format with a header row. Each station record contains five fields that define the station's core properties.

#### CSV Structure

| Field | Type | Description | Example |
| :--- | :--- | :--- | :--- |
| **ID** | Integer | Unique station identifier | 205 |
| **Name** | String | Human-readable station name | Haifa Port |
| **NumPorts** | Integer | Total number of charging ports | 3 |
| **CoordX** | Double | Geographic X coordinate (latitude) | 32.79 |
| **CoordY** | Double | Geographic Y coordinate (longitude) | 34.99 |

#### Sample Data Format

```csv
ID,Name,NumPorts,CoordX,CoordY
205,Haifa Port,3,32.79,34.99
307,Beer Sheva North,5,31.25,34.79
412,Jerusalem East,6,31.77,35.21
518,Herzliya Marina,6,32.16,34.81
623,Ashdod South,3,31.80,34.64


Station Data Entity Structure
![alt text](Assets/station_entity_structure.png)

Data Loading Process
The system loads station configuration data during startup through the 'loadFiles()' function. The loading process parses each CSV record and creates corresponding 'Station' structures in the Binary Search Tree.

Loading Workflow
![alt text](Assets/station_data_loading_flow.png)

Parsing Implementation
The parsing logic uses sscanf() with a specific format string to extract station data:

sscanf(line_buffer, "%d,%[^,],%d,%lf,%lf", &id, name, &numPorts, &x, &y);

The format pattern %[^,] captures the station name string up to the next comma, handling names that contain spaces.

Data Persistence Process
Station data is persisted back to Stations.txt during system shutdown or when explicitly saving state through the updateFiles() function.

Saving Workflow
![alt text](Assets/station_data_persistence_flow.png)

Save Format Generation
The saveStationsRecursive() function generates CSV records using fprintf():

fprintf(stations_fp, "%d,%s,%d,%.2f,%.2f\n", 
        root->id, root->name, root->num_Ports, 
        root->coord.x, root->coord.y);

Integration with Station Management
The station configuration data serves as the foundation for the station management BST structure. Each record in Stations.txt corresponds to a Station node in the Binary Search Tree organized by station ID.

Memory Structure Mapping
![alt text](Assets/station_bst_integration.png)

Station Identification and Lookup
The ID field from Stations.txt serves as the primary key for BST operations. The system uses this identifier for:

Station lookup: searchStationByID() traverses the BST using station IDs.

Nearest station calculation: Geographic coordinates enable distance calculations.

Port association: Port data files reference stations by ID.

Queue management: Vehicle queues are associated with specific station IDs.

Data Validation and Error Handling
The system includes basic error handling for file operations but relies on proper CSV formatting. The loading process:

Checks file existence with perror() reporting.

Skips malformed lines during parsing.

Continues processing after individual record failures.

Creates stations with default values for missing fields.