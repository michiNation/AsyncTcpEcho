cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(TcpProg VERSION 1.0)

find_package(Threads)

add_executable(TcpProg src/main.cpp src/TcpClient.cpp src/TcpServer.cpp)

target_link_libraries(TcpProg PUBLIC Threads::Threads)  

target_include_directories(TcpProg PUBLIC 
                                    ${CMAKE_CURRENT_SOURCE_DIR}/inc/)