add_library(
    lovebird_syntax_test_lib
    STATIC
    equal.cpp
)

target_link_libraries(
    lovebird_syntax_test_lib
    PUBLIC
        lovebird_lib
        GTest::gtest
)
