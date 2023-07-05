cmake_minimum_required (VERSION 3.6)

set(ADP_ROOT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/analog-dance-pad/adp-tool")
set(HIDAPI_PATH "${ADP_ROOT_PATH}/lib/hidapi")

if(APPLE)
    list(APPEND HIDAPI_SRC ${HIDAPI_PATH}/mac/hid.c)
elseif(UNIX)
    list(APPEND HIDAPI_SRC ${HIDAPI_PATH}/linux/hid.c)
elseif(WIN32)
    list(APPEND HIDAPI_SRC ${HIDAPI_PATH}/windows/hid.c)
endif()

add_library(hidapi ${HIDAPI_SRC})
target_include_directories(hidapi PUBLIC ${HIDAPI_PATH}/hidapi)

set(ADP_PATH "${ADP_ROOT_PATH}/src")
add_library(adp
    ${ADP_PATH}/Model/Device.cpp
    ${ADP_PATH}/Model/Reporter.cpp
    ${ADP_PATH}/Model/Utils.cpp
    ${ADP_PATH}/Model/Log.cpp
)

# option(MINGW_STDTHREADS_GENERATE_STDHEADERS "" ON)
# add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/extern/mingw-std-threads")
# target_include_directories(${PROJECT_NAME} PRIVATE )

add_subdirectory (${ADP_ROOT_PATH}/lib/fmt)
target_link_libraries(adp hidapi fmt)
target_include_directories(adp PUBLIC
    ${ADP_PATH}
    ${ADP_ROOT_PATH}/lib/json/include
)

set_target_properties(adp PROPERTIES
	CXX_STANDARD 20
	CXX_EXTENSIONS OFF
)
