cmake_minimum_required(VERSION 3.16)

project(chat)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)
add_subdirectory(logger)
add_subdirectory(chat-grpc-submodule)
add_subdirectory(crypto)
add_subdirectory(sql)
#------------------#Server library
add_library(chat-server server.cpp server.h)
target_link_libraries(chat-server PRIVATE mycrypto mylibsql logging proto-objects)
target_link_libraries(chat-server PRIVATE gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts)
target_link_libraries(chat-server PRIVATE protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
target_compile_features(chat-server PRIVATE cxx_std_17) #https://stackoverflow.com/questions/71991758/c-filesystem-lib-not-importing
#------------------#Server CONSOLE programm
add_executable(chat-server-console  chat-net-serv.cpp)
target_link_libraries(chat-server-console PRIVATE chat-server gRPC::grpc++)
target_compile_features(chat-server-console PRIVATE cxx_std_17) #https://stackoverflow.com/questions/71991758/c-filesystem-lib-not-importing
#------------------#Server gui programm
add_executable(chat-server-gui  adminpanel.cpp)
target_link_libraries(chat-server-gui PRIVATE chat-server chat-client wx::core wx::base gRPC::grpc++)

#------------------#Client library
add_library(chat-client server.h client.h client.cpp)
target_link_libraries(chat-client PRIVATE logging)
target_link_libraries(chat-client PRIVATE proto-objects)
target_link_libraries(chat-client PRIVATE mylibsql)
target_link_libraries(chat-client PRIVATE mycrypto)
target_link_libraries(chat-client PRIVATE gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

#------------------#Client CONSOLE application
add_executable(chat-client-console chat-net-client.cpp gui.h gui.cpp)
target_link_libraries(chat-client-console PRIVATE chat-client gRPC::grpc gRPC::grpc++)
#------------------#Client GUI application
add_executable(chat-client-gui authorized.cpp chat.cpp chatlist.cpp initial.cpp searchusers.cpp signin.cpp signup.cpp)
target_link_libraries(chat-client-gui PRIVATE wx::core wx::base chat-client)
if(WIN32)
    # Windows specific
    set_target_properties(chat-client-gui PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(WIN32)
    # Windows specific
    set_target_properties(chat-server-gui PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
# Add additional CPack settings as needed
