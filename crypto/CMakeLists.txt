cmake_minimum_required(VERSION 3.12)
project(mycrypto)

# Set the source files for the library
set(SOURCES crypto.cpp crypto.h)
# Create the library target
add_library(mycrypto ${SOURCES})
target_link_libraries(mycrypto PRIVATE OpenSSL::SSL OpenSSL::Crypto)
