#include "rendering.h"

void draw_grid(SDL_Renderer *renderer, struct GridDimensions *dimensions) {
  float cell_size = dimensions->size / 9.0f;
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  size_t i = 0;
  for (; i <= 9; i++) {
    float thickness = (i % 3 == 0) ? 3.0f : 1.0f;
    SDL_FRect vertical_line = {
        .x = i * cell_size + dimensions->offset_x,
        .y = dimensions->offset_y,
        .w = thickness,
        .h = dimensions->size + thickness};
    SDL_RenderFillRect(renderer, &vertical_line);
    SDL_FRect horizontal_line = {
        .x = dimensions->offset_x,
        .y = i * cell_size + dimensions->offset_y,
        .w = dimensions->size + thickness,
        .h = thickness};
    SDL_RenderFillRect(renderer, &horizontal_line);
  }
}

void draw_numbers(SDL_Renderer *renderer, TTF_Font *font, int grid[9][9], struct GridDimensions *dimensions) {
  float cell_size = dimensions->size / 9.0f;
  SDL_Color color = {0, 0, 0, 255};
  int row = 0;
  for (; row < 9; row++) {
    int col = 0;
    for (; col < 9; col++) {
      if (grid[row][col] != 0) {
        char number[2];
        snprintf(number, sizeof(number), "%d", grid[row][col]);
        SDL_Surface *textSurface = TTF_RenderText_Blended(font, number, strlen(number), color);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int text_width = textSurface->w;
        int text_height = textSurface->h;
        SDL_DestroySurface(textSurface);

        SDL_FRect destRect = {
            col * cell_size + (cell_size - text_width) / 2 + dimensions->offset_x,
            row * cell_size + (cell_size - text_height) / 2 + dimensions->offset_y,
            text_width,
            text_height};

        SDL_RenderTexture(renderer, textTexture, NULL, &destRect);
        SDL_DestroyTexture(textTexture);
      }
    }
  }
}

void draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_FRect rect, int background_color[4]) {
  SDL_SetRenderDrawColor(renderer, background_color[0], background_color[1], background_color[2], background_color[3]);
  SDL_RenderFillRect(renderer, &rect);

  SDL_Color color = {0, 0, 0, 255};
  SDL_Surface *textSurface = TTF_RenderText_Blended(font, text, strlen(text), color);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  int text_width = textSurface->w;
  int text_height = textSurface->h;
  SDL_DestroySurface(textSurface);

  SDL_FRect textRect = {
      rect.x + (rect.w - text_width) / 2.0f,
      rect.y + (rect.h - text_height) / 2.0f,
      text_width,
      text_height};
  SDL_RenderTexture(renderer, textTexture, NULL, &textRect);
  SDL_DestroyTexture(textTexture);
}
