# ⚡ EasyCharge System
![Language](https://img.shields.io/badge/Language-C-blue.svg)
![Status](https://img.shields.io/badge/Status-Complete-green.svg)
![Field](https://img.shields.io/badge/Field-Electrical%20Engineering-orange.svg)
![License](https://img.shields.io/badge/License-MIT-yellow.svg)

**A professional EV Charging Station Management System built for high-scale simulation and data persistence.**

---

## 📝 A Personal Note
As a first-year **Electrical Engineering** student, this was my final project for the "Advanced C Programming" course. While the academic requirement was to build a functional system, my goal was to transform a university assignment into a professionally structured and well-documented portfolio piece.

---

## 📚 Documentation Hub
Explore the detailed engineering and system documentation (Now organized in the `/docs` directory):

| 📘 Architecture & Logic | ⚙️ System Operations | 💾 Data & Files |
| :--- | :--- | :--- |
| [➡️ System Architecture](docs/SYSTEM_ARCHITECTURE.md) | [➡️ Charging Workflow](docs/CHARGING_WORKFLOW.md) | [➡️ Data Persistence](docs/DATA_PERSISTENCE.md) |
| [➡️ Data Structures](docs/DATA_STRUCTURES.md) | [➡️ Station Management](docs/STATION_MANAGEMENT.md) | [➡️ File Formats](docs/DATA_FILES_AND_FORMATS.md) |
| [➡️ Entity Relationships](docs/ENTITY_RELATIONSHIPS.md) | [➡️ Queue Management](docs/QUEUE_MANAGEMENT.md) | [➡️ Build System](docs/BUILD_SYSTEM.md) |

---

## 🚀 Key Capabilities
* **Efficient Management:** BST-based station searching and dynamic port allocation.
* **Customer-Centric:** FIFO waiting queues and automated registration.
* **Data Persistence:** Full state recovery from optimized flat files.
* **Maintenance:** Automated port timeout release and status monitoring.

---

## 🏗️ System Architecture
The system follows a modular architecture. Below is a high-level overview of the entity relationships:

![Core Entity Relationships Diagram](docs/Architecture%20Documentation/entity-relationships.png)

---

## 🛠️ Installation & Build
1. **Clone the repository:**
   ```bash
   git clone [https://github.com/DanoV1m/EasyCharge-System.git](https://github.com/DanoV1m/EasyCharge-System.git)