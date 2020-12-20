set(BINARY tests)

set(SOURCES string_utilities_tests.cpp
            graph_tests.cpp
			grid_tests.cpp
            main.cpp
)

add_executable(${BINARY} ${SOURCES})


target_link_libraries(${BINARY} gtest, gtest_main)