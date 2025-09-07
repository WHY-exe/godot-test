cmake_minimum_required(VERSION 3.13)

project(example1)

set(PRJ_COMPILE_FLAGS)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
	set(PRJ_COMPILE_FLAGS "/utf-8 /W4") # /GF /MP

	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} /MDd") # /Od /RTC1 /Zi
	else()
		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} /MD /O2") # /Oy /GL /Gy
		STRING(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	endif(CMAKE_BUILD_TYPE MATCHES Debug)

	add_definitions(-DNOMINMAX)
else()  # GCC/Clang
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -fno-omit-frame-pointer -O0 -g")
	else()
		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -O3")
	endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif()
# disable exception
# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#     set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -D_HAS_EXCEPTIONS=0")
# else()
#     set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -fno-exceptions")
# endif()

if(NOT DEFINED BITS)
	set(BITS 32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(BITS 64)
	endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
endif()

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME)
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)


file(GLOB SRC
    "${PROJECT_SOURCE_DIR}/*.h" 
    "${PROJECT_SOURCE_DIR}/*.hpp"
    "${PROJECT_SOURCE_DIR}/*.c" 
    "${PROJECT_SOURCE_DIR}/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SRC} )
target_link_libraries(${PROJECT_NAME} PUBLIC godot-cpp spdlog::spdlog)

set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${PRJ_COMPILE_FLAGS})

set(OUTPUT_NAME "${PROJECT_NAME}.${SYSTEM_NAME}.${BUILD_TYPE}.${BITS}")

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_options(${PROJECT_NAME} 
        PRIVATE 
        -static-libgcc 
        -static-libstdc++ 
    )
endif()

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_EXTENSIONS           OFF
        OUTPUT_NAME              ${OUTPUT_NAME}
        # LIBRARY_OUTPUT_DIRECTORY ${PRJ_BIN_DIR}
        # ARCHIVE_OUTPUT_DIRECTORY ${PRJ_BIN_DIR}
        RUNTIME_OUTPUT_DIRECTORY ${PRJ_BIN_DIR}
)
