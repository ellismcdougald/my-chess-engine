#include <catch2/catch_test_macros.hpp>
#include"iostream"

#include "../Move.hpp"
#include "../Board.hpp"
#include "../MoveGenerator.hpp"

TEST_CASE("starting position") {
  Board board;
  MoveGenerator move_gen;
  board.initialize_board_starting_position();

  /*
  std::vector<Move> legal_moves = move_gen.generate_legal_moves(board, BoardConstants::WHITE);
  print_move_vector(legal_moves);
  */

  /*
  SECTION("depth 1") {
    uint64_t result = move_gen.perft(1, board, BoardConstants::WHITE);
    REQUIRE(result == 20);
  }

  SECTION("depth 2") {
    uint64_t result = move_gen.perft(2, board, BoardConstants::WHITE);
    REQUIRE(result == 400);
  }

  SECTION("depth 3") {
    uint64_t result = move_gen.perft(3, board, BoardConstants::WHITE);
    REQUIRE(result == 8902);
  }

  SECTION("depth 4") {
    uint64_t result = move_gen.perft(4, board, BoardConstants::WHITE);
    REQUIRE(result == 197281);
  }
  */

  SECTION("depth 5") {
    uint64_t result = move_gen.divide(5, board, BoardConstants::WHITE);
    REQUIRE(result == 4865609);
  }
}
