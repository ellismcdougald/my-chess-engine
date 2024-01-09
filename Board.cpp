#ifndef BOARD_CPP // GUARD
#define BOARD_CPP // GUARD

#include"Board.hpp"

// Constructors:
Board::Board() {}

// Initializers:
void Board::initialize_board() {}

// Getters:

bitboard Board::get_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color) {
  return color == BoardConstants::WHITE ? white_bitboards[piece] : black_bitboards[piece];
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

bitboard Board::set_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color, bitboard new_positions) {
  if(color == BoardConstants::WHITE) {
    white_bitboards[piece] = new_positions;
  } else {
    black_bitboards[piece] = new_positions;
  }
}

/**
 * TODO
 
 * Calls get_attacks_to_position on the color's king position. If the resulting bitboard is non-zero, then the king is in check.
 */
bool Board::is_checked(BoardConstants::COLOR color) {}

/**
 * TODO
 
 * Executes the move, checks if the moving color's king is in check, then undoes the move.
 * If moving color's king is in check, return false.
 */
bool Board::is_move_legal(Move &move) {}

/**
 * TODO
 
 * Clears the start bit of the moving piece. Sets the end bit of the moving piece. If capture, clears the bit for the captured piece.
 */
void Board::execute_move(Move &move) {}

/**
 * TODO
 
 * Performs the opposite of execute_move.
 * Sets the start bit of the moving piece. Clears the end bit of the moving piece. If capture, set the bit for the captured piece.
 */
void Board::undo_move(Move &move) {}

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
bitboard Board::get_pawn_attacks(bitboard position, BoardConstants::COLOR color) {}

/**
 * Gets knight attacks for the given position and color from the lookup table.
 */
bitboard Board::get_knight_attacks(bitboard position, BoardConstants::COLOR color) {}

/**
 * Calls get_sliding_attacks for the given position and color on directions NORTHWEST, NORTHEAST, SOUTHWEST, and SOUTHEAST. Returns the union of these.
 */
bitboard Board::get_bishop_attacks(bitboard position, BoardConstants::COLOR color) {}

/**
 * Calls get_sliding_attacks for the given position and color on directions NORTH, SOUTH, EAST, and WEST. Returns the union of these.
 */
bitboard Board::get_rook_attacks(bitboard position, BoardConstants::COLOR color) {}

/**
 * Queen moves like a bishop/rook hybrid so returns the union of get_bishop_attacks and get_rook_attacks on the same parameters.
 */
bitboard Board::get_queen_attacks(bitboard position, BoardConstants::COLOR color) {}

/**
 * Gets king attacks for the given position and color from the lookup table.
 */
bitboard Board::get_king_attacks(bitboard position, BoardConstants::COLOR color) {}

/**
 * Beginning from the start square, repeatedly move one square in the given direction until another piece is encountered. If that piece is the same color as the moving piece, exclude that bit. If it is the opposite color, include that bit.
 */
bitboard Board::get_sliding_attacks(bitboard position, BoardConstants::COLOR color, BoardConstants::DIRECTION direction) {}

/**
 * Call each of the get_[piece]_attacks functions for each piece and take the intersect of the result with the positions of each of the pieces of the opposing color. Return the union of these six bitboards.
 */
bitboard Board::get_attacks_to_position(bitboard position, BoardConstants::COLOR color) {}


#endif // END GUARD


