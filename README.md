# EasyCharge System Overview

This document provides a comprehensive introduction to the EasyCharge system, a command-line electric vehicle charging station management application written in C. It covers the system's purpose, key capabilities, high-level architecture, and core design principles.

### A Personal Note
This project is more than just code to me.
As a first-year Electrical Engineering student, this was my final project for the "Advanced C Programming" course.
While the academic requirement was to build a functional system, I decided to take it a step further.
My goal was to transform a university assignment into a comprehensive, professionally structured, and well-documented project.
I'm uploading it to GitHub as the first step in building a quality portfolio that will accompany me throughout my career.

---

## System Purpose and Capabilities

EasyCharge is a CLI application that simulates a comprehensive management system for a network of electric vehicle charging stations. The system manages stations, customers, and charging ports through an interactive menu interface, with all data persisted to text files for session continuity.

### Core Functional Areas

| Area | Capabilities |
| :--- | :--- |
| **Station Management** | Load station data, add new stations, locate nearest station by coordinates, safely close stations, remove malfunctioning ports. |
| **Customer Operations** | Auto-register customers, assign vehicles to appropriate ports, manage FIFO waiting queues, calculate dynamic pricing. |
| **Monitoring & Reporting** | Display system status, generate port statistics, track customer rankings, monitor charging sessions. |
| **Automated Maintenance**| Release ports after timeout, process waiting queues, maintain data consistency. |

---

## System Architecture

The EasyCharge system follows a modular, layered architecture. The diagram below illustrates the core entities of the system—Stations, Customers, Ports, and Queues—and their relationships. It also highlights the data structures chosen for performance optimization.

![Core Entity Relationships Diagram](Assets/entity-relationships.png)

---

## Building and Running the Project

To compile and run the application, follow these steps:

1.  **Clone the repository:**
    ```bash
    git clone [your-repository-url]
    ```

2.  **Navigate to the project directory:**
    ```bash
    cd [project-directory-name]
    ```

3.  **Compile the project:**
    The `Makefile` will create `build/` and `bin/` directories and place the final executable inside `bin/`.
    ```bash
    make all
    ```

4.  **Run the application:**
    ```bash
    ./bin/easycharge
    ```

5.  **Clean the project:**
    To remove all build artifacts, use the `clean` command.
    ```bash
    make clean
    ```
---

## Project Documentation

This project is extensively documented. The following sections provide links to detailed documents covering every aspect of the system's design and implementation.

### System Design and Architecture
*   **[➡️ View Full System Architecture Document](SYSTEM_ARCHITECTURE.md)**
*   **[➡️ View Full Data Structures and Algorithms Document](DATA_STRUCTURES.md)**
*   **[➡️ View Full Core Entity Relationships Document](ENTITY_RELATIONSHIPS.md)**

### Core System Modules
*   **[➡️ View Full Station Management Document](STATION_MANAGEMENT.md)**
*   **[➡️ View Full Customer and Vehicle Management Document](CUSTOMER_MANAGEMENT.md)**
*   **[➡️ View Full Charging Port Operations Document](CHARGING_PORT_OPERATIONS.md)**
*   **[➡️ View Full Queue Management System Document](QUEUE_MANAGEMENT.md)**
*   **[➡️ View Full Charging Process Workflow Document](CHARGING_WORKFLOW.md)**
*   **[➡️ View Full Implementation Details Document](IMPLEMENTATION_DETAILS.md)**

### Data Persistence and File Formats
This section details the strategy for saving and loading data.
*   **[➡️ View Full Data Persistence Strategy Document](DATA_PERSISTENCE.md)**
*   **[➡️ View Full Data Files and Formats Overview](DATA_FILES_AND_FORMATS.md)**
    *   **[📄 Station Configuration Data](STATION_CONFIGURATION.md)**
    *   **[📄 Vehicle and Customer Data](VEHICLE_CUSTOMER_DATA.md)**
    *   **[📄 Port Configuration and Status](PORT_CONFIGURATION.md)**
    *   **[📄 Queue State Management](QUEUE_STATE_MANAGEMENT.md)**

### Build and Execution
This section covers how the application is compiled and run.
*   **[➡️ View Full Build System Document](BUILD_SYSTEM.md)**
    *   **[📄 Compilation and Build Process](COMPILATION_PROCESS.md)**
    *   **[📄 Application Entry Point and Menu System](APPLICATION_ENTRY_POINT.md)**

