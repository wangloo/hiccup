# ---- Name of test target ----

set(TEST_TARGET ${TARGET_LIB}_test)

# ---- Source files of test ----

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
    "*.c" "*.C" "*.cc" "*.CC" "*.cpp" "*.CPP" "*.c++")

# ---- Define test target ----

add_executable(${TEST_TARGET} ${SOURCES})

target_include_directories(${TEST_TARGET}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
            ${PROJECT_SOURCE_DIR}/src
            PRIVATE ${PROJECT_SOURCE_DIR}/deps )

target_compile_options(${TEST_TARGET} PRIVATE -Wno-invalid-offsetof)
target_compile_features(${TEST_TARGET} PRIVATE cxx_std_17)

target_link_libraries(${TEST_TARGET} PRIVATE ${TARGET_LIB})

# ---- Code coverage ----

if(ENABLE_TEST_COVERAGE)
    target_compile_options(${TEST_TARGET} PUBLIC -O0 -g -fprofile-arcs -ftest-coverage)
    target_link_options(${TEST_TARGET} PUBLIC -fprofile-arcs -ftest-coverage)
endif()

# ---- Add test for CTest ----

include(CTest)
enable_testing()
add_test(NAME test COMMAND ${TEST_TARGET})
