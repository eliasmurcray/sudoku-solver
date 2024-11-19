#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#include "sudoku_helpers.h"
#include "rendering.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define PADDING 150

void copy_grid(int old_grid[9][9], int new_grid[9][9]) {
  size_t i = 0;
  for (; i < 9; i++) {
    size_t j = 0;
    for (; j < 9; j++) {
      new_grid[i][j] = old_grid[i][j];
    }
  }
}

bool intersects_frect(SDL_FRect *rect, float x, float y) {
  return x >= rect->x && x <= rect->x + rect->w && y >= rect->y && y <= rect->y + rect->h;
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
  int reset_grid[9][9];
  copy_grid(grid, reset_grid);

  if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL or SDL_ttf: %s", SDL_GetError());
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Sudoku Solver", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_HIGH_PIXEL_DENSITY);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  int rw = 0, rh = 0;
  SDL_GetCurrentRenderOutputSize(renderer, &rw, &rh);
  if (rw != WINDOW_WIDTH) {
    float widthScale = (float)rw / (float)WINDOW_WIDTH;
    float heightScale = (float)rh / (float)WINDOW_HEIGHT;
    SDL_SetRenderScale(renderer, widthScale, heightScale);
  }
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  TTF_Font *font = TTF_OpenFont("assets/Roboto-Medium.ttf", 24);
  TTF_Font *small_font = TTF_OpenFont("assets/Roboto-Medium.ttf", 18);
  if (!font || !small_font) {
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

  float textWidth, textHeight;
  SDL_GetTextureSize(textTexture, &textWidth, &textHeight);
  SDL_FRect textRect = {
      .x = (WINDOW_WIDTH - textWidth) / 2.0f,
      .y = PADDING / 4.0f - textHeight / 2.0f,
      .w = textWidth,
      .h = textHeight,
  };

  struct GridDimensions dimensions = {
    .offset_x = PADDING / 2.0f,
    .offset_y = PADDING / 2.0f,
    .size = WINDOW_WIDTH - PADDING
  };
  SDL_FRect solve_button = {PADDING / 2.0f, WINDOW_HEIGHT - 55, WINDOW_WIDTH / 2.0f - PADDING / 2.0f - 10.0f, 40};
  SDL_FRect reset_button = {WINDOW_WIDTH - PADDING / 2.0f - WINDOW_WIDTH / 2.0f + PADDING / 2.0f + 10.0f, WINDOW_HEIGHT - 55, WINDOW_WIDTH / 2.0f - PADDING / 2.0f - 10.0f, 40};
  SDL_Event event;
  int running = 1;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_EVENT_QUIT:
          running = 0;
          break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
          float mx = event.button.x, my = event.button.y;
          if (intersects_frect(&solve_button, mx, my)) {
            solve_sudoku(grid);
          }
          if (intersects_frect(&reset_button, mx, my)) {
            copy_grid(reset_grid, grid);
          }
          break;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
    draw_grid(renderer, &dimensions);
    draw_numbers(renderer, font, grid, &dimensions);
    draw_button(renderer, small_font, "Solve", solve_button, (int []) {0, 176, 255, 255});
    draw_button(renderer, small_font, "Reset", reset_button, (int []) {200, 200, 200, 255});
    SDL_RenderPresent(renderer);
  }

  TTF_CloseFont(font);
  TTF_Quit();
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
