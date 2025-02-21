#include <stdlib.h>

#define FPS 60
#define GRAVITY -9.81
#define DENSITY 997
#define SPACING 1
#define OVERRELAXATION 1.9

typedef struct Cell Cell;
struct Cell {
  float x, y;
  float pressure;
};

typedef struct Field Field;
struct Field {
   Cell **velocities;
   int width, height;
};

void ApplyDivergence(Field *f) {
  for (int i = 0; i < f->width; i++) {
    for (int j = 0; j < f->height; j++) {
      float divergence = 0.0;
      float lx = 0.0, rx = 0.0, ty = 0.0, by = 0.0;
      int slx = 1, srx = 1, sty = 1, sby = 1;

      if (i == 0) { slx = 0; }
      if (i == f->width - 1) { srx = 0; }
      if (j == 0) { sby = 0; }
      if (j == f->height - 1) { sty = 0; }
      int scalar = slx + srx + sby + sty;
      if (scalar == 0) continue;

      lx = -1 * f->velocities[i][j].x;
      rx = f->velocities[i+1][j].x;
      by = -1 * f->velocities[i][j].y;
      ty = f->velocities[i][j+1].y;
      divergence = OVERRELAXATION * (rx - lx + ty - by);

      lx = lx + (divergence * slx) / scalar;
      rx = rx - (divergence * srx) / scalar;
      by = by + (divergence * sby) / scalar;
      ty = ty - (divergence * sty) / scalar;

      f->velocities[i][j].pressure =
        f->velocities[i][j].pressure +
        divergence/scalar *
        (DENSITY * SPACING)/(1.0/FPS);
    }
  }
}

void ApplyGravity(Field *f) {
  for (int i = 0; i < f->width; i++) {
    for (int j = 0; j < f->height; j++) {
      f->velocities[i][j].y += 1.0/FPS * GRAVITY;
    }
  }
}

Field GenerateField(int width, int height) {
  Cell **line = calloc(width, sizeof *line);
  for (int i = 0; i < width; i++) {
    line[i] = calloc(height, sizeof **line);
    for (int j = 0; j < height; j++) {
      line[i][j] = (Cell){};
    }
  }
  return (Field){ .velocities = line, .width = width, .height = height };
}

int main() {

}
