#pragma once

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <thread>
#include <tuple>
#include <vector>
#include "board.h"
#include "padding.h"
#include "threads.h"

const Cell EMPTY = 16;

uint64_t val64(Cell* ns) {
  return *reinterpret_cast<uint64_t*>(ns);
}

void nextBoardSection(const uint startY, const uint endY, const uint width, Cell* input, Cell* output) {
  Cell neighbours[3] = {0, 0, 0};
  uint nextYBase = 0;
  uint middleYBase = 0;
  uint lastYBase = 0;

  Cell* neighboursBelow = nullptr;
  Cell* neighboursMiddle = nullptr;
  Cell* neighboursAbove = nullptr;

  const auto realWidth = width + 2;
  for (uint y = startY + 1; y < endY + 1; y++) {
    // Compute new Y levels
    lastYBase = ((y - 1) * realWidth);
    middleYBase = y * realWidth;
    nextYBase = ((y + 1) * realWidth);

    neighboursBelow = &input[lastYBase];
    neighboursMiddle = &input[middleYBase];
    neighboursAbove = &input[nextYBase];

    neighbours[0] = EMPTY;
    neighbours[1] = EMPTY;

    for (uint x = 1; x < width + 1; x++) {
      const auto i = y * realWidth + x;

      // Skip if we see sequential 0 neighbour counts
      constexpr uint MAXIMUM_SKIP = 6;
      if (x < width &&
          val64(&neighboursAbove[x - 1]) + val64(&neighboursMiddle[x - 1]) + val64(&neighboursBelow[x - 1]) == 0) {
        neighbours[0] = EMPTY;
        neighbours[1] = EMPTY;
        neighbours[2] = EMPTY;
        for (uint ii = 0; ii < MAXIMUM_SKIP; ii++)
          output[i + ii] = DEAD;
        x += MAXIMUM_SKIP - 1;
        continue;
      }

      const auto currentStateBool = input[i];

      if (x > 1) {
        // Shift neighbour counts down
        neighbours[0] = neighbours[1];
        neighbours[1] = neighbours[2];
      }

      // Left neighbours
      if (neighbours[0] == EMPTY) {
        const auto prevX = x - 1;
        neighbours[0] = neighboursBelow[prevX] + neighboursMiddle[prevX] + neighboursAbove[prevX];
      }

      // Middle neighbours
      if (neighbours[1] == EMPTY) {
        neighbours[1] = neighboursBelow[x] + neighboursAbove[x] + currentStateBool;
      }

      const auto nextX = x + 1;
      neighbours[2] = neighboursBelow[nextX] + neighboursMiddle[nextX] + neighboursAbove[nextX];

      // Compute new cell state
      const auto totalNeighbours = neighbours[0] + neighbours[1] + neighbours[2];

      if (currentStateBool && (totalNeighbours < 3 || totalNeighbours > 4)) {
        output[i] = DEAD;
      } else if (!currentStateBool && totalNeighbours == 3) {
        output[i] = ALIVE;
      } else {
        output[i] = currentStateBool;
      }
    }
  }
}

void nextBoard(BoardMeta& board) {
  board.flip();

  const auto& width = board.width;
  const auto& height = board.height;
  const auto& input = board.input;
  const auto& output = board.output;

  const auto totalThreads = std::min(board.threadsPerBoard, (uint)height);
  const auto threadLines = height / totalThreads;
  const auto threadLinesRemaining = height % totalThreads;

  std::vector<std::thread> threads;
  for (uint t = 0; t < totalThreads; t++) {
    // Compute start and end indexes for threads
    const auto startY = t * threadLines;
    auto endY = (t + 1) * threadLines;

    // In the case of an uneven divide, the last thread gets the left-overs
    if (t == totalThreads - 1)
      endY += threadLinesRemaining;

    threads.push_back(
        std::thread([startY, endY, width, input, output]() { nextBoardSection(startY, endY, width, input, output); }));
  }

  for (auto& thread : threads) {
    thread.join();
  }

  board.generation++;

  padding(board.output, board.width, board.height);
}
