#include "../board/generate.h"
#include "../board/next.h"
#include "../board/sdl.h"
#include "../util/profile.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <iostream>

int main() {
  // Initialize SDL
  SDL_Init(SDL_INIT_VIDEO);
  SDL_Event event;

  // Create window
  SDL_Renderer *renderer;
  SDL_Window *window;
  SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);

  // Generate initial board
  auto p1 = startProfiling();
  auto board = boardForSdlWindow(window);
  stopProfiling(p1, "Generated first board");

  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      // Exit when told, or Escape is pressed
      if ((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) ||
          (event.type == SDL_QUIT) ||
          (event.type == SDL_WINDOWEVENT &&
           event.window.event == SDL_WINDOWEVENT_CLOSE))
        running = false;
      // Re-create board when Enter is pressed
      else if (event.type == SDL_KEYDOWN &&
               event.key.keysym.scancode == SDL_SCANCODE_RETURN)
        board = boardForSdlWindow(window);
    }

    auto p2 = startProfiling();
    board = nextBoard(board);
    stopProfiling(p2, "Calculated next board");

    auto p3 = startProfiling();
    renderBoardSdl(board, renderer);
    stopProfiling(p3, "Rendered next board");
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
