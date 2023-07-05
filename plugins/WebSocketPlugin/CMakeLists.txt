cmake_minimum_required(VERSION 3.10)
project("WebSocketPlugin")

include(SMSDK)
REGISTER_PLUGIN(${CMAKE_CURRENT_SOURCE_DIR} "${PROJECT_NAME}")

include(FindLibWebsocketpp.cmake)

target_sources(${PROJECT_NAME} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/SocketHandler.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/SocketHandler.h"
)

target_include_directories(${PROJECT_NAME} PUBLIC
	"${SM_BASE}/extern/jsoncpp/include"
)

target_link_libraries(${PROJECT_NAME}
	websocketpp::websocketpp
)