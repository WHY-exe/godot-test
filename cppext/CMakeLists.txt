cmake_minimum_required(VERSION 3.21)
project(game-ext VERSION 1.0.0)
configure_file(
    ${PROJECT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
    @ONLY
)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    # active multi-thread compliation flag on msvc
    # cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${CPU_NUMBER_OF_LOGICAL_CORES}")    
    # Replace /MD with /MT for static runtime library
    foreach(config "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS${config} "${CMAKE_CXX_FLAGS${config}}")
        string(REGEX REPLACE "/MD" "/MT" CMAKE_C_FLAGS${config} "${CMAKE_C_FLAGS${config}}")
    endforeach()
endif()

add_subdirectory(vendor)

set(PRJ_BIN_DIR "${PROJECT_SOURCE_DIR}/Demo/ext")
# disable exception
# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#     set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -D_HAS_EXCEPTIONS=0")
# else()
#     set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -fno-exceptions")
# endif()
include(cmake/UtilityTargets.cmake)

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME)
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)


file(GLOB_RECURSE SRC
    "${PROJECT_SOURCE_DIR}/src/*.h" 
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.c" 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

set(PRJ_COMPILE_FLAGS)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(PRJ_COMPILE_FLAGS "/utf-8") # /GF /MP
# else()  # GCC/Clang
# 	if(CMAKE_BUILD_TYPE MATCHES Debug)
# 		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -fno-omit-frame-pointer -O0 -g")
# 	else()
# 		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -O3")
# 	endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif()

add_library(${PROJECT_NAME} SHARED ${SRC} )
target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
	godot::cpp
	spdlog::spdlog
    breakpad_client
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${PRJ_COMPILE_FLAGS})

# set(OUTPUT_NAME "${PROJECT_NAME}.${SYSTEM_NAME}.${BUILD_TYPE}.${ARCH}")
# set_target_properties(${PROJECT_NAME} 
#     PROPERTIES
#         CXX_EXTENSIONS OFF
#         OUTPUT_NAME    ${OUTPUT_NAME}
# )

install(TARGETS ${PROJECT_NAME}    
    LIBRARY DESTINATION ${INSTALL_DIR_NO_PREFIX}
    ARCHIVE DESTINATION ${INSTALL_DIR_NO_PREFIX}
    RUNTIME DESTINATION ${INSTALL_DIR_NO_PREFIX}
)
