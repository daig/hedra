cmake_minimum_required(VERSION 3.10)

# Library name and version
project(prelude VERSION 1.0)

# Specify the C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/ptx_array_shape.c
    src/ptx_initializer_array.c
)

# Header files
set(HEADERS
    include/prelude.h
    include/prelude/prelude.h
    include/prelude/ptx_array_shape.h
    include/prelude/ptx_initializer_array.h
)

# Create library
add_library(prelude STATIC ${SOURCES})

# Set include directories for targets that link against this library
target_include_directories(prelude
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Install targets
install(TARGETS prelude 
    EXPORT preludeTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Install headers
install(FILES ${HEADERS} DESTINATION include)
install(DIRECTORY include/prelude DESTINATION include)

# Export targets
install(EXPORT preludeTargets
    FILE preludeTargets.cmake
    NAMESPACE prelude::
    DESTINATION lib/cmake/prelude
) 