#include <assert.h>
#include <stdio.h>
#include "../model/board.h"
#include "../model/model.h"

static void test_isolated_cell_dies(void) {
  Board b = {.rows = 3, .cols = 3};
  b.cells[1][1] = 1;
  board_step(&b);
  for(int r = 0; r < 3; ++r)
    for(int c = 0; c < 3; ++c)
      assert(b.cells[r][c] == 0);
}

static void test_block_stays_stable(void) {
  Board b = {.rows = 4, .cols = 4};
  b.cells[1][1] = b.cells[1][2] = 1;
  b.cells[2][1] = b.cells[2][2] = 1;
  board_step(&b);
  for(int r = 0; r < 4; ++r)
    for(int c = 0; c < 4; ++c)
      assert(b.cells[r][c] == (r >= 1 && r <= 2 && c >= 1 && c <= 2));
}

static void test_blinker_rotates(void) {
  Board b = {.rows = 5, .cols = 5};
  b.cells[2][1] = b.cells[2][2] = b.cells[2][3] = 1;
  assert(countNeighbors((const int (*)[MAX_COLS])b.cells, 5, 5, 2, 2) == 2);
  board_step(&b);
  for(int r = 0; r < 5; ++r)
    for(int c = 0; c < 5; ++c)
      assert(b.cells[r][c] == (c == 2 && r >= 1 && r <= 3));
}

static void test_small_board(void) {
  Board b = {.rows = 1, .cols = 1};
  b.cells[0][0] = 1;
  assert(countNeighbors((const int (*)[MAX_COLS])b.cells, 1, 1, 0, 0) == 0);
  board_step(&b);
  assert(b.cells[0][0] == 0);
}

int main(void) {
  test_isolated_cell_dies();
  test_block_stays_stable();
  test_blinker_rotates();
  test_small_board();
  puts("Todos los tests pasaron.");
  return 0;
}
