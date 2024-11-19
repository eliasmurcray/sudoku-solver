#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "sudoku_helpers.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define PADDING 100

void draw_grid(SDL_Renderer *renderer) {
  float cell_size = (WINDOW_WIDTH - PADDING) / 9.0f;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  size_t i = 0;
  for (; i <= 9; i++) {
    float thickness = (i % 3 == 0) ? 3.0f : 1.0f;
    SDL_FRect vertical_line = {
        .x = i * cell_size + PADDING / 2.0f,
        .y = PADDING / 2.0f,
        .w = thickness,
        .h = (float)(WINDOW_HEIGHT - PADDING) + thickness};
    SDL_RenderFillRect(renderer, &vertical_line);
    SDL_FRect horizontal_line = {
        .x = PADDING / 2.0f,
        .y = i * cell_size + PADDING / 2.0f,
        .w = (float)(WINDOW_WIDTH - PADDING) + thickness,
        .h = thickness};
    SDL_RenderFillRect(renderer, &horizontal_line);
  }
}

int main() {
  int grid[9][9] = {
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {0, 0, 0, 0, 0, 0, 0, 0, 0},
      {1, 0, 2, 0, 8, 0, 0, 0, 0}};

  if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL or SDL_ttf: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Sudoku Solver", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  TTF_Font *font = TTF_OpenFont("assets/Terminus.ttf", 24);
  if (!font) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load font: %s", SDL_GetError());
    return 1;
  }
  SDL_Color color = {0, 0, 0, 255};
  SDL_Surface *textSurface = TTF_RenderText_Blended(font, "Sudoku Solver", 0, color);

  if (!textSurface) {
    fprintf(stderr, "TTF_RenderText_Solid Error: %s", SDL_GetError());
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  SDL_DestroySurface(textSurface);

  float w, h;
  SDL_GetTextureSize(textTexture, &w, &h);
  SDL_FRect textRect = {
      .x = (int)((WINDOW_WIDTH - w) / 2),
      .y = 10,
      .w = (int)w,
      .h = (int)h,
  };

  print_sudoku(grid);
  if (solve_sudoku(grid)) {
    printf("Sudoku solved!\n");
    print_sudoku(grid);
  } else {
    printf("No solution exists for the given Sudoku.\n");
  }

  SDL_Event event;
  int running = 1;

  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = 0;
          break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
    draw_grid(renderer);
    SDL_RenderPresent(renderer);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
