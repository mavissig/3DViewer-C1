#include "../headers/calculate.h"

/**
 * @brief Афинные преобразования
 *
 * @param vertex структура вершин
 * @param calculate_matrix матрица афинных преобразований
 */
static void calculate(vertex_s* vertex, const matrix_s* calculate_matrix);
static void fastSort(int* array);

void move_model(vertex_s* vertex, const float offsetX, const float offsetY,
                const float offsetZ) {
  matrix_s move_matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &move_matrix);
  move_matrix.matrix[0][0] = move_matrix.matrix[1][1] =
      move_matrix.matrix[2][2] = move_matrix.matrix[3][3] = 1;
  move_matrix.matrix[0][3] = offsetX;
  move_matrix.matrix[1][3] = offsetY;
  move_matrix.matrix[2][3] = offsetZ;
  calculate(vertex, &move_matrix);
  remove_matrix(&move_matrix);
}

void scale_model(vertex_s* vertex, const float scaleX, const float scaleY,
                 const float scaleZ) {
  matrix_s scale_matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &scale_matrix);
  scale_matrix.matrix[0][0] = scaleX;
  scale_matrix.matrix[1][1] = scaleY;
  scale_matrix.matrix[2][2] = scaleZ;
  scale_matrix.matrix[3][3] = 1;
  calculate(vertex, &scale_matrix);
  remove_matrix(&scale_matrix);
}

void rotate_model_X(vertex_s* vertex, float angle) {
  angle = fmodf(angle, 360) * M_PI / 180;
  matrix_s rotate_matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &rotate_matrix);
  rotate_matrix.matrix[0][0] = rotate_matrix.matrix[3][3] = 1;
  rotate_matrix.matrix[1][1] = rotate_matrix.matrix[2][2] = cos(angle);
  rotate_matrix.matrix[1][2] = -sin(angle);
  rotate_matrix.matrix[2][1] = sin(angle);
  calculate(vertex, &rotate_matrix);
  remove_matrix(&rotate_matrix);
}

void rotate_model_Y(vertex_s* vertex, float angle) {
  angle = fmodf(angle, 360) * M_PI / 180;
  matrix_s rotate_matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &rotate_matrix);
  rotate_matrix.matrix[1][1] = rotate_matrix.matrix[3][3] = 1;
  rotate_matrix.matrix[0][0] = rotate_matrix.matrix[2][2] = cos(angle);
  rotate_matrix.matrix[0][2] = sin(angle);
  rotate_matrix.matrix[2][0] = -sin(angle);
  calculate(vertex, &rotate_matrix);
  remove_matrix(&rotate_matrix);
}

void rotate_model_Z(vertex_s* vertex, float angle) {
  angle = fmodf(angle, 360) * M_PI / 180;
  matrix_s rotate_matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &rotate_matrix);
  rotate_matrix.matrix[2][2] = rotate_matrix.matrix[3][3] = 1;
  rotate_matrix.matrix[0][0] = rotate_matrix.matrix[1][1] = cos(angle);
  rotate_matrix.matrix[0][1] = -sin(angle);
  rotate_matrix.matrix[1][0] = sin(angle);
  calculate(vertex, &rotate_matrix);
  remove_matrix(&rotate_matrix);
}

int get_amount_of_edges(const surface_s* surface, const size_t size) {
  int amount_of_edges = 0;
  if (surface != NULL) {
    int* points = (int*)calloc(3, sizeof(int));
    int* nodes = (int*)calloc(size + (size - 1) * 11, sizeof(int));
    for (size_t i = 0; i < surface->size; i += 3) {
      points[0] = surface->points[i];
      points[1] = surface->points[i + 1];
      points[2] = surface->points[i + 2];
      fastSort(points);
      const int node1 = points[0] * 10 + points[1];
      const int node2 = points[0] * 10 + points[2];
      const int node3 = points[1] * 10 + points[2];

      int ftrue = (0 == nodes[node1]);
      int ffalse = ftrue ^ 1;
      nodes[node1] = ftrue + ffalse * nodes[node1];
      amount_of_edges += ftrue;

      ftrue = (0 == nodes[node2]);
      ffalse = ftrue ^ 1;
      nodes[node2] = ftrue + ffalse * nodes[node2];
      amount_of_edges += ftrue;

      ftrue = (0 == nodes[node3]);
      ffalse = ftrue ^ 1;
      nodes[node3] = ftrue + ffalse * nodes[node3];
      amount_of_edges += ftrue;
    }
    free(points);
    points = NULL;
    free(nodes);
    nodes = NULL;
  }
  return amount_of_edges;
}

static void calculate(vertex_s* vertex, const matrix_s* calculate_matrix) {
  matrix_s start_vector = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 1, &start_vector);
  start_vector.matrix[3][0] = 1;
  matrix_s result = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 1, &result);
  for (size_t i = 0; i < vertex->size; i += 3) {
    start_vector.matrix[0][0] = vertex->coords[i];
    start_vector.matrix[1][0] = vertex->coords[i + 1];
    start_vector.matrix[2][0] = vertex->coords[i + 2];
    mult_matrix(calculate_matrix, &start_vector, &result);
    vertex->coords[i] = result.matrix[0][0];
    vertex->coords[i + 1] = result.matrix[1][0];
    vertex->coords[i + 2] = result.matrix[2][0];
    result.matrix[0][0] = result.matrix[1][0] = result.matrix[2][0] =
        result.matrix[3][0] = 0;
  }
  remove_matrix(&start_vector);
  remove_matrix(&result);
}

static inline void fastSort(int* array) {
  int temp = array[0];
  int ftrue = (temp > array[1]);
  int ffalse = ftrue ^ 1;
  array[0] = ftrue * array[1] + ffalse * array[0];
  array[1] = ftrue * temp + ffalse * array[1];

  ftrue = (array[1] > array[2]);
  ffalse = ftrue ^ 1;
  temp = ftrue * array[1] + ffalse * temp;
  array[1] = ftrue * array[2] + ffalse * array[1];
  array[2] = ftrue * temp + ffalse * array[2];

  temp = array[0];
  ftrue = (temp > array[1]);
  ffalse = ftrue ^ 1;
  array[0] = ftrue * array[1] + ffalse * array[0];
  array[1] = ftrue * temp + ffalse * array[1];
}
