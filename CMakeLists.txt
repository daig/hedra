cmake_minimum_required(VERSION 3.10)

# Project name and version
project(HelloProject VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Add executable
add_executable(hello_app 
    src/main.c
    src/hello.c
)

# Install target
install(TARGETS hello_app DESTINATION bin) 