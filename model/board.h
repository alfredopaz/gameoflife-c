#ifndef BOARD_H
#define BOARD_H

#define MAX_ROWS 80
#define MAX_COLS 120
#define DEAD 0
#define ALIVE 1

typedef struct {
  int rows;
  int cols;
  int cells[MAX_ROWS][MAX_COLS];
} Board;

int board_load(Board *board, const char *filename);
int board_blinker(Board *board, int width, int height);
void board_step(Board *board);

#endif
