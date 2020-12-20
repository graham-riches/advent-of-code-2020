set(BINARY day-19)

set(SOURCES day_19.cpp)

add_executable(${BINARY} ${SOURCES})

target_include_directories(${BINARY} PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}
      )