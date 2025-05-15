cmake_minimum_required(VERSION 3.10)
project(BankSimulator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(BankSimulator
        main.cpp
        Transaction.cpp
        Transaction.h
        BankAccount.cpp
        BankAccount.h
        BankSystem.cpp
        BankSystem.h
        BankAccount.cpp)

if(MSVC)
    target_compile_options(BankSimulator PRIVATE /W4)
else()
    target_compile_options(BankSimulator PRIVATE -Wall -Wextra -pedantic)
endif()
