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

# ── libpqxx (Homebrew) ────────────────────────────────────────────────────────
set(LIBPQXX_PREFIX /opt/homebrew/opt/libpqxx)
set(PG_PREFIX      /opt/homebrew/opt/postgresql@16)

find_library(PQXX_LIB pqxx HINTS ${LIBPQXX_PREFIX}/lib REQUIRED)
find_library(PQ_LIB   pq   HINTS ${PG_PREFIX}/lib       REQUIRED)

add_library(libpqxx INTERFACE)
target_include_directories(libpqxx INTERFACE ${LIBPQXX_PREFIX}/include)
target_link_libraries(libpqxx INTERFACE ${PQXX_LIB} ${PQ_LIB})

# ── Executable ────────────────────────────────────────────────────────────────
add_executable(ZusLogic src/main.cpp)

target_include_directories(ZusLogic PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(ZusLogic PRIVATE libpqxx)

# ── Tests ─────────────────────────────────────────────────────────────────────
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()