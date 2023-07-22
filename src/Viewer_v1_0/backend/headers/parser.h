/**
 * @file parser.h
 *
 * @author Egor Kondratov(https://github.com/mavissig)
 * @brief В этом файле объявлены функции для парсинга данных.
 */
#ifndef C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_PARSER_H_
#define C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_PARSER_H_

#include <float.h>
#include <stdio.h>

#include "parser_s.h"

/**
 * @brief Парсит данные из файла.
 *
 * @details Функция читает данные из указанного файла и заполняет структуры
 * vertex и surface соответствующими значениями.
 *
 * @param file Указатель на файл для чтения данных.
 * @param vertex Указатель на структуру vertex_s, в которую будут записаны
 * вершины модели.
 * @param surface Указатель на структуру surface_s, в которую будут записаны
 * поверхности модели.
 */
void parser(FILE* file, vertex_s* vertex, surface_s* surface, float* maxX,
            float* maxY, float* maxZ);

#endif  // C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_PARSER_H_
