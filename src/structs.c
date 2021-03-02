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


/////////////////////////////////////////////////////////////
// TEST FUNCTION DECLARATIONS
//

void array_tests() {
  printf("|---------- ARRAY STRUCT TESTS ----------|\n");
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
    printf("TEST%u: Test get invalid\t[SUCCESS]\n", ++t);
  else
    printf("TEST%u: Test get invalid\t[FAILURE]\n", ++t);

  // Test the freeing of dynamically added memory
  array_free(array1);
  array_free(array2);
  array_free(array3);
}


/////////////////////////////////////////////////////////////
// MAIN FUNCTION IMPLEMENTATION
//

int main(const int argc, const char *argv[]) {
  // Function to run the array struct tests
  array_tests();

  return 0;
}
