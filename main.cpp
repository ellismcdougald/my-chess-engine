#include"iostream"

#include "BoardConstants.hpp"
#include "Move.cpp"
#include "Board.cpp"
#include "MoveGenerator.cpp"

void print_bitboard(bitboard bb) {
  bitboard mask = (bitboard) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}	

int main() {
  return 0;
}
