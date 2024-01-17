#include"iostream"
#include <catch2/catch_test_macros.hpp>

#include "../Move.hpp"
#include "../Board.hpp"
#include "../MoveGenerator.hpp"

bool move_vector_contains(Move &move, std::vector<Move> move_vector) {
  for(int i = 0; i < move_vector.size(); i++) {
    if(move.move_equals(move_vector[i])) return true;
  }
  return false;
}

void print_move_vector(std::vector<Move> move_vector) {
  for(int i = 0; i < move_vector.size(); i++) {
    move_vector[i].print_move_hex();
  }
}

TEST_CASE("generate_pawn_pseudo_legal_moves works properly", "[pawn pseudo legal]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("single white pawn on otherwise white board") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x800);

    std::vector<Move> pawn_pseudo_legal_moves = move_gen.generate_pawn_pseudo_legal_moves(board, BoardConstants::WHITE);
    
    Move expected_move_one(0x800, 0x80000, BoardConstants::PAWN, BoardConstants::NONE, false);
    Move expected_move_two(0x800, 0x8000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    
    REQUIRE(move_vector_contains(expected_move_one, pawn_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, pawn_pseudo_legal_moves) == true);
    REQUIRE(pawn_pseudo_legal_moves.size() == 2);
  }
  
  SECTION("single white pawn on d4 vs single black pawn on e5") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x10000000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x800000000);

    // Test white:

    std::vector<Move> pawn_pseudo_legal_moves_white = move_gen.generate_pawn_pseudo_legal_moves(board, BoardConstants::WHITE);

    Move white_expected_move_one(0x10000000, 0x1000000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    Move white_expected_move_two(0x10000000, 0x800000000, BoardConstants::PAWN, BoardConstants::PAWN, false);

    REQUIRE(move_vector_contains(white_expected_move_one, pawn_pseudo_legal_moves_white) == true);
    REQUIRE(move_vector_contains(white_expected_move_two, pawn_pseudo_legal_moves_white) == true);
    REQUIRE(pawn_pseudo_legal_moves_white.size() == 2);

    
    // Test black:
      
    std::vector<Move> pawn_pseudo_legal_moves_black = move_gen.generate_pawn_pseudo_legal_moves(board, BoardConstants::BLACK);

    Move black_expected_move_one(0x800000000, 0x8000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    Move black_expected_move_two(0x800000000, 0x10000000, BoardConstants::PAWN, BoardConstants::PAWN, false);

    REQUIRE(move_vector_contains(black_expected_move_one, pawn_pseudo_legal_moves_black) == true);
    REQUIRE(move_vector_contains(black_expected_move_two, pawn_pseudo_legal_moves_black) == true);
    REQUIRE(pawn_pseudo_legal_moves_black.size() == 2);
  }
}

TEST_CASE("generate_pseudo_legal_en_passant_moves works properly", "[en passant]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("generates both en passant moves when moving pawn is flanked on both sides") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x800);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x14000000);

    Move pawn_push(0x800, 0x8000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    board.execute_move(pawn_push, BoardConstants::WHITE);

    std::vector<Move> black_en_passant_pseudo_legal_moves = move_gen.generate_pseudo_legal_en_passant_moves(board, BoardConstants::BLACK);

    Move expected_move_one(0x10000000, 0x80000, BoardConstants::PAWN, BoardConstants::PAWN, false);
    Move expected_move_two(0x4000000, 0x80000, BoardConstants::PAWN, BoardConstants::PAWN, false);

    REQUIRE(black_en_passant_pseudo_legal_moves.size() == 2);
    REQUIRE(move_vector_contains(expected_move_one, black_en_passant_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, black_en_passant_pseudo_legal_moves) == true);
  }

  SECTION("no en passant moves if last move isn't a double push") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x8000000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x10000000);

    std::vector<Move> black_en_passant_pseudo_legal_moves = move_gen.generate_pseudo_legal_en_passant_moves(board, BoardConstants::BLACK);
    REQUIRE(black_en_passant_pseudo_legal_moves.size() == 0);
  }
}

/*
TEST_CASE("generate_knight_pseudo_legal_moves works properly", "[knight psuedo legal]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("single white knight with central pawns") {
    board.set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, 0x200000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x8000000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x1000000000);

    std::vector<Move> white_knight_pseudo_legal_moves = move_gen.generate_knight_pseudo_legal_moves(board, BoardConstants::WHITE);

    Move expected_move_one(0x200000, 0x4000000000, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_two(0x200000, 0x1000000000, BoardConstants::KNIGHT, BoardConstants::PAWN, false);
    Move expected_move_three(0x200000, 0x80000000, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_four(0x200000, 0x800, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_five(0x200000, 0x40, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_six(0x200000, 0x10, BoardConstants::KNIGHT, BoardConstants::NONE, false);

    REQUIRE(white_knight_pseudo_legal_moves.size() == 6);
    REQUIRE(move_vector_contains(expected_move_one, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_three, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_four, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_five, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_six, white_knight_pseudo_legal_moves) == true);
  }
}
*/
