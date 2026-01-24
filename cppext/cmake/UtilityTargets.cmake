# Utility targets for build automation
# This file contains custom targets for symbol dumping, stripping, copying to demo, and JAR building
# Requires variables: INSTALL_DIR, STRIP_DIR, SYM_DIR, DEMO_DIR, DUMP_SYMS_EXE, ARCH
set(ARCH )
if (WIN32)
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
string(TOLOWER "${ARCH}" ARCH)

set(INSTALL_DIR_NO_PREFIX ${PROJECT_VERSION}/${CMAKE_SYSTEM_NAME}/${CMAKE_BUILD_TYPE}/${ARCH})

set(DUMP_SYMS_EXE ${CMAKE_CURRENT_LIST_DIR}/../tools/dump/${CMAKE_HOST_SYSTEM_NAME}/dump_syms)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Windows")
    string( APPEND DUMP_SYMS_EXE ".exe")
else()
    # Ensure tool is executable on non-Windows hosts
    if(EXISTS ${DUMP_SYMS_EXE})
        file(CHMOD ${DUMP_SYMS_EXE} OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
    endif()
endif()
set(DEMO_DIR    ${CMAKE_SOURCE_DIR}/Demo/${CMAKE_SYSTEM_NAME}/TestApp)
set(INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR_NO_PREFIX})
set(STRIP_DIR   ${CMAKE_INSTALL_PREFIX}/${PROJECT_VERSION}/${CMAKE_SYSTEM_NAME}/StripRel/${ARCH})
set(SYM_DIR     ${CMAKE_INSTALL_PREFIX}/${PROJECT_VERSION}/${CMAKE_SYSTEM_NAME}/sym/${ARCH})

if (WIN32)
    install(
        FILES 
            $<TARGET_PDB_FILE:${PROJECT_NAME}>
        DESTINATION ${SYM_DIR}
        OPTIONAL)

    add_custom_target(DumpSymForBreakpad
        WORKING_DIRECTORY "${SYM_DIR}"
        USES_TERMINAL
        # windows 下 dump_syms需要设置编码为 utf-8，并且将换行符替换为unix格式
        COMMAND chcp 65001
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SYM_DIR}
        COMMAND ${DUMP_SYMS_EXE} ./${PROJECT_NAME}.pdb > ${SYM_DIR}/${PROJECT_NAME}.sym
        # git 默认安装dos2unix
        # COMMAND dos2unix ${SYM_DIR}/${PROJECT_NAME}.sym
        DEPENDS install
    )
endif()

if (IOS)
    add_custom_target(DumpSymForBreakpad
        WORKING_DIRECTORY ${INSTALL_DIR}
        USES_TERMINAL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SYM_DIR}
        COMMAND dsymutil ./lib${PROJECT_NAME}.dylib -o ./lib${PROJECT_NAME}.dylib.dSYM
        COMMAND ${DUMP_SYMS_EXE} ./lib${PROJECT_NAME}.dylib.dSYM > ${SYM_DIR}/lib${PROJECT_NAME}.dylib.sym
        COMMAND ${CMAKE_COMMAND} -E remove_directory ./lib${PROJECT_NAME}.dylib.dSYM
        DEPENDS install
    )

    add_custom_target(StripDebugInfo
        WORKING_DIRECTORY ${INSTALL_DIR}
        USES_TERMINAL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${STRIP_DIR}
        COMMAND strip -rSx lib${PROJECT_NAME}.dylib -o ${STRIP_DIR}/lib${PROJECT_NAME}.dylib
        DEPENDS PackAllStaticLib DumpSymForBreakpad
    )

endif()

if (ANDROID)
    set (STRIP_LOCATION ${CMAKE_ANDROID_NDK}/toolchains/llvm/prebuilt/windows-x86_64/bin/llvm-strip.exe)

    add_custom_target(DumpSymForBreakpad
        WORKING_DIRECTORY ${INSTALL_DIR}
        USES_TERMINAL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SYM_DIR}
        COMMAND ${DUMP_SYMS_EXE} ./lib${PROJECT_NAME}.so > ${SYM_DIR}/lib${PROJECT_NAME}.so.sym
        DEPENDS install
    )

    add_custom_target(StripDebugInfo
        WORKING_DIRECTORY ${INSTALL_DIR}
        USES_TERMINAL
        COMMAND ${CMAKE_COMMAND} -E make_directory ${STRIP_DIR}
        COMMAND ${STRIP_LOCALTION} -s lib${PROJECT_NAME}.so -o ${STRIP_DIR}/lib${PROJECT_NAME}.so
        DEPENDS DumpSymForBreakpad)

endif()

# Set target folders for IDE organization
set_property(TARGET DumpSymForBreakpad PROPERTY FOLDER "SymUtils")
if (NOT WIN32)
    set_property(TARGET StripDebugInfo PROPERTY FOLDER "SymUtils")
endif()