include(FetchContent)

# ── GoogleTest ────────────────────────────────────────────────────────────────
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# ── Silence GTest warnings ────────────────────────────────────────────────────
foreach(_gtest_target gtest gtest_main gmock gmock_main)
    if(TARGET ${_gtest_target})
        target_compile_options(${_gtest_target} PRIVATE -w)
        get_target_property(_inc ${_gtest_target} INTERFACE_INCLUDE_DIRECTORIES)
        if(_inc)
            set_target_properties(${_gtest_target} PROPERTIES
                INTERFACE_SYSTEM_INCLUDE_DIRECTORIES "${_inc}"
            )
        endif()
    endif()
endforeach()

# ── Test sources ──────────────────────────────────────────────────────────────
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES})

target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
)

target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        GTest::gtest_main
        GTest::gmock
)

target_compile_options(${PROJECT_NAME}_tests PRIVATE -Wall -Wextra -Wpedantic)

# ── CTest integration ─────────────────────────────────────────────────────────
include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)