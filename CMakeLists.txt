# CMakeLists.txt
# Author: Charles Emerson
#
# Created: 7 December 2020
# Updated: 8 December 2020
#
# RC5 Project for CS 665 Fall 2020.

    
########################################################################
# Setup
########################################################################
cmake_minimum_required(VERSION 2.6)

project(rc5)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ansi -pedantic -Wextra -Wall -Wconversion -std=c++14 -O3")
include_directories("include")


########################################################################
# Encryption executable (rc5-encrypt)
########################################################################

set(EXECUTABLE_NAME rc5-encrypt)
set(SOURCE_FILES
    src/rc5_encrypt_main.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)


########################################################################
# Decryption executable (rc5-decrypt)
########################################################################

set(EXECUTABLE_NAME rc5-decrypt)
set(SOURCE_FILES
    src/rc5_decrypt_main.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
install(TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
