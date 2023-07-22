/**
 * @file calculate.h
 *
 * @author Egor Kondratov(https://github.com/mavissig)
 * @brief В этом файле объявлены функции для калькуляции и работы с объектами
 */
#ifndef C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_CALCULATE_H_
#define C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_CALCULATE_H_

#include <math.h>

#include "matrix_s.h"
#include "parser_s.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * @brief Применяет сдвиг модели по осям X, Y и Z.
 *
 * @param vertex Указатель на структуру, содержащую вершины модели.
 * @param offsetX Величина сдвига по оси X.
 * @param offsetY Величина сдвига по оси Y.
 * @param offsetZ Величина сдвига по оси Z.
 */
void move_model(vertex_s* vertex, const float offsetX, const float offsetY,
                const float offsetZ);

/**
 * @brief Применяет масштабирование модели по осям X, Y и Z.
 *
 * @param vertex Указатель на структуру, содержащую вершины модели.
 * @param scaleX Масштабный коэффициент по оси X.
 * @param scaleY Масштабный коэффициент по оси Y.
 * @param scaleZ Масштабный коэффициент по оси Z.
 */
void scale_model(vertex_s* vertex, const float scaleX, const float scaleY,
                 const float scaleZ);

/**
 * @brief Поворачивает модель вокруг оси X на заданный угол.
 *
 * @param vertex Указатель на структуру, содержащую вершины модели
 * @param angle Угол поворота в градусах.
 */
void rotate_model_X(vertex_s* vertex, float angle);

/**
 * @brief Поворачивает модель вокруг оси Y на заданный угол.
 *
 * @param vertex Указатель на структуру, содержащую вершины модели
 * @param angle Угол поворота в градусах.
 */
void rotate_model_Y(vertex_s* vertex, float angle);

/**
 * @brief Поворачивает модель вокруг оси Z на заданный угол.
 *
 * @param vertex Указатель на структуру, содержащую вершины модели
 * @param angle Угол поворота в градусах.
 */
void rotate_model_Z(vertex_s* vertex, float angle);

int get_amount_of_edges(const surface_s* surface, const size_t size);

#endif  // C8_3DVIEWER_V1_0_1_SRC_VIEWER_V1_0_BACKEND_HEADERS_CALCULATE_H_
