set(BINARY day-20)

set(SOURCES day_20.cpp)

add_executable(${BINARY} ${SOURCES})

target_include_directories(${BINARY} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
      )