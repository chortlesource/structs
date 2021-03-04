////////////////////////////////////////////////////////////////////////////
//
// structs - heap.c
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
// HEAP FUNCTION IMPLEMENTATION
//

struct heap* heap_create(int type) {
  struct heap *heap = malloc(sizeof(struct heap));

  if(heap) {
    heap->array = array_create(0);
    heap->type  = type;

    if(!heap->array) {
      free(heap);
      heap = NULL;
    }
  }

  return heap;
}


void heap_free(struct heap *heap) {
  if(heap) {
    if(heap->array) {
      if(heap_size(heap)) {
        // Free all owned data with elem structs
        for(size_t i = 0; i < heap->array->count; i++) {
          struct elem *elem = NULL;
          elem = heap->array->data[i];

          if(elem != NULL)
            free(elem->data);
        }
      }
      // Free the array struct
      array_free(heap->array);
    }
    // Free the heap struct
    free(heap);
  }
}


int heap_add(struct heap *heap, void *data, size_t value, size_t size) { return 0; }


int heap_swap(struct heap *heap, size_t elem1, size_t elem2) {
  int rvalue = 0;

  if(heap) {
    if(heap_size(heap) > elem1 && heap_size(heap) > elem2) {
      void *data =
      rvalue = H_OK
    }
  }

  return rvalue;
}


int heap_heapify_up(struct heap *heap, size_t index) { return 0; }
int heap_heapify_down(struct heap *heap, size_t index) { return 0; }
int heap_for_each(struct heap *heap, heap_func func) { return 0; }
void* heap_pop(struct heap *heap) { return NULL; }


size_t       heap_size(struct heap *heap) {
  size_t rvalue = 0;

  if(heap) {
    if(heap->array) {
      rvalue = array_size(heap->array);
    }
  }

  return rvalue;
}
