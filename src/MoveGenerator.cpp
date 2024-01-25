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
 * NEEDS TEST
 
 * Generates pseudo-legal moves, then removes those that leave the king in check.
 * Appends castle moves (these are legally generated, not pseudo-legal like the others).
 */
std::vector<Move> MoveGenerator::generate_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> legal_moves;

  std::vector<Move> pseudo_legal_moves = generate_pseudo_legal_moves(board, color);
  for(int i = 0; i < pseudo_legal_moves.size(); i++) {
    if(board.is_move_legal(pseudo_legal_moves[i], color)) {
      legal_moves.push_back(pseudo_legal_moves[i]);
    }
  }

  append_legal_castle_moves(legal_moves, board, color);

  return legal_moves;
}

/**
 * NEEDS TEST
   
 * Calls pseudo-legal move generation functions for each piece tyoe.
 */

std::vector<Move> MoveGenerator::generate_pseudo_legal_moves(Board &board, BoardConstants::COLOR color) {
  std::vector<Move> pseudo_legal_moves;

  append_pawn_pseudo_legal_moves(pseudo_legal_moves, board, color);
  append_pseudo_legal_en_passant_moves(pseudo_legal_moves, board, color);
  append_piece_pseudo_legal_moves(pseudo_legal_moves, board, BoardConstants::KNIGHT, color);
  append_piece_pseudo_legal_moves(pseudo_legal_moves, board, BoardConstants::BISHOP, color);
  append_piece_pseudo_legal_moves(pseudo_legal_moves, board, BoardConstants::ROOK, color);
  append_piece_pseudo_legal_moves(pseudo_legal_moves, board, BoardConstants::QUEEN, color);
  append_piece_pseudo_legal_moves(pseudo_legal_moves, board, BoardConstants::KING, color);
 
  return pseudo_legal_moves;
}

// Piece move generation:

/**
 * Receives a bitboard encoding the positions of the pawns. For each pawn, get eligible single push positions, double push positions, and attack positions (using Board methods and other piece locations). Then calls generate_pseudo_legal_en_passant_moves and appends these moves.
 */
void  MoveGenerator::append_pawn_pseudo_legal_moves(std::vector<Move> &pawn_pseudo_legal_moves, Board &board, BoardConstants::COLOR color) {
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
}

/**
 * Receives a bitboard encoding the position of the piece to move. Gets possible piece destination squares and generates a move for each destination square. Appends to the given vector.
 */
void MoveGenerator::append_piece_pseudo_legal_moves(std::vector<Move> &piece_pseudo_legal_moves, Board &board, BoardConstants::PIECE piece, BoardConstants::COLOR color) {
  bitboard piece_positions = board.get_piece_positions(piece, color);
  bitboard other_piece_positions = board.get_all_piece_positions(color);
  bitboard opponent_piece_positions = board.get_all_piece_positions(color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE);

  bitboard temp_position, all_destinations, capture_destinations, non_capture_destinations;
  for(bitboard mask = 1; mask > 0; mask <<= 1) {
    temp_position = piece_positions & mask;
    all_destinations = board.get_piece_destinations(temp_position, piece, color);
    capture_destinations = all_destinations & ~other_piece_positions & opponent_piece_positions;
    non_capture_destinations = all_destinations & ~other_piece_positions & ~opponent_piece_positions;
    append_non_castle_moves_from_destinations(capture_destinations, temp_position, piece, true, piece_pseudo_legal_moves, board, color);
    append_non_castle_moves_from_destinations(non_capture_destinations, temp_position, piece, false, piece_pseudo_legal_moves, board, color);
  }
}

// Special Moves:

/**
 * NEEDS REFACTOR: split into functions

 * Calls Board::can_castle_queen and Board::can_castle_king to determine if the color has castle rights to either side. If yes, verifies that the castle lane is unobstructed and is not attacked.
 */
void MoveGenerator::append_legal_castle_moves(std::vector<Move> &legal_castle_moves, Board &board, BoardConstants::COLOR color) {
  BoardConstants::COLOR other_color = color == BoardConstants::WHITE ? BoardConstants::BLACK : BoardConstants::WHITE;
  bitboard king_position = board.get_piece_positions(BoardConstants::KING, color);
  bitboard castle_path_position;

  // King side
  bool can_castle_king = board.can_castle_king(color);
  if(can_castle_king) {
    castle_path_position = board.get_piece_positions(BoardConstants::KING, color);
    for(int i = 0; i < 2; i++) {
      castle_path_position >>= 1;
      if(board.get_attacks_to_position(castle_path_position, other_color)) {
	can_castle_king = false;
	break;
      }
    }
  }
  bool can_castle_queen = board.can_castle_queen(color);
  if(can_castle_queen) {
    castle_path_position = board.get_piece_positions(BoardConstants::KING, color);
    for(int i = 0; i < 3; i++) {
      castle_path_position <<= 1;
      if(board.get_attacks_to_position(castle_path_position, other_color)) {
	can_castle_queen = false;
	break;
      }
    }
  }

  bitboard king_end_position;
  if(can_castle_king) {
    king_end_position = color == BoardConstants::WHITE ? 0x2 : 0x200000000000000;
    Move king_castle(king_position, king_end_position, BoardConstants::KING, BoardConstants::NONE, true);
    legal_castle_moves.push_back(king_castle);
  }
  if(can_castle_queen) {
    king_end_position = color == BoardConstants::WHITE ? 0x20 : 0x2000000000000000;
    Move queen_castle(king_position, king_end_position, BoardConstants::KING, BoardConstants::NONE, true);
    legal_castle_moves.push_back(queen_castle);
  }
}

/**
 * Calls Board::get_last_move for the opposing color and checks if the last move was a double pawn push. If yes, gets pawns that are in position for an en passant move and generates a move for each of those pawns.
 */
std::vector<Move> MoveGenerator::append_pseudo_legal_en_passant_moves(std::vector<Move> &pseudo_legal_en_passant_moves, Board &board, BoardConstants::COLOR color) {
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
