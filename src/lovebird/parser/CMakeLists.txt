target_sources(
    lovebird_lib
    PRIVATE
        lexer.cpp
        parser.cpp
)

add_subdirectory(test)
