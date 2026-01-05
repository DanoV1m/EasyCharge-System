# Charging Process Workflow

This page documents the complete charging session lifecycle, from vehicle arrival through payment calculation and port release. It covers the core workflow functions that manage vehicle registration, port assignment, session tracking, and queue processing.

---

## Charging Session Lifecycle Overview
The system manages a complete charging workflow that handles vehicle arrival, auto-registration, intelligent port assignment, session monitoring, payment calculation, and queue management.

![Full Charging Workflow](Architecture%20Documentation/Charging%20Process%20Workflow/full-charging-workflow.png)

---

## Port Assignment Algorithm
The port assignment logic scans the station's linked list of ports to find the first available port matching the vehicle's charging type. If no suitable port is found, the vehicle is offered a place in the queue.

![Port Assignment and Queue Logic](Architecture%20Documentation/Charging%20Process%20Workflow/port-assignment-logic.png)

The `assignCar2port` function creates bidirectional links between the `Car` and `Port` structures, sets the port status to `OCCUPIED`, and records the charging start time.

---

## Session Termination and Billing
The `stopCharge` function handles session termination, cost calculation, and automatic queue processing. It calculates the session duration and applies port-type-specific billing rates.

![Session Termination and Billing Flow](Architecture%20Documentation/Charging%20Process%20Workflow/session-termination-flow.png)

---

## Queue Processing and Fair Assignment
The system implements intelligent queue processing that automatically assigns freed ports to waiting vehicles with matching charging requirements. The `findAndDequeueMatchingCar` function searches the FIFO queue for the first vehicle requiring the specific port type that just became available.

![Queue Processing on Port Release](Architecture%20Documentation/Charging%20Process%20Workflow/queue-processing-on-release.png)

This algorithm ensures that when a port becomes available, the longest-waiting vehicle requiring that specific port type gets immediate access, maintaining both fairness and type compatibility.