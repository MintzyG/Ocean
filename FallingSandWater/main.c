#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef SIZE_Y
#define SIZE_Y 32
#endif

#ifndef SIZE_X
#define SIZE_X 128
#endif

int** create() {
  int** sandbox = calloc(SIZE_Y, sizeof *sandbox);
  for (int i = 0; i < SIZE_Y; i++) {
    sandbox[i] = calloc(SIZE_X, sizeof *(sandbox[0]));
    for (int j = 0; j < SIZE_X; j++) {
      sandbox[i][j] = !(rand() % 5);
    }
  }

  return sandbox;
}

void print(int** sandbox) {
  for (int i = 0; i < SIZE_Y; i++) {
    for (int j = 0; j < SIZE_X; j++) {
      printf("%c", sandbox[i][j] ? '@' : '.');
    }
    printf("\n");
  }
}

int proccess(int** sandbox) {
  int moved = 0, tmp = 0;
  for (int i = SIZE_Y - 1; i >= 0; i--) {
    for (int j = SIZE_X - 1; j >= 0; j--) {
      // Move Down
      if (i > 0) {
        if (sandbox[i][j] == 0 && sandbox[i-1][j] == 1) {
          tmp = sandbox[i][j];
          sandbox[i][j] = sandbox[i-1][j];
          sandbox[i-1][j] = tmp;
          moved = 1;
          continue;
        }
      }

      // Slide Left
      if (j > 0 && i > 0) {
        if (sandbox[i][j] == 1 && sandbox[i][j-1] == 0 && sandbox[i-1][j] == 1) {
          tmp = sandbox[i-1][j];
          sandbox[i-1][j] = sandbox[i][j-1];
          sandbox[i][j-1] = tmp;
          moved = 1;
          continue;
        }
      }

      // Slide Right
      if (j != SIZE_X - 1 && i > 0) {
        if (sandbox[i][j] == 1 && sandbox[i][j+1] == 0 && sandbox[i-1][j] == 1) {
          tmp = sandbox[i-1][j];
          sandbox[i-1][j] = sandbox[i][j+1];
          sandbox[i][j+1] = tmp;
          moved = 1;
          continue;
        }
      }

      int side = rand() % 2;
      if (side) {
        // Move Left
        if (j > 0) {
          if (sandbox[i][j] == 1 && sandbox[i][j-1] == 0) {
            tmp = sandbox[i][j];
            sandbox[i][j] = sandbox[i][j-1];
            sandbox[i][j-1] = tmp;
            moved = 1;
            continue;
          }
        }
      } else {
        // Move Right
        if (j != SIZE_X - 1) {
          if (sandbox[i][j] == 1 && sandbox[i][j+1] == 0) {
            tmp = sandbox[i][j];
            sandbox[i][j] = sandbox[i][j+1];
            sandbox[i][j+1] = tmp;
            moved = 1;
            continue;
          }
        }
      }
    }
  }
  return moved;
}

int main() {
  srand(time(NULL));
  int **sandbox = create();
  print(sandbox);
  while (proccess(sandbox)) {
    printf("\e[1;1H\e[2J");
    print(sandbox);
    usleep(100 * 1000);
  }
  return 0;
}
