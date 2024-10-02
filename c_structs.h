#include <stdbool.h>
#include <pthread.h>

#ifndef C_STRUCTS_H
#define C_STRUCTS_H

  //####################
  // SHARED
  //####################
  typedef char* (*ToStringFn)(void* const);
  typedef void (*FreeFn)(void** const);


  //####################
  // ARRAY
  //####################
  typedef struct Array {
    pthread_mutex_t lock;
    int capacity;
    int size;
    void** elements;
  } Array;

  Array* array_new(int capacity);
  int array_clear(Array* const array, FreeFn const free_element);
  int array_free(Array** const array, FreeFn const free_element);

  int array_append(Array* const array, void* const element);
  int array_prepend(Array* const array, void* const element);
  int array_insert(Array* const array, int index, void* const element);

  void* array_get(Array* const array, int index);
  void* array_remove(Array* const array, int index);

  typedef void (*ArrayEachFn)(void* const);
  void array_for_each(Array* const array, ArrayEachFn const fn);

  typedef void* (*ArrayMapFn)(void* const);
  Array* array_map(Array* const array, ArrayMapFn const fn);

  char* array_to_string(Array* const array, ToStringFn const to_string);

  bool array_index_valid(Array* const array, int index);
  bool array_has_capacity(Array* const array);


  //####################
  // MATRIX
  //####################
  typedef struct Position {
    int row;
    int column;
  } Position;

  Position position_new(int row, int column);
  char* position_to_string(Position* position);

  typedef struct Matrix {
    pthread_mutex_t lock;
    int rows;
    int columns;
    int capacity;
    int size;
    void** elements;
  } Matrix;

  Matrix* matrix_new(int rows, int columns);
  int matrix_clear(Matrix* const matrix, FreeFn const free_element);
  int matrix_free(Matrix** const matrix, FreeFn const free_element);

  int matrix_insert(Matrix* const matrix, Position* const position, void* const element);

  void* matrix_get(Matrix* const matrix, Position* const position);
  void* matrix_remove(Matrix* const matrix, Position* const position);

  typedef void (*MatrixEachFn)(void* const);
  void matrix_for_each(Matrix* const matrix, MatrixEachFn const fn);

  typedef void* (*MatrixMapFn)(void* const);
  Matrix* matrix_map(Matrix* const matrix, MatrixMapFn const fn);

  char* matrix_to_string(Matrix* const matrix, ToStringFn const to_string);

  bool matrix_position_valid(Matrix* const matrix, Position* const position);
  bool matrix_has_capacity(Matrix* const matrix);

#endif
