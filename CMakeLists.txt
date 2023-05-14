cmake_minimum_required(VERSION 3.16)

project(chat)

add_subdirectory(logger)
add_subdirectory(chat-grpc-submodule)

add_executable(chat-net-server ./chat-grpc-submodule/chat.grpc.pb.cc ./chat-grpc-submodule/chat.grpc.pb.h ./chat-grpc-submodule/chat.pb.cc ./chat-grpc-submodule/chat.pb.h chat-net-serv.cpp server.cpp server.h sql.h sql.cpp ./logger/timestamp.cpp ./logger/logentry.cpp ./logger/logger.cpp)

#https://stackoverflow.com/questions/71991758/c-filesystem-lib-not-importing
target_compile_features(chat-net-server PRIVATE cxx_std_17)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
target_link_libraries(chat-net-server PRIVATE unofficial::sqlite3::sqlite3)

find_package(gRPC CONFIG REQUIRED)
target_link_libraries(chat-net-server PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)

find_package(protobuf CONFIG REQUIRED)
target_link_libraries(chat-net-server PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)