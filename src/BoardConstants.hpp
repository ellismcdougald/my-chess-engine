#ifndef BOARD_CONSTANTS_HPP // GUARD
#define BOARD_CONSTANTS_HPP // GUARD

namespace BoardConstants {
  enum PIECE {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NONE};
  enum COLOR {WHITE, BLACK};
  enum DIRECTION {NORTH, EAST, SOUTH, WEST, NORTHEAST, NORTHWEST, SOUTHEAST, SOUTHWEST};
}

#endif // END GUARD
