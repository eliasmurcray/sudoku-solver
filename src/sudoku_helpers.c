#include "sudoku_helpers.h"

static bool is_valid(int grid[9][9], int row, int col, int num);

bool is_valid_grid(int grid[9][9]) {
  int row = 0;
  for (; row < 9; row++) {
    int col = 0;
    for (; col < 9; col++) {
      int num = grid[row][col];
      if (num != 0) {
        grid[row][col] = 0;
        if (!is_valid(grid, row, col, num)) {
          grid[row][col] = num;
          return false;
        }
        grid[row][col] = num;
      }
    }
  }
  return true;
}

bool solve_sudoku(int grid[9][9]) {
  int row = -1, col = -1;
  bool isEmpty = false;

  int i = 0;
  for (; i < 9 && !isEmpty; i++) {
    int j = 0;
    for (; j < 9 && !isEmpty; j++) {
      if (grid[i][j] == 0) {
        row = i;
        col = j;
        isEmpty = true;
      }
    }
  }

  if (!isEmpty) return true;

  int num = 1;
  for (; num <= 9; num++) {
    if (is_valid(grid, row, col, num)) {
      grid[row][col] = num;
      if (solve_sudoku(grid)) return true;
      grid[row][col] = 0;
    }
  }
  return false;
}

void print_sudoku(int grid[9][9]) {
  int row = 0;
  for (; row < 9; row++) {
    int col = 0;
    for (; col < 9; col++) {
      printf("%2d", grid[row][col]);
    }
    printf("\n");
  }
}

static bool is_valid(int grid[9][9], int row, int col, int num) {
  int x = 0;
  for (; x < 9; x++)
    if (grid[row][x] == num) return false;
  int y = 0;
  for (; y < 9; y++)
    if (grid[y][col] == num) return false;
  int startRow = row - row % 3, startCol = col - col % 3;
  int i = 0;
  for (; i < 3; i++) {
    int j = 0;
    for (; j < 3; j++) {
      if (grid[i + startRow][j + startCol] == num) return false;
    }
  }
  return true;
}
