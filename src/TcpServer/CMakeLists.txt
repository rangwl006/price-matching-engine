add_library(TcpServer TcpServer.cpp Session.cpp)
target_include_directories(TcpServer PUBLIC ${CMAKE_SOURCE_DIR}/inc)
target_link_libraries(TcpServer PUBLIC asio apputils pricematchingengine)