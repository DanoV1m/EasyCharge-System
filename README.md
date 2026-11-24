# EasyCharge System
![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Status](https://img.shields.io/badge/Status-Complete-green.svg)
![Field](https://img.shields.io/badge/Field-Electrical%20Engineering-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

**A professional EV Charging Station Management System built for high-scale simulation and data persistence.**

---

## A Personal Note
As a first-year **Electrical Engineering** student, this was my final project for the "Advanced C Programming" course. My goal was to transform a university assignment into a professionally structured and well-documented portfolio piece.

---

## Documentation Hub
Explore the detailed engineering and system documentation:

|  Architecture & Logic |  System Operations |  Data & Files |
| :--- | :--- | :--- |
| [➡️ System Architecture](docs/SYSTEM_ARCHITECTURE.md) | [➡️ Charging Workflow](docs/CHARGING_WORKFLOW.md) | [➡️ Data Persistence](docs/DATA_PERSISTENCE.md) |
| [➡️ Data Structures](docs/DATA_STRUCTURES.md) | [➡️ Station Management](docs/STATION_MANAGEMENT.md) | [➡️ File Formats](docs/DATA_FILES_AND_FORMATS.md) |
| [➡️ Entity Relationships](docs/ENTITY_RELATIONSHIPS.md) | [➡️ Queue Management](docs/QUEUE_MANAGEMENT.md) | [➡️ Build System](docs/BUILD_SYSTEM.md) |

---

## System Architecture
The system follows a modular architecture. Below is a high-level overview of the entity relationships:

![Entity Relationship Schema](docs/Architecture%20Documentation/Core%20Entity%20Relationships/entity-relationship-schema.png)

---

## Installation & Build

From the repository root, with GCC and Make installed:

```sh
make
./bin/easycharge
```

Run against a disposable copy of the sample data; this simulator modifies its data files. It is not a production EV charging service.

## Validation and limitations

The review branch fixes two-child BST deletion, bounded CSV parsing, queue allocation and EOF handling. Run sanitizer-enabled builds and regression tests before use. Multi-file persistence is still non-transactional and requires a separate redesign.

### Persistence review
All four outputs are staged before replacement. POSIX per-file rename is used; group atomicity, crash durability, concurrent writers and Windows replacement semantics are not guaranteed. Use a disposable data directory and keep backups. The CLI regression tests deletion of two-child nodes and a forced staging failure.

Windows uses `MoveFileExA` with replace-existing semantics; POSIX uses rename. Both are tested with disposable synthetic data via CI. Multi-file crash consistency still requires a transaction or a journaled database; do not use for irreplaceable data.

Startup now aborts without writing when a required input file is missing, unreadable or a row fails the expected field parsing. This is not full semantic validation or transactional storage. Regression cases exercise missing and malformed inputs against disposable data.
