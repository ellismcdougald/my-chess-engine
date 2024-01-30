/*
 * MoveGenerator class.
 * Collection of functions that generate a list of legal moves for a given board state.
 */

#ifndef MOVEGENERATOR_HPP // GUARD
#define MOVEGENERATOR_HPP // GUARD

#include"BoardConstants.hpp"
#include"Board.hpp"
#include"Move.hpp"

#include <stdint.h>
#include <vector>

typedef uint64_t bitboard;

class MoveGenerator {
public:
  // Constructors:

  MoveGenerator();

  // Move Generation:

  /**
   * Generates a list of all legal moves for a given board position and player color.
   */
  std::vector<Move> generate_legal_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of all pseudo-legal moves for a given board position and player color.
   * Pseudo-legal moves are moves that may put the king in check, but otherwise obey the rules of chess.
   */
  std::vector<Move> generate_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  // Piece move generation:
  // inputs can be refined
  
  /**
   * Generates a list of all pseudo-legal moves for the king of the given color.
   */
  std::vector<Move> generate_king_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of all pseudo-legal moves for the queen of the given color.
   */
  std::vector<Move> generate_queen_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of all pseudo-legal moves for the rook of the given color.
   */
  std::vector<Move> generate_rook_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of all pseudo-legal moves for the bishop of the given color.
   */
  std::vector<Move> generate_bishop_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of all pseudo-legal moves for the knight of the given color.
   */
  std::vector<Move> generate_knight_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of all pseudo-legal moves for the pawn of the given color.
   * Handles single pushes, double pushes, attacks, and en passant.
   */
  void append_pawn_pseudo_legal_moves(std::vector<Move> &pawn_pseudo_legal_moves, Board &board, BoardConstants::COLOR color);

  /**
   *
   * Generates a list of all pseudo-legal moves for the given piece and color.
   */
  void append_piece_pseudo_legal_moves(std::vector<Move> &piece_pseudo_legal_moves, Board &board, BoardConstants::PIECE piece, BoardConstants::COLOR color);
  

  // Special moves:
  /**
   * Generates a list of legal castle moves.
   */
 void append_legal_castle_moves(std::vector<Move> &legal_castle_moves, Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of pseudo-legal en passant pawn moves.
   */
  std::vector<Move> append_pseudo_legal_en_passant_moves(std::vector<Move> &pseudo_legal_en_passant_moves, Board &board, BoardConstants::COLOR color);

  // Printers:
  void print_bitboard(bitboard bb);

  // Debugging:
  /**
   * Perft function.
   * Walks through legal move tree and counts all moves to a certain depth.
   * See more: https://www.chessprogramming.org/Perft
   */
  uint64_t perft(int depth, Board &board, BoardConstants::COLOR color);
  uint64_t divide(int depth, Board &board, BoardConstants::COLOR color);

private:
  /**
   * Receives bitboard encoding the destination squares, a start position, the move piece, whether or not the move is a capture, and a move vector. Generates a move for each destination and appends to the given move vector.
   * Castle moves not handled by this function.
   */
  void append_non_castle_moves_from_destinations(bitboard destinations, bitboard start_position, BoardConstants::PIECE move_piece, bool capture, std::vector<Move> &move_vector, Board& board, BoardConstants::COLOR color);

  
};
  

#endif //END GUARD
  
