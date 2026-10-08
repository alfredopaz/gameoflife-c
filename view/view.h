#ifndef VIEW_H
#define VIEW_H
#include "../model/board.h"
int view_open(const Board *board);
int view_event(void);
void view_draw(const Board *board, const Board *previous,
               int generation, int paused, int fps);
void view_close(void);
#endif
