cmake_minimum_required(VERSION 3.10)
project("LuaDebugPlugin")

include(SMSDK)
REGISTER_PLUGIN(${CMAKE_CURRENT_SOURCE_DIR} "${PROJECT_NAME}")

target_link_libraries(${PROJECT_NAME} lua-5.1)
target_include_directories(${PROJECT_NAME} PRIVATE
    "extern/lrdb"
    "extern/picojson"
    "extern/asio/asio/include"
)