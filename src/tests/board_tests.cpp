#include"iostream"
#include <catch2/catch_test_macros.hpp>
#include "../Move.hpp"
#include "../Board.hpp"

/*
void print_bitboard(bitboard bb) {
  bitboard mask = (bitboard) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}
*/


TEST_CASE( "move_direction works properly", "[move_direction]") {
  Board board;
  bitboard position = 0x800;

  SECTION("testing north direction") {
    bitboard result = board.move_direction(position, BoardConstants::NORTH);
    bitboard expected = 0x80000;

    REQUIRE(result == expected);
  }

  SECTION("testing southwest direction") {
    bitboard result = board.move_direction(position, BoardConstants::SOUTHWEST);
    bitboard expected = 0x10;

    REQUIRE(result == expected);
  }
}

TEST_CASE( "get_king_attacks works properly", "[get_king_attacks]") {
  Board board;

  SECTION("testing king attacks in middle of empty board") {
    bitboard king_position = 0x80000;
    bitboard actual = board.get_king_attacks(king_position);
    bitboard expected = 0x1C141C00;

    REQUIRE(actual == expected);
  }

  SECTION("testing king attacks in corner of empty board") {
    bitboard king_position = 1;
    bitboard actual = board.get_king_attacks(king_position);
    bitboard expected = 0x302;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("get_knight_attacks works properly", "[get_knight_attacks]") {
  Board board;

  SECTION("testing knight attacks in middle of empty board") {
    bitboard knight_position = 0x80000;
    bitboard actual = board.get_knight_attacks(knight_position);
    bitboard expected = 0x1422002214;

    REQUIRE(actual == expected);
  }

  SECTION("testing knight attacks in corner of empty board") {
    bitboard knight_position = 1;
    bitboard actual = board.get_knight_attacks(knight_position);
    bitboard expected = 0x20400;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("get_pawn_attacks works properly", "[get_pawn_attacks]") {
  Board board;

  SECTION("Testing white pawn attacks in middle of empty board") {
    bitboard pawn_position = 0x80000;
    bitboard actual = board.get_pawn_attacks(pawn_position, BoardConstants::WHITE);
    bitboard expected = 0x14000000;

    REQUIRE(actual == expected);
  }

  SECTION("Testing black pawn attacks in middle of empty board") {
    bitboard pawn_position = 0x80000;
    bitboard actual = board.get_pawn_attacks(pawn_position, BoardConstants::BLACK);
    bitboard expected = 0x1400;

    REQUIRE(actual == expected);
  }

  SECTION("Testing white pawn attacks in corner of empty board") {
    bitboard pawn_position = 1;
    bitboard actual = board.get_pawn_attacks(pawn_position, BoardConstants::WHITE);
    bitboard expected = 0x200;

    REQUIRE(actual == expected);
  }

  SECTION("Testing black pawn attacks in corner of empty board") {
    bitboard pawn_position = 1;
    bitboard actual = board.get_pawn_attacks(pawn_position, BoardConstants::BLACK);
    bitboard expected = 0;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("get_queen_attacks works properly", "[get_queen_attacks]") {
  Board board;

  SECTION("Testing queen attacks in centre of empty board") {
    bitboard queen_position = 0x800000000;
    bitboard actual = board.get_queen_attacks(queen_position);
    bitboard expected = 0x492A1CF71C2A4988;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("get_rook_attacks works properly", "[get_rook_attacks]") {
  Board board;

  SECTION("Testing rook attacks on empty board") {
    Board board;
    bitboard rook_position = 0x800000000;

    bitboard actual = board.get_rook_attacks(rook_position);
    bitboard expected = 0x080808F708080808;

    REQUIRE(actual == expected);
  }

  SECTION("Testing rook attacks with another rook on the board") {
    bitboard white_rook_position_one = 0x200;
    bitboard white_rook_position_two = 0x20000000000;

    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, white_rook_position_one);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, white_rook_position_two);

    bitboard actual = board.get_rook_attacks(white_rook_position_one);
    bitboard expected = 0x000002020202FD02;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("get_bishop_attacks works properly", "[get_bishop_attacks]") {
  Board board;

  SECTION("Testing bishop attacks in middle of an empty board") {
    bitboard bishop_position = 0x8000000;
    
    board.set_piece_positions(BoardConstants::BISHOP,
			      BoardConstants::WHITE,
			      bishop_position);
    bitboard result = board.get_bishop_attacks(bishop_position);
    bitboard expected = 0x8041221400142241;

    REQUIRE(result == expected);
  }

  SECTION("testing bishop attacks on crowded board") {
    bitboard white_bishop_position = 0x2000000;
    bitboard white_pawn_position_one = 0x10000;
    bitboard white_pawn_position_two = 0x800;
    bitboard black_bishop_position = 0x80000000000;

    board.set_piece_positions(BoardConstants::BISHOP, BoardConstants::WHITE, white_bishop_position);
    board.set_piece_positions(BoardConstants::PAWN,  BoardConstants::WHITE, white_pawn_position_one);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, white_pawn_position_two);
    board.set_piece_positions(BoardConstants::BISHOP,BoardConstants::BLACK, black_bishop_position);

    bitboard result = board.get_bishop_attacks(white_bishop_position);
    bitboard expected = 0x0000080500050800;

    REQUIRE(result == expected);
  }	  
}

TEST_CASE("get_sliding_attacks works properly", "[get_sliding_attacks]") {
  Board board;

  SECTION("Tests sliding attacks WEST on an empty file") {
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, 1);

    bitboard actual = board.get_sliding_attacks(1, BoardConstants::WEST);
    bitboard expected = 0b11111110;
    
    REQUIRE(actual == expected);
  }

  SECTION("Tests sliding attacks on a file with a pawn on the file") {
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x20);

    bitboard actual = board.get_sliding_attacks(1, BoardConstants::WEST);
    bitboard expected = 0b111110;

    REQUIRE(actual == expected);
  }
}

TEST_CASE("get_attacks_to_position works properly", "[get_attacks_to_position]") {
  Board board;
  // Place white rook on h1
  board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, 1);
  // Place white queen on a8
  board.set_piece_positions(BoardConstants::QUEEN, BoardConstants::WHITE, 0x8000000000000000);
  // Place white pawn on g7
  board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, 0x2000000000000);

  SECTION("h8 is attacked by all three pieces") {
    bitboard attacks_to_position = board.get_attacks_to_position(0x100000000000000, BoardConstants::WHITE);
    bitboard expected = 0x8002000000000001;
    
    REQUIRE(attacks_to_position == expected);
  }

  SECTION("a1 is attacked by rook and queen") {
    bitboard attacks_to_position = board.get_attacks_to_position(0x80, BoardConstants::WHITE);
    bitboard expected = 0x8000000000000001;

    
    REQUIRE(attacks_to_position == expected);
  }

  SECTION("g2 is attacked by queen") {
    bitboard attacks_to_position = board.get_attacks_to_position(0x200, BoardConstants::WHITE);
    bitboard expected = 0x8000000000000000;
    
    REQUIRE(attacks_to_position == expected);
  }

  SECTION("d4 is not attacked") {
    bitboard attacks_to_position = board.get_attacks_to_position(0x10000000, BoardConstants::WHITE);
    bitboard expected = 0;
    
    REQUIRE(attacks_to_position == expected);
  }
  
}


TEST_CASE("is_checked works properly", "[is_checked]") {
  Board board;

  SECTION("King on empty board is not in check") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 1);
    bool white_is_checked_actual = board.is_checked(BoardConstants::WHITE);

    REQUIRE(white_is_checked_actual == false);
  }

  SECTION("Attacked king is in check") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 1);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::BLACK, 0x08);

    bool white_is_checked_actual = board.is_checked(BoardConstants::WHITE);

    REQUIRE(white_is_checked_actual == true);
  }
}

TEST_CASE("execute_move works properly", "[execute_move]") {
  Board board;

  SECTION("Moves white king from a8 to a7") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 1);
    Move move((bitboard) 1, (bitboard) 0x100, BoardConstants::KING, BoardConstants::NONE, false);
    board.execute_move(move, BoardConstants::WHITE);

    REQUIRE(board.white_bitboards[BoardConstants::KING] == 0x100);
  }
  
  SECTION("Black rook captures white queen at d4") {
    board.set_piece_positions(BoardConstants::QUEEN, BoardConstants::WHITE, 0x10000000);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::BLACK, 0x1000000000000000);

    Move move(0x1000000000000000, 0x10000000, BoardConstants::ROOK, BoardConstants::QUEEN, false);
    board.execute_move(move, BoardConstants::BLACK);

    REQUIRE(board.white_bitboards[BoardConstants::QUEEN] == 0);
    REQUIRE(board.black_bitboards[BoardConstants::ROOK] == 0x10000000);
  }
}

TEST_CASE("undo_move works properly", "[undo_move]") {
  Board board;

  SECTION("Undoes move white king from a8 to a7") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 0x100);
    Move move((bitboard) 1, (bitboard) 0x100, BoardConstants::KING, BoardConstants::NONE, false);

    board.undo_move(move, BoardConstants::WHITE);

    REQUIRE(board.white_bitboards[BoardConstants::KING] == 1);
  }

  SECTION("undoes move black rook captures white queen at d4") {
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::BLACK, 0x10000000);

    Move move(0x1000000000000000, 0x10000000, BoardConstants::ROOK, BoardConstants::QUEEN, false);
    board.undo_move(move, BoardConstants::BLACK);

    REQUIRE(board.white_bitboards[BoardConstants::QUEEN] == 0x10000000);
    REQUIRE(board.black_bitboards[BoardConstants::ROOK] == 0x1000000000000000);
  }
}

TEST_CASE("is_move_legal works properly", "[is_move_legal]") {
  Board board;

  SECTION("king move on empty board is legal") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 1);
    Move move(1, 0x100, BoardConstants::KING, BoardConstants::NONE, false);
    bool move_legal = board.is_move_legal(move, BoardConstants::WHITE);

    REQUIRE(move_legal == true);
  }

  SECTION("king move into check is illegal") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 1);
    board.set_piece_positions(BoardConstants::QUEEN, BoardConstants::BLACK, 0x8000);
    Move move(1, 0x100, BoardConstants::KING, BoardConstants::NONE, false);
    
    bool move_legal = board.is_move_legal(move, BoardConstants::WHITE);

    REQUIRE(move_legal == false);
  }

  SECTION("moving pinned knight is illegal") {
    board.set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, 8);
    board.set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, 0x200000);
    board.set_piece_positions(BoardConstants::BISHOP, BoardConstants::BLACK, 0x40000000);

    Move move(0x200000, 0x8000000, BoardConstants::KNIGHT, BoardConstants::NONE, false);

    bool move_legal = board.is_move_legal(move, BoardConstants::WHITE);

    REQUIRE(move_legal == false);
  }
}


