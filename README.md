# Hello Project

A simple C project using CMake build system.

## Project Structure

```
.
├── CMakeLists.txt    # Main CMake configuration file
├── build/            # Build directory (empty, for build artifacts)
├── include/          # Header files
│   └── hello.h
└── src/              # Source files
    ├── hello.c
    └── main.c
```

## Building the Project

To build the project, follow these steps:

```bash
# Navigate to the build directory
cd build

# Generate build files
cmake ..

# Build the project
cmake --build .
```

## Running the Application

After building, you can run the application:

```bash
# From the build directory
./hello_app
```

## Requirements

- CMake (version 3.10 or higher)
- C compiler (supporting C99 standard) 