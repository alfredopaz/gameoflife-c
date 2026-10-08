#include <stdio.h>
#include "board.h"
#include "model.h"

static void board_clear(Board *board, int rows, int cols) {
  board->rows = rows;
  board->cols = cols;

  for(int row = 0; row < rows; ++row)
    for(int col = 0; col < cols; ++col)
      board->cells[row][col] = DEAD;
}

int board_blinker(Board *board, int width, int height) {
  if(width < 3 || width > MAX_COLS || height < 1 || height > MAX_ROWS)
    return 0;

  board_clear(board, height, width);

  int row = height / 2;
  int col = width / 2 - 1;
  board->cells[row][col] = ALIVE;
  board->cells[row][col + 1] = ALIVE;
  board->cells[row][col + 2] = ALIVE;
  return 1;
}

int board_load(Board *board, const char *filename) {
  FILE *file = fopen(filename, "r");
  if(file == NULL)
    return 0;

  char line[MAX_COLS + 3];
  int rows = 0;
  int cols = -1;

  while(fgets(line, sizeof line, file) != NULL) {
    int len = 0;
    while(line[len] != '\0' && line[len] != '\n' && line[len] != '\r')
      ++len;

    if(len == 0)
      continue;

    if(len > MAX_COLS || rows >= MAX_ROWS) {
      fclose(file);
      return 0;
    }

    if(cols == -1)
      cols = len;
    else if(len != cols) {
      fclose(file);
      return 0;
    }

    for(int col = 0; col < len; ++col) {
      if(line[col] != '1' && line[col] != '0') {
        fclose(file);
        return 0;
      }
      board->cells[rows][col] = line[col] - '0';
    }

    ++rows;
  }

  fclose(file);

  if(rows == 0 || cols <= 0)
    return 0;

  board->rows = rows;
  board->cols = cols;
  return 1;
}

void board_step(Board *board) {
  int next[MAX_ROWS][MAX_COLS];

  nextGeneration((const int (*)[MAX_COLS])board->cells,
                 board->rows, board->cols, next);

  for(int row = 0; row < board->rows; ++row)
    for(int col = 0; col < board->cols; ++col)
      board->cells[row][col] = next[row][col];
}
