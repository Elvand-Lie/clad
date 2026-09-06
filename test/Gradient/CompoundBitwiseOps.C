// RUN: %cladclang %s -I%S/../../include -oCompoundBitwiseOps.out 2>&1 | %filecheck %s
// RUN: ./CompoundBitwiseOps.out | %filecheck_exec %s

#include "clad/Differentiator/Differentiator.h"
#include <iostream>

// 1. %= operator
double f_rem(double x, int y) {
  int n = 10;
  n %= y;
  return x * n;
}

// CHECK-LABEL: f_rem_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = 10;
// CHECK-NEXT:     int &_ref0 = n %= y;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * n;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_n = 0;
// CHECK-NEXT: }

// 2. &= operator
double f_and(double x, int y) {
  int n = y;
  n &= 7;
  return x * n;
}

// CHECK-LABEL: f_and_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = y;
// CHECK-NEXT:     int &_ref0 = n &= 7;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * n;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_n = 0;
// CHECK-NEXT:     _d_y += _d_n;
// CHECK-NEXT: }

// 3. |= operator
double f_or(double x, int y) {
  int n = y;
  n |= 3;
  return x * n;
}

// CHECK-LABEL: f_or_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = y;
// CHECK-NEXT:     int &_ref0 = n |= 3;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * n;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_n = 0;
// CHECK-NEXT:     _d_y += _d_n;
// CHECK-NEXT: }

// 4. ^= operator
double f_xor(double x, int y) {
  int n = y;
  n ^= 5;
  return x * n;
}

// CHECK-LABEL: f_xor_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = y;
// CHECK-NEXT:     int &_ref0 = n ^= 5;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * n;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_n = 0;
// CHECK-NEXT:     _d_y += _d_n;
// CHECK-NEXT: }

// 5. <<= operator
double f_shl(double x, int y) {
  int n = y;
  n <<= 2;
  return x * n;
}

// CHECK-LABEL: f_shl_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = y;
// CHECK-NEXT:     int &_ref0 = n <<= 2;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * n;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_n = 0;
// CHECK-NEXT:     _d_y += _d_n;
// CHECK-NEXT: }

// 6. >>= operator
double f_shr(double x, int y) {
  int n = y;
  n >>= 1;
  return x * n;
}

// CHECK-LABEL: f_shr_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = y;
// CHECK-NEXT:     int &_ref0 = n >>= 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * n;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_n = 0;
// CHECK-NEXT:     _d_y += _d_n;
// CHECK-NEXT: }

// 7. Compound assignment inside a loop
double f_loop(double x, int n) {
  double res = x;
  int mask = 15;
  for (int i = 0; i < n; ++i) {
    mask %= 4;
    res += x * mask;
    mask += 5;
  }
  return res;
}

// CHECK-LABEL: f_loop_grad
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     int i = 0;
// CHECK-NEXT:     clad::tape<int> _t1 = {};
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = x;
// CHECK-NEXT:     int _d_mask = 0;
// CHECK-NEXT:     int mask = 15;
// CHECK-NEXT:     unsigned long _t0 = 0;
// CHECK-NEXT:     for (i = 0; i < n; ++i) {
// CHECK-NEXT:         _t0++;
// CHECK-NEXT:         int &_ref0 = mask %= 4;
// CHECK-NEXT:         res += x * mask;
// CHECK-NEXT:         clad::push(_t1, mask);
// CHECK-NEXT:         mask += 5;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     for (; _t0; _t0--) {
// CHECK-NEXT:         mask = clad::pop(_t1);
// CHECK-NEXT:         {
// CHECK-NEXT:             *_d_x += _d_res * mask;
// CHECK-NEXT:             _d_mask += x * _d_res;
// CHECK-NEXT:         }
// CHECK-NEXT:         _d_mask = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     *_d_x += _d_res;
// CHECK-NEXT: }

// 8. Nested %= expression
double f_nested_rem(double x, int y) {
  int n = 10;
  return x * (n %= y);
}

// CHECK-LABEL: f_nested_rem_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = 10;
// CHECK-NEXT:     int &_ref0 = n %= y;
// CHECK-NEXT:     int _t1 = _ref0;
// CHECK-NEXT:     double _t0 = _ref0;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * _t0;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:         _d_n = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }

// 9. Nested &= expression
double f_nested_and(double x, int y) {
  int n = 11;
  return x * (n &= y);
}

// CHECK-LABEL: f_nested_and_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = 11;
// CHECK-NEXT:     int &_ref0 = n &= y;
// CHECK-NEXT:     int _t1 = _ref0;
// CHECK-NEXT:     double _t0 = _ref0;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * _t0;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:         _d_n = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: inline void next_pullback(int &y, int _d_y0, int *_d_y) {
// CHECK-NEXT:     *_d_y += _d_y0;
// CHECK-NEXT: }

int next(int& y) {
  return ++y;
}

// 10. Nested compound assignment with RHS side effect
double f_nested_side_effect(double x, int y) {
  int n = 10;
  return x * (n %= next(y));
}

// CHECK-LABEL: f_nested_side_effect_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = 10;
// CHECK-NEXT:     int _t1 = y;
// CHECK-NEXT:     int &_ref0 = n %= next(y);
// CHECK-NEXT:     int _t2 = _ref0;
// CHECK-NEXT:     double _t0 = _ref0;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * _t0;
// CHECK-NEXT:         _d_n += x * 1;
// CHECK-NEXT:         _d_n = 0;
// CHECK-NEXT:         y = _t1;
// CHECK-NEXT:         next_pullback(y, 0, &_d_y);
// CHECK-NEXT:     }
// CHECK-NEXT: }

// 11. Compound assignment inside a loop with RHS side effect
double f_loop_side_effect(double x, int n, int y) {
  double res = x;
  int mask = 15;
  for (int i = 0; i < n; ++i) {
    mask %= next(y);
    res += x * mask;
  }
  return res;
}

// CHECK-LABEL: f_loop_side_effect_grad
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     int i = 0;
// CHECK-NEXT:     clad::tape<int> _t1 = {};
// CHECK-NEXT:     clad::tape<int> _t2 = {};
// CHECK-NEXT:     double _d_res = 0.;
// CHECK-NEXT:     double res = x;
// CHECK-NEXT:     int _d_mask = 0;
// CHECK-NEXT:     int mask = 15;
// CHECK-NEXT:     unsigned long _t0 = 0;
// CHECK-NEXT:     for (i = 0; i < n; ++i) {
// CHECK-NEXT:         _t0++;
// CHECK-NEXT:         clad::push(_t1, mask);
// CHECK-NEXT:         clad::push(_t2, y);
// CHECK-NEXT:         int &_ref0 = mask %= next(y);
// CHECK-NEXT:         res += x * mask;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_res += 1;
// CHECK-NEXT:     for (; _t0; _t0--) {
// CHECK-NEXT:         {
// CHECK-NEXT:             *_d_x += _d_res * mask;
// CHECK-NEXT:             _d_mask += x * _d_res;
// CHECK-NEXT:         }
// CHECK-NEXT:         {
// CHECK-NEXT:             mask = clad::pop(_t1);
// CHECK-NEXT:             _d_mask = 0;
// CHECK-NEXT:             y = clad::pop(_t2);
// CHECK-NEXT:             next_pullback(y, 0, &_d_y);
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     *_d_x += _d_res;
// CHECK-NEXT: }

// 12. Nested XOR under parent compound assignment (*=)
double nested_xor_mul_assign(double x, int y) {
  double result = x;
  int n = 12;
  result *= (n ^= y);
  return result;
}

// CHECK-LABEL: nested_xor_mul_assign_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     double _d_result = 0.;
// CHECK-NEXT:     double result = x;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = 12;
// CHECK-NEXT:     double _t0 = result;
// CHECK-NEXT:     int &_ref0 = n ^= y;
// CHECK-NEXT:     int _t1 = _ref0;
// CHECK-NEXT:     result *= _ref0;
// CHECK-NEXT:     _d_result += 1;
// CHECK-NEXT:     {
// CHECK-NEXT:         result = _t0;
// CHECK-NEXT:         double _r_d0 = _d_result;
// CHECK-NEXT:         _d_result = 0.;
// CHECK-NEXT:         _d_result += _r_d0 * _t1;
// CHECK-NEXT:         _d_n += result * _r_d0;
// CHECK-NEXT:         _d_n = 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     *_d_x += _d_result;
// CHECK-NEXT: }

// 13. Nested left shift under parent compound assignment (*=)
double nested_shl_mul_assign(double x, int shift) {
  double result = x;
  int n = 3;
  result *= (n <<= shift);
  return result;
}

// 14. Nested right shift under parent compound assignment (*=)
double nested_shr_mul_assign(double x, int shift) {
  double result = x;
  int n = 20;
  result *= (n >>= shift);
  return result;
}

// 15. Narrowing conversion (unsigned char)
double nested_narrow_shift(double x) {
  double result = x;
  unsigned char n = 200;
  result *= (n <<= 1);
  return result;
}

// 16. Loop-aware preservation for discrete compound assignment
double nested_xor_loop(double x, int count, int y) {
  double result = x;
  int n = 12;
  for (int i = 0; i < count; ++i)
    result *= (n ^= y);
  return result;
}

// CHECK-LABEL: nested_xor_loop_grad
// CHECK-NEXT:     int _d_count = 0;
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_i = 0;
// CHECK-NEXT:     int i = 0;
// CHECK-NEXT:     clad::tape<double> _t1 = {};
// CHECK-NEXT:     clad::tape<int> _t2 = {};
// CHECK-NEXT:     clad::tape<int> _t3 = {};
// CHECK-NEXT:     double _d_result = 0.;
// CHECK-NEXT:     double result = x;
// CHECK-NEXT:     int _d_n = 0;
// CHECK-NEXT:     int n = 12;
// CHECK-NEXT:     unsigned long _t0 = 0;
// CHECK-NEXT:     for (i = 0; i < count; ++i) {
// CHECK-NEXT:         _t0++;
// CHECK-NEXT:         clad::push(_t1, result);
// CHECK-NEXT:         clad::push(_t2, n);
// CHECK-NEXT:         int &_ref0 = n ^= y;
// CHECK-NEXT:         clad::push(_t3, _ref0);
// CHECK-NEXT:         result *= _ref0;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_result += 1;
// CHECK-NEXT:     for (; _t0; _t0--) {
// CHECK-NEXT:         result = clad::pop(_t1);
// CHECK-NEXT:         double _r_d0 = _d_result;
// CHECK-NEXT:         _d_result = 0.;
// CHECK-NEXT:         _d_result += _r_d0 * clad::back(_t3);
// CHECK-NEXT:         _d_n += result * _r_d0;
// CHECK-NEXT:         n = clad::pop(_t2);
// CHECK-NEXT:         _d_n = 0;
// CHECK-NEXT:         clad::pop(_t3);
// CHECK-NEXT:     }
// CHECK-NEXT:     *_d_x += _d_result;
// CHECK-NEXT: }

// 17. Bit-field LHS: reference binding is ill-formed ([class.bit]/5),
// so the generated code must not bind T& to the result.
struct Bits { int flags : 8; };
double f_bitfield(double x, int mask) {
  Bits b;
  b.flags = 12;
  b.flags &= mask;
  return x * b.flags;
}

// CHECK-LABEL: f_bitfield_grad
// CHECK-NOT: int &{{.*}} = b.flags &= mask
// CHECK-NEXT:     int _d_mask = 0;
// CHECK-NEXT:     Bits _d_b = {0};
// CHECK-NEXT:     Bits b;
// CHECK-NEXT:     b.flags = 12;
// CHECK-NEXT:     b.flags &= mask;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * b.flags;
// CHECK-NEXT:         _d_b.flags += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_b.flags = 0;
// CHECK-NEXT:     {
// CHECK-NEXT:         int _r_d0 = _d_b.flags;
// CHECK-NEXT:         _d_b.flags = 0;
// CHECK-NEXT:     }
// CHECK-NEXT: }

// 18. Array subscript LHS
double f_arr_rem(double x, int y) {
  int arr[2] = {10, 20};
  arr[0] %= y;
  return x * arr[0];
}

// CHECK-LABEL: f_arr_rem_grad
// CHECK-NEXT:     int _d_y = 0;
// CHECK-NEXT:     int _d_arr[2] = {0};
// CHECK-NEXT:     int arr[2] = {10, 20};
// CHECK-NEXT:     int &_ref0 = arr[0] %= y;
// CHECK-NEXT:     {
// CHECK-NEXT:         *_d_x += 1 * arr[0];
// CHECK-NEXT:         _d_arr[0] += x * 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     _d_arr[0] = 0;
// CHECK-NEXT: }

int main() {
  // Test 1: rem (10 % 3 = 1 -> df/dx = 1)
  auto df_rem = clad::gradient(f_rem, "x");
  double dx_rem = 0;
  df_rem.execute(5.0, 3, &dx_rem);
  std::cout << "df_rem/dx = " << dx_rem << std::endl;
  // CHECK-EXEC: df_rem/dx = 1

  // Test 2: and (11 & 7 = 3 -> df/dx = 3)
  auto df_and = clad::gradient(f_and, "x");
  double dx_and = 0;
  df_and.execute(4.0, 11, &dx_and);
  std::cout << "df_and/dx = " << dx_and << std::endl;
  // CHECK-EXEC: df_and/dx = 3

  // Test 3: or (4 | 3 = 7 -> df/dx = 7)
  auto df_or = clad::gradient(f_or, "x");
  double dx_or = 0;
  df_or.execute(2.0, 4, &dx_or);
  std::cout << "df_or/dx = " << dx_or << std::endl;
  // CHECK-EXEC: df_or/dx = 7

  // Test 4: xor (12 ^ 5 = 9 -> df/dx = 9)
  auto df_xor = clad::gradient(f_xor, "x");
  double dx_xor = 0;
  df_xor.execute(3.0, 12, &dx_xor);
  std::cout << "df_xor/dx = " << dx_xor << std::endl;
  // CHECK-EXEC: df_xor/dx = 9

  // Test 5: shl (3 << 2 = 12 -> df/dx = 12)
  auto df_shl = clad::gradient(f_shl, "x");
  double dx_shl = 0;
  df_shl.execute(2.0, 3, &dx_shl);
  std::cout << "df_shl/dx = " << dx_shl << std::endl;
  // CHECK-EXEC: df_shl/dx = 12

  // Test 6: shr (10 >> 1 = 5 -> df/dx = 5)
  auto df_shr = clad::gradient(f_shr, "x");
  double dx_shr = 0;
  df_shr.execute(2.0, 10, &dx_shr);
  std::cout << "df_shr/dx = " << dx_shr << std::endl;
  // CHECK-EXEC: df_shr/dx = 5

  // Test 7: loop
  auto df_loop = clad::gradient(f_loop, "x");
  double dx_loop = 0;
  df_loop.execute(2.0, 3, &dx_loop);
  std::cout << "df_loop/dx = " << dx_loop << std::endl;
  // CHECK-EXEC: df_loop/dx = 5

  // Test 8: nested_rem (x * (n %= y) with x=5, n=10, y=3 -> n becomes 1 -> df/dx = 1)
  auto df_nested_rem = clad::gradient(f_nested_rem, "x");
  double dx_nested_rem = 0;
  df_nested_rem.execute(5.0, 3, &dx_nested_rem);
  std::cout << "df_nested_rem/dx = " << dx_nested_rem << std::endl;
  // CHECK-EXEC: df_nested_rem/dx = 1

  // Test 9: nested_and (x * (n &= y) with x=4, n=11, y=7 -> n becomes 3 -> df/dx = 3)
  auto df_nested_and = clad::gradient(f_nested_and, "x");
  double dx_nested_and = 0;
  df_nested_and.execute(4.0, 7, &dx_nested_and);
  std::cout << "df_nested_and/dx = " << dx_nested_and << std::endl;
  // CHECK-EXEC: df_nested_and/dx = 3

  // Test 10: nested_side_effect
  auto df_nested_side_effect = clad::gradient(f_nested_side_effect, "x");
  double dx_nested_side_effect = 0;
  df_nested_side_effect.execute(5.0, 2, &dx_nested_side_effect);
  std::cout << "df_nested_side_effect/dx = " << dx_nested_side_effect << std::endl;
  // CHECK-EXEC: df_nested_side_effect/dx = 1

  // Test 11: loop_side_effect
  auto df_loop_side_effect = clad::gradient(f_loop_side_effect, "x");
  double dx_loop_side_effect = 0;
  df_loop_side_effect.execute(2.0, 2, 2, &dx_loop_side_effect);
  std::cout << "df_loop_side_effect/dx = " << dx_loop_side_effect << std::endl;
  // CHECK-EXEC: df_loop_side_effect/dx = 1

  // Test 12: nested_xor_mul_assign (12 ^ 5 = 9 -> expected df/dx = 9)
  auto df_nested_xor = clad::gradient(nested_xor_mul_assign, "x");
  double dx_nested_xor = 0;
  df_nested_xor.execute(2.0, 5, &dx_nested_xor);
  std::cout << "nested_xor_mul_assign df/dx = " << dx_nested_xor << std::endl;
  // CHECK-EXEC: nested_xor_mul_assign df/dx = 9

  // Test 13: nested_shl_mul_assign (3 << 2 = 12 -> expected df/dx = 12)
  auto df_nested_shl = clad::gradient(nested_shl_mul_assign, "x");
  double dx_nested_shl = 0;
  df_nested_shl.execute(2.0, 2, &dx_nested_shl);
  std::cout << "nested_shl_mul_assign df/dx = " << dx_nested_shl << std::endl;
  // CHECK-EXEC: nested_shl_mul_assign df/dx = 12

  // Test 14: nested_shr_mul_assign (20 >> 1 = 10 -> expected df/dx = 10)
  auto df_nested_shr = clad::gradient(nested_shr_mul_assign, "x");
  double dx_nested_shr = 0;
  df_nested_shr.execute(2.0, 1, &dx_nested_shr);
  std::cout << "nested_shr_mul_assign df/dx = " << dx_nested_shr << std::endl;
  // CHECK-EXEC: nested_shr_mul_assign df/dx = 10

  // Test 15: nested_narrow_shift (200 <<= 1 -> unsigned char 144 -> expected df/dx = 144)
  auto df_nested_narrow = clad::gradient(nested_narrow_shift, "x");
  double dx_nested_narrow = 0;
  df_nested_narrow.execute(2.0, &dx_nested_narrow);
  std::cout << "nested_narrow_shift df/dx = " << dx_nested_narrow << std::endl;
  // CHECK-EXEC: nested_narrow_shift df/dx = 144

  // Test 16: nested_xor_loop (iter 1: 12^5=9, iter 2: 9^5=12 -> 9 * 12 = 108 -> expected df/dx = 108)
  auto df_nested_xor_loop = clad::gradient(nested_xor_loop, "x");
  double dx_nested_xor_loop = 0;
  df_nested_xor_loop.execute(2.0, 2, 5, &dx_nested_xor_loop);
  std::cout << "nested_xor_loop df/dx = " << dx_nested_xor_loop << std::endl;
  // CHECK-EXEC: nested_xor_loop df/dx = 108

  // Test 17: bit-field LHS (12 & 5 = 4 -> df/dx = 4)
  auto df_bitfield = clad::gradient(f_bitfield, "x");
  double dx_bitfield = 0;
  df_bitfield.execute(3.0, 5, &dx_bitfield);
  std::cout << "f_bitfield df/dx = " << dx_bitfield << std::endl;
  // CHECK-EXEC: f_bitfield df/dx = 4

  // Test 18: array subscript LHS (10 % 3 = 1 -> df/dx = 1)
  auto df_arr_rem = clad::gradient(f_arr_rem, "x");
  double dx_arr_rem = 0;
  df_arr_rem.execute(5.0, 3, &dx_arr_rem);
  std::cout << "f_arr_rem df/dx = " << dx_arr_rem << std::endl;
  // CHECK-EXEC: f_arr_rem df/dx = 1

  return 0;
}
