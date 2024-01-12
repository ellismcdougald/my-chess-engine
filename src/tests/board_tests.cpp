#include <catch2/catch_test_macros.hpp>
#include "../Board.hpp"

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
