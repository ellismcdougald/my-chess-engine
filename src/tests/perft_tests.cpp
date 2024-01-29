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
  */

  /*
  SECTION("perft after pawn push h2 to h3") {
    Move move(0x100, 0x10000, BoardConstants::PAWN, BoardConstants::NONE, false);
    board.execute_move(move, BoardConstants::WHITE);

    uint64_t result = move_gen.perft(1, board, BoardConstants::WHITE);
    std::cout << result << "\n";
  }
  */

}
