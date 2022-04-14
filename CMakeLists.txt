cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AsyncTcp VERSION 1.0)

find_package(Threads)

add_executable(AsyncTcp src/main.cpp src/TcpClient.cpp src/TcpServer.cpp src/FileAbstraction.cpp src/StopWatch.cpp)

target_link_libraries(AsyncTcp PUBLIC Threads::Threads)  

target_include_directories(AsyncTcp PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/inc/)