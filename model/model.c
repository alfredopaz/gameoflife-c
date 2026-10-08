#include "model.h"

int countNeighbors(const int a[][MAX_COLS],
                   int rows, int cols, int r, int c) {
  /* TODO: contar las ocho posiciones vecinas validas con valor 1.
   * No contar la propia posicion ni acceder fuera de la matriz. */
  (void)a;
  (void)rows;
  (void)cols;
  (void)r;
  (void)c;
  return 0;
}

void nextGeneration(const int current[][MAX_COLS],
                    int rows, int cols,
                    int next[][MAX_COLS]) {
  /* TODO: calcular toda la siguiente generacion en next, usando
   * unicamente valores de current (que no debe modificarse). */
  (void)current;
  for(int r = 0; r < rows; ++r)
    for(int c = 0; c < cols; ++c)
      next[r][c] = DEAD; /* Sustituir por la regla correcta. */
}
