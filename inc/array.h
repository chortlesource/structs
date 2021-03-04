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

#ifndef _ARRAY_H
#define _ARRAY_H


/////////////////////////////////////////////////////////////
// ARRAY DESCRIPTION
//
// The array struct is a dynamic array of a single type that can be
// populated by the user to store data on the heap. It is used where
// stack allocation cannot be utilised.
//
// The dynamic array takes ownership of memory meaning that it will
// copy data from the stack to heap and is responsible for freeing
// this memory when it is no longer required.


/////////////////////////////////////////////////////////////
// ARRAY TYPES
//

struct array {
  void   **data;
  size_t capacity;
  size_t count;
};

typedef void(*array_func)(void*);

enum array_e {
  A_ERR = 0, A_OK
};


/////////////////////////////////////////////////////////////
// ARRAY FUNCTION DECLARATION
//

// Functions to create and free memory allocated to arrays
struct array* array_create(size_t size);
void          array_free(struct array *array);

// Functions to add to, remove from and manipulate arrays
int           array_resize(struct array *array, size_t size);
int           array_insert(struct array *array, size_t pos, void *data, size_t size);
int           array_append(struct array *array, void *data, size_t size);
int           array_set(struct array *array, size_t pos, void *data, size_t size);
void          array_for_each(struct array *array, array_func func);

// Functions to obtain data from the array
void*         array_front(struct array *array);
void*         array_back(struct array *array);
void*         array_get(struct array *array, size_t pos);
void*         array_pop_beg(struct array *array);
void*         array_pop_end(struct array *array);
void*         array_pop_pos(struct array *array, size_t pos);
size_t        array_size(struct array *array);

#endif // _ARRAY_H
