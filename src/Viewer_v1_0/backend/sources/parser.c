#include "../headers/parser.h"

static void recallocVertex(vertex_s *vertex);
static void recallocSurface(surface_s *surface);

void parser(FILE *file, vertex_s *vertex, surface_s *surface, float *maxX,
            float *maxY, float *maxZ) {
  char buf[256] = {'\0'};
  float x = -__FLT_MAX__, y = -__FLT_MAX__, z = -__FLT_MAX__;
  int ftrueMax = 0, ffalseMax = 0;
  while (fgets(buf, sizeof(buf), file) != NULL) {
    if ('v' == buf[0] && ' ' == buf[1]) {
      if (vertex->size == vertex->capacity) {
        recallocVertex(vertex);
      }
      float w = 0.0;
      sscanf(buf + 2, "%f %f %f %f", &vertex->coords[vertex->size],
             &vertex->coords[vertex->size + 1],
             &vertex->coords[vertex->size + 2], &w);
      const int ftrue = (0 == w);
      w = ftrue + w * (float)(ftrue ^ 1);
      vertex->coords[vertex->size] /= w;
      vertex->coords[vertex->size + 1] /= w;
      vertex->coords[vertex->size + 2] /= w;

      ftrueMax = (vertex->coords[vertex->size] > x);
      ffalseMax = ftrueMax ^ 1;
      x = (float)ftrueMax * vertex->coords[vertex->size] + (float)ffalseMax * x;

      ftrueMax = (vertex->coords[vertex->size + 1] > y);
      ffalseMax = ftrueMax ^ 1;
      y = (float)ftrueMax * vertex->coords[vertex->size + 1] +
          (float)ffalseMax * y;

      ftrueMax = (vertex->coords[vertex->size + 2] > z);
      ffalseMax = ftrueMax ^ 1;
      z = (float)ftrueMax * vertex->coords[vertex->size + 2] +
          (float)ffalseMax * z;

      vertex->size += 3;
    }

    if ('f' == buf[0] && ' ' == buf[1]) {
      if (surface->size == surface->capacity) {
        recallocSurface(surface);
      }
      sscanf(buf + 2, "%u %u %u", &surface->points[surface->size],
             &surface->points[surface->size + 1],
             &surface->points[surface->size + 2]);
      --surface->points[surface->size];
      --surface->points[surface->size + 1];
      --surface->points[surface->size + 2];
      surface->size += 3;
    }
  }
  *maxX = x;
  *maxY = y;
  *maxZ = z;
}

static inline void recallocVertex(vertex_s *vertex) {
  vertex->capacity *= 2;
  vertex->coords =
      (float *)realloc(vertex->coords, sizeof(float) * vertex->capacity);
  vertex->coords[vertex->size] = 0.0f;
  vertex->coords[vertex->size + 1] = 0.0f;
  vertex->coords[vertex->size + 2] = 0.0f;
}

static void recallocSurface(surface_s *surface) {
  surface->capacity *= 2;
  surface->points = (unsigned int *)realloc(
      surface->points, sizeof(unsigned int) * surface->capacity);
  surface->points[surface->size] = 0;
  surface->points[surface->size + 1] = 0;
  surface->points[surface->size + 2] = 0;
}