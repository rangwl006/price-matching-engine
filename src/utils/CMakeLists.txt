add_library(apputils app_utils.cpp)
target_link_libraries(apputils PUBLIC asio)
target_include_directories(apputils PUBLIC ${CMAKE_SOURCE_DIR}/inc)