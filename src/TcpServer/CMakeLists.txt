add_library(tcp_server TcpServer.cpp Session.cpp)
target_include_directories(tcp_server PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_link_libraries(tcp_server PUBLIC asio yaml-cpp app_utils pricematchingengine)