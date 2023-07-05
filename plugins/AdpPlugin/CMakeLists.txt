cmake_minimum_required(VERSION 3.10)
project("AdpPlugin")

include(SMSDK)
REGISTER_PLUGIN(${CMAKE_CURRENT_SOURCE_DIR} "${PROJECT_NAME}")

include("extern/analog-dance-pad.cmake")
target_link_libraries(${PROJECT_NAME} adp lua-5.1)