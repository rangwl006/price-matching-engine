add_library(tcp_client TcpClient.cpp)
target_link_libraries(tcp_client PUBLIC asio app_utils yaml-cpp)
target_include_directories(tcp_client PUBLIC ${CMAKE_SOURCE_DIR}/inc)
install(TARGETS tcp_client)