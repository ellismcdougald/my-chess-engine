#ifndef BOARD_TESTS_CPP
#define BOARD_TESTS_CPP	    

class BoardTests {
public:
  // All Tests;
  void run_all_tests() {
    test_move_direction();
    test_get_sliding_attacks();
    test_get_bishop_attacks();
    test_get_rook_attacks();
    test_get_rook_attacks();
    test_get_queen_attacks();
  }

  // Test get_queen_attacks
  /**
   * Runs all get_queen_attacks tests
   */
  void test_get_queen_attacks() {
    test_get_queen_attacks_one();
  }

  /**
   * Verifies get_queen_attacks on an empty board.
   */
  void test_get_queen_attacks_one() {
    Board board;
    bitboard queen_position = 0x800000000;

    bitboard actual = board.get_queen_attacks(queen_position, BoardConstants::BLACK);
    bitboard expected = 0x492A1CF71C2A4988;

    assert(actual == expected);
  }

  // Test get_rook_attacks
  /**
   * Runs all get_rook_attacks tests
   */
  void test_get_rook_attacks() {
    test_get_rook_attacks_one();
    test_get_rook_attacks_two();
  }

  /**
   * Verifies rook attacks on otherwise empty board.
   */
  void test_get_rook_attacks_one() {
    Board board;
    bitboard black_rook_position = 0x800000000;

    bitboard actual = board.get_rook_attacks(black_rook_position, BoardConstants::BLACK);
    bitboard expected = 0x080808F708080808;

    assert(actual == expected);
  }

  /**
   * Verifies rook attacks with another white rook on the board.
   */
  void test_get_rook_attacks_two() {
    Board board;
    bitboard white_rook_position_one = 0x200;
    bitboard white_rook_position_two = 0x20000000000;

    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, white_rook_position_one);
    board.set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, white_rook_position_two);

    bitboard actual = board.get_rook_attacks(white_rook_position_one, BoardConstants::WHITE);
    print_bitboard(actual);
    bitboard expected = 0x000002020202FD02;

    assert(actual == expected);
  }

  // Test get_bishop_attacks
  /**
   * Runs all get_bishop_attacks tests.
   */
  void test_get_bishop_attacks() {
    test_get_bishop_attacks_one();
    test_get_bishop_attacks_two();
  }
  
  /**
   * Verifies bishop attacks on otherwise empty board.
   */
  void test_get_bishop_attacks_one() {
    Board board;
    bitboard bishop_position = 0x8000000;
    BoardConstants::COLOR color = BoardConstants::WHITE;
    
    board.set_piece_positions(BoardConstants::BISHOP,
			      BoardConstants::WHITE,
			      bishop_position);
    bitboard result = board.get_bishop_attacks(bishop_position, color);
    bitboard expected = 0x8041221400142241;

    assert(result == expected);
  }

  /**
   * Verifies bishop attacks on crowded board.
   */
  void test_get_bishop_attacks_two() {
    Board board;
    bitboard white_bishop_position = 0x2000000;
    bitboard white_pawn_position_one = 0x10000;
    bitboard white_pawn_position_two = 0x800;
    bitboard black_bishop_position = 0x80000000000;

    board.set_piece_positions(BoardConstants::BISHOP, BoardConstants::WHITE, white_bishop_position);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, white_pawn_position_one);
    board.set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, white_pawn_position_two);
    board.set_piece_positions(BoardConstants::BISHOP, BoardConstants::BLACK, black_bishop_position);

    bitboard result = board.get_bishop_attacks(white_bishop_position, BoardConstants::WHITE);
    bitboard expected = 0x0000080500050800;

    assert(result == expected);
  }
  

  // Move Direction
  void test_move_direction() {
    test_move_direction_one();
    test_move_direction_two();
  }
  
  void test_move_direction_one() {
    Board board;
    
    bitboard actual = board.move_direction(1, BoardConstants::WEST);
    bitboard expected = 2;
    assert(actual == expected);
  }

  void test_move_direction_two() {
    Board board;

    bitboard position = 0x8000000;

    bitboard actual = board.move_direction(0x8000000, BoardConstants::NORTHEAST);
    bitboard expected = 0x400000000;

    assert(actual == expected);
  }
  
  // Sliding Attacks

  /**
   * Runs all get_sliding_attacks tests.
   */
  void test_get_sliding_attacks() {
    //test_get_sliding_attacks_one();
    //test_get_sliding_attacks_two();
    //test_get_sliding_attacks_three();
    //test_get_sliding_attacks_four();
    test_get_sliding_attacks_five();
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

  /**
   * Tests sliding attacks NORTHWEST on otherwise empty board.
   */
  void test_get_sliding_attacks_four() {
    Board board;
    bitboard bishop_position = 0x8000000;
    BoardConstants::COLOR color = BoardConstants::WHITE;
    board.set_piece_positions(BoardConstants::BISHOP,
			      BoardConstants::WHITE,
			      bishop_position);

    bitboard result = board.get_sliding_attacks(bishop_position, color, BoardConstants::NORTHWEST);
    bitboard expected = 0x8040201000000000;

    assert(result == expected);
  }

  /**
   * Tests sliding attacks NORTHEAST on otherwise empty board.
   */
  void test_get_sliding_attacks_five() {
    Board board;
    bitboard bishop_position = 0x8000000;
    BoardConstants::COLOR color = BoardConstants::WHITE;
    board.set_piece_positions(BoardConstants::BISHOP,
			      BoardConstants::WHITE,
			      bishop_position);

    bitboard result = board.get_sliding_attacks(bishop_position, color, BoardConstants::NORTHEAST);
    bitboard expected = 0x0001020400000000;

    assert(result == expected);
  }
};

#endif
