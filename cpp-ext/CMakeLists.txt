cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # active multi-thread compliation flag on msvc
    # cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
    string(REGEX REPLACE "/W[3|4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set(THIRD_PARTY_DIR "${PROJECT_SOURCE_DIR}/third-party")

set(PRJ_BIN_DIR "${PROJECT_SOURCE_DIR}/Demo/ext")

add_subdirectory(src)
add_subdirectory(third-party)

if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
endif()