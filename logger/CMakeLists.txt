cmake_minimum_required(VERSION 3.12)
project(logging)

# Set the source files for the library
set(SOURCES timestamp.cpp logentry.cpp logger.cpp)

# Create the library target
add_library(logging ${SOURCES})

set_target_properties(logging PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)