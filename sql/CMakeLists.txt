cmake_minimum_required(VERSION 3.16)
#project(mylibsql)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)

# Create the library target
add_library(mylibsql sqlconnection.h sqlconnection.cpp)
target_link_libraries(mylibsql PRIVATE unofficial::sqlite3::sqlite3)
if(WIN32)
    target_link_libraries(mylibsql PRIVATE SOCI::soci_core SOCI::soci_empty SOCI::soci_sqlite3)
elseif(UNIX)
    target_link_libraries(mylibsql PRIVATE SOCI::soci_core_static SOCI::soci_sqlite3_static)
endif()
set_target_properties(mylibsql PROPERTIES
    CXX_STANDARD 14  # Set your desired C++ standard here
    CXX_STANDARD_REQUIRED ON
)