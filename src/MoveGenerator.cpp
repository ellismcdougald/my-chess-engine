/*
 * MoveGenerator implementation.
 */

#ifndef MOVEGENERATOR_CPP // GUARD
#define MOVEGENERATOR_CPP // GUARD

#include<iostream>
#include "MoveGenerator.hpp"

// Move Generation:
MoveGenerator::MoveGenerator() {}

/**
 * TODO
 
 * Generates pseudo-legal moves, then removes those that leave the king in check.
 * Appends castle moves (these are legally generated, not pseudo-legal like the others).
 */
std::vector<Move> MoveGenerator::generate_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> legal_moves;

  return legal_moves;
}

/**
 * TODO
 
 * Calls pseudo-legal move generation functions for each piece tyoe.
 */
std::vector<Move> MoveGenerator::generate_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> pseudo_legal_moves;

  return pseudo_legal_moves;
}

// Piece move generation:

/**
 * TEST
 
 * Receives a bitboard encoding the positions of the pawns. For each pawn, get eligible single push positions, double push positions, and attack positions (using Board methods and other piece locations). Then calls generate_pseudo_legal_en_passant_moves and appends these moves.
 */
std::vector<Move> MoveGenerator::generate_pawn_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> pawn_pseudo_legal_moves;

  bitboard pawn_positions = board.get_piece_positions(BoardConstants::PAWN, color);
  bitboard opponent_positions = board.get_all_piece_positions(color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE);
  
  bitboard push_destinations, attack_destinations;
  bitboard temp_position;
  for(bitboard mask = 1; mask > 0; mask <<= 1) {
    temp_position = pawn_positions & mask;
    if(temp_position) {
      // pushes
      push_destinations = board.get_pawn_single_push(temp_position, color) | board.get_pawn_double_push(temp_position, color) & ~opponent_positions;
      append_non_castle_moves_from_destinations(push_destinations, temp_position, BoardConstants::PAWN, false, pawn_pseudo_legal_moves, board, color);
      // attacks
      attack_destinations = board.get_pawn_attacks(temp_position, color) & opponent_positions;
      append_non_castle_moves_from_destinations(attack_destinations, temp_position, BoardConstants::PAWN, true, pawn_pseudo_legal_moves, board, color);
    }
  }

  return pawn_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the knights. For each knight, calls Board::get_knight_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> MoveGenerator::generate_knight_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> knight_pseudo_legal_moves;

  return knight_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the knights. For each bishop, calls Board::get_bishop_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> MoveGenerator::generate_bishop_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> bishop_pseudo_legal_moves;

  return bishop_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the rooks. For each rook, calls Board::get_rook_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> MoveGenerator::generate_rook_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> rook_pseudo_legal_moves;

  return rook_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the queens. For each queen, calls Board::get_queen_attacks to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> MoveGenerator::generate_queen_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> queen_pseudo_legal_moves;

  return queen_pseudo_legal_moves;
}

/**
 * TODO
 
 * Receives a bitboard encoding the position of the king. Uses lookup table to generate possible move positions. Positions that land on another piece of the same color are removed.
 */
std::vector<Move> MoveGenerator::generate_king_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> king_pseudo_legal_moves;

  return king_pseudo_legal_moves;
}

// Special Moves:

/**
 * TODO
 * Calls Board::can_castle_queen and Board::can_castle_king to determine if the color has castle rights to either side. If yes, verifies that the castle lane is unobstructed and is not attacked.
 */
std::vector<Move> MoveGenerator::generate_legal_castle_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> legal_castle_moves;

  return legal_castle_moves;
}

/**
 * NEEDS TEST
 
 * Calls Board::get_last_move for the opposing color and checks if the last move was a double pawn push. If yes, gets pawns that are in position for an en passant move and generates a move for each of those pawns.
 */
std::vector<Move> MoveGenerator::generate_pseudo_legal_en_passant_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> pseudo_legal_en_passant_moves;

  BoardConstants::COLOR other_color = color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE;

  try {
    Move last_move = board.get_last_move(other_color);
 
    if(last_move.is_double_pawn_push(other_color)) {
      bitboard vulnerable_pawn = last_move.get_to_position();
      bitboard attack_pawns = board.get_piece_positions(BoardConstants::PAWN, color) & (board.west(vulnerable_pawn) | board.east(vulnerable_pawn));
      bitboard destination_square = (color == BoardConstants::WHITE ? board.north(vulnerable_pawn) : board.south(vulnerable_pawn));

      for(bitboard mask = 1; mask > 0; mask <<= 1) {
	if(attack_pawns & mask) {
	  Move move(attack_pawns & mask, destination_square, BoardConstants::PAWN, BoardConstants::PAWN, false);
	  pseudo_legal_en_passant_moves.push_back(move);
	}
      }
    }
  } catch(std::logic_error &e) {
    return pseudo_legal_en_passant_moves;
  }

  return pseudo_legal_en_passant_moves;
}

// Printers:
void MoveGenerator::print_bitboard(bitboard bb) {
  bitboard mask = (bitboard) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}

// Helpers:
/**
 * For each set bit in destinations, creates the appropriate move and appends to the move vector.
 */
void MoveGenerator::append_non_castle_moves_from_destinations(bitboard destinations, bitboard start_position, BoardConstants::PIECE move_piece, bool capture, std::vector<Move> &move_vector, Board& board, BoardConstants::COLOR color) {
  bitboard temp_position;
  BoardConstants::PIECE capture_piece;
  for(bitboard mask = 1; mask > 0; mask <<=1) {
    temp_position = destinations & mask;
    if(temp_position) {
      capture_piece = capture ? board.get_piece_at_position(temp_position, color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE) : BoardConstants::NONE;
      Move move(start_position, temp_position, move_piece, capture_piece, false);
      move_vector.push_back(move);
    }
  }
}

#endif // END GUARD
