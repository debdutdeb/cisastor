#include "testing.h"

#include "array_list.h"

array_list_init(int);

IT(should_behave_like_normal_array) {
  struct array_list_int *al = array_list_create_int();
  cassert(al != NULL, "failed to create array_list of size 10");
  for (int i = 0; i < 10000; i++) {
    array_list_append_int(al, i);
  }
  int *j;
  for (int i = 0; i < 10000; i++) {
    cassertf((j = array_list_get_int_at(al, i)) != NULL && *j == i,
             "expected %d got %d\n", i, j);
  }
}

typedef struct foo {
  int prop1;
  long long prop3;
} foo;
array_list_init(foo);

IT(should_behave_like_a_normal_array_for_custom_struct) {
  struct array_list_foo *alf = array_list_create_foo();
  cassert(alf != NULL, "failed to create array_list for foo");
  for (int i = 0; i < 10000; i++) {
    foo f = {.prop1 = i, .prop3 = i};
    array_list_append_foo(alf, f);
  }
  for (int i = 0; i < 10000; i++) {
    foo *f = array_list_get_foo_at(alf, i);
    if (f == NULL || f->prop1 != i || f->prop3 != i) {
      failf("invalid value at index %d for foo array list\n", i);
    }
  }
}

IT(should_be_created_with_custom_capacity) {
  struct array_list_int *al = array_list_create_int_with_capacity(10);
  cassert(al != NULL, "failed to create array_list with capacity 10");
  cassert(array_list_get_capacity((struct array_list *)al) == 10,
          "expected capacity 10");
  cassert(array_list_get_length((struct array_list *)al) == 0,
          "expected length 0");
}

IT(should_return_null_for_out_of_bounds_index) {
  struct array_list_int *al = array_list_create_int();
  cassert(array_list_get_int_at(al, 0) == NULL,
          "expected NULL for empty list index 0");
  cassert(array_list_get_int_at(al, -1) == NULL,
          "expected NULL for negative index");

  array_list_append_int(al, 42);
  cassert(array_list_get_int_at(al, 0) != NULL,
          "expected non-NULL for valid index 0");
  cassert(array_list_get_int_at(al, 1) == NULL,
          "expected NULL for index 1 (length 1)");
}

IT(should_reallocate_correctly) {
  uint32_t initial_capacity = 2;
  struct array_list_int *al =
      array_list_create_int_with_capacity(initial_capacity);
  cassert(array_list_get_capacity((struct array_list *)al) == initial_capacity,
          "initial capacity mismatch");

  array_list_append_int(al, 1);
  array_list_append_int(al, 2);
  cassert(array_list_get_length((struct array_list *)al) == 2,
          "length should be 2");
  cassert(array_list_get_capacity((struct array_list *)al) == 2,
          "capacity should still be 2");

  // This should trigger reallocation
  array_list_append_int(al, 3);
  cassert(array_list_get_length((struct array_list *)al) == 3,
          "length should be 3");
  // default_capacity is 5, so new capacity should be 2 + 5 = 7
  cassert(array_list_get_capacity((struct array_list *)al) == 7,
          "capacity should be 7 after realloc");

  cassert(*array_list_get_int_at(al, 0) == 1, "element 0 mismatch");
  cassert(*array_list_get_int_at(al, 1) == 2, "element 1 mismatch");
  cassert(*array_list_get_int_at(al, 2) == 3, "element 2 mismatch");
}

IT(should_handle_empty_list_operations) {
  struct array_list_int *al = array_list_create_int_with_capacity(0);
  cassert(al != NULL, "failed to create array_list with capacity 0");
  cassert(array_list_get_length((struct array_list *)al) == 0,
          "length should be 0");

  // Append to 0-capacity list should trigger realloc
  array_list_append_int(al, 100);
  cassert(array_list_get_length((struct array_list *)al) == 1,
          "length should be 1");
  cassert(array_list_get_capacity((struct array_list *)al) == 5,
          "capacity should be 5 (default_capacity)");
  cassert(*array_list_get_int_at(al, 0) == 100, "element 0 mismatch");
}

IT(should_handle_many_reallocations) {
  struct array_list_int *al = array_list_create_int_with_capacity(1);
  int num_elements = 1000;
  for (int i = 0; i < num_elements; i++) {
    array_list_append_int(al, i);
  }
  cassert(array_list_get_length((struct array_list *)al) ==
              (uint32_t)num_elements,
          "length mismatch after many reallocs");
  for (int i = 0; i < num_elements; i++) {
    cassertf(*array_list_get_int_at(al, i) == i, "data corruption at index %d",
            i);
  }
}

typedef struct large_struct {
  char data[1024];
  int id;
} large_struct;
array_list_init(large_struct);

IT(should_handle_large_elements) {
  struct array_list_large_struct *al = array_list_create_large_struct();
  int num_elements = 100;
  for (int i = 0; i < num_elements; i++) {
    large_struct ls = {.id = i};
    memset(ls.data, i % 256, sizeof(ls.data));
    array_list_append_large_struct(al, ls);
  }

  cassert(array_list_get_length((struct array_list *)al) ==
              (uint32_t)num_elements,
          "length mismatch for large elements");
  for (int i = 0; i < num_elements; i++) {
    large_struct *ls = array_list_get_large_struct_at(al, i);
    cassertf(ls->id == i, "id mismatch at index %d", i);
    for (size_t j = 0; j < sizeof(ls->data); j++) {
      if (ls->data[j] != (char)(i % 256)) {
        failf("data corruption at index %d, byte %zu", i, j);
      }
    }
  }
}

IT(should_return_pointer_to_appended_element) {
  struct array_list_int *al = array_list_create_int();
  int val = 42;
  int *appended = array_list_append_int(al, val);
  cassert(appended != NULL, "append should return non-null pointer");
  cassert(*appended == val, "appended value mismatch");

  int *retrieved = array_list_get_int_at(al, 0);
  cassert(appended == retrieved,
          "pointer returned by append should match pointer from get");
}

IT(should_handle_maximum_capacity_growth) {
  // This is more of a stress test for realloc logic with small increments
  // default_capacity is 5
  struct array_list_int *al = array_list_create_int_with_capacity(0);
  for (int i = 0; i < 50; i++) {
    array_list_append_int(al, i);
    // Capacity should grow by 5 each time it's full
    // 0 -> 5 -> 10 -> 15 ...
    uint32_t expected_cap = ((i / 5) + 1) * 5;
    cassertf(array_list_get_capacity((struct array_list *)al) == expected_cap,
            "unexpected capacity at index %d, expected %u got %zu", i,
            expected_cap, array_list_get_capacity((struct array_list *)al));
  }
}

