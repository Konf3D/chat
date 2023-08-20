cmake_minimum_required(VERSION 3.12)
project(mylibsql)

# Set the source files for the library
set(SOURCES sqlconnection.h sqlconnection.cpp)
# Create the library target
add_library(mylibsql ${SOURCES})
target_link_libraries(mylibsql PRIVATE SOCI::soci_core SOCI::soci_empty SOCI::soci_sqlite3)
target_link_libraries(mylibsql PRIVATE unofficial::sqlite3::sqlite3)
set_target_properties(mylibsql PROPERTIES
    CXX_STANDARD 11  # Set your desired C++ standard here
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)