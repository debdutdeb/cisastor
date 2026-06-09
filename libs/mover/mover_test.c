#include "testing.h"
#include "mover.h"
#include <string.h>

IT(should_claim_int_correctly) {
  int val = 42;
  int_mover_t *mi = mover_claim_int(&val);
  cassert(mi != NULL, "mover_claim_int returned NULL");
  cassert(mover_borrow_int(mi) == 42, "borrowed value should be 42");
}

IT(should_physically_move_data_and_invalidate_borrows) {
  int val = 100;
  int_mover_t *mi = mover_claim_int(&val);
  byte *old_addr = mover_borrow(mi);
  
  int_mover_t *mi2 = mover_move_int(mi);
  byte *new_addr = mover_borrow(mi2);
  
  cassert(mi2 != NULL, "mover_move_int returned NULL");
  cassert(new_addr != old_addr, "data should have physically moved to a new address");
  cassert(mover_borrow_int(mi2) == 100, "new mover should hold value 100");
  
  // The old address should have been scrubbed (set to 0)
  cassert(*(int *)old_addr == 0, "old borrowed reference should be invalidated (scrubbed to 0)");
  cassert(mi->data == NULL, "original mover data handle should be NULL");
}

IT(should_handle_pointer_moves_correctly) {
  int x = 500;
  void_ptr_t p = &x;
  void_ptr_t_mover_t *mp = mover_claim_void_ptr_t(&p);
  
  void_ptr_t *borrow1 = (void_ptr_t *)mover_borrow(mp);
  cassert(*borrow1 == &x, "borrowed pointer should point to x");
  
  void_ptr_t_mover_t *mp2 = mover_move_void_ptr_t(mp);
  void_ptr_t *borrow2 = (void_ptr_t *)mover_borrow(mp2);
  
  cassert(borrow2 != borrow1, "pointer storage location should have moved");
  cassert(*borrow2 == &x, "moved pointer should still point to x");
  cassert(*borrow1 == NULL, "old pointer storage should be scrubbed to NULL");
}

IT(should_handle_various_primitive_types) {
  double d = 3.14159;
  double_mover_t *md = mover_claim_double(&d);
  cassert(mover_borrow_double(md) == 3.14159, "double mismatch");
  
  uint64_t u64 = 0xFFFFFFFFFFFFFFFFULL;
  uint64_t_mover_t *mu64 = mover_claim_uint64_t(&u64);
  cassert(mover_borrow_uint64_t(mu64) == u64, "uint64 mismatch");
  
  int8_t i8 = -120;
  int8_t_mover_t *mi8 = mover_claim_int8_t(&i8);
  cassert(mover_borrow_int8_t(mi8) == i8, "int8 mismatch");
}

IT(should_return_zero_on_borrow_from_invalid_mover) {
  int val = 10;
  int_mover_t *mi = mover_claim_int(&val);
  mover_move_int(mi);
  
  cassert(mover_borrow_int(mi) == 0, "borrow from moved mover should return 0-value");
}

IT(should_handle_nested_movers) {
  int x = 99;
  int_mover_t *mi = mover_claim_int(&x);
  byte *final_data_addr = mover_borrow(mi);
  
  // mm owns the mi struct itself
  mover_t_mover_t *mm = mover_claim_mover_t(mi);
  byte *inner_mover_struct_addr1 = mover_borrow(mm);
  
  cassert(*(int *)((mover_t *)inner_mover_struct_addr1)->data == 99, "nested mover should point to 99");
  
  // Move the outer mover
  mover_t_mover_t *mm2 = mover_move_mover_t(mm);
  byte *inner_mover_struct_addr2 = mover_borrow(mm2);
  
  cassert(inner_mover_struct_addr2 != inner_mover_struct_addr1, "inner mover struct should have physically moved");
  cassert(((mover_t *)inner_mover_struct_addr2)->data == final_data_addr, "inner mover should still point to same final data");
  cassert(*(int *)((mover_t *)inner_mover_struct_addr2)->data == 99, "can still reach 99 through nested move");
  
  // Old inner mover struct should be zeroed
  cassert(((mover_t *)inner_mover_struct_addr1)->data == NULL, "old inner mover struct should be zeroed");
}

IT(should_handle_typedefed_pointers) {
  int x = 7;
  int_ptr_t p = &x;
  int_ptr_t_mover_t *mp = mover_claim_int_ptr_t(&p);
  
  cassert(mover_borrow_int_ptr_t(mp) == &x, "borrowed pointer mismatch");
  
  int_ptr_t_mover_t *mp2 = mover_move_int_ptr_t(mp);
  cassert(mover_borrow_int_ptr_t(mp2) == &x, "moved pointer mismatch");
}
