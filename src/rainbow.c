// rainbow.c - print a sine-shaped 6-color rainbow

#include <math.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "ecma-48.h"

// Full angle in radians
#define TAU 6.28318530717958647692

int main(void) {
  // Red, yellow, green, cyan, blue, magenta
  int sgr_colors[] = {31, 33, 32, 36, 34, 35};
  struct winsize winsz;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz) == -1)
    return 1;

  --winsz.ws_row;

  fputs(ECMA_48_ED("1"), stdout);

  for (int bow = 6; bow >= 1; --bow) {
    int last_col = winsz.ws_col - bow + 1;
    int peak_row = winsz.ws_row - bow;
    int width = last_col - bow;
    int color = sgr_colors[bow - 1];

    for (int col = bow; col <= last_col; ++col)
      printf(ECMA_48_CUP("%d;%d") ECMA_48_SGR("%d") "\u2588", // FULL BLOCK
        winsz.ws_row - (int) round(sin(TAU / 2 * (col - bow) / width) * peak_row),
        col,
        color);
  }

  fputs(ECMA_48_SGR(), stdout);
}
