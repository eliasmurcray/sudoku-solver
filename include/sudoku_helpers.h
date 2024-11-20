#ifndef SUDOKU_HELPERS_H
#define SUDOKU_HELPERS_H

#include <stdbool.h>
#include <stdio.h>

bool is_valid_grid(int grid[9][9]);

bool solve_sudoku(int grid[9][9]);

void print_sudoku(int grid[9][9]);

#endif
