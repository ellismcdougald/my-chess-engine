#include"iostream"

void print_bitboard(uint64_t bb) {
  uint64_t mask = (uint64_t) 1 << 63;
  for(int i = 0; i < 64; i++) {
    std::cout << (mask & bb ? 1 : 0);
    mask >>= 1;
    if((i + 1) % 8 == 0) std::cout << "\n";
  }
}	

#include "BoardConstants.hpp"
#include "Move.cpp"
#include "Board.cpp"
#include "MoveGenerator.cpp"

#include "BoardTests.cpp"


int main() {
  BoardTests board_tests;
  board_tests.run_all_tests();
  
  return 0;
}
