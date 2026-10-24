add_subdirectory(lovebird)
add_subdirectory(tools)

add_executable(
    lbi
    main.cpp
)

target_link_libraries(
    lbi
    lovebird_lib
)
