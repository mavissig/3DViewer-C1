/**
 * @file matrix_s.h
 *
 * @author Egor Kondratov(https://github.com/mavissig)
 * @brief В этом файле объявлены функции и структуры для работы с матрицами
 */
#ifndef C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_MATRIX_S_H_
#define C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_MATRIX_S_H_

#include <stdlib.h>

/**
 * @brief Представляет матрицу.
 *
 * @param matrix Данные матрицы
 * @param rows Количество строк в матрице
 * @param columns Количество столбцов в матрице
 *
 */
typedef struct matrix_struct {
  float** matrix;
  int rows;
  int columns;
} matrix_s;

/**
 * @brief Создает новую матрицу с заданным количеством строк и столбцов.
 *
 * @param rows Количество строк в матрице.
 * @param columns Количество столбцов в матрице.
 * @param result Созданная матрица.
 */
void create_matrix(const int rows, const int columns, matrix_s* result);

/**
 * @brief Удаляет матрицу, освобождая выделенную память.
 *
 * @param A Матрица для удаления.
 */
void remove_matrix(matrix_s* A);

/**
 * @brief Умножает две матрицы и сохраняет результат в третьей матрице.
 *
 * @param A Первая матрица.
 * @param B Вторая матрица.
 * @param result Матрица для сохранения результата умножения.
 */
void mult_matrix(const matrix_s* A, const matrix_s* B, matrix_s* result);

#endif  // C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_MATRIX_S_H_
