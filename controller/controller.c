#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include "controller.h"
#include "../view/view.h"

static void sleep_frame(void) {
  struct timespec delay = { .tv_sec = 0, .tv_nsec = 10000000L };
  nanosleep(&delay, NULL);
}

void controller_run(Board *board, int fps, int steps) {
  if(!view_open(board)) return;
  Board initial = *board;
  Board previous = *board;
  int generation = 0;
  int paused = 0;
  int tick = 0;
  int running = 1;
  view_draw(board, NULL, generation, paused, fps);
  while(running) {
    int key = view_event();
    if(key == 'q') break;
    if(key == 'p') paused = !paused;
    if(key == 'r') {
      *board = initial;
      previous = initial;
      generation = 0;
      tick = 0;
      paused = 0;
    }
    if(key == '+') {
      if(fps < 30) ++fps;
      tick = 0;
    }
    if(key == '-') {
      if(fps > 1) --fps;
      tick = 0;
    }
    if(key == 'n') {
      if(steps < 0 || generation < steps) {
        previous = *board;
        board_step(board);
        ++generation;
      }
      paused = 1;
    }
    if(!paused && (steps < 0 || generation < steps)) {
      ++tick;
      if(tick >= 100 / fps) {
        tick = 0;
        previous = *board;
        board_step(board);
        ++generation;
      }
    }
    if(steps >= 0 && generation >= steps) paused = 1;
    if(key != 0 || (!paused && tick == 0))
      view_draw(board, generation == 0 ? NULL : &previous,
                generation, paused, fps);
    sleep_frame();
  }
  view_close();
}
