/*
 * MoveGenerator implementation.
 */

#ifndef MOVEGENERATOR_CPP // GUARD
#define MOVEGENERATOR_CPP // GUARD

#include "MoveGenerator.hpp"

// Move Generation:

/**
 * TODO
 
 * Generates pseudo-legal moves, then removes those that leave the king in check.
 * Appends castle moves (these are legally generated, not pseudo-legal like the others).
 */
std::vector<Move> generate_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> legal_moves;

  return legal_moves;
}

/**
 * TODO
 
 * Calls pseudo-legal move generation functions for each piece tyoe.
 */
std::vector<Move> generate_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> pseudo_legal_moves;

  return pseudo_legal_moves;
}

// Piece move generation:

/**
 * TODO
 
 * Receives a bitboard encoding the positions of the pawns. For each pawn, get eligible single push positions, double push positions, and attack positions (using Board methods and other piece locations). Then calls generate_pseudo_legal_en_passant_moves and appends these moves.
 */
std::vector<Move> generate_pawn_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> pawn_pseudo_legal_moves;

  return pawn_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the knights. For each knight, calls Board::get_knight_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> generate_knight_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> knight_pseudo_legal_moves;

  return knight_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the knights. For each bishop, calls Board::get_bishop_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> generate_bishop_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> bishop_pseudo_legal_moves;

  return bishop_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the rooks. For each rook, calls Board::get_rook_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> generate_rook_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> rook_pseudo_legal_moves;

  return rook_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the queens. For each queen, calls Board::get_queen_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> generate_queen_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> queen_pseudo_legal_moves;

  return queen_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the king. Uses lookup table to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> generate_king_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> king_pseudo_legal_moves;

  return king_pseudo_legal_moves;
}

// Special Moves:

/**
 * TODO
 * Calls Board::can_castle_queen and Board::can_castle_king to determine if the color has castle rights to either side. If yes, verifies that the castle lane is unobstructed and is not attacked.
 */
std::vector<Move> generate_legal_castle_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> legal_castle_moves;

  return legal_castle_moves;
}

/**
 * TODO
 * Calls Board::get_last_move for the opposing color and checks if the last move was a double pawn push. If yes, gets pawns that are in position for an en passant move and generates a move for each of those pawns.
 */
std::vector<Move> generate_pseudo_legal_en_passant_moves(Board &board, BoardConstants::COLOR) {
  std::vector<Move> pseudo_legal_en_passant_moves;

  return pseudo_legal_en_passant_moves;
}

#endif // END GUARD
