add_executable(
    lovebird_parser_test
    parser_test.cpp
)

target_link_libraries(
    lovebird_parser_test
    lovebird_lib
    lovebird_syntax_test_lib
    GTest::gmock_main
)

gtest_discover_tests(lovebird_parser_test)
