#ifndef BOARD_CPP // GUARD
#define BOARD_CPP // GUARD

#include"Board.hpp"

#include<iostream>
void print_bitboard(bitboard bb) {
  bitboard mask = (bitboard) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}

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

// Constructors:
Board::Board() {
  white_bitboards.fill(0);
  black_bitboards.fill(0);
  initialize_lookups();
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

// Setters:

void Board::set_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color, bitboard new_positions) {
  if(color == BoardConstants::WHITE) {
    white_bitboards[piece] = new_positions;
  } else {
    black_bitboards[piece] = new_positions;
  }
}

/**
 * Calls get_attacks_to_position on the color's king position. If the resulting bitboard is non-zero, then the king is in check.
 */
bool Board::is_checked(BoardConstants::COLOR color) {
  return get_attacks_to_position(get_piece_positions(BoardConstants::KING, color), color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE);
}

/**
 * TODO
 
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
 * Clears the start bit of the moving piece. Sets the end bit of the moving piece. If capture, clears the bit for the captured piece.
 */
void Board::execute_move(Move &move, BoardConstants::COLOR color) {
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
 * Performs the opposite of execute_move.
 * Sets the start bit of the moving piece. Clears the end bit of the moving piece. If capture, set the bit for the captured piece.
 */
void Board::undo_move(Move &move, BoardConstants::COLOR color) {
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

// Castling:

bool Board::can_castle_queen(BoardConstants::COLOR color) {
  return color == BoardConstants::WHITE ? white_can_castle_queen : black_can_castle_queen;
}

bool Board::can_castle_king(BoardConstants::COLOR color) {
  return color == BoardConstants::WHITE ? white_can_castle_king : black_can_castle_king;
}

/**
 * TODO
 
 * Checks move to see if king or rook is moved.
 * If king moves, set can_castle_queen and can_castle_king for that color to false.
 * If queen side rook moves, set can_castle_queen for that color to false.
 * If king side rook moves, set can_castle_king for that color to false.
 */
void Board::update_castle_rights(Move &move) {}
 
/**
 * TODO
 
 * DIFFICULT IMPLEMENTATION
 * King could be moved off start square, returned to start square, then moved off start square again. This function should not set castle rights back to true if the third move is undone.
 * So we would need to track whether castle rights were still valid beforehand somehow.

 * Is this function neccessary? Castle rights could be determined by the move list (but this isn't most efficient). Could additional member variables simplify the process?
 */
void Board::reverse_update_castle_rights(Move &move) {}

// Attacks:

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


