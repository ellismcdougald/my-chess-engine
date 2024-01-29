#ifndef MOVE_CPP // GUARD
#define MOVE_CPP // GUARD

#include<iostream>
#include<iomanip>
#include"Move.hpp"

void print_bitboard(bitboard bb) {
  bitboard mask = (bitboard) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}


Move::Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, bool is_castle) {
  from_position = from;
  to_position = to;
  move_piece = move_p;
  capture_piece = capture_p;
  promotion_piece = BoardConstants::NONE;
  castle = is_castle;
}

Move::Move(bitboard from, bitboard to, BoardConstants::PIECE move_p, BoardConstants::PIECE capture_p, BoardConstants::PIECE promotion_p) {
  from_position = from;
  to_position = to;
  move_piece = move_p;
  capture_piece = capture_p;
  promotion_piece = promotion_p;
  castle = false;
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

bool Move::is_double_pawn_push(BoardConstants::COLOR color) {
   if(move_piece == BoardConstants::PAWN) {
    if(color == BoardConstants::WHITE) {
      return to_position == (from_position << 16);
    } else {
      return to_position == (from_position >> 16);
    }
  }
  return false;
}
  
void Move::print_move_hex() {
  std::cout << "0x" << std::hex << from_position << " -> " << "0x" << std::hex << to_position << ": " << move_piece << "/" << capture_piece << " -- " << castle << "\n" << std::dec;
}

#endif // END GUARD
