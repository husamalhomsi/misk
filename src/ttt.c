// ttt.c - tic-tac-toe, aka noughts-and-crosses

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "ecma-48.h"
#include "tcem.h"

#define GRID_ROWS 3
#define GRID_COLS 3
#define GRID_AREA 9

#define RED_DISPLAY   31
#define GREEN_DISPLAY 32

#define RED_BACKGROUND   41
#define GREEN_BACKGROUND 42

char grid[GRID_AREA] = {
  '1', '2', '3',
  '4', '5', '6',
  '7', '8', '9'
};

char *magic[GRID_AREA] = {
  "\x0\x1\x2" "\x0\x3\x6" "\x0\x4\x8",
  "\x0\x1\x2" "\x1\x4\x7",
  "\x0\x1\x2" "\x2\x4\x6" "\x2\x5\x8",
  "\x0\x3\x6" "\x3\x4\x5",
  "\x0\x4\x8" "\x1\x4\x7" "\x2\x4\x6" "\x3\x4\x5",
  "\x2\x5\x8" "\x3\x4\x5",
  "\x0\x3\x6" "\x2\x4\x6" "\x6\x7\x8",
  "\x1\x4\x7" "\x6\x7\x8",
  "\x0\x4\x8" "\x2\x5\x8" "\x6\x7\x8"
};

int main(void) {
  struct termios old_term, new_term;

  if (tcgetattr(STDIN_FILENO, &old_term) == -1)
    return 1;

  new_term = old_term;
  new_term.c_lflag &= ~(ECHO | ICANON);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &new_term) == -1)
    return 2;

  setbuf(stdout, 0);
  fputs(TCEM("l") ECMA_48_ED("1") ECMA_48_CUP(), stdout);

  // Print the grid
  for (int row = 0; row < GRID_ROWS; ++row) {
    for (int col = 0; col < GRID_COLS; ++col) {
      putchar(grid[row * GRID_COLS + col]);

      if (col != GRID_COLS - 1)
        fputs("\u2502", stdout); // BOX DRAWINGS LIGHT VERTICAL
    }

    putchar('\n');

    if (row != GRID_ROWS - 1) {
      for (int col = 0; col < GRID_COLS; ++col) {
        fputs("\u2500", stdout); // BOX DRAWINGS LIGHT HORIZONTAL

        if (col != GRID_COLS - 1)
          fputs("\u253c", stdout); // BOX DRAWINGS LIGHT VERTICAL AND HORIZONTAL
      }

      putchar('\n');
    }
  }

  int plays = 0;
  char player = 'X', c;

  // Play
  while (1) {
    printf(ECMA_48_CUP("7") "Player: %c", player);

    while (1) {
      c = getchar();

      if (c == 'e')
        goto exit;

      if (c >= '1' && c <= '9') {
        c -= '0' + 1; // Map from ['1', '9'] to [0, 8]

        if (grid[c] != 'X' && grid[c] != 'O')
          break;
      }
    }

    printf(ECMA_48_CUP("%d;%d") ECMA_48_SGR("%d") "%c" ECMA_48_SGR(),
      c / GRID_COLS * 2 + 1,
      c % GRID_COLS * 2 + 1,
      player == 'X' ? RED_DISPLAY : GREEN_DISPLAY,
      player);

    grid[c] = player;

    if (++plays >= 5) {
      do if ((grid[magic[c][0]] & grid[magic[c][1]] & grid[magic[c][2]]) == player) {
        printf(ECMA_48_SGR("%d"), player == 'X' ? RED_BACKGROUND : GREEN_BACKGROUND);

        for (int i = 0; i < 3; ++i)
          printf(ECMA_48_CUP("%d;%d") "%c",
            magic[c][i] / GRID_COLS * 2 + 1,
            magic[c][i] % GRID_COLS * 2 + 1,
            player);

        printf(ECMA_48_SGR() ECMA_48_CUP("7") ECMA_48_EL() "Winner: %c", player);
        goto exit;
      } while ((magic[c] += 3)[0]);

      if (plays == GRID_AREA) {
        fputs(ECMA_48_CUP("7") "Winner: none", stdout);
        goto exit;
      }
    }

    player = player == 'X' ? 'O' : 'X';
  }

exit:
  fputs(TCEM("h") "\n", stdout);

  if (tcsetattr(STDIN_FILENO, TCSANOW, &old_term) == -1)
    return 2;
}
