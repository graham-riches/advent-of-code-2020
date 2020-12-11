set(BINARY day-5)
set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi /Ob0 /Od /RTC1")

set(SOURCES day_5.cpp)

add_executable(${BINARY} ${SOURCES})

target_include_directories(${BINARY} PRIVATE
      source
      )