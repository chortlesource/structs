////////////////////////////////////////////////////////////////////////////
//
// structs - structs.c
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


static void array_print(struct array *array, size_t row_len) {
  if(array) {
    if(array->data != NULL) {
      size_t row = 0;
      printf("\t%zu:", row++);

      for(size_t i = 0; i < array->count; i++) {
        // Print out each row on a new line
        if(i == row * row_len)
          printf("|\n\t%zu:", row++);

        printf("|%s", (char*)array_get(array, i));
      }
      printf("|\n");
    }
  }
}


static void array_print_int(struct array *array, size_t row_len) {
  if(array) {
    if(array->data != NULL) {
      size_t row = 0;
      printf("\t%zu:", row++);

      for(size_t i = 0; i < array->count; i++) {
        // Print out each row on a new line
        if(i == row * row_len)
          printf("|\n\t%zu:", row++);

        printf("|%d", (*(int*)array_get(array, i)));
      }
      printf("|\n");
    }
  }
}


static void heap_print_char(struct heap *heap, size_t row_len) {
  if(heap) {
    size_t size = heap_size(heap);

    if(size) {
      size_t row  = 0;

      printf("\t%zu:", row++);

      for(size_t i = 0; i < size; i++) {
        if(i == row * row_len)
          printf("|\n\t%zu:", row++);

        struct elem *elem = (struct elem*)array_get(heap->array, i);
        printf("|%c[%zu][%zu]", (*(char*)elem->data), elem->value, elem->size);
      }
      printf("|\n");
    }
  }
}


static void heap_print_string(struct heap *heap, size_t row_len) {
  if(heap) {
    size_t size = heap_size(heap);

    if(size) {
      size_t row  = 0;

      printf("\t%zu:", row++);

      for(size_t i = 0; i < size; i++) {
        if(i == row * row_len)
          printf("|\n\t%zu:", row++);

        struct elem *elem = (struct elem*)array_get(heap->array, i);
        printf("|%s[%zu][%zu]", (char*)elem->data, elem->value, elem->size);
      }
      printf("|\n");
    }
  }
}


/////////////////////////////////////////////////////////////
// TEST FUNCTION DECLARATIONS
//

void array_tests() {
  printf("|---------- ARRAY STRUCT TEST ----------|\n");
  unsigned int t = 0;

  // Test the allocation of the array structure
  struct array *array1 = array_create(0);
  struct array *array2 = array_create(10);

  // Test the ability to resize both arrays
  if(array_resize(array1, 10))
    printf("TEST%u: Resized empty array\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Resized empty array\t[FAILURE]\n", ++t);

  if(array_resize(array2, 10))
    printf("TEST%u: Resized allocated array\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Resized allocated array\t[FAILURE]\n", ++t);

  // Test the ability to append to the array
  char temp1[11] = "0123456789\0";
  if(array_append(array1, temp1, sizeof(char) * 11))
    printf("TEST%u: Test append from stack\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Test append from stack\t[FAILURE]\n", ++t);

  array_print(array1, 1); // Print array to verify added

  {
    char temp2[11] = "abcdefghij\0";
    array_append(array2, temp1, sizeof(char) * 11);
    array_append(array2, temp2, sizeof(char) * 11);
  }

  printf("TEST%u: Test append scope\n", ++t);
  array_print(array2, 1);

  // Test access to the front and back of the array
  printf("TEST%u: Test print front\n", ++t);
  printf("\t%s\n", (char*)array_front(array2));

  printf("TEST%u: Test print back\n", ++t);
  printf("\t%s\n", (char*)array_back(array2));

  // Test the ability to set a particular pos of the array
  char temp3[11] = "abcdefghij\0";
  printf("TEST%u: Test set position 3\n", ++t);
  struct array *array3 = array_create(0);
  for(int i = 0; i < 5; i++)
    array_append(array3, temp1, sizeof(char) * 11);
  array_set(array3, 3, temp3, sizeof(char) * 11);
  array_print(array3, 1);

  // Test that array get returns null if invalid
  if(array_get(array3, 6) == NULL)
    printf("TEST%u: Test get invalid item\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Test get invalid item\t[FAILURE]\n", ++t);

  // Test that the array can handle inserts
  struct array *array4 = array_create(0);
  int success = 0;

  for(int i = 0; i < 30; i++)
    success += array_insert(array4, 1, &i, sizeof(int));

  if(success)
    printf("TEST%u: Test insert 30 ints\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Test insert 30 ints\t[FAILURE]\n", ++t);

  array_print_int(array4, 10);

  // Test popping items from the array
  void *data = NULL;
  data = array_pop_beg(array4);

  if(data) {
    printf("TEST%u: Test pop front item\t[SUCCESS]\n", ++t);
    printf("\tItem was: %d\n", (*(int*)data));
    free(data);
  } else {
    printf("TEST%u: Test pop front item\t[FAILURE]\n", ++t);
  }

  data = NULL;
  data = array_pop_end(array4);

  if(data) {
    printf("TEST%u: Test pop last item\t[SUCCESS]\n", ++t);
    printf("\tItem was: %d\n", (*(int*)data));
    free(data);
  } else {
    printf("TEST%u: Test pop last item\t[FAILURE]\n", ++t);
  }

  data = NULL;
  data = array_pop_pos(array4, 10);

  if(data) {
    printf("TEST%u: Test pop 10th item\t[SUCCESS]\n", ++t);
    printf("\tItem was: %d\n", (*(int*)data));
    free(data);
  } else {
    printf("TEST%u: Test pop front item\t[FAILURE]\n", ++t);
  }

  array_print_int(array4, 10);

  // Test the freeing of dynamically added memory
  array_free(array1);
  array_free(array2);
  array_free(array3);
  array_free(array4);
}


void heap_tests() {
  printf("|---------- HEAP STRUCT TESTS ----------|\n");
  unsigned int t = 0;

  // Test the allocation of the array structure
  struct heap *heap1 = heap_create(MINHEAP);
  struct heap *heap2 = heap_create(MINHEAP);
  struct heap *heap3 = heap_create(MAXHEAP);

  // Test loop adding to the heap
  char temp1[11] = "0123456789\0";
  int rvalue = 0;

  for(size_t i = 0; i < 10; i++) {
    rvalue += heap_add(heap2, &temp1[i], i, sizeof(char));
    rvalue += heap_add(heap3, &temp1[i], i, sizeof(char));
  }

  if(rvalue == 20)
    printf("TEST%u: Loop add to heap\t\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Loop add to heap\t\t[FAILURE]\n", ++t);

  heap_print_char(heap2, 5);

  // Test scoped adding to the heap
  rvalue = 0;
  {
    char temp2[11] = "abcdefghij\0";
    rvalue = heap_add(heap1, temp2, 69, sizeof(char) * 11);
  }

  if(rvalue)
    printf("TEST%u: Scoped addition to heap\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Scoped addition to heap\t[FAILURE]\n", ++t);

  heap_print_string(heap1, 5);


  // Test print the heap
  printf("TEST%u: Print the minheap tree...\t\n", ++t);
  heap_print(heap2);
  printf("TEST%u: Print the maxheap tree...\t\n", ++t);
  heap_print(heap3);

  // Test pop from the min heap
  size_t heapsize = heap_size(heap2);
  size_t items = 0;

  printf("TEST%u: Testing minheap pop...\n", ++t);

  for(size_t i = 0; i < heapsize; i++) {
    struct elem *elem = heap_pop(heap2);

    if(elem) {
      printf("\tPOPPED: %c [%zu][%zu]\n", (*(char*)elem->data), elem->value, elem->size);

      heap_free_elem(elem);
      ++items;
    }
  }

  if(items == 10)
    printf("\t...[SUCCESS]\n");
  else
    printf("\t...[FAILURE]\n");


  printf("TEST%u: Testing maxheap pop...\n", ++t);

  for(size_t i = 0; i < heapsize; i++) {
    struct elem *elem = heap_pop(heap3);

    if(elem) {
      printf("\tPOPPED: %c [%zu][%zu]\n", (*(char*)elem->data), elem->value, elem->size);

      heap_free_elem(elem);
      ++items;
    }
  }

  if(items == 20)
    printf("\t...[SUCCESS]\n");
  else
    printf("\t...[FAILURE]\n");

  // Test invalid heap pop
  if(heap_pop(heap2) == NULL)
    printf("TEST%u: Invalid pop from heap\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Invalid pop from heap\t[FAILURE]\n", ++t);

  // Test the freeing of heap memory
  heap_free(heap1);
  heap_free(heap2);
  heap_free(heap3);
}


/////////////////////////////////////////////////////////////
// MAIN FUNCTION IMPLEMENTATION
//

int main(const int argc, const char *argv[]) {
  // Function to run the array struct tests
  array_tests();

  // Function to run the heap struct tests
  heap_tests();

  return 0;
}
