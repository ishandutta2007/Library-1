#pragma once
#include <cmath>
#include <cassert>
#include "src/Internal/function_type.hpp"
#include "src/Optimization/MinMaxEnum.hpp"
// [l,r]
template <MinMaxEnum obj, class F> std::pair<long double, result_type_t<F>> golden_search(const F &f, long double l, long double r, int iter= 100) {
 static constexpr long double c= 2 / (3 + std::sqrt(5.0l));
 assert(l <= r);
 long double x= l + (r - l) * c, y= r - (r - l) * c;
 result_type_t<F> fx= f(x), fy= f(y);
 for (bool g; iter--;) {
  if constexpr (obj == MINIMIZE) g= fx < fy;
  else g= fx > fy;
  if (g) r= y, y= x, fy= fx, fx= f(x= l + (r - l) * c);
  else l= x, x= y, fx= fy, fy= f(y= r - (r - l) * c);
 }
 return {x, fx};
}