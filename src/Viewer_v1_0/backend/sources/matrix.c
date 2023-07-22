#include "../headers/matrix_s.h"

void create_matrix(const int rows, const int columns, matrix_s *result) {
  result->rows = rows;
  result->columns = columns;
  result->matrix = (float **)calloc(rows, sizeof(float *));
  for (int i = 0; i < rows; ++i) {
    result->matrix[i] = (float *)calloc(columns, sizeof(float));
  }
}

void remove_matrix(matrix_s *A) {
  for (int i = 0; i < A->rows; ++i) {
    free(A->matrix[i]);
    A->matrix[i] = NULL;
  }
  free(A->matrix);
  A->rows = 0;
  A->columns = 0;
  A->matrix = NULL;
}

void mult_matrix(const matrix_s *A, const matrix_s *B, matrix_s *result) {
  for (int i = 0; i < A->rows; ++i) {
    for (int k = 0; k < A->columns; ++k) {
      for (int j = 0; j < B->columns; ++j) {
        result->matrix[i][j] += A->matrix[i][k] * B->matrix[k][j];
      }
    }
  }
}
