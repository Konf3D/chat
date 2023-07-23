cmake_minimum_required(VERSION 3.16)

project(chat)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)
add_subdirectory(logger)
add_subdirectory(chat-grpc-submodule)
#Server programm
add_executable(chat-net-server sqlconnection.hpp crypto.h crypto.cpp chat-net-serv.cpp server.cpp server.h ./logger/timestamp.cpp ./logger/logentry.cpp ./logger/logger.cpp)
target_compile_features(chat-net-server PRIVATE cxx_std_17) #https://stackoverflow.com/questions/71991758/c-filesystem-lib-not-importing
target_link_libraries(chat-net-server PRIVATE logging)
target_link_libraries(chat-net-server PRIVATE proto-objects)
target_link_libraries(chat-net-server PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(chat-net-server PRIVATE gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)
target_link_libraries(chat-net-server PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(chat-net-server PRIVATE SOCI::soci_core SOCI::soci_empty SOCI::soci_sqlite3)

#Client programm
add_executable(chat-net-client crypto.h crypto.cpp server.h chat-net-client.cpp client.cpp gui.cpp ./logger/logger.cpp ./logger/logentry.cpp ./logger/timestamp.cpp)
target_link_libraries(chat-net-client PRIVATE logging)
target_link_libraries(chat-net-client PRIVATE proto-objects)
target_link_libraries(chat-net-client PRIVATE gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(chat-net-client PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(chat-net-client PRIVATE SOCI::soci_core SOCI::soci_empty SOCI::soci_sqlite3)