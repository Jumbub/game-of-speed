#pragma once

#include "../../build/_deps/imgui-src/imgui.h"
#include "../../build/_deps/sfml-src/include/SFML/Graphics.hpp"
#include "board.h"
#include "generate.h"
#include "threads.h"

// ImGui decleration
void drawGui(
    Board& board,
    const sf::RenderWindow& window,
    const sf::Time& renderDelta,
    const uint& totalComputations,
    const uint& computationsSinceLastRender,
    uint& targetRendersPerSecond,
    uint& threadCount) {
  ImGui::Begin("Configuration");

  // Basic
  ImGui::Text("Total computations: %d", totalComputations);
  ImGui::Text("Board size: %dx%d", board.width, board.height);
  ImGui::Text("Window size: %dx%d", window.getSize().x, window.getSize().y);

  // Render
  ImGui::Text("Renders/second: %.2f", 1.0 / renderDelta.asSeconds());
  int rps = (int)targetRendersPerSecond;
  ImGui::SliderInt("Target renders/second", &rps, 1, 300);
  if ((uint)rps != targetRendersPerSecond) {
    std::scoped_lock gaurd(board.modifyingMemory);
    targetRendersPerSecond = (uint)rps;
  }

  // Compute
  ImGui::Text("Computations/second: %.2f", (float)computationsSinceLastRender / renderDelta.asSeconds());
  int tpb = (int)threadCount;
  ImGui::SliderInt("Threads/computation", &tpb, 1, (int)PROBABLY_OPTIMAL_THREAD_COUNT * 4);
  if ((uint)tpb != threadCount) {
    std::scoped_lock gaurd(board.modifyingMemory);
    threadCount = (uint)tpb;
  }

  // Restart
  if (ImGui::Button("Restart")) {
    std::scoped_lock gaurd(board.modifyingMemory);
    assignBenchmarkCells(board);
  }

  ImGui::End();
}