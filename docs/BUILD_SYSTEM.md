# Build System Overview

The EasyCharge system uses a standard GNU Make build system with `gcc` as the compiler. The build process compiles C source files from the `src/` directory and produces a single executable binary in the `bin/` directory.

## Build System Architecture
![Build System Architecture](Architecture%20Documentation/Build%20System%20and%20Development/build_system_architecture.png)

## Directory Structure and Build Configuration

| Directory | Purpose | Contents |
| :--- | :--- | :--- |
| `src/` | Source code | C implementation files (`*.c`) |
| `include/` | Header files | C header files (`*.h`) |
| `build/` | Build artifacts | Compiled object files (`*.o`) |
| `bin/` | Final binary | `easycharge` executable |

## Makefile Configuration

The build system is configured through key variables and targets:

| Variable | Value | Purpose |
| :--- | :--- | :--- |
| `CC` | `gcc` | C compiler |
| `CFLAGS` | `-Wall -g -Iinclude` | Compiler flags: warnings, debug info, include path |
| `TARGET` | `bin/easycharge` | Final executable name |
| `SOURCES` | `src/*.c` | Wildcard pattern for source files |

## Compilation Process

### Compilation Workflow
The build process automatically handles dependency management through Make's pattern rules and ensures build directories exist before compilation.

![Compilation Workflow](Architecture%20Documentation/Build%20System%20and%20Development/build_command_workflow.png)