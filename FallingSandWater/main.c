#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

int** create(int size_x, int size_y) {
  int** sandbox = calloc(size_y, sizeof *sandbox);
  for (int i = 0; i <= size_y; i++) {
    sandbox[i] = calloc(size_x, sizeof *(sandbox[0]));
    for (int j = 0; j < size_x; j++) {
      sandbox[i][j] = !(rand() % 5);
    }
  }

  return sandbox;
}

void print(int** sandbox, int size_x, int size_y) {
  for (int i = 0; i <= size_y; i++) {
    for (int j = 0; j < size_x; j++) {
      printw("%c", sandbox[i][j] ? '@' : '.');
    }
    move(i, 0);
  }
}

int proccess(int** sandbox, int size_x, int size_y) {
  int moved = 0, tmp = 0;
  for (int i = size_y; i > 0; i--) {
    for (int j = size_x - 1; j >= 0; j--) {
      int l = (j + size_x - 1) % size_x;
      int r = (j + size_x + 1) % size_x;

      // Move Down
      if (sandbox[i][j] == 0 && sandbox[i-1][j] == 1) {
        tmp = sandbox[i][j];
        sandbox[i][j] = sandbox[i-1][j];
        sandbox[i-1][j] = tmp;
        moved = 1;
        continue;
      }


      // Slide Left
      if (sandbox[i][j] == 1 && sandbox[i][l] == 0 && sandbox[i-1][j] == 1) {
        tmp = sandbox[i-1][j];
        sandbox[i-1][j] = sandbox[i][j-1];
        sandbox[i][l] = tmp;
        moved = 1;
        continue;
      }

      // Slide Right
      if (sandbox[i][j] == 1 && sandbox[i][r] == 0 && sandbox[i-1][j] == 1) {
        tmp = sandbox[i-1][j];
        sandbox[i-1][j] = sandbox[i][r];
        sandbox[i][r] = tmp;
        moved = 1;
        continue;
      }

      int side = rand() % 2;

      if (side) {
        // Move Left
        if (sandbox[i][j] == 1 && sandbox[i][l] == 0) {
          tmp = sandbox[i][j];
          sandbox[i][j] = sandbox[i][l];
          sandbox[i][l] = tmp;
          moved = 1;
          continue;
        }
      } else {
        // Move Right
        if (sandbox[i][j] == 1 && sandbox[i][r] == 0) {
          tmp = sandbox[i][j];
          sandbox[i][j] = sandbox[i][r];
          sandbox[i][r] = tmp;
          moved = 1;
          continue;
        }
      }
    }
  }
  return moved;
}

int main() {
  int size_x, size_y;
  char str;

  initscr();
  timeout(100);
  curs_set(0);

  srand(time(NULL));
  while (str != 'q'){
    getmaxyx(stdscr, size_y, size_x);
    int **sandbox = create(size_x, size_y);
    str = ' ';
    while (proccess(sandbox, size_x, size_y) && str != 'q' && str != 'r') {
      print(sandbox, size_x, size_y);
      str = getch();
      refresh();
      clear();
    }
  }

  endwin();
  return 0;
}
