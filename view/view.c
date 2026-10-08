#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "view.h"

#define CELL 20
#define HEADER 82
#define FOOTER 44
#define MARGIN 22
#define MIN_WIDTH 600

static Display *display;
static Window window;
static GC gc;
static Atom delete_atom;
static unsigned long background, header, panel, grid, alive, accent, muted, changed;
static int opened, win_width, win_height, grid_x, grid_y;

static unsigned long color(const char *name, unsigned long fallback) {
  XColor exact, screen;
  Colormap map = DefaultColormap(display, DefaultScreen(display));
  if(XAllocNamedColor(display, map, name, &screen, &exact))
    return screen.pixel;
  return fallback;
}

static void text(int x, int y, unsigned long ink, const char *label) {
  XSetForeground(display, gc, ink);
  XDrawString(display, window, gc, x, y, label, (int)strlen(label));
}

int view_open(const Board *board) {
  display = XOpenDisplay(NULL);
  if(display == NULL) {
    fprintf(stderr, "No se encontro X11. Compruebe la variable DISPLAY.\n");
    return 0;
  }
  int screen = DefaultScreen(display);
  background = color("#0b1120", BlackPixel(display, screen));
  header = color("#151f33", BlackPixel(display, screen));
  panel = color("#172338", BlackPixel(display, screen));
  grid = color("#2a3a4e", WhitePixel(display, screen));
  alive = color("#e2f4fd", WhitePixel(display, screen));
  accent = color("#65e0b1", WhitePixel(display, screen));
  muted = color("#aac3d9", WhitePixel(display, screen));
  changed = color("#f7bf69", WhitePixel(display, screen));
  int bw = board->cols * CELL;
  int bh = board->rows * CELL;
  win_width = bw + 2 * MARGIN;
  if(win_width < MIN_WIDTH) win_width = MIN_WIDTH;
  win_height = HEADER + bh + MARGIN + FOOTER;
  grid_x = (win_width - bw) / 2;
  grid_y = HEADER + MARGIN / 2;
  window = XCreateSimpleWindow(display, RootWindow(display, screen),
                               80, 80, (unsigned)win_width, (unsigned)win_height,
                               0, background, background);
  XStoreName(display, window, "Juego de la Vida | Programacion I");
  XSelectInput(display, window, ExposureMask | KeyPressMask | StructureNotifyMask);
  delete_atom = XInternAtom(display, "WM_DELETE_WINDOW", False);
  XSetWMProtocols(display, window, &delete_atom, 1);
  XSizeHints hints = {0};
  hints.flags = PMinSize | PMaxSize;
  hints.min_width = hints.max_width = win_width;
  hints.min_height = hints.max_height = win_height;
  XSetWMNormalHints(display, window, &hints);
  gc = XCreateGC(display, window, 0, NULL);
  XMapWindow(display, window);
  XFlush(display);
  opened = 1;
  return 1;
}

/* 0: sin accion, 'p': pausa, 'r': reiniciar, 'n': avanzar,
   '+': mas rapido, '-': mas lento, 'q': salir, 'e': redibujar. */
int view_event(void) {
  if(!opened) return 'q';
  int redraw = 0;
  while(XPending(display)) {
    XEvent event;
    XNextEvent(display, &event);
    if(event.type == ClientMessage &&
       (Atom)event.xclient.data.l[0] == delete_atom) return 'q';
    if(event.type == DestroyNotify) return 'q';
    if(event.type == Expose) redraw = 1;
    if(event.type == KeyPress) {
      KeySym key = XLookupKeysym(&event.xkey, 0);
      if(key == XK_q || key == XK_Q || key == XK_Escape) return 'q';
      if(key == XK_p || key == XK_P || key == XK_space) return 'p';
      if(key == XK_r || key == XK_R) return 'r';
      if(key == XK_n || key == XK_N || key == XK_Right) return 'n';
      if(key == XK_plus || key == XK_equal || key == XK_KP_Add) return '+';
      if(key == XK_minus || key == XK_KP_Subtract) return '-';
    }
  }
  return redraw ? 'e' : 0;
}

void view_draw(const Board *board, const Board *previous,
               int generation, int paused, int fps) {
  if(!opened) return;
  int bw = board->cols * CELL;
  int bh = board->rows * CELL;
  int count = 0;
  for(int r = 0; r < board->rows; ++r)
    for(int c = 0; c < board->cols; ++c)
      count += board->cells[r][c] == ALIVE;

  XSetForeground(display, gc, background);
  XFillRectangle(display, window, gc, 0, 0,
                 (unsigned)win_width, (unsigned)win_height);
  XSetForeground(display, gc, header);
  XFillRectangle(display, window, gc, 0, 0, (unsigned)win_width, HEADER);
  text(22, 25, alive, "JUEGO DE LA VIDA");
  text(22, 47, muted, "Programacion I  /  Conway");
  char info[160];
  snprintf(info, sizeof info, "Generacion: %d   |   Vivas: %d   |   %s   |   %d gen/s",
           generation, count, paused ? "PAUSA" : "EJECUCION", fps);
  text(22, 69, accent, info);

  XSetForeground(display, gc, panel);
  XFillRectangle(display, window, gc, grid_x, grid_y, (unsigned)bw, (unsigned)bh);
  XSetForeground(display, gc, grid);
  for(int r = 0; r <= board->rows; ++r)
    XDrawLine(display, window, gc, grid_x, grid_y + r * CELL,
              grid_x + bw, grid_y + r * CELL);
  for(int c = 0; c <= board->cols; ++c)
    XDrawLine(display, window, gc, grid_x + c * CELL, grid_y,
              grid_x + c * CELL, grid_y + bh);

  for(int r = 0; r < board->rows; ++r)
    for(int c = 0; c < board->cols; ++c)
      if(board->cells[r][c] == ALIVE) {
        unsigned long ink = previous != NULL && previous->cells[r][c] == DEAD
                            ? changed : alive;
        XSetForeground(display, gc, ink);
        XFillRectangle(display, window, gc,
                       grid_x + c * CELL + 3, grid_y + r * CELL + 3,
                       CELL - 5, CELL - 5);
      }
  int footer_y = grid_y + bh + 23;
  text(22, footer_y, muted, "ESPACIO/P Pausa  |  N Paso  |  R Reiniciar  |  +/- Velocidad  |  Q Salir");
  XFlush(display);
}

void view_close(void) {
  if(!opened) return;
  XFreeGC(display, gc);
  XDestroyWindow(display, window);
  XCloseDisplay(display);
  opened = 0;
}
