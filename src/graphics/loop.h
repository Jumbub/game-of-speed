#pragma once

#include "../logic/board.h"
#include "SFML/Graphics.hpp"

struct Loop {
  Loop(const uint width, const uint height, const std::string title, const bool resizable);
  void run(const long maxComputations, uint threadCount, const uint startTargetRendersPerSecond);

  sf::RenderWindow window;
  sf::Texture texture;
  sf::Sprite sprite;
  sf::Image image;
  sf::Uint32* pixels;
  Board board;
};
