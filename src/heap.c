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


void heap_free_elem(struct elem *elem) {
  if(elem) {
    free(elem->data);
    free(elem);
  }
}


int heap_add(struct heap *heap, void *data, size_t value, size_t size) {
  int rvalue = 0;

  if(heap) {
    struct elem *elem = malloc(sizeof(struct elem));

    if(elem) {
      // Copy memory accross to the heap
      void *copy = calloc(1, size);

      if(copy) {
        memcpy(copy, data, size);

        // Initialize variables
        elem->data  = copy;
        elem->size  = size;
        elem->value = value;

        rvalue = array_append(heap->array, elem, sizeof(struct elem));

        if(heap_size(heap) > 1)
          heap_heapify_up(heap, heap_size(heap) - 1);


      }
      free(elem);
    }
  }

  return rvalue;
}


int heap_swap(struct heap *heap, size_t elem1, size_t elem2) {
  int rvalue = 0;

  if(heap) {
    if(heap_size(heap) > elem1 && heap_size(heap) > elem2) {
      // Switch the pointers out using a temp pointer
      void *temp = array_get(heap->array, elem1);

      heap->array->data[elem1] = heap->array->data[elem2];
      heap->array->data[elem2] = temp;
      rvalue = H_OK;
    }
  }

  return rvalue;
}


void heap_heapify_up(struct heap *heap, size_t index) {
  if(heap) {
    size_t size         = heap_size(heap);
    size_t parent_index = (index - 1) / 2;

    if(index < size && parent_index < size) {
      size_t child_value  = heap_get_value(heap, index);
      size_t parent_value = heap_get_value(heap, parent_index);

      switch(heap->type) {
      case MINHEAP:
        {
          if(child_value < parent_value) {
            heap_swap(heap, index, parent_index);

            if(parent_index > 0)
              heap_heapify_up(heap, parent_index);
          }
        }
        break;
      case MAXHEAP:
        {
          if(parent_value < child_value) {
            heap_swap(heap, index, parent_index);

            if(parent_index > 0)
              heap_heapify_up(heap, parent_index);
          }
        }
        break;
      default:
        break;
      };
    }
  }
}


void heap_heapify_down(struct heap *heap, size_t index) {
  if(heap) {
    size_t size = heap_size(heap);

    if(size > 1) {
      // Obtain the parent node and calculate indexes
      size_t parent_value = heap_get_value(heap, index);
      size_t left_index   = (index * 2) + 1;
      size_t left_value   = heap_get_value(heap, left_index);
      size_t right_index  = left_index + 1;
      size_t right_value  = heap_get_value(heap, right_index);

      // Helper vars for recursion
      size_t swapped       = 0;
      size_t swapped_index = parent_value;

      switch(heap->type) {
      case MINHEAP:
        {
          if(right_index < size && right_value < left_value && parent_value > right_value) {
            heap_swap(heap, index, right_index);
            swapped_index = right_index;
            ++swapped;
          }
          else if(parent_value > left_value) {
            heap_swap(heap, index, left_index);
            swapped_index = left_index;
            ++swapped;
          }
        }
        break;
      case MAXHEAP:
        {
          if(right_index < size && right_value > left_value && parent_value < right_value) {
            heap_swap(heap, index, right_index);
            swapped_index = right_index;
            ++swapped;
          }
          else if(parent_value < size && parent_value < left_value) {
            heap_swap(heap, index, left_index);
            swapped_index = left_index;
            ++swapped;
          }
        }
        break;
      default:
        break;
      };

      if(swapped && ((swapped_index * 2) + 1) < size - 1)
        heap_heapify_down(heap, swapped_index);

    }
  }
}


void heap_for_each(struct heap *heap, heap_func func) {
  if(heap) {
    if(heap->array != NULL) {
      array_for_each(heap->array, (array_func)func);
    }
  }
}


static void heap_print_nodes(struct heap *heap, struct array *string, struct array *padding, char *pointer, size_t index, size_t right_sibling) {
  size_t size = heap_size(heap);

  if(size) {
    // Characters and Padding for convenience
    char *pad[5] = { "\n", "|  ", "   ", "├──", "└──" };

    array_append(string, pad[0], sizeof(char) * strlen(pad[0]) + 1);
    array_copy_from(string, padding, 0);
    array_append(string, pointer, sizeof(char) + strlen(pointer));

    // Calculate the indices
    size_t left_index   = (index * 2) + 1;
    size_t right_index  = left_index + 1;
    size_t node_value   = heap_get_value(heap, index);

    if(right_index > size) right_index = 0; // Set to 0 if the next
    if(left_index > size) left_index   = 0; // node is not set

    // Print our node value to string
    char value[256];
    snprintf(value, 256 * sizeof(char), "%zu", node_value);
    array_append(string, value, sizeof(char) * 256);

    // Construct the padding for this node
    struct array *newpadding = array_create(0);
    array_copy_from(newpadding, padding, 0);

    // Assign padding
    if(right_sibling)
      array_append(newpadding, pad[1], sizeof(char) * strlen(pad[1]) + 1);
    else
      array_append(newpadding, pad[2], sizeof(char) * strlen(pad[2]) + 1);

    // Recursively print nodes
    if(left_index)
      heap_print_nodes(heap, string, newpadding, (right_index) ? pad[3] : pad[4], left_index, right_index);
    if(right_index)
      heap_print_nodes(heap, string, newpadding, pad[4], right_index, right_index);

    array_free(newpadding);
  }
}


void heap_print(struct heap *heap) {
  if(heap) {
    size_t size = heap_size(heap);

    if(size) {
      struct array *string  = array_create(0);
      struct array *padding = array_create(0);

      // Add the relevant padding to our padding array
      char *pad[3]  = { "├──", "└──", "" };
      array_append(padding, pad[2], sizeof(char));

      // Add the value of the root node to our string
      size_t rootval = heap_get_value(heap, 0);
      char rv[256]   = "";
      snprintf(rv, 256 * sizeof(char), "%zu", rootval);
      array_append(string, rv, sizeof(char) * strnlen(rv, 256) + 1);

      // Recursively explore the left and right nodes
      if(size > 2)
        heap_print_nodes(heap, string, padding, (3 < size) ? pad[0] : pad[1], 1, 1);
      heap_print_nodes(heap, string, padding, pad[1], 2, 0);

      // Print the tree to the screen
      if(string)
        array_print_as_string(string);
      printf("\n");

      // Free memory for string and padding
      array_free(string);
      array_free(padding);
    }
  }
}


struct elem* heap_pop(struct heap *heap) {
  struct elem *data = NULL;

  if(heap) {
    size_t size = heap_size(heap);
    if(size) {
      // If there are othere elem move to end
      if(size > 1)
        heap_swap(heap, 0, size - 1);

      data = array_pop_end(heap->array); // Pop from end

      if((size - 1) > 1)
        heap_heapify_down(heap, 0);
    }
  }

  return data;
}


size_t       heap_get_value(struct heap *heap, size_t index) {
  size_t rvalue = 0;

  if(heap) {
    if(index < heap_size(heap)) {
      rvalue = ((struct elem*)array_get(heap->array, index))->value;
    }
  }

  return rvalue;
}


size_t heap_size(struct heap *heap) {
  size_t rvalue = 0;

  if(heap) {
    if(heap->array) {
      // Return the number of elem
      rvalue = array_size(heap->array);
    }
  }

  return rvalue;
}
