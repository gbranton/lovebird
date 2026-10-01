add_executable(
    lovebird_lexer_test
    lexer_test.cpp
)

target_link_libraries(
    lovebird_lexer_test
    lovebird_lib
    GTest::gmock_main
)

gtest_discover_tests(lovebird_lexer_test)
