cmake_minimum_required(VERSION 3.20)

project(ZusLogic
    VERSION 1.0.0
    DESCRIPTION "ZUS System - domain logic"
    LANGUAGES CXX
)

# ── Global settings ───────────────────────────────────────────────────────────
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(ZusLogic src/main.cpp)

target_include_directories(ZusLogic
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

# ── Tests ─────────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()