cmake_minimum_required(VERSION 3.2)
project(ChessEngine VERSION 1.0)

add_executable(main main.cpp)

set (CMAKE_CXX_FLAGS "-Dprivate=public")

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(board_tests tests/board_tests.cpp Board.cpp Move.cpp)
target_link_libraries(board_tests PRIVATE Catch2::Catch2WithMain)

add_executable(tests tests/tests.cpp Board.cpp Move.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
