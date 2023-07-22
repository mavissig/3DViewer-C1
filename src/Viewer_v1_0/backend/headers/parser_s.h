/**
 * @file parser_s.h
 *
 * @author Egor Kondratov(https://github.com/mavissig)
 * @brief В этом файле объявлены структуры для парсинга данных.
 */
#ifndef C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_PARSER_S_H_
#define C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_PARSER_S_H_

#include <stdlib.h>

/**
 * @struct vertex_s
 * @brief Представляет вершины модели.
 */
typedef struct vertex_s {
  float *coords;
  size_t size;
  size_t capacity;
} vertex_s;

/**
 * @struct surface_s
 * @brief Представляет поверхности модели.
 */
typedef struct surface_s {
  unsigned int *points;
  size_t size;
  size_t capacity;
} surface_s;

#endif  // C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_PARSER_S_H_