set(BINARY day-25)

set(SOURCES day_25.cpp)

add_executable(${BINARY} ${SOURCES})

target_include_directories(${BINARY} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
      )