cmake_minimum_required(VERSION 3.16)

project(chat)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(SOCI CONFIG REQUIRED)

find_package(wxWidgets CONFIG REQUIRED)

find_package(Qt5Core CONFIG REQUIRED)
find_package(Qt5Gui CONFIG REQUIRED)
find_package(Qt5Widgets CONFIG REQUIRED)

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
#------------------#Server GUI application-----CONSOLE
#add_executable(chat-server-console  chat-net-serv.cpp)
#target_link_libraries(chat-server-console PRIVATE chat-server gRPC::grpc++)
#target_compile_features(chat-server-console PRIVATE cxx_std_17) #https://stackoverflow.com/questions/71991758/c-filesystem-lib-not-importing
#------------------#Server GUI application-----WX
#add_executable(chat-server-wx  "wxadminpanel.cpp")
#target_link_libraries(chat-server-wx PRIVATE chat-server chat-client wx::core wx::base gRPC::grpc++)
#------------------#Server GUI application-----QT
add_executable(chat-server-qt qtadminpanel)
target_link_libraries(chat-server-qt PRIVATE chat-client chat-server gRPC::grpc++ Qt5::Core Qt5::Gui Qt5::Widgets)

#------------------#Client library
add_library(chat-client server.h client.h client.cpp)
target_link_libraries(chat-client PRIVATE logging)
target_link_libraries(chat-client PRIVATE proto-objects)
target_link_libraries(chat-client PRIVATE mylibsql)
target_link_libraries(chat-client PRIVATE mycrypto)
target_link_libraries(chat-client PRIVATE gRPC::grpc gRPC::grpc++ gRPC::grpc++_alts protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)

#------------------#Client GUI application-----CONSOLE
#add_executable(chat-client-console chat-net-client.cpp gui.h gui.cpp)
#target_link_libraries(chat-client-console PRIVATE chat-client gRPC::grpc gRPC::grpc++)
#------------------#Client GUI application-----WX
#add_executable(chat-client-wx "wxauthorized.cpp" "wxchat.cpp" "wxchatlist.cpp" "wxinitial.cpp" "wxsearchusers.cpp" "wxsignin.cpp" "wxsignup.cpp")
#target_link_libraries(chat-client-wx PRIVATE wx::core wx::base chat-client)
#------------------#Client GUI application-----QT
add_executable(chat-client-qt qtmain.cpp qtmainwindow.h qtmainwindow.cpp qtsignin.h qtsignin.cpp qtsignup.h qtsignup.cpp qtauthorized.h qtauthorized.cpp)
target_link_libraries(chat-client-qt PRIVATE chat-client gRPC::grpc++ Qt5::Core Qt5::Gui Qt5::Widgets)



if(WIN32)
    # Windows specific
    #set_target_properties(chat-client-wx PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(chat-client-qt PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
if(WIN32)
    # Windows specific
    #set_target_properties(chat-server-wx PROPERTIES WIN32_EXECUTABLE TRUE)
    set_target_properties(chat-server-qt PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
# Add additional CPack settings as needed
