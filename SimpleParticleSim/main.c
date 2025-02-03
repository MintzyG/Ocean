#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct Camera Camera;
struct Camera {
  int x, y, z;
  int height, width;
  bool is_2d;
  int **PixelBuffer;
};

typedef enum ShapeType ShapeType;
enum ShapeType {
  Circle,
  Square
};

typedef struct Shape Shape;
struct Shape {
  ShapeType type;
  int posX, posY;
  int vX, vY;
  int radius;
  int size;
  void (*Gravity)(Shape *s);
};

typedef struct Scene Scene;
struct Scene {
  Camera c;
  Shape *sBuff;
};

int** CreatePixelBuffer(int height, int width) {
  int **pixelBuffer = calloc(height, sizeof *pixelBuffer);
  for (int i = 0; i < height; i++) {
    pixelBuffer[i] = calloc(width, sizeof **pixelBuffer);
    #ifdef RAND
    for (int j = 0; j < width; j++) {
      pixelBuffer[i][j] = rand() % 2;
    }
    #endif
  }
  return pixelBuffer;
}

int Magnitude(int x, int y, int x2, int y2) {
  double x3 = pow((double)x - (double)x2, 2.0f);
  double y3 = pow((double)y - (double)y2, 2.0f);
  int res = (int)sqrt(x3 + y3);
  return res;
}

void Debug(Scene s) {
  printf("Screen Size: %d-%d\n", s.c.height, s.c.width);
  printf("Shape Pos: %d-%d\n", s.sBuff[0].posY, s.sBuff[0].posX);
  printf("Shape Radius: %d\n", s.sBuff[0].radius);

  int mag = Magnitude(0, 0, s.sBuff[0].posY, s.sBuff[0].posX);
  if (mag < s.sBuff[0].radius) {
    printf("point 0:0 is less: %d\n", mag);
  }

  mag = Magnitude(24, 97, s.sBuff[0].posY, s.sBuff[0].posX);
  if (mag < s.sBuff[0].radius) {
    printf("point 24:97 is less: %d\n", mag);
  }
}

void Render(Scene s, int color) {
  // Debug(s);
  for (int i = 0; i < s.c.height; i++) {
    for (int j = 0; j < s.c.width; j++) {
      #ifdef RAND
      printf("%c", s.c.PixelBuffer[i][j] ? '@' : ' ');
      #endif
      if (Magnitude(i, j, s.sBuff[0].posY, s.sBuff[0].posX) < s.sBuff[0].radius) {
        printf("\033[48;5;%dm \033[0m", color); // Background color
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

#define GRAVITY 1

void ApplyGravity(Shape *s) {
    if (s == NULL) return;
    s->vY += GRAVITY;
}

void RenderLoop(Scene s) {
  int i = 0, color = 22;
  float angle = 0.0f, radius = 10.0f;
  int centerX = s.sBuff[0].posX;
  int centerY = s.sBuff[0].posY;
  while (i++ < 1000) {
    Render(s, color);
    s.sBuff[0].Gravity(&s.sBuff[0]);
    if (s.sBuff[0].posX + s.sBuff[0].radius > s.c.width || s.sBuff[0].posX - s.sBuff[0].radius < 0) {
      s.sBuff[0].vX = (int)(-s.sBuff[0].vX * 0.9);
    }
    if (s.sBuff[0].posY + s.sBuff[0].radius > s.c.height || s.sBuff[0].posY - s.sBuff[0].radius < 0) {
      s.sBuff[0].vY = (int)(-s.sBuff[0].vY * 0.9);
    }
    s.sBuff[0].posX += s.sBuff[0].vX;
    s.sBuff[0].posY += s.sBuff[0].vY;
    usleep(100 * 1000);
    printf("\e[1:1H\e[2J");
  }
  return;
}

int main() {
  srand(time(NULL));

  struct winsize w;

  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
    perror("ioctl");
    return -1;
  }

  Camera c = { .x = 1, .y = 0, .z = 0, .height = w.ws_row, .width = w.ws_col, .is_2d = true, .PixelBuffer = CreatePixelBuffer(w.ws_row, w.ws_col) };
  Shape circle = { .posX = w.ws_col/2, .posY = w.ws_row/2, .vX = 10, .vY = -1, .type = Circle, .radius = 10, .Gravity = ApplyGravity };
  Shape *sBuff = calloc(12, sizeof *sBuff);
  sBuff[0] = circle;
  Scene s = { .c = c, .sBuff = sBuff };
  RenderLoop(s);
}
