# Customer and Vehicle Management

This document covers the customer and vehicle management subsystem, focusing on customer registration, vehicle tracking, payment history, and the Binary Search Tree (BST) organization used for efficient customer lookups.

---

## Core Data Structures
The customer management system uses two primary data structures: the `Car` struct for data and the `tCar` struct for BST organization. This enables efficient O(log n) lookups by license plate.

![Customer Data Structures](Architecture%20Documentation/Customer%20and%20Vehicle%20Management/customer-data-structures.png)

---

## BST Organization and Operations
The BST uses lexicographic ordering of license plates (`strcmp`) for organization. New customers are automatically inserted during charging requests.

![Customer BST Operations](Architecture%20Documentation/Customer%20and%20Vehicle%20Management/customer-bst-operations.png)

---

## Customer Auto-Registration
The system implements automatic customer registration during charging requests, eliminating the need for pre-registration. If a license plate is not found, a new customer record is created and added to the BST.

![Auto-Registration Process](Architecture%20Documentation/Customer%20and%20Vehicle%20Management/auto-registration-process.png)

---

## Search and Lookup
The BST provides efficient customer lookups with O(log n) time complexity. The search is iterative to avoid recursion stack overhead.

![Customer Search Algorithm](Architecture%20Documentation/Customer%20and%20Vehicle%20Management/customer-search-algorithm.png)

---

## Payment History and Analytics
The system tracks customer payment history in the `totalPayed` field. The `dispTopCustomers()` function provides analytics by ranking customers by total payments using `qsort`.

![Top Customer Analytics Process](Architecture%20Documentation/Customer%20and%20Vehicle%20Management/top-customer-analytics.png)

---

## Customer Removal Process
The system supports complete customer removal with comprehensive validation to ensure data integrity (e.g., preventing removal of a customer who is currently charging or in a queue).

![Customer Removal Workflow](Architecture%20Documentation/Customer%20and%20Vehicle%20Management/customer-removal-workflow.png)