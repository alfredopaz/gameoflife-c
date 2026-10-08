#ifndef MODEL_H
#define MODEL_H

#include "board.h"

int countNeighbors(const int a[][MAX_COLS],
                   int rows, int cols, int r, int c);

void nextGeneration(const int current[][MAX_COLS],
                    int rows, int cols,
                    int next[][MAX_COLS]);

#endif
