cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(blocklist)

add_executable(${PROJECT_NAME}
    main.cpp
    blocklist.h

)
