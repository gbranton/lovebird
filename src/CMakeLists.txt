add_subdirectory(lovebird)

add_executable(
    lbi
    main.cpp
)

target_link_libraries(
    lbi
    lovebird_lib
)
