#ifndef MOVE_HPP // GUARD
#define MOVE_HPP // GUARD

#include <stdint.h>

#include "BoardConstants.hpp"

typedef uint64_t bitboard;

class Move {
public:
  // Constructors:
  Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, bool is_castle);
  Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, BoardConstants::PIECE promotion_p);

  // Getters:
  /**
   * Returns the bitboard representing the starting position of the moving piece.
   */
  bitboard get_from_position();

  /**
   * Returns the bitboard representing the end position of the moving piece.
   */
  bitboard get_to_position();

  /**
   * Returns the move piece;
   */
  BoardConstants::PIECE get_move_piece();

  /**
   * Returns the capture piece;
   */
  BoardConstants::PIECE get_capture_piece();

  /**
   * Returns the promotion piece.
   */
  BoardConstants::PIECE get_promotion_piece();

  /**
   * Returns true if move is castle, false otherwise;
   */
  bool is_castle();

  // Equality:

  /**
   * Returns true if two moves are the same, false otherwise.
   */
  bool move_equals(Move &other_move);

  // Move type:
  bool is_double_pawn_push(BoardConstants::COLOR color);

  // Printers:
  void print_move_hex();

private:
  bitboard from_position;
  bitboard to_position;
  BoardConstants::PIECE move_piece;
  BoardConstants::PIECE capture_piece;
  BoardConstants::PIECE promotion_piece;
  bool castle;
};

#endif // END GUARD
