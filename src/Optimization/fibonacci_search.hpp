#pragma once
#include <algorithm>
#include <cassert>
#include "src/Optimization/MinMaxEnum.hpp"
// [l,r]
template <class T, MinMaxEnum obj, class F> std::pair<int64_t, T> fibonacci_search(const F &f, int64_t l, int64_t r) {
 assert(l <= r);
 int64_t s= 1, t= 2, a= l - 1, x, b, y;
 for (int64_t e= r - l + 2; t < e;) std::swap(s+= t, t);
 b= a + t, x= b - s;
 T fx= f(x), fy;
 for (bool g; a + b != 2 * x;) {
  if (y= a + b - x; r < y) b= a, a= y;
  else {
   if constexpr (obj == MINIMIZE) g= fx < (fy= f(y));
   else g= fx > (fy= f(y));
   if (g) b= a, a= y;
   else a= x, x= y, fx= fy;
  }
 }
 return {x, fx};
}