include(GoogleTest)
add_executable(elixir_loops_tests)

target_sources(
    elixir_loops_tests
    PRIVATE
    main.cpp
    views/just_view_tests.cpp
    views/filter_view_tests.cpp
    views/transform_view_tests.cpp
    views/pass_through_view_tests.cpp
    views/range_adapter_tests.cpp
)

target_link_libraries(
    elixir_loops_tests
    PRIVATE
    elixir::headers
    GTest::gtest
)

gtest_discover_tests(elixir_loops_tests)