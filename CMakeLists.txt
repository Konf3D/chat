cmake_minimum_required(VERSION 3.16)

project(chat)

add_subdirectory(logger)
add_subdirectory(chat-grpc-submodule)
#Server programm
add_executable(chat-net-server ./chat-grpc-submodule/chat.grpc.pb.cc ./chat-grpc-submodule/chat.grpc.pb.h ./chat-grpc-submodule/chat.pb.cc ./chat-grpc-submodule/chat.pb.h chat-net-serv.cpp server.cpp server.h sql.h sql.cpp ./logger/timestamp.cpp ./logger/logentry.cpp ./logger/logger.cpp)

#https://stackoverflow.com/questions/71991758/c-filesystem-lib-not-importing
target_compile_features(chat-net-server PRIVATE cxx_std_17)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

target_link_libraries(chat-net-server PRIVATE unofficial::sqlite3::sqlite3)
target_link_libraries(chat-net-server PRIVATE gRPC::gpr gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)
target_link_libraries(chat-net-server PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(chat-net-server PRIVATE logging)
target_link_libraries(chat-net-server PRIVATE chat-proto)

#Client programm
add_executable(chat-net-client server.h chat-net-client.cpp client.cpp gui.cpp ./chat-grpc-submodule/chat.grpc.pb.cc ./chat-grpc-submodule/chat.pb.cc ./logger/logger.cpp ./logger/logentry.cpp ./logger/timestamp.cpp)
target_link_libraries(chat-net-client PRIVATE gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_link_libraries(chat-net-client PRIVATE OpenSSL::SSL OpenSSL::Crypto)
target_link_libraries(chat-net-client PRIVATE logging)
target_link_libraries(chat-net-client PRIVATE chat-proto)
