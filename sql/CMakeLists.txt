cmake_minimum_required(VERSION 3.16)
project(mylibsql)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)

# Create the library target
add_library(mylibsql sqlconnection.h sqlconnection.cpp)
target_link_libraries(mylibsql PUBLIC SOCI::soci_core SOCI::soci_empty SOCI::soci_sqlite3)
target_link_libraries(mylibsql PUBLIC unofficial::sqlite3::sqlite3)
#target_link_libraries(mylibsql PUBLIC SOCI::soci_core_static SOCI::soci_empty_static SOCI::soci_sqlite3_static)
set_target_properties(mylibsql PROPERTIES
    CXX_STANDARD 11  # Set your desired C++ standard here
    CXX_STANDARD_REQUIRED ON
)