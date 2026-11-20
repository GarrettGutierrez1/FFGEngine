cmake_minimum_required(VERSION 4.4)
project(
    FFGEngine
    VERSION 1.0
    SPDX_LICENSE "Apache-2.0"
    DESCRIPTION "A simple game engine written in C++"
    HOMEPAGE_URL "https://www.finalfactorgames.com/"
    LANGUAGES C CXX
)

# VARIABLES --------------------------------------------------------------------

option(FFG_ENGINE_BUILD_EXAMPLES "If examples should be built" ON)

set(FFG_ENGINE_SOURCES_DIRECTORY "src")
set(FFG_ENGINE_INCLUDE_DIRECTORY "include")
set(FFG_ENGINE_EXAMPLES_DIRECTORY "examples")
set(FFG_ENGINE_SOURCE_DIRECTORIES
    "${FFG_ENGINE_SOURCES_DIRECTORY}"
    "${FFG_ENGINE_EXAMPLES_DIRECTORY}"
)
set(FFG_ENGINE_HEADER_DIRECTORIES "${FFG_ENGINE_INCLUDE_DIRECTORY}")
set(FFG_ENGINE_TARGETS "")

# DEPENDENCIES -----------------------------------------------------------------

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(cmake/macros.cmake)

find_package(glad REQUIRED)
find_package(SDL3 REQUIRED)
find_package(SDL3_image REQUIRED)
find_package(SDL3_mixer REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(sol2 REQUIRED)

print_imported_targets()

# TARGETS ----------------------------------------------------------------------

add_library(FFGEngine)

target_sources(
    FFGEngine
    PRIVATE
        FILE_SET SOURCES
        BASE_DIRS "${FFG_ENGINE_SOURCES_DIRECTORY}"
        FILES
            "${FFG_ENGINE_SOURCES_DIRECTORY}/FFG/ENGINE/Engine.cpp"
            "${FFG_ENGINE_SOURCES_DIRECTORY}/FFG/ENGINE/Error.cpp"
            "${FFG_ENGINE_SOURCES_DIRECTORY}/FFG/ENGINE/StateManager.cpp"
            "${FFG_ENGINE_SOURCES_DIRECTORY}/FFG/ENGINE/Timer.cpp"
            "${FFG_ENGINE_SOURCES_DIRECTORY}/FFG/ENGINE/Window.cpp"
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS "${FFG_ENGINE_INCLUDE_DIRECTORY}"
        FILES
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/Constants.h"
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/Engine.h"
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/Error.h"
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/State.h"
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/StateManager.h"
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/Timer.h"
            "${FFG_ENGINE_INCLUDE_DIRECTORY}/FFG/ENGINE/Window.h"
)

target_compile_features(FFGEngine
    PUBLIC c_std_23
    PUBLIC cxx_std_23
)

set_target_properties(FFGEngine PROPERTIES
    C_EXTENSIONS OFF
    CXX_EXTENSIONS OFF
)

target_link_libraries(
    FFGEngine
    PUBLIC glad::glad
    PUBLIC nlohmann_json::nlohmann_json
    PUBLIC SDL3::SDL3-static
    PUBLIC SDL3_image::SDL3_image-static
    PUBLIC SDL3_mixer::SDL3_mixer-static
    PUBLIC SDL3_ttf::SDL3_ttf-static
    PUBLIC sol2::sol2
)

add_executable(hello_world WIN32)

target_sources(
    hello_world
    PRIVATE
        FILE_SET SOURCES
        BASE_DIRS "${FFG_ENGINE_EXAMPLES_DIRECTORY}"
        FILES
            "${FFG_ENGINE_EXAMPLES_DIRECTORY}/hello_world.cpp"
)

target_link_libraries(
    hello_world
    PRIVATE FFGEngine
)

if(NOT FFG_ENGINE_BUILD_EXAMPLES)
    set_target_properties(hello_world PROPERTIES EXCLUDE_FROM_ALL TRUE)
endif()

set(FFG_ENGINE_TARGETS
    FFGEngine
    hello_world
)

warn_untracked_files(
    FFG_ENGINE_TARGETS
    FFG_ENGINE_SOURCE_DIRECTORIES
    FFG_ENGINE_HEADER_DIRECTORIES
)

# INSTALL ----------------------------------------------------------------------

install(
    TARGETS FFGEngine
    EXPORT FFGEngineTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

install(
    EXPORT FFGEngineTargets
    FILE FFGEngineTargets.cmake
    NAMESPACE FFGEngine::
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/FFGEngine"
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/FFGEngineConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FFGEngineConfig.cmake"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/FFGEngine"
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FFGEngineConfigVersion.cmake"
    VERSION "${PROJECT_VERSION}"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/FFGEngineConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/FFGEngineConfigVersion.cmake"
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/FFGEngine"
)

# EXPORT -----------------------------------------------------------------------

export(
    EXPORT FFGEngineTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/FFGEngineTargets.cmake"
    NAMESPACE FFGEngine::
)
