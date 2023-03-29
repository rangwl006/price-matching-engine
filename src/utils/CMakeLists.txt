add_library(app_utils app_utils.cpp)
target_link_libraries(app_utils PUBLIC asio)
target_include_directories(app_utils PUBLIC ${CMAKE_SOURCE_DIR}/inc)