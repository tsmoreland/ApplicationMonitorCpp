project(shared CXX)

# Source groups
set(no_group_source_files)
source_group("" FILES ${no_group_source_files})

set(public_header_files
    "${CMAKE_SOURCE_DIR}/include/shared/shared_export.h"
    "${CMAKE_SOURCE_DIR}/include/shared/bad_owner_access.h"
    "${CMAKE_SOURCE_DIR}/include/shared/data_member.h"
    "${CMAKE_SOURCE_DIR}/include/shared/environment_repository.h"
    "${CMAKE_SOURCE_DIR}/include/shared/invalid_handle.h"
    "${CMAKE_SOURCE_DIR}/include/shared/not_found_exception.h"
    "${CMAKE_SOURCE_DIR}/include/shared/null_handle.h"
    "${CMAKE_SOURCE_DIR}/include/shared/unique_handle.h"
    "${CMAKE_SOURCE_DIR}/include/shared/collection.h"
    "${CMAKE_SOURCE_DIR}/include/shared/string_extensions.h"
    "${CMAKE_SOURCE_DIR}/include/shared/future.h"
    "${CMAKE_SOURCE_DIR}/include/shared/command_result.h"
    "${CMAKE_SOURCE_DIR}/include/shared/process.h"
    "${CMAKE_SOURCE_DIR}/include/shared/file_service.h"
    "${CMAKE_SOURCE_DIR}/include/shared/process_service.h"
)

set(private_header_files
    "pch.h"
    "environment_repository_impl.h"
    "process_impl.h"
    "file_service_impl.h"
    "process_service_impl.h"
)
source_group("header files" FILES ${public_header_fiels} ${private_header_files})

set(source_files
    "pch.cpp"
    "environment_repository_impl.cpp"
    "process_impl.cpp"
    "file_service_impl.cpp"
    "process_service_impl.cpp"
)

set(ALL_FILES ${public_header_files} ${private_header_files} ${source_files})

# Target
add_library(${PROJECT_NAME} SHARED ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE shared)

set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_KEYWORD "Win32Proj")

# Output directory
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/${CMAKE_VS_PLATFORM_NAME}/$<CONFIG>/"
    )
endif()

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

# Include directories
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
endif()

# Compile definitions
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "SHARED_DLL_EXPORT;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
        "SHARED_DLL_EXPORT"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "SHARED_DLL_EXPORT;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
        "SHARED_DLL_EXPORT"
    )
endif()

# Compile and link options
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /std:c++latest;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                ${DEFAULT_CXX_DEBUG_RUNTIME_LIBRARY}
            >
            $<$<CONFIG:Release>:
                /Oi;
                ${DEFAULT_CXX_RUNTIME_LIBRARY};
                /Gy
            >
            /permissive-;
            /std:c++latest;
            /sdl;
            /W4;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG;
                /INCREMENTAL
            >
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

