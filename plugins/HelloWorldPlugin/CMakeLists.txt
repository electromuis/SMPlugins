cmake_minimum_required(VERSION 3.10)
project("HelloWorldPlugin")

include(SMSDK)
REGISTER_PLUGIN(${CMAKE_CURRENT_SOURCE_DIR} "${PROJECT_NAME}")