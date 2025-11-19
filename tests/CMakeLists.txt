# Tests CMakeLists.txt

find_package(Threads REQUIRED)

# Logger tests
add_executable(logger_tests
    logging/LoggerTest.cpp
    logging/LogLevelTest.cpp
    logging/LogCategoryTest.cpp
)

target_link_libraries(logger_tests
    PRIVATE
        vroom
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
)

target_include_directories(logger_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Enable coverage if requested
if(VROOM_ENABLE_COVERAGE)
    # Check if compiler supports coverage flags (GCC/Clang)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        # Use atomic profile updates to avoid race conditions in multi-threaded code
        target_compile_options(logger_tests PRIVATE --coverage -fprofile-arcs -ftest-coverage -fprofile-update=atomic)
        target_link_options(logger_tests PRIVATE --coverage)
        
        # Also enable coverage for the vroom library
        target_compile_options(vroom PRIVATE --coverage -fprofile-arcs -ftest-coverage -fprofile-update=atomic)
        target_link_options(vroom PRIVATE --coverage)
        
        message(STATUS "Code coverage enabled for ${CMAKE_CXX_COMPILER_ID}")
    else()
        message(WARNING "Code coverage not supported for compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()
endif()

# Add test to CTest
add_test(NAME LoggerTests COMMAND logger_tests)

# Configure test to output JUnit XML for GitHub Actions
set_tests_properties(LoggerTests PROPERTIES
    ENVIRONMENT "GTEST_OUTPUT=xml:${CMAKE_BINARY_DIR}/test-results.xml"
)

