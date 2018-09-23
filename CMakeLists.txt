cmake_minimum_required(VERSION 3.0)
project(snake)

file(GLOB sources *.cpp)

set(CMAKE_CXX_STANDARD 17)
add_executable(snake ${sources})
target_compile_options(snake PUBLIC -Wall)

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
target_link_libraries(snake PUBLIC
  ${CURSES_LIBRARIES}
)