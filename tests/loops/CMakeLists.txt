add_executable(elixir_loops_tests)

target_sources(
    elixir_loops_tests
    PRIVATE
    main.cpp
)

target_link_libraries(
    elixir_loops_tests
    PRIVATE
    elixir::headers
)
