# Build System and Development

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `Makefile` | 1-35 | Defines build targets, compiler flags, and rules for compilation. |
| `src/Main.c` | 6-61 | Contains the application's entry point (`main`), initialization, and main loop. |

## Purpose and Scope

This document covers the build system, compilation process, and development workflow for the EasyCharge CLI application. It explains how to compile the system from source code and describes the application's entry point and initialization sequence.

## Build System Overview

The EasyCharge system uses a standard GNU Make build system with `gcc` as the compiler. The build process compiles C source files from the `src/` directory and produces a single executable binary in the `bin/` directory.

### Build System Architecture

![Diagram of the build system architecture](Assets/build_system_architecture.png)

### Directory Structure and Build Configuration

| Directory | Purpose | Contents |
| :--- | :--- | :--- |
| **src/** | Source code | C implementation files (`*.c`) |
| **include/**| Header files | C header files (`*.h`) |
| **build/**| Build artifacts | Compiled object files (`*.o`) |
| **bin/** | Final binary | `easycharge` executable |

#### Makefile Configuration
The build system is configured through key variables and targets:

| Variable | Value | Purpose |
| :--- | :--- | :--- |
| `CC` | `gcc` | C compiler |
| `CFLAGS` | `-Wall -g -Iinclude` | Compiler flags: warnings, debug info, include path |
| `TARGET` | `bin/easycharge` | Final executable name |
| `SOURCES`| `src/*.c` | Wildcard pattern for source files |

## Compilation Process

### Compilation Workflow

The build process automatically handles dependency management through Make's pattern rules and ensures build directories exist before compilation.

![Flowchart of the compilation workflow](Assets/build_command_workflow.png)

### Build Commands

```bash
# Compile the entire project
make all

# Clean build artifacts and binaries
make clean

# Run the application after a successful build
./bin/easycharge