#ifndef MOVE_CPP // GUARD
#define MOVE_CPP // GUARD

#include"Move.hpp"

Move::Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, bool is_castle) {
  from_position = from;
  to_position = to;
  move_piece = move_p;
  capture_piece = capture_p;
  castle = is_castle;
}

bitboard Move::get_from_position() {
  return from_position;
}

bitboard Move::get_to_position() {
  return to_position;
}

BoardConstants::PIECE Move::get_move_piece() {
  return move_piece;
}

BoardConstants::PIECE Move::get_capture_piece() {
  return capture_piece;
}

bool Move::is_castle() {
  return castle;
}

bool Move::move_equals(Move &other_move) {
  return from_position == other_move.get_from_position() &&
    to_position == other_move.get_to_position() &&
    move_piece == other_move.get_move_piece() &&
    capture_piece == other_move.get_capture_piece() &&
    castle == other_move.is_castle();
}

#endif // END GUARD
