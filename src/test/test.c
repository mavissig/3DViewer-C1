#include <check.h>
#include <stdio.h>
#include <time.h>

#include "../Viewer_v1_0/backend/headers/calculate.h"
#include "../Viewer_v1_0/backend/headers/parser.h"
static double wtime();
static double getResultTime(const double begin, const double end);

START_TEST(test_parsing_1) {
  const double start = wtime();
  const char *filename = "test/test_1.txt";

  FILE *file;
  file = fopen(filename, "r");
  if (NULL != file) {
    vertex_s vertex = {.coords = NULL, .size = 0, .capacity = 3};
    vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
    surface_s surface = {.points = NULL, .size = 0, .capacity = 3};
    surface.points =
        (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
    float x = 0, y = 0, z = 0;
    parser(file, &vertex, &surface, &z, &x, &y);

    free(vertex.coords);
    vertex.coords = NULL;

    free(surface.points);
    surface.points = NULL;
    fclose(file);
  }
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed test_parsing_1 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(test_parsing_2) {
  const char *filename = "test/test_2.txt";
  const double start = wtime();

  FILE *file;
  file = fopen(filename, "r");
  if (NULL != file) {
    vertex_s vertex = {.coords = NULL, .size = 0, .capacity = 3};
    vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
    surface_s surface = {.points = NULL, .size = 0, .capacity = 3};
    surface.points =
        (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));

    float x = 0, y = 0, z = 0;
    parser(file, &vertex, &surface, &z, &x, &y);

    free(vertex.coords);
    vertex.coords = NULL;

    free(surface.points);
    surface.points = NULL;
    fclose(file);
  }
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed test_parsing_2 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(test_parsing_3) {
  const char *filename = "test/test_3.txt";
  const double start = wtime();

  FILE *file;
  file = fopen(filename, "r");
  if (NULL != file) {
    vertex_s vertex = {.coords = NULL, .size = 0, .capacity = 3};
    vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
    surface_s surface = {.points = NULL, .size = 0, .capacity = 3};
    surface.points =
        (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
    float x = 0, y = 0, z = 0;
    parser(file, &vertex, &surface, &z, &x, &y);

    free(vertex.coords);
    vertex.coords = NULL;

    free(surface.points);
    surface.points = NULL;
    fclose(file);
  }
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed test_parsing_3 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(test_parsing_4) {
  const double start = wtime();
  const char *filename = "test/test_4.txt";

  FILE *file;
  file = fopen(filename, "r");

  if (NULL != file) {
    vertex_s vertex = {.coords = NULL, .size = 0, .capacity = 3};
    vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
    surface_s surface = {.points = NULL, .size = 0, .capacity = 3};
    surface.points =
        (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
    float x = 0, y = 0, z = 0;
    parser(file, &vertex, &surface, &z, &x, &y);

    free(vertex.coords);
    vertex.coords = NULL;

    free(surface.points);
    surface.points = NULL;
    fclose(file);
  }
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed test_parsing_1 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(matrix_test_1) {
  const double start = wtime();
  matrix_s matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(10, 10, &matrix);
  ck_assert_int_eq(matrix.rows + matrix.columns, 20);
  remove_matrix(&matrix);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  printf("Elapsed matrix_test_1 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(matrix_test_2) {
  const double start = wtime();
  matrix_s matrix = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &matrix);
  matrix_s matrix_2 = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &matrix_2);
  ck_assert_int_eq(matrix.rows + matrix.columns, 8);
  ck_assert_int_eq(matrix_2.rows + matrix_2.columns, 8);
  matrix_s result = {.columns = 0, .rows = 0, .matrix = NULL};
  create_matrix(4, 4, &result);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      matrix.matrix[i][j] = matrix_2.matrix[i][j] = j + 1;
    }
  }
  mult_matrix(&matrix, &matrix_2, &result);
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      ck_assert_int_eq(result.matrix[i][j], (j + 1) * 10);
    }
  }
  remove_matrix(&matrix_2);
  remove_matrix(&matrix);
  remove_matrix(&result);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  printf("Elapsed matrix_test_2 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(calculate_test_1) {
  const double start = wtime();
  FILE *file = fopen("test/test_4.txt", "r");
  ck_assert_ptr_nonnull(file);
  surface_s surface = {.capacity = 3, .size = 0, .points = NULL};
  vertex_s vertex = {.capacity = 3, .size = 0, .coords = NULL};
  vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
  surface.points =
      (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
  float x = 0, y = 0, z = 0;
  parser(file, &vertex, &surface, &x, &y, &z);
  ck_assert_float_eq(x, 28.1348);
  ck_assert_float_eq(y, 50.0000);
  ck_assert_float_eq(z, 26.1574);
  ck_assert_int_eq(18, get_amount_of_edges(&surface, vertex.size / 3));
  free(vertex.coords);
  vertex.coords = NULL;

  free(surface.points);
  surface.points = NULL;
  fclose(file);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed calculate_test_1 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(calculate_test_2) {
  const double start = wtime();
  FILE *file = fopen("test/test_4.txt", "r");
  ck_assert_ptr_nonnull(file);
  surface_s surface = {.capacity = 3, .size = 0, .points = NULL};
  vertex_s vertex = {.capacity = 3, .size = 0, .coords = NULL};
  vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
  surface.points =
      (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
  float x = 0, y = 0, z = 0;
  parser(file, &vertex, &surface, &x, &y, &z);
  vertex_s copy = {
      .size = vertex.size, .capacity = vertex.capacity, .coords = NULL};
  copy.coords = (float *)calloc(copy.size, sizeof(float));
  for (size_t i = 0; i < copy.size; ++i) {
    copy.coords[i] = vertex.coords[i];
  }
  move_model(&vertex, 100, 100, 100);
  for (size_t i = 0; i < copy.size; ++i) {
    ck_assert_float_eq(vertex.coords[i] - copy.coords[i], 100);
  }
  free(vertex.coords);
  vertex.coords = NULL;

  free(surface.points);
  surface.points = NULL;
  fclose(file);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed calculate_test_2 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(calculate_test_3) {
  const double start = wtime();
  FILE *file = fopen("test/test_4.txt", "r");
  ck_assert_ptr_nonnull(file);
  surface_s surface = {.capacity = 3, .size = 0, .points = NULL};
  vertex_s vertex = {.capacity = 3, .size = 0, .coords = NULL};
  vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
  surface.points =
      (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
  float x = 0, y = 0, z = 0;
  parser(file, &vertex, &surface, &x, &y, &z);
  vertex_s copy = {
      .size = vertex.size, .capacity = vertex.capacity, .coords = NULL};
  copy.coords = (float *)calloc(copy.size, sizeof(float));
  for (size_t i = 0; i < copy.size; ++i) {
    copy.coords[i] = vertex.coords[i];
  }
  scale_model(&vertex, 100, 100, 100);
  for (size_t i = 0; i < copy.size; ++i) {
    ck_assert_float_eq(vertex.coords[i] / 100, copy.coords[i]);
  }
  free(vertex.coords);
  vertex.coords = NULL;

  free(surface.points);
  surface.points = NULL;
  fclose(file);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed calculate_test_3 time:%f seconds\n", resultTime);
}
END_TEST

START_TEST(calculate_test_4) {
  const double start = wtime();
  FILE *file = fopen("test/test_4.txt", "r");
  ck_assert_ptr_nonnull(file);
  surface_s surface = {.capacity = 3, .size = 0, .points = NULL};
  vertex_s vertex = {.capacity = 3, .size = 0, .coords = NULL};
  vertex.coords = (float *)calloc(vertex.capacity, sizeof(float));
  surface.points =
      (unsigned int *)calloc(surface.capacity, sizeof(unsigned int));
  float x = 0, y = 0, z = 0;
  parser(file, &vertex, &surface, &x, &y, &z);
  vertex_s copy = {
      .size = vertex.size, .capacity = vertex.capacity, .coords = NULL};
  copy.coords = (float *)calloc(copy.size, sizeof(float));
  for (size_t i = 0; i < copy.size; ++i) {
    copy.coords[i] = vertex.coords[i];
  }
  rotate_model_X(&vertex, 90);
  rotate_model_Y(&vertex, 90);
  rotate_model_Z(&vertex, 90);
  for (size_t i = 0; i < copy.size; ++i) {
    ck_assert_float_ge(fabs(vertex.coords[i] / copy.coords[i] - 1.09044), 1e-6);
  }
  free(vertex.coords);
  vertex.coords = NULL;

  free(surface.points);
  surface.points = NULL;
  fclose(file);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  ck_assert_double_le(resultTime, 0.5);
  printf("Elapsed calculate_test_4 time:%f seconds\n", resultTime);
}
END_TEST

int main() {
  const double timeout = 10;
  Suite *suite = suite_create("Viewer backend tests");
  TCase *tests = tcase_create("Tests");
  SRunner *runner = srunner_create(suite);
  suite_add_tcase(suite, tests);
  srunner_set_fork_status(runner, CK_FORK);
  tcase_set_timeout(tests, timeout);
  srunner_set_log(runner, "logs/test_log.txt");

  tcase_add_test(tests, test_parsing_1);
  tcase_add_test(tests, test_parsing_2);
  tcase_add_test(tests, test_parsing_3);
  tcase_add_test(tests, test_parsing_4);
  tcase_add_test(tests, matrix_test_1);
  tcase_add_test(tests, matrix_test_2);
  tcase_add_test(tests, calculate_test_1);
  tcase_add_test(tests, calculate_test_2);
  tcase_add_test(tests, calculate_test_3);
  tcase_add_test(tests, calculate_test_4);

  printf("__________________Viewer backend tests________________\n");
  const double start = wtime();
  srunner_run_all(runner, CK_ENV);
  const double end = wtime();
  const double resultTime = getResultTime(start, end);
  printf("Elapsed Viewer backend tests time:%f seconds\n", resultTime);
  printf("______________________________________________________\n");
  srunner_free(runner);
  return 0;
}

static double wtime() {
  double time = 0;
#ifdef CLOCK_MONOTONIC
  struct timespec ts = {.tv_nsec = 0, .tv_sec = 0};
  clock_gettime(CLOCK_MONOTONIC, &ts);
  time = ts.tv_sec + ts.tv_nsec * 1E-9;
#else
  time = clock();
#endif
  return time;
}

static double getResultTime(const double begin, const double end) {
  double time = 0;
#ifdef CLOCK_MONOTONIC
  time = end - begin;
#else
  time = (end - begin) / CLOCKS_PER_SEC;
#endif
  return time;
}
