cmake_minimum_required(VERSION 3.10)

project(PME)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

message(INFO "System processor: ${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install/${CMAKE_SYSTEM_PROCESSOR})

# include dependency packages
include(cmake/CPM.cmake)
include(cmake/AsioStandalone.cmake)
include(cmake/Catch2.cmake)
include(cmake/yamlcpp.cmake)

add_subdirectory(src)

add_executable(tcpClient client.cpp)
target_link_libraries(tcpClient PUBLIC asio tcp_client app_utils)
target_include_directories(tcpClient PUBLIC ${CMAKE_SOURCE_DIR}/inc)
install(TARGETS tcpClient)

add_executable(tcpServer server.cpp)
target_link_libraries(tcpServer PUBLIC asio tcp_server pricematchingengine app_utils)
target_include_directories(tcpServer PUBLIC ${CMAKE_SOURCE_DIR}/inc)
install(TARGETS tcpServer)