# Compilation and Build Process

### Relevant source files

| File | Lines | Purpose |
| :--- | :--- | :--- |
| `Makefile` | 1-35 | Defines the entire build system, including targets, rules, and variables. |

## Purpose and Scope

This document covers the build system for the EasyCharge application, including directory structure, compilation flags, build targets, and the complete build pipeline. The build system is implemented using a standard `Makefile` that compiles C source files into an executable binary.

For information about running the application after building, see `Application Entry Point and Menu System`.

## Build System Overview

The EasyCharge system uses a `Makefile`-based build system with `gcc` as the compiler. The build process compiles all C source files from the `src/` directory into object files, then links them with the math library to create the final executable `bin/easycharge`.

### Directory Structure

The build system organizes files into the following directory hierarchy:

![Diagram of the build directory structure and organization](Assets/project_directory_structure.png)

### Compilation Configuration

The build system uses specific compiler flags and settings defined in the `Makefile`:

| Configuration | Value | Purpose |
| :--- | :--- | :--- |
| **Compiler** | `gcc` | GNU C Compiler |
| **Warning Flags** | `-Wall` | Enable all common warnings |
| **Debug Info** | `-g` | Include debugging symbols |
| **Include Path** | `-Iinclude` | Add `include/` directory to header search path |
| **Math Library** | `-lm` | Link with math library for calculations |

The `CFLAGS` variable (`Makefile:4`) combines these flags: `-Wall -g -Iinclude`.

## Build Pipeline Process

The compilation process follows a two-stage pipeline: source compilation to object files, followed by linking into the final executable. The `Makefile` automatically discovers all `.c` files in the `src/` directory (`Makefile:13`) and generates corresponding object files in the `build/` directory using pattern substitution (`Makefile:15`).

![Diagram of the build pipeline from source files to executable](Assets/makefile_pipeline_flow.png)

## Build Targets

The `Makefile` provides several build targets for different operations.

### Primary Targets

| Target | Command | Description |
| :--- | :--- | :--- |
| **`all`** | `make` or `make all` | Default target - builds the complete application. |
| **`clean`** | `make clean` | Removes all build artifacts and directories. |

### Target Dependencies

The `all` target (`Makefile:17`) serves as the default build target. Both `all` and `clean` are declared as phony targets (`Makefile:35`) to prevent conflicts with files of the same name and ensure they always run.

![Diagram of build target dependencies and clean operations](Assets/makefile_targets_dependencies.png)

## Build Process Execution

### Standard Build Process
1.  **Directory Creation**: `build/` and `bin/` directories are created automatically.
2.  **Source Compilation**: Each `.c` file is compiled to a corresponding `.o` file in `build/`.
3.  **Linking**: All object files are linked with the math library to create the executable.
4.  **Success Message**: A build completion message is displayed with execution instructions.

### Build Commands
```bash
# Clean previous build artifacts
make clean

# Build the application (using the default 'all' target)
make

# Alternative explicit build command
make all