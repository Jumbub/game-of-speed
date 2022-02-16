#pragma once

#include "board.h"
#include "breeder.h"
#include "generate.h"
#include "padding.h"

/**
 * Generates a board with 4 regions:
 * - top-left:     seeded random cells
 * - top-right:    chess grid of 8x8 alive/dead cells
 * - bottom:       breeder
 */
void assignBenchmarkCells(Board& board) {
  srand(0);
  const auto& width = board.width;
  const auto& height = board.height;
  for (uint ry = 1; ry < height + 1; ++ry) {
    for (uint rx = 1; rx < width + 1; ++rx) {
      const uint i = ry * (width + 2) + rx;
      const uint x = rx - 1;
      const uint y = ry - 1;
      if (y > height / 2) {
        if (x < width / 2)
          board.output[i][0] = rand() % 2 ? ALIVE : DEAD;
        if (x >= width / 2)
          board.output[i][0] = (x / 8) % 2 != (y / 8) % 2 ? ALIVE : DEAD;
      } else {
        const uint breederMarginY = (height / 2 - BREEDER_HEIGHT) / 2;
        const uint breederY = y - breederMarginY;
        if (breederY > 0 && breederY < BREEDER_HEIGHT && x < BREEDER_WIDTH) {
          board.output[i][0] = BREEDER[breederY][x] ? ALIVE : DEAD;
        } else {
          board.output[i][0] = DEAD;
        }
      }
    }
  }

  assignBoardNeighbours(board);
  assignBorders(board);
}
