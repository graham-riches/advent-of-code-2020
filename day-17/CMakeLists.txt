set(BINARY day-17)

set(SOURCES day_17.cpp)

add_executable(${BINARY} ${SOURCES})

target_include_directories(${BINARY} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
      )