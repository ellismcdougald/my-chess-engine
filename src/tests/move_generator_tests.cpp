#include"iostream"
#include <catch2/catch_test_macros.hpp>

#include "../Move.hpp"
#include "../Board.hpp"
#include "../MoveGenerator.hpp"

void print_bb(bitboard bb) {
  bitboard mask = (bitboard) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}

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

TEST_CASE("append_pawn_pseudo_legal_moves works properly", "[pawn pseudo legal]") {
  Board board;
  MoveGenerator move_gen;

  
  SECTION("single white pawn on otherwise white board") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x800);

    std::vector<Move> pawn_pseudo_legal_moves;
    move_gen.append_pawn_pseudo_legal_moves(pawn_pseudo_legal_moves, board, BoardConstants::WHITE);

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

    std::vector<Move> pawn_pseudo_legal_moves_white;
    move_gen.append_pawn_pseudo_legal_moves(pawn_pseudo_legal_moves_white, board, BoardConstants::WHITE);

    Move white_expected_move_one(0x10000000, 0x1000000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    Move white_expected_move_two(0x10000000, 0x800000000, BoardConstants::PAWN, BoardConstants::PAWN, false);

    REQUIRE(move_vector_contains(white_expected_move_one, pawn_pseudo_legal_moves_white) == true);
    REQUIRE(move_vector_contains(white_expected_move_two, pawn_pseudo_legal_moves_white) == true);
    REQUIRE(pawn_pseudo_legal_moves_white.size() == 2);

    
    // Test black:

    std::vector<Move> pawn_pseudo_legal_moves_black;
    move_gen.append_pawn_pseudo_legal_moves(pawn_pseudo_legal_moves_black, board, BoardConstants::BLACK);

    Move black_expected_move_one(0x800000000, 0x8000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    Move black_expected_move_two(0x800000000, 0x10000000, BoardConstants::PAWN, BoardConstants::PAWN, false);

    REQUIRE(move_vector_contains(black_expected_move_one, pawn_pseudo_legal_moves_black) == true);
    REQUIRE(move_vector_contains(black_expected_move_two, pawn_pseudo_legal_moves_black) == true);
    REQUIRE(pawn_pseudo_legal_moves_black.size() == 2);
  }

  SECTION("pawn pseudo legal moves pawn on h3") {
    board.initialize_board_starting_position();
    Move move(0x100, 0x10000, BoardConstants::PAWN, BoardConstants::NONE, false);
    board.execute_move(move, BoardConstants::WHITE);

    std::vector<Move> pawn_pseudo_legal_moves_white;
    move_gen.append_pawn_pseudo_legal_moves(pawn_pseudo_legal_moves_white, board, BoardConstants::WHITE);

    REQUIRE(pawn_pseudo_legal_moves_white.size() == 15);
  }
}


TEST_CASE("append_pseudo_legal_en_passant_moves works properly", "[en passant]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("generates both en passant moves when moving pawn is flanked on both sides") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x800);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x14000000);

    Move pawn_push(0x800, 0x8000000, BoardConstants::PAWN, BoardConstants::NONE, false);
    board.execute_move(pawn_push, BoardConstants::WHITE);

    
    std::vector<Move> black_en_passant_pseudo_legal_moves;
    move_gen.append_pseudo_legal_en_passant_moves(black_en_passant_pseudo_legal_moves, board, BoardConstants::BLACK);

    Move expected_move_one(0x10000000, 0x80000, BoardConstants::PAWN, BoardConstants::PAWN, false);
    Move expected_move_two(0x4000000, 0x80000, BoardConstants::PAWN, BoardConstants::PAWN, false);

    REQUIRE(black_en_passant_pseudo_legal_moves.size() == 2);
    REQUIRE(move_vector_contains(expected_move_one, black_en_passant_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, black_en_passant_pseudo_legal_moves) == true);
  }

  SECTION("no en passant moves if last move isn't a double push") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x8000000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x10000000);

    
   // std::vector<Move> black_en_passant_pseudo_legal_moves = move_gen.generate_pseudo_legal_en_passant_moves(board, BoardConstants::BLACK);
    

    std::vector<Move> black_en_passant_pseudo_legal_moves;
    move_gen.append_pseudo_legal_en_passant_moves(black_en_passant_pseudo_legal_moves, board, BoardConstants::BLACK);
    
    REQUIRE(black_en_passant_pseudo_legal_moves.size() == 0);
  }
}


TEST_CASE("append_knight_pseudo_legal_moves works properly", "[knight psuedo legal]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("single white knight with central pawns") {
    board.set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, 0x200000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x8000000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x1000000000);

    std::vector<Move> white_knight_pseudo_legal_moves;
    move_gen.append_piece_pseudo_legal_moves(white_knight_pseudo_legal_moves, board, BoardConstants::KNIGHT, BoardConstants::WHITE);

    Move expected_move_one(0x200000, 0x4000000000, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_two(0x200000, 0x1000000000, BoardConstants::KNIGHT, BoardConstants::PAWN, false);
    Move expected_move_three(0x200000, 0x80000000, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_four(0x200000, 0x800, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_five(0x200000, 0x40, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_six(0x200000, 0x10, BoardConstants::KNIGHT, BoardConstants::NONE, false);
    Move expected_move_seven(0x200000, 0x8000, BoardConstants::KNIGHT, BoardConstants::NONE, false);

    REQUIRE(white_knight_pseudo_legal_moves.size() == 7);
    REQUIRE(move_vector_contains(expected_move_one, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_three, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_four, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_five, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_six, white_knight_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_seven, white_knight_pseudo_legal_moves) == true);
  }
}



TEST_CASE("append_bishop_pseudo_legal_moves works properly", "[bishop pseudo legal]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("white bishop with variety of blocking pieces") {
    board.set_piece_positions(BoardConstants::QUEEN, BoardConstants::WHITE, 0x10);
    board.set_piece_positions(BoardConstants::BISHOP, BoardConstants::WHITE, 0x2000);
    board.set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, 0x400000000);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x80000000);

    std::vector<Move> white_bishop_pseudo_legal_moves;
    move_gen.append_piece_pseudo_legal_moves(white_bishop_pseudo_legal_moves, board, BoardConstants::BISHOP, BoardConstants::WHITE);

    Move expected_move_one(0x2000, 0x100000, BoardConstants::BISHOP, BoardConstants::NONE, false);
    Move expected_move_two(0x2000, 0x8000000, BoardConstants::BISHOP, BoardConstants::NONE, false);
    Move expected_move_three(0x2000, 0x40, BoardConstants::BISHOP, BoardConstants::NONE, false);
    Move expected_move_four(0x2000, 0x400000, BoardConstants::BISHOP, BoardConstants::NONE, false);
    Move expected_move_five(0x2000, 0x80000000, BoardConstants::BISHOP, BoardConstants::PAWN, false);

    REQUIRE(white_bishop_pseudo_legal_moves.size() == 5);
    REQUIRE(move_vector_contains(expected_move_one, white_bishop_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, white_bishop_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_three, white_bishop_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_four, white_bishop_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_five, white_bishop_pseudo_legal_moves) == true);
  }
}


TEST_CASE("append_rook_pseudo_legal_moves works properly", "[rook pseudo legal]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("black rook with black blocker and white blocker") {
    board.set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, 0x100000000);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::BLACK, 0x100000000000000);
    board.set_piece_positions(BoardConstants::KING, BoardConstants::BLACK, 0x800000000000000);

    std::vector<Move> black_rook_pseudo_legal_moves;
    move_gen.append_piece_pseudo_legal_moves(black_rook_pseudo_legal_moves, board, BoardConstants::ROOK, BoardConstants::BLACK);

    Move expected_move_one(0x100000000000000, 0x200000000000000, BoardConstants::ROOK, BoardConstants::NONE, false);
    Move expected_move_two(0x100000000000000, 0x400000000000000, BoardConstants::ROOK, BoardConstants::NONE, false);
    Move expected_move_three(0x100000000000000, 0x1000000000000, BoardConstants::ROOK, BoardConstants::NONE, false);
    Move expected_move_four(0x100000000000000, 0x10000000000, BoardConstants::ROOK, BoardConstants::NONE, false);
    Move expected_move_five(0x100000000000000, 0x100000000, BoardConstants::ROOK, BoardConstants::KNIGHT, false);

    REQUIRE(black_rook_pseudo_legal_moves.size() == 5);
    REQUIRE(move_vector_contains(expected_move_one, black_rook_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, black_rook_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_three, black_rook_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_four, black_rook_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_five, black_rook_pseudo_legal_moves) == true);
  }
}

TEST_CASE("append queen pseudo legal moves works properly", "[queen pseudo legal]") {
  Board board;
  MoveGenerator move_gen;

  SECTION("white queen with variety of blockers") {
    board.set_piece_positions(BoardConstants::QUEEN, BoardConstants::WHITE, 0x8);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, 0x81);
    board.set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, 0x200000);
    board.set_piece_positions(BoardConstants::BISHOP, BoardConstants::BLACK, 0x9000000);

    std::vector<Move> white_queen_pseudo_legal_moves;
    move_gen.append_piece_pseudo_legal_moves(white_queen_pseudo_legal_moves, board, BoardConstants::QUEEN, BoardConstants::WHITE);

    Move expected_move_one(0x8, 0x4, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_two(0x8, 0x2, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_three(0x8, 0x10, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_four(0x8, 0x20, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_five(0x8, 0x40, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_six(0x8, 0x400, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_seven(0x8, 0x20000, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_eight(0x8, 0x800, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_nine(0x8, 0x80000, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_ten(0x8, 0x20000, BoardConstants::QUEEN, BoardConstants::NONE, false);
    Move expected_move_eleven(0x8, 0x8000000, BoardConstants::QUEEN, BoardConstants::BISHOP, false);
    Move expected_move_twelve(0x8, 0x1000000, BoardConstants::QUEEN, BoardConstants::BISHOP, false);

    REQUIRE(white_queen_pseudo_legal_moves.size() == 12);
    REQUIRE(move_vector_contains(expected_move_one, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_two, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_three, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_four, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_five, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_six, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_seven, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_eight, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_nine, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_ten, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_eleven, white_queen_pseudo_legal_moves) == true);
    REQUIRE(move_vector_contains(expected_move_twelve, white_queen_pseudo_legal_moves) == true);
  }
}


TEST_CASE("append legal castle moves works properly") {
  Board board;
  MoveGenerator move_gen;

  SECTION("Can castle both ways on otherwise empty board other than king and two rooks") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 0x8);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, 0x81);

    std::vector<Move> castle_moves;
    move_gen.append_legal_castle_moves(castle_moves, board, BoardConstants::WHITE);

    Move expected_move_one(0x8, 0x2, BoardConstants::KING, BoardConstants::NONE, true);
    Move expected_move_two(0x8, 0x20, BoardConstants::KING, BoardConstants::NONE, true);

    REQUIRE(castle_moves.size() == 2);
    REQUIRE(move_vector_contains(expected_move_one, castle_moves) == true);
    REQUIRE(move_vector_contains(expected_move_one, castle_moves) == true);
  }

  
  SECTION("Can only castle king side if queen side path is attacked") {
     board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 0x8);
     board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, 0x81);
     board.set_piece_positions(BoardConstants::ROOK, BoardConstants::BLACK, 0x400000);
    
     std::vector<Move> castle_moves;
     move_gen.append_legal_castle_moves(castle_moves, board, BoardConstants::WHITE);

    Move expected_move_one(0x8, 0x2, BoardConstants::KING, BoardConstants::NONE, true);
    REQUIRE(castle_moves.size() == 1);
    REQUIRE(move_vector_contains(expected_move_one, castle_moves) == true);
  }
}
