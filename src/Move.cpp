#ifndef MOVE_CPP // GUARD
#define MOVE_CPP // GUARD

#include"Move.hpp"

Move::Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, bool is_castle) {
  from_position = from;
  to_position = to;
  move_piece = move_p;
  capture_p = capture_piece;
  castle = is_castle;
}

bitboard Move::get_from_position() {
  return from_position;
}

bitboard Move::get_to_position() {
  return to_position;
}

#endif // END GUARD
