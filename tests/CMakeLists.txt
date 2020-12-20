set(BINARY tests)

set(SOURCES string_utilities_tests.cpp
            graph_tests.cpp
            main.cpp
)

add_executable(${BINARY} ${SOURCES})

target_include_directories(${BINARY} PRIVATE
      source
      )
target_link_libraries(${BINARY} gtest, gtest_main)