#ifndef MOVE_HPP // GUARD
#define MOVE_HPP // GUARD

#include <stdint.h>

#include "BoardConstants.hpp"

typedef uint64_t bitboard;

class Move {
public:
  // Constructors:
  Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, bool is_castle);

  // Getters:
  /**
   * Returns the bitboard representing the starting position of the moving piece.
   */
  bitboard get_from_position();

  /**
   * Returns the bitboard representing the end position of the moving piece.
   */
  bitboard get_to_position();

private:
  bitboard from_position;
  bitboard to_position;
  BoardConstants::PIECE move_piece;
  BoardConstants::PIECE capture_piece;
  bool castle;
};

#endif // END GUARD
