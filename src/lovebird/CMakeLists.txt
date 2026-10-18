add_library(lovebird_lib)

add_executable(lovebird_test)

target_link_libraries(
    lovebird_test
    lovebird_lib
    GTest::gmock_main
)

gtest_discover_tests(lovebird_test)

add_subdirectory(diagnostic)
add_subdirectory(parser)
add_subdirectory(syntax)
add_subdirectory(unicode)
