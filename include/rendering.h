#ifndef RENDERING_H
#define RENDERING_H

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

struct GridDimensions {
  float offset_x;
  float offset_y;
  float size;
};

void draw_grid(SDL_Renderer *renderer, struct GridDimensions *dimensions);

void draw_numbers(SDL_Renderer *renderer, TTF_Font *font, int grid[9][9], struct GridDimensions *dimensions);

void draw_button(SDL_Renderer *renderer, TTF_Font *font, const char *text, SDL_FRect rect, int background_color[4]);

#endif
