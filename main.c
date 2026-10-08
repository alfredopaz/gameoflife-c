#include <stdio.h>
#include <stdlib.h>
#include "model/board.h"
#include "controller/controller.h"

static int text_equal(const char *a, const char *b) {
  while(*a != '\0' && *b != '\0' && *a == *b) {
    ++a;
    ++b;
  }
  return *a == *b;
}

static void usage(const char *program) {
  fprintf(stderr,
          "Uso: %s [-f patron.txt] [--fps N] [--steps N]\n"
          "  -f, --pattern-file  patron con '1' y '0'\n"
          "  --fps               generaciones por segundo (por defecto 5)\n"
          "  --steps             numero de generaciones; -1 = infinito\n",
          program);
}

int main(int argc, char *argv[]) {
  const char *pattern_file = NULL;
  int fps = 5;
  int steps = -1;

  for(int i = 1; i < argc; ++i) {
    if(text_equal(argv[i], "-f") || text_equal(argv[i], "--pattern-file")) {
      if(i + 1 >= argc) {
        usage(argv[0]);
        return 1;
      }
      pattern_file = argv[++i];
    } else if(text_equal(argv[i], "--fps")) {
      if(i + 1 >= argc) {
        usage(argv[0]);
        return 1;
      }
      fps = atoi(argv[++i]);
    } else if(text_equal(argv[i], "--steps")) {
      if(i + 1 >= argc) {
        usage(argv[0]);
        return 1;
      }
      steps = atoi(argv[++i]);
    } else {
      usage(argv[0]);
      return 1;
    }
  }

  if(fps <= 0) {
    fprintf(stderr, "Error: fps debe ser positivo.\n");
    return 1;
  }

  Board board;
  if(pattern_file != NULL) {
    if(!board_load(&board, pattern_file)) {
      fprintf(stderr, "Error al leer el patron '%s'.\n", pattern_file);
      return 1;
    }
  } else if(!board_blinker(&board, 10, 5)) {
    fprintf(stderr, "No se pudo crear el patron inicial.\n");
    return 1;
  }

  controller_run(&board, fps, steps);
  return 0;
}
