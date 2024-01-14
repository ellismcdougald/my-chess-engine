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
  std::vector<Move> generate_pawn_pseudo_legal_moves(Board &board, BoardConstants::COLOR color);

  // Special moves:
  /**
   * Generates a list of legal castle moves.
   */
  std::vector<Move> generate_legal_castle_moves(Board &board, BoardConstants::COLOR color);

  /**
   * Generates a list of pseudo-legal en passant pawn moves.
   */
  std::vector<Move> generate_pseudo_legal_en_passant_moves(Board &board, BoardConstants::COLOR color);

  // Printers:
  void print_bitboard(bitboard bb);

private:

  
};
  

#endif //END GUARD
  
