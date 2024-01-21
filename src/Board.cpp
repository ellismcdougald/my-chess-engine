#ifndef BOARD_CPP // GUARD
#define BOARD_CPP // GUARD

#include"Board.hpp"

#include<iostream>

// Initialize constants:
const bitboard Board::FILE_A = 0x8080808080808080;
const bitboard Board::FILE_H = 0x0101010101010101;
const bitboard Board::RANK_1 = 0xFF;
const bitboard Board::RANK_8 = 0xFF00000000000000;

const bitboard Board::starting_white_king_position = 0x8;
const bitboard Board::starting_white_queen_position = 0x10;
const bitboard Board::starting_white_rook_position = 0x81;
const bitboard Board::starting_white_bishop_position = 0x24;
const bitboard Board::starting_white_knight_position = 0x42;
const bitboard Board::starting_white_pawn_position = 0xFF00;

const bitboard Board::starting_black_king_position = 0x800000000000000;
const bitboard Board::starting_black_queen_position = 0x1000000000000000;
const bitboard Board::starting_black_rook_position = 0x8100000000000000;
const bitboard Board::starting_black_bishop_position = 0x2400000000000000;
const bitboard Board::starting_black_knight_position = 0x4200000000000000;
const bitboard Board::starting_black_pawn_position = 0x00FF000000000000;

const bitboard Board::white_queenside_castle_king_position = 0x20;
const bitboard Board::white_queenside_castle_rook_position = 0x10;
const bitboard Board::white_kingside_castle_king_position = 0x2;
const bitboard Board::white_kingside_castle_rook_position = 0x4;
const bitboard Board::black_queenside_castle_king_position = 0x2000000000000000;
const bitboard Board::black_queenside_castle_rook_position = 0x1000000000000000;
const bitboard Board::black_kingside_castle_king_position = 0x200000000000000;
const bitboard Board::black_kingside_castle_rook_position = 0x400000000000000;

// Constructors:
Board::Board() {
  white_bitboards.fill(0);
  black_bitboards.fill(0);
  initialize_lookups();

  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      can_castle[i][j] = true;
    }
  }
}

// Initializers:
void Board::initialize_board_starting_position() {
  set_piece_positions(BoardConstants::KING, BoardConstants::WHITE, starting_white_king_position);
  set_piece_positions(BoardConstants::QUEEN, BoardConstants::WHITE, starting_white_queen_position);
  set_piece_positions(BoardConstants::ROOK, BoardConstants::WHITE, starting_white_rook_position);
  set_piece_positions(BoardConstants::BISHOP, BoardConstants::WHITE, starting_white_bishop_position);
  set_piece_positions(BoardConstants::KNIGHT, BoardConstants::WHITE, starting_white_knight_position);
  set_piece_positions(BoardConstants::PAWN, BoardConstants::WHITE, starting_white_pawn_position);

  set_piece_positions(BoardConstants::KING, BoardConstants::BLACK, starting_black_king_position);
  set_piece_positions(BoardConstants::QUEEN, BoardConstants::BLACK, starting_black_queen_position);
  set_piece_positions(BoardConstants::ROOK, BoardConstants::BLACK, starting_black_rook_position);
  set_piece_positions(BoardConstants::BISHOP, BoardConstants::BLACK, starting_black_bishop_position);
  set_piece_positions(BoardConstants::KNIGHT, BoardConstants::BLACK, starting_black_knight_position);
  set_piece_positions(BoardConstants::PAWN, BoardConstants::BLACK, starting_black_pawn_position);
}

// Getters:

bitboard Board::get_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color) {
  return color == BoardConstants::WHITE ? white_bitboards[piece] : black_bitboards[piece];
}

bitboard Board::get_all_piece_positions(BoardConstants::COLOR color) {
  return get_piece_positions(BoardConstants::PAWN, color) |
    get_piece_positions(BoardConstants::KNIGHT, color) |
    get_piece_positions(BoardConstants::BISHOP, color) |
    get_piece_positions(BoardConstants::ROOK, color) |
    get_piece_positions(BoardConstants::QUEEN, color) |
    get_piece_positions(BoardConstants::KING, color);  
}

Move Board::get_last_move(BoardConstants::COLOR color) {
  if(color == BoardConstants::WHITE) {
    if(white_moves.empty()) {
      throw std::logic_error("Can't get last move because move vector is empty.");
    }
    return white_moves.back();
  } else {
    if(black_moves.empty()) {
      throw std::logic_error("Can't get last move because move vector is empty.");
    }
    return black_moves.back();
  }
}

/**
 * Goes through the piece bitboards for the given color and looks for a set bit at the given position.
 */
BoardConstants::PIECE Board::get_piece_at_position(bitboard position, BoardConstants::COLOR color) {
  for(int i = 0; i < 6; i++) {
    if(color == BoardConstants::WHITE) {
      if(white_bitboards[i] & position) {
	return get_piece_from_index(i);
      }
    } else {
      if(black_bitboards[i] & position) {
	return get_piece_from_index(i);
      }
    }
  }
  return BoardConstants::NONE;
}

// Setters:

void Board::set_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color, bitboard new_positions) {
  if(color == BoardConstants::WHITE) {
    white_bitboards[piece] = new_positions;
  } else {
    black_bitboards[piece] = new_positions;
  }
}

void Board::set_can_castle_queen_side(BoardConstants::COLOR color, bool new_can_castle) {
  can_castle[color][0] = new_can_castle;
}

void Board::set_can_castle_king_side(BoardConstants::COLOR color, bool new_can_castle) {
  can_castle[color][1] = new_can_castle;
}

/**
 * Calls get_attacks_to_position on the color's king position. If the resulting bitboard is non-zero, then the king is in check.
 */
bool Board::is_checked(BoardConstants::COLOR color) {
  return get_attacks_to_position(get_piece_positions(BoardConstants::KING, color), color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE);
}

/**
 * Executes the move, checks if the moving color's king is in check, then undoes the move.
 * If moving color's king is in check, return false.
 */
bool Board::is_move_legal(Move &move, BoardConstants::COLOR color) {
  execute_move(move, color);
  bool king_in_check = is_checked(color);
  undo_move(move, color);

  return !king_in_check;
}

/**
 * Calls execute_castle_move if move is castle, calls execute_non_castle_move if move is not a castle.
 */
void Board::execute_move(Move &move, BoardConstants::COLOR color) {
  bool move_is_castle = move.is_castle();
  if(move_is_castle) {
    execute_castle_move(move, color);
  } else {
    undo_castle_move(move, color);
  }

  update_castle_rights(move, color);

  if(color == BoardConstants::WHITE) {
    white_moves.push_back(move);
  } else {
    black_moves.push_back(move);
  }
}

/**
 * Calls undo_castle_move if move is castle, calls undo_non_castle_move if move is not a castle.
 */
void Board::undo_move(Move &move, BoardConstants::COLOR color) {
  bool move_is_castle = move.is_castle();
  if(move_is_castle) {
    undo_castle_move(move, color);
  } else {
    undo_castle_move(move, color);
  }

  reverse_update_castle_rights();

  if(color == BoardConstants::WHITE) {
    white_moves.pop_back();
  } else {
    black_moves.pop_back();
  }
}

/**
 * Clears the start bit of the moving piece. Sets the end bit of the moving piece. If capture, clears the bit for the captured piece.
 */
void Board::execute_non_castle_move(Move &move, BoardConstants::COLOR color) {
  BoardConstants::PIECE move_piece = move.get_move_piece();
  BoardConstants::PIECE capture_piece = move.get_capture_piece();
  bitboard old_position = move.get_from_position();
  bitboard new_position = move.get_to_position();

  bitboard* move_piece_bitboard = color == BoardConstants::WHITE ? &white_bitboards[move_piece] : &black_bitboards[move_piece];
  *move_piece_bitboard ^= (old_position | new_position);

  if(capture_piece != BoardConstants::NONE) {
    bitboard *capture_piece_bitboard = color == BoardConstants::WHITE ? &black_bitboards[capture_piece] : &white_bitboards[capture_piece];
    *capture_piece_bitboard &= ~new_position;
  }
}

/**
 * Performs the opposite of execute_non_castle_move.
 * Sets the start bit of the moving piece. Clears the end bit of the moving piece. If capture, set the bit for the captured piece.
 */
void Board::undo_non_castle_move(Move &move, BoardConstants::COLOR color) {
  BoardConstants::PIECE move_piece = move.get_move_piece();
  BoardConstants::PIECE capture_piece = move.get_capture_piece();
  bitboard old_position = move.get_from_position();
  bitboard new_position = move.get_to_position();

  bitboard* move_piece_bitboard = color == BoardConstants::WHITE ? &white_bitboards[move_piece] : &black_bitboards[move_piece];
  *move_piece_bitboard ^= (old_position | new_position);

  if(capture_piece != BoardConstants::NONE) {
    bitboard *capture_piece_bitboard = color == BoardConstants::WHITE ? &black_bitboards[capture_piece] : &white_bitboards[capture_piece];
    *capture_piece_bitboard |= new_position;
  }
}

/**
 ** Clears starting bits for king and rook. Sets new bits for king and rook.
 */
void Board::execute_castle_move(Move &move, BoardConstants::COLOR color) {
  bitboard old_king_position = move.get_from_position();
  bitboard new_king_position = move.get_to_position();

  bitboard* king_bitboard = (color == BoardConstants::WHITE ? &white_bitboards[BoardConstants::KING] : &black_bitboards[BoardConstants::KING]);
  bitboard* rook_bitboard = (color == BoardConstants::WHITE ? &white_bitboards[BoardConstants::ROOK] : &black_bitboards[BoardConstants::ROOK]);

  bitboard new_rook_position, old_rook_position;
  if(color == BoardConstants::WHITE) {
    if(new_king_position == white_queenside_castle_king_position) {
      new_rook_position = white_queenside_castle_rook_position;
      old_rook_position = 0x80;
    } else {
      new_rook_position = white_kingside_castle_rook_position;
      old_rook_position = 0x1;
    }
  } else {
    if(new_king_position == black_queenside_castle_king_position) {
      new_rook_position = black_queenside_castle_rook_position;
      old_rook_position = 0x8000000000000000;
    } else {
      new_rook_position = black_kingside_castle_rook_position;
      old_rook_position = 0x100000000000000;
    }
  }

  *king_bitboard ^= (old_king_position | new_king_position);
  *rook_bitboard ^= (old_rook_position | new_rook_position);
}

/**
 ** Does opposite of execute_castle_move.
 */
void Board::undo_castle_move(Move &move, BoardConstants::COLOR color) {
  bitboard old_king_position = move.get_from_position();
  bitboard new_king_position = move.get_to_position();

  bitboard* king_bitboard = (color == BoardConstants::WHITE ? &white_bitboards[BoardConstants::KING] : &black_bitboards[BoardConstants::KING]);
  bitboard* rook_bitboard = (color == BoardConstants::WHITE ? &white_bitboards[BoardConstants::ROOK] : &black_bitboards[BoardConstants::ROOK]);

  bitboard new_rook_position, old_rook_position;
  if(color == BoardConstants::WHITE) {
    if(new_king_position == white_queenside_castle_king_position) {
      new_rook_position = white_queenside_castle_rook_position;
      old_rook_position = 0x80;
    } else {
      new_rook_position = white_kingside_castle_rook_position;
      old_rook_position = 0x1;
    }
  } else {
    if(new_king_position == black_queenside_castle_king_position) {
      new_rook_position = black_queenside_castle_rook_position;
      old_rook_position = 0x8000000000000000;
    } else {
      new_rook_position = black_kingside_castle_rook_position;
      old_rook_position = 0x100000000000000;
    }
  }

  *king_bitboard ^= (old_king_position | new_king_position);
  *rook_bitboard ^= (old_rook_position | new_rook_position);
}

// Castling:

bool Board::can_castle_queen(BoardConstants::COLOR color) {
  return can_castle[color][0];
}

bool Board::can_castle_king(BoardConstants::COLOR color) {
  return can_castle[color][1];
}

/**
 * Checks move to see if king or rook is moved.
 * If king moves, set can_castle_queen and can_castle_king for that color to false.
 * If queen side rook moves, set can_castle_queen for that color to false.
 * If king side rook moves, set can_castle_king for that color to false.
 */
void Board::update_castle_rights(Move &move, BoardConstants::COLOR color) {
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      previous_can_castle[i][j] = can_castle[i][j];
    }
  }
  
  BoardConstants::PIECE move_piece = move.get_move_piece();
  bitboard from_position = move.get_from_position();

  if(move_piece == BoardConstants::KING) {
    can_castle[color][0] = false;
    can_castle[color][1] = false;
  } else if(move_piece == BoardConstants::ROOK) {
    if(from_position & 1) {
      can_castle[color][1] = false;
    } else if(from_position & 0x80) {
      can_castle[color][0] = false;
    }
  }
}
 
/**
 * Copies previous_can_castle into can_castle.
 */
void Board::reverse_update_castle_rights() {
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      can_castle[i][j] = previous_can_castle[i][j];
    }
  }
}

// Attacks:

/**
 * Returns a bitbaord encoding the eligible single push square for a pawn on a given square and of a given color.
 */
bitboard Board::get_pawn_single_push(bitboard position, BoardConstants::COLOR color) {
  return pawn_single_pushes_lookups[color][position];
}

/**
 * Returns a bitbaord encoding the eligible double push square for a pawn on a given square and of a given color.
 */
bitboard Board::get_pawn_double_push(bitboard position, BoardConstants::COLOR color) {
  return pawn_double_pushes_lookups[color][position];
}

/**
 * Gets pawn attacks for the given position and color from the lookup table.
 */
bitboard Board::get_pawn_attacks(bitboard position, BoardConstants::COLOR color) {
  return pawn_attacks_lookups[color][position];
}

/**
 * Gets knight attacks for the given position and color from the lookup table.
 */
bitboard Board::get_knight_attacks(bitboard position) {
  return knight_moves_lookup[position];
}

/**
 * Calls get_sliding_attacks for the given position and color on directions NORTHWEST, NORTHEAST, SOUTHWEST, and SOUTHEAST. Returns the union of these.
 */
bitboard Board::get_bishop_attacks(bitboard position) {
  return get_sliding_attacks(position, BoardConstants::NORTHWEST) |
    get_sliding_attacks(position, BoardConstants::NORTHEAST) |
    get_sliding_attacks(position, BoardConstants::SOUTHWEST) |
    get_sliding_attacks(position, BoardConstants::SOUTHEAST);
}

/**
 * Calls get_sliding_attacks for the given position and color on directions NORTH, SOUTH, EAST, and WEST. Returns the union of these.
 */
bitboard Board::get_rook_attacks(bitboard position) {
  return get_sliding_attacks(position, BoardConstants::NORTH) |
    get_sliding_attacks(position, BoardConstants::SOUTH) |
    get_sliding_attacks(position, BoardConstants::EAST) |
    get_sliding_attacks(position, BoardConstants::WEST);
}

/**
 * Queen moves like a bishop/rook hybrid so returns the union of get_bishop_attacks and get_rook_attacks on the same parameters.
 */
bitboard Board::get_queen_attacks(bitboard position) {
  return get_bishop_attacks(position) | get_rook_attacks(position);
}

/**
 * Gets king attacks for the given position and color from the lookup table.
 */
bitboard Board::get_king_attacks(bitboard position) {
  return king_moves_lookup[position];
}

/**
 * Switch statements calls the appropriate method given the piece type.
 */
bitboard Board::get_piece_attacks(bitboard position, BoardConstants::PIECE piece_type, BoardConstants::COLOR color) {
    switch(piece_type) {
      case BoardConstants::PAWN: return get_pawn_attacks(position, color);
      case BoardConstants::KNIGHT: return get_knight_attacks(position);
      case BoardConstants::BISHOP: return get_bishop_attacks(position);
      case BoardConstants::ROOK: return get_rook_attacks(position);
      case BoardConstants::QUEEN: return get_queen_attacks(position);
      case BoardConstants::KING: return get_king_attacks(position);
    default: return 0;
    }
  }

/**
 * Beginning from the start square, repeatedly move one square in the given direction until another piece is encountered. If that piece is the same color as the moving piece, exclude that bit. If it is the opposite color, include that bit.
 */
bitboard Board::get_sliding_attacks(bitboard position, BoardConstants::DIRECTION direction) {
  bitboard other_pieces = get_all_piece_positions(BoardConstants::WHITE) | get_all_piece_positions(BoardConstants::BLACK);
  
  bitboard result = 0;
  while(position) {
    position = move_direction(position, direction);
    result |= position;
    if(position & other_pieces) break;
  }

  return result;
}

/**
 * Call each of the get_[piece]_attacks functions for each piece and take the intersect of the result with the positions of each of the pieces of the opposing color. Return the union of these six bitboards.
 */
bitboard Board::get_attacks_to_position(bitboard position, BoardConstants::COLOR color) {
  bitboard attacking_kings = get_king_attacks(position) & get_piece_positions(BoardConstants::KING, color);
  bitboard attacking_queens = get_queen_attacks(position) & get_piece_positions(BoardConstants::QUEEN, color);
  bitboard attacking_rooks = get_rook_attacks(position) & get_piece_positions(BoardConstants::ROOK, color);
  bitboard attacking_bishops = get_bishop_attacks(position) & get_piece_positions(BoardConstants::BISHOP, color);
  bitboard attacking_knights = get_knight_attacks(position) & get_piece_positions(BoardConstants::KNIGHT, color);
  bitboard attacking_pawns = get_pawn_attacks(position, color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE) & get_piece_positions(BoardConstants::PAWN, color);

  return attacking_kings | attacking_queens | attacking_rooks | attacking_bishops | attacking_knights | attacking_pawns;
}

// Helpers
bitboard Board::move_direction(bitboard position, BoardConstants::DIRECTION direction) {
  switch(direction) {
  case BoardConstants::NORTH: return north(position);
  case BoardConstants::SOUTH: return south(position);
  case BoardConstants::EAST: return east(position);
  case BoardConstants::WEST: return west(position);
  case BoardConstants::NORTHEAST: return east(north(position));
  case BoardConstants::NORTHWEST: return west(north(position));
  case BoardConstants::SOUTHEAST: return east(south(position));
  case BoardConstants::SOUTHWEST: return west(south(position));
  }
}

BoardConstants::PIECE Board::get_piece_from_index(int index) {
  switch(index) {
  case 0: return BoardConstants::PAWN;
  case 1: return BoardConstants::KNIGHT;
  case 2: return BoardConstants::BISHOP;
  case 3: return BoardConstants::ROOK;
  case 4: return BoardConstants::QUEEN;
  case 5: return BoardConstants::KING;
  default: return BoardConstants::NONE;
  }
}

// Initialize Lookup Tables:
void Board::initialize_lookups() {
  initialize_single_pawn_pushes_lookups();
  initialize_double_pawn_pushes_lookups();
  initialize_pawn_attacks_lookups();
  initialize_knight_moves_lookup();
  initialize_king_moves_lookup();
}

void Board::initialize_single_pawn_pushes_lookups() {
  for(bitboard position = 1; position > 0; position <<= 1) {
    pawn_single_pushes_lookups[BoardConstants::WHITE][position] = north(position);
    pawn_single_pushes_lookups[BoardConstants::BLACK][position] = south(position);
  }
}

void Board::initialize_double_pawn_pushes_lookups() {
  for(bitboard position = 0x100; position <= 0x8000; position <<= 1) {
    pawn_double_pushes_lookups[BoardConstants::WHITE][position] = north(north(position));
  }
  for(bitboard position = 0x1000000000000; position <= 0x80000000000000; position <<= 1) {
    pawn_double_pushes_lookups[BoardConstants::BLACK][position] = south(south(position));
  }
}

void Board::initialize_pawn_attacks_lookups() {
  for(bitboard position = 1; position > 0; position <<= 1) {
    pawn_attacks_lookups[BoardConstants::WHITE][position] = west(north(position)) | east(north(position));
    pawn_attacks_lookups[BoardConstants::BLACK][position] = west(south(position)) | east(south(position));
  }
}

void Board::initialize_knight_moves_lookup() {
  for(bitboard position = 1; position > 0; position <<= 1) {
    knight_moves_lookup[position] = east(north(north(position))) | west(north(north(position))) | east(south(south(position))) | west(south(south(position))) | north(east(east(position))) | north(west(west(position))) | south(east(east(position))) | south(west(west(position)));
  }
}

void Board::initialize_king_moves_lookup() {
  for(bitboard position = 1; position > 0; position <<= 1) {
    king_moves_lookup[position] = north(position) | east(position) | south(position) | west(position) | east(north(position)) | west(north(position)) | east(south(position)) | west(south(position));
  }
}

#endif // END GUARD


