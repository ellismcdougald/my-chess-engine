/*
 * Board class.
 * Stores all information regarding the current game state.
 */

#ifndef BOARD_HPP // GUARD
#define BOARD_HPP // GUARD

#include "BoardConstants.hpp"
#include "Move.hpp"

#include <stdint.h>
#include <array>
#include <vector>
#include <map>

typedef uint64_t bitboard;

class Board {
  friend class BoardTests;
  
public:
  // Constructors:
  
  Board();

  // Initializers:
  /*
   * Initializes the bitboards to the starting positions.
   */
  void initialize_board_starting_position();
  
  // Getters:

  /**
   * Returns a bitboard encoding the positions of a given piece for a given color.
   */
  bitboard get_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color);

  /**
   * Returns a bitboard encoding the positions of all pieces for a given color.
   */
  bitboard get_all_piece_positions(BoardConstants::COLOR color);

  /**
   * Returns the last move made by the given color.
   */
  Move get_last_move(BoardConstants::COLOR color);

  /**
   * Returns the piece at a given position for a given color.
   */
  BoardConstants::PIECE get_piece_at_position(bitboard position, BoardConstants::COLOR color);

  // Setters:
  /**
   * Sets the bitboard encoding the positions of a given piece for a given color.
   */
  void set_piece_positions(BoardConstants::PIECE piece, BoardConstants::COLOR color, bitboard new_positions);

  /**
   * Sets whether the given color can castle queen side.
   */
  void set_can_castle_queen_side(BoardConstants::COLOR color, bool new_can_castle);

  /**
   * Sets whether the given color can castle king side.
   */
  void set_can_castle_king_side(BoardConstants::COLOR color, bool new_can_castle);

  // Moves:
  
  /**
   * Returns true if the given color's king is in check, false otherwise.
   */
  bool is_checked(BoardConstants::COLOR color);

  /**
   * Returns true if the given move is legal (i.e. does not place the moving player's king in check), false otherwise.
   */
  bool is_move_legal(Move &move, BoardConstants::COLOR color);

  /**
   * Executes a given move on the board and pushes the move to the moving color's move vector.
   */
  void execute_move(Move &move, BoardConstants::COLOR color);

  /**
   * Undoes a given move on the board and removes the move from the moving color's move vector.
   */
  void undo_move(Move &move, BoardConstants::COLOR color);

  /**
   * Executes a non-castle move on the board and pushes the move to the moving color's move vector.
   */
  void execute_non_castle_move(Move &move, BoardConstants::COLOR color);

  /**
   * Undoes a non-castle move on the board and removes the move from the moving color's move vector.
   */
  void undo_non_castle_move(Move &move, BoardConstants::COLOR color);

  /**
   * Executes a castle move on the board and pushes the move to the moving color's move vector.
   */
  void execute_castle_move(Move &move, BoardConstants::COLOR color);

  /**
   * Undoes a castle move on the board and removes the move from the moving color's move vector.
   */
  void undo_castle_move(Move &move, BoardConstants::COLOR color);

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
  void update_castle_rights(Move &move, BoardConstants::COLOR color);

  /**
   * Reverses an update to castle rights if neccessary (if the king or one of the rooks was moved from the starting squares).
   * Called by undo_move.
   */
  void reverse_update_castle_rights();

  // Attacks:

  /**
   * Returns a bitbaord encoding the eligible single push square for a pawn on a given square and of a given color.
   */
  bitboard get_pawn_single_push(bitboard position, BoardConstants::COLOR color);

  /**
   * Returns a bitbaord encoding the eligible double push square for a pawn on a given square and of a given color.
   */
  bitboard get_pawn_double_push(bitboard position, BoardConstants::COLOR color);

  /**
   * Returns a bitboard encoding the squares attacked by a pawn on a given square and of a given color.
   */
  bitboard get_pawn_attacks(bitboard position, BoardConstants::COLOR color);

  /**
   * Returns a bitboard encoding the squares attacked by a knight on a given square.
   */
  bitboard get_knight_attacks(bitboard position);

  /**
   * Returns a bitboard encoding the squares attacked by a bishop on a given square.
   */
  bitboard get_bishop_attacks(bitboard position);

  /**
   * Returns a bitboard encoding the squares attacked by a rook on a given square.
   */
  bitboard get_rook_attacks(bitboard position);

  /**
   * Returns a bitboard encoding the squares attacked by a queen on a given square.
   */
  bitboard get_queen_attacks(bitboard position);

  /**
   * Returns a bitboard encoding the positions attacked by a king on a given square.
   */
  bitboard get_king_attacks(bitboard positionr);

  /**
   * Returns a bitboard encoding the positions attacked by a piece of the given type and color  on a given square.
   */
  bitboard get_piece_attacks(bitboard position, BoardConstants::PIECE piece_type, BoardConstants::COLOR color);

  /**
   * Returns a bitboard encoding the positions attacked by a sliding move from a given square and color in a given direction.
   */
  bitboard get_sliding_attacks(bitboard position, BoardConstants::DIRECTION direction);

  /**
   * Returns a bitboard encoding the positions of the pieces of a given color that are attacking a given position.
   */
  bitboard get_attacks_to_position(bitboard position, BoardConstants::COLOR color);

private:
  std::array<bitboard, 6> white_bitboards;
  std::array<bitboard, 6> black_bitboards;
  std::vector<Move> white_moves;
  std::vector<Move> black_moves;

  static const bitboard FILE_A; //= 0x8080808080808080;
  static const bitboard FILE_H; //= 0x0808080808080808;
  static const bitboard RANK_1; //= 0xFF;
  static const bitboard RANK_8; // 0xFF00000000000000;

  static const bitboard starting_white_king_position;
  static const bitboard starting_white_queen_position;
  static const bitboard starting_white_rook_position;
  static const bitboard starting_white_bishop_position;
  static const bitboard starting_white_knight_position;
  static const bitboard starting_white_pawn_position;
  
  static const bitboard starting_black_king_position;
  static const bitboard starting_black_queen_position;
  static const bitboard starting_black_rook_position;
  static const bitboard starting_black_bishop_position;
  static const bitboard starting_black_knight_position;
  static const bitboard starting_black_pawn_position;

  static const bitboard white_queenside_castle_king_position;
  static const bitboard white_queenside_castle_rook_position;
  static const bitboard white_kingside_castle_king_position;
  static const bitboard white_kingside_castle_rook_position;
  static const bitboard black_queenside_castle_king_position;
  static const bitboard black_queenside_castle_rook_position;
  static const bitboard black_kingside_castle_king_position;
  static const bitboard black_kingside_castle_rook_position;

  inline bitboard north(bitboard position) { return (position & ~RANK_8) << 8; }
  inline bitboard south(bitboard position) { return (position & ~RANK_1) >> 8; }
  inline bitboard east(bitboard position) { return (position & ~FILE_H) >> 1; }
  inline bitboard west(bitboard position) { return (position & ~FILE_A) << 1; }

  /*
   * can_castle[0][0]: white queen side
   * can_castle[0][1]: white king side
   * can_castle[1][0]: black queen side
   * can_castle[1][1]: black king side
   */
  bool can_castle[2][2];

  /*
   * Need this in order to undo updates to castle rights for undo_move.
   * Same format as can_castle.
   */
  bool previous_can_castle[2][2];

  // Lookup Tables:
  std::map<bitboard, bitboard> pawn_single_pushes_lookups[2];
  std::map<bitboard, bitboard> pawn_double_pushes_lookups[2];
  std::map<bitboard, bitboard> pawn_attacks_lookups[2];
  std::map<bitboard, bitboard> knight_moves_lookup;
  std::map<bitboard, bitboard> king_moves_lookup;

  // Helpers:
  bitboard move_direction(bitboard position, BoardConstants::DIRECTION direction);
  BoardConstants::PIECE get_piece_from_index(int index);

  // Initialize Lookup Tables:
  /**
   * Initializes all lookup tables.
   */
  void initialize_lookups();
  
  /**
   * Initializes single pawn push lookups for white and black.
   */
  void initialize_single_pawn_pushes_lookups();

  /**
   * Initializes double pawn push lookups for white and black.
   */
  void initialize_double_pawn_pushes_lookups();

  /**
   * Initializes pawn attack lookups for white and black.
   */
  void initialize_pawn_attacks_lookups();

  /**
   * Initializes knight move lookup.
   * There is a common lookup table for both white and black.
   */
  void initialize_knight_moves_lookup();

  /**
   * Initializes king move lookup.
   * There is a common lookup table for both white and black.
   */
  void initialize_king_moves_lookup();

};

#endif // END GUARD
