#ifndef BOARD_TESTS_CPP
#define BOARD_TESTS_CPP	    

class BoardTests {
public:
  // All Tests;
  void run_all_tests() {
    test_move_direction();
    test_get_sliding_attacks();
  }

  // Move Direction
  void test_move_direction() {
    Board board;
    
    bitboard actual = board.move_direction(1, BoardConstants::WEST);
    bitboard expected = 2;
    assert(actual == expected);
  }
  
  // Sliding Attacks

  /**
   * Runs all get_sliding_attacks tests.
   */
  void test_get_sliding_attacks() {
    test_get_sliding_attacks_one();
    test_get_sliding_attacks_two();
    test_get_sliding_attacks_three();
  }
  
  /**
   * Tests sliding attacks on an empty file.
   */
  void test_get_sliding_attacks_one() {
    BoardConstants::COLOR color = BoardConstants::WHITE;
    BoardConstants::DIRECTION direction = BoardConstants::WEST;
    bitboard position = 1;
    
    Board board;
    board.set_piece_positions(BoardConstants::ROOK, color, 1);

    bitboard actual = board.get_sliding_attacks(position, color, direction);
    bitboard expected = 0b11111110;
    
    assert(actual == expected);
  };

  /**
   * Tests sliding attacks on a file with a piece of the same color.
   */
  void test_get_sliding_attacks_two() {
    BoardConstants::COLOR color = BoardConstants::WHITE;
    BoardConstants::DIRECTION direction = BoardConstants::WEST;
    bitboard position = 1;
    
    Board board;
    board.set_piece_positions(BoardConstants::ROOK, color, 1);
    board.set_piece_positions(BoardConstants::PAWN, color, 0x20);

    bitboard actual = board.get_sliding_attacks(position, color, direction);
    bitboard expected = 0b111110;

    assert(actual == expected);
  };

  /**
   * Tests sliding attacks on a file with a piece of the other color.
   */
  void test_get_sliding_attacks_three() {
    BoardConstants::COLOR color = BoardConstants::WHITE;
    BoardConstants::DIRECTION direction = BoardConstants::WEST;
    bitboard position = 1;
    
    Board board;
    board.set_piece_positions(BoardConstants::ROOK, color, 1);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, 0x20);

    bitboard actual = board.get_sliding_attacks(position, color, direction);
    bitboard expected = 0b111110;
    
    assert(actual == expected);
  };
};

#endif
