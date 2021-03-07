////////////////////////////////////////////////////////////////////////////
//
// structs - heap.h
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

#ifndef _HEAP_H
#define _HEAP_H


/////////////////////////////////////////////////////////////
// HEAP DESCRIPTION
//
// The heap struct is a data structure that takes the form of a binary
// tree. The heap structure can specify either min or max heap by use
// of the heap_e enumeration.
//
// For min-heaps to pop the parent node guarentee's a value either
// smaller than or equal to it's children. For max-heaps the value is
// either greater than or equal to it's children.
//
// The min-heap uses the array struct and as such will own data
// contained within until it is popped from the heap. When items are
// popped from the heap they should be subsequently freed.


/////////////////////////////////////////////////////////////
// HEAP TYPES
//

enum heap_e {
  H_ERR = 0, H_OK, MINHEAP, MAXHEAP
};

struct elem {
  void   *data;
  size_t size;
  size_t value;
};

struct heap {
  struct array *array;
  enum heap_e type;
};

typedef void(*heap_func)(void*);


/////////////////////////////////////////////////////////////
// HEAP FUNCTION DECLARATION
//

// Functions to create and free memory allocated to heaps
struct heap* heap_create(int type);
void         heap_free(struct heap *heap);
void         heap_free_elem(struct elem *elem);

// Functions to add items to and manipulate heaps
int          heap_add(struct heap *heap, void *data, size_t value, size_t size);
int          heap_swap(struct heap *heap, size_t elem1, size_t elem2);
void         heap_heapify_up(struct heap *heap, size_t index);
void         heap_heapify_down(struct heap *heap, size_t index);
void         heap_for_each(struct heap *heap, heap_func func);
void         heap_print(struct heap *heap);

// Functions to obtain values from the heap
struct elem* heap_pop(struct heap *heap);
size_t       heap_get_value(struct heap *heap, size_t index);
size_t       heap_size(struct heap *heap);


#endif // _HEAP_H
