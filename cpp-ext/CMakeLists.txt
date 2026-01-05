cmake_minimum_required(VERSION 3.21)
project(example1)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    # active multi-thread compliation flag on msvc
    # cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    
    # Replace /MD with /MT for static runtime library
    foreach(config "" "_DEBUG" "_RELEASE" "_MINSIZEREL" "_RELWITHDEBINFO")
        string(REGEX REPLACE "/MD" "/MT" CMAKE_CXX_FLAGS${config} "${CMAKE_CXX_FLAGS${config}}")
        string(REGEX REPLACE "/MD" "/MT" CMAKE_C_FLAGS${config} "${CMAKE_C_FLAGS${config}}")
    endforeach()
endif()

set(PRJ_BIN_DIR "${PROJECT_SOURCE_DIR}/Demo/ext")

find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-godot-cpp CONFIG REQUIRED)
set(PRJ_COMPILE_FLAGS)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# using Visual Studio C++
    set(PRJ_COMPILE_FLAGS "/utf-8 /W4") # /GF /MP
# else()  # GCC/Clang
# 	if(CMAKE_BUILD_TYPE MATCHES Debug)
# 		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -fno-omit-frame-pointer -O0 -g")
# 	else()
# 		set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -O3")
# 	endif(CMAKE_BUILD_TYPE MATCHES Debug)
endif()

# disable exception
# if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
#     set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -D_HAS_EXCEPTIONS=0")
# else()
#     set(PRJ_COMPILE_FLAGS "${PRJ_COMPILE_FLAGS} -fno-exceptions")
# endif()
set(ARCH )
if (WIN32 AND MSVC)
    if (MSVC)
        set(ARCH ${MSVC_CXX_ARCHITECTURE_ID})
    else()
        set(ARCH ${CMAKE_SYSTEM_PROCESSOR})
    endif()
elseif (IOS)
    set (ARCH ${CMAKE_OSX_ARCHITECTURES})
elseif (ANDROID)
    set (ARCH ${CMAKE_ANDROID_ARCH_ABI})
else()
    set(ARCH ${CMAKE_SYSTEM_PROCESSOR})
endif()

string(TOLOWER "${CMAKE_SYSTEM_NAME}" SYSTEM_NAME)
string(TOLOWER "${CMAKE_BUILD_TYPE}" BUILD_TYPE)
string(TOLOWER "${ARCH}" ARCH)


file(GLOB SRC
    "${PROJECT_SOURCE_DIR}/src/*.h" 
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
    "${PROJECT_SOURCE_DIR}/src/*.c" 
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_library(${PROJECT_NAME} SHARED ${SRC} )
target_link_libraries(${PROJECT_NAME} 
	PRIVATE 
	unofficial::godot::cpp
	spdlog::spdlog)

set_property(TARGET ${PROJECT_NAME} APPEND_STRING PROPERTY COMPILE_FLAGS ${PRJ_COMPILE_FLAGS})

set(OUTPUT_NAME "${PROJECT_NAME}.${SYSTEM_NAME}.${BUILD_TYPE}.${ARCH}")

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
        CXX_EXTENSIONS           OFF
        OUTPUT_NAME              ${OUTPUT_NAME}
        # LIBRARY_OUTPUT_DIRECTORY ${PRJ_BIN_DIR}
        # ARCHIVE_OUTPUT_DIRECTORY ${PRJ_BIN_DIR}
        # RUNTIME_OUTPUT_DIRECTORY ${PRJ_BIN_DIR}
)

install(TARGETS example1)

if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
endif()