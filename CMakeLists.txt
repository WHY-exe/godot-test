cmake_minimum_required(VERSION 3.12)
project(godot-cppext)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# Hide all symbols by default except those explicitly marked for export
# This applies to all targets in the project
if (NOT WIN32)
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        add_compile_options(
            -fvisibility=hidden
            -fvisibility-inlines-hidden
        )
    endif()
endif()

add_subdirectory(cppext)

if (EXISTS "${CMAKE_BINARY_DIR}/compile_commands.json")
    file(COPY "${CMAKE_BINARY_DIR}/compile_commands.json" DESTINATION "${CMAKE_SOURCE_DIR}")
endif()