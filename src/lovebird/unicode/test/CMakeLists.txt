add_executable(
    lovebird_unicode_test
    utf8_test.cpp
)

target_link_libraries(
    lovebird_unicode_test
    lovebird_lib
    GTest::gmock_main
)

gtest_discover_tests(lovebird_unicode_test)
