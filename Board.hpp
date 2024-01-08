/*
 * Board class.
 * Stores all information regarding the current game state.
 */

#ifndef BOARD_HPP // GUARD
#define BOARD_HPP // GUARD

#include <stdint.h>
#include <array>
#include <vector>

typedef uint64_t bitboard;

class Board {
public:
  // Constructors:
  
  Board();
  
  // Getters:

  /**
   * Returns a bitboard encoding the positions of a given piece for a given color.
   */
  bitboard get_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color);

  // Setters:
  /**
   * Sets the bitboard encoding the positions of a given piece for a given color.
   */
  bitboard set_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color, bitboard new_positions);

  // Moves:
  
  /**
   * Returns true if the given color's king is in check, false otherwise.
   */
  bool is_checked(BoardConstants::COLOR color);

  /**
   * Returns true if the given move is legal (i.e. does not place the moving player's king in check), false otherwise.
   */
  bool is_move_legal(Move &move);

  /**
   * Executes a given move on the board and pushes the move to the moving color's move vector.
   */
  void execute_move(Move &move);

  /**
   * Undoes a given move on the board and removes the move from the moving color's move vector.
   */
  void undo_move(Move &move);

  /**
   * Returns the last move made by the given color.
   */
  Move get_last_move(BoardConstants::COLOR color);

  // Castling:
  /**
   * Returns true if the given color can castle to the queen side, false otherwise.
   */
  bool can_castle_queen(BoardConstants::COLOR color);

  /**
   * Returns true if the given color can castle to the king side, false otherwise.
   */
  bool can_castle_king(BoardConstants::COLOR color);

  /**
   * Updates castle rights if neccessary (if the king or one of the rooks is moved from the starting squares).
   * Called by execute_move.
   */
  void update_castle_rights(Move &move);

  /**
   * Reverses an update to castle rights if neccessary (if the king or one of the rooks was moved from the starting squares).
   * Called by undo_move.
   */
  void reverse_update_castle_rights(Move &move);

  // Attacks:
  

private:
  std::array<bitboard, 6> white_bitboards;
  std::array<bitboard, 6> black_bitboards;
  std::vector<Move> white_moves;
  std::vector<Move> black_moves;

  /*
   * Color can castle if they have not yet castled and have not yet moved their king or the rook on that side.
   */
  bool white_can_castle_queen;
  bool white_can_castle_king;
  bool black_can_castle_queen;
  bool black_can_castle_king;
};

#endif // END GUARD
