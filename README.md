# EasyCharge - An EV Charging Station Management System

### A Personal Note

This project is more than just code to me.
 As a first-year Electrical Engineering student, this was my final project for the "Advanced C Programming" course. 
While the academic requirement was to build a functional system, I decided to take it a step further.
 My goal was to transform a university assignment into a comprehensive, professionally structured, and well-documented project.
 I'm uploading it to GitHub as the first step in building a quality portfolio that will accompany me throughout my career.

## 📖 Project Overview

EasyCharge is a command-line interface (CLI) application written in C that simulates a comprehensive management system for a network of electric vehicle (EV) charging stations. The system manages a database of stations, customers (vehicles), and charging ports, allowing for a wide range of administrative actions.

The entire state of the system is loaded from and saved to text files, ensuring data persistence across sessions.

## ✨ Key Features

*   **Station Management:**
    *   Load station data from external files.
    *   Add new stations and charging ports to the system.
    *   Locate the nearest station based on user coordinates.
    *   Safely close a station (only if no cars are charging or waiting).
    *   Remove a malfunctioning port from a station.

*   **Customer & Charging Process Management:**
    *   Automatically register new customers to the database.
    *   Assign a vehicle to a free port that matches its required charging type (Fast, Mid, Slow).
    *   Manage an orderly queue (FIFO) for vehicles when no suitable port is available.
    *   Stop a charging session, dynamically calculate the cost based on duration and port type, and update the customer's total payment.
    *   Remove a customer from the system.

*   **Reporting & Monitoring:**
    *   Display all stations in the system with a status summary.
    *   Show a detailed list of all vehicles (both charging and waiting) at a specific station.
    *   Generate a detailed statistical report on port status (Occupied, Free, Out of Order) for any station.
    *   Check the status of a specific vehicle in the system (Charging, Waiting, or Idle).
    *   Display a ranking of the top 5 customers by total payments.

*   **Automated Maintenance:**
    *   Automatically release ports that have been occupied for longer than a predefined time limit (e.g., two hours).

## 🧠 Data Structures in Use

This project demonstrates the use of classic data structures for efficient data management:
*   **Binary Search Tree (BST):** Used for fast management and lookup of stations (by ID) and customers (by license plate number).
*   **Queue (FIFO):** Implemented using a linked list to fairly manage waiting vehicles at busy stations.
*   **Linked List:** Used to manage the collection of charging ports belonging to each station.

## 🛠️ Setup and Usage

A C development environment is required to compile and run the project.

### Prerequisites
*   A C compiler (like `gcc`).
*   The `make` build tool.
    *   *On Windows, you can easily get these tools by installing **MSYS2**.*

### Building the Project

1.  Clone the repository to your local machine:
    ```bash
    git clone https://github.com/DanoV1m/EasyCharge-System.git
    cd EasyCharge-System
    ```

2.  To compile the project, run the following command in the root directory:
    ```bash
    make
    ```
    This command will automatically create a `build` directory (for object files) and a `bin` directory (for the final executable).

### Running the Program

After compiling, run the program with:
```bash

./bin/easycharge
