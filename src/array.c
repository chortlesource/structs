////////////////////////////////////////////////////////////////////////////
//
// structs - array.h
//
// Copyright (c) 2021 Christopher M. Short
//
// This file is part of structs.
//
// structs is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// structs is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with structs. If not, see <https://www.gnu.org/licenses/>.
//
////////////////////////////////////////////////////////////////////////////

#include "structs.h"


/////////////////////////////////////////////////////////////
// ARRAY FUNCTION IMPLEMENTATION
//

// Functions to create and free memory allocated to arrays
struct array* array_create(size_t size) {
  // Allocate memory to the array struct
  struct array *array = malloc(sizeof(struct array));

  if(array) {
    if(size) {
      // Allocate and initialize memory
      array->data = calloc(1, sizeof(void*) * size);

      if(array->data) {
        array->capacity = size;
        array->count    = 0;
      } else {
        // On fail returns null
        free(array);
        array = NULL;
      }
    } else {
      // If we initialize with 0 we don't allocate memory until resize
      array->data     = NULL;
      array->capacity = 0;
      array->count    = 0;
    }
  }

  return array;
}


void array_free(struct array *array) {
  if(array) {
    if(array->data != NULL) {

      if(array->count)
        array_for_each(array, free); // Free each element

      free(array->data); // Free array data
    }

    free(array); // Free our array struct
  }
}


int array_resize(struct array *array, size_t size) {
  int rvalue = A_ERR; // Fail by design

  if(array) {
    size_t newsize = 5;

    if(size)
      newsize = size;

    if(array->data != NULL) {
      // Realocate memory either the default of +5 or user specified
      void **tarray = realloc(array->data, (array->capacity + newsize) * sizeof(void*));

      array->capacity += newsize;
      array->data      = tarray;
      rvalue           = A_OK;
    } else {
      array->data      = malloc(sizeof(void*) * newsize);
      array->capacity += newsize;
      rvalue           = A_OK;
    }
  }

  return rvalue;
}


int array_insert(struct array *array, size_t pos, void *data, size_t size) { return 0; }


int array_append(struct array *array, void *data, size_t size) {
  int rvalue = A_ERR;

  if(array) {
    // If there is not room in the array resize
    if(array->data == NULL || array->capacity == array->count)
      array_resize(array, 0);

    // Copy the data and add to our array
    void *copy = calloc(1, size);
    memcpy(copy, data, size);
    array->data[array->count++] = copy;
    rvalue = A_OK;
  }

  return rvalue;
}


int array_set(struct array *array, size_t pos, void *data, size_t size) {
  int rvalue = A_ERR;

  if(array) {
    // Handle resizing the array if the pos is unavailable
    if(array->data != NULL && pos < array->count) {
      // Copy data to our array
      void *copy = calloc(1, size);
      memcpy(copy, data, size);

      if(array->data[pos] != NULL)
        free(array->data[pos]);

      array->data[pos] = copy;
    }
  }
  return rvalue;
}


void array_for_each(struct array *array, array_func func) {
  if(array) {
    if(array->data != NULL) {
      for(size_t i = 0; i < array->count; i++)
        func(array->data[i]);
    }
  }
}


void* array_front(struct array *array) {
  void *data = NULL;

  if(array) {
    if(array->data != NULL && array->count > 0) {
      data = array->data[0];
    }
  }

  return data;
}


void* array_back(struct array *array) {
  void *data = NULL;

  if(array) {
    if(array->data != NULL && array->count > 0) {
      data = array->data[array->count - 1];
    }
  }

  return data;
}


void* array_get(struct array *array, size_t pos) {
  void *data = NULL;

  if(array) {
    if(array->data != NULL && pos < array->count) {
      data = array->data[pos];
    }
  }

  return data;
}


void* array_pop_beg(struct array *array) { return NULL; }
void* array_pop_end(struct array *array) { return NULL; }
void* array_pop_pos(struct array *array, size_t pos) { return NULL; }
