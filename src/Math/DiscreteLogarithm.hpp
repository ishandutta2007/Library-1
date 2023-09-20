#pragma once
#include <cmath>
#include <vector>
#include "src/Internal/function_type.hpp"
// mp : E × T -> T
// op : E × E -> E
// hash : T -> int
// s,t ∈ T, x ∈ E
// return min{ i : x^i(s) = t and i ∈ [0,N) } or -1 (not found)
template <class F, class G, class H> class DiscreteLogarithm {
 const F &mp;
 const G &op;
 const H &hash;
 const int64_t lim;
 using T= result_type_t<F>;
 using E= result_type_t<G>;
public:
 DiscreteLogarithm(const F &mp, const G &op, const H &hash, int64_t lim= 1ll << 50): mp(mp), op(op), hash(hash), lim(lim) { static_assert(std::is_convertible_v<std::invoke_result_t<H, T>, int>); }
 int64_t operator()(const E &x, T s, const T &t, int64_t N= -1) const {
  if (N < 0) N= lim;
  const int m= 1 << std::__lg(int(std::sqrt(N) + 1)), mask= m - 1;
  std::vector<T> val(m), vs(m);
  std::vector<int> os(m + 1), so(m);
  T s1= t;
  for (int i= 0; i < m; ++i) ++os[so[i]= hash(val[i]= s1= mp(x, s1)) & mask];
  for (int i= 0; i < m; ++i) os[i + 1]+= os[i];
  for (int i= 0; i < m; ++i) vs[--os[so[i]]]= val[i];
  E y= x;
  for (int k= m; k>>= 1;) y= op(y, y);
  bool failed= false;
  for (int64_t n= 0;; s= s1) {
   for (int a= hash(s1= mp(y, s)) & mask, j= os[a]; j < os[a + 1]; ++j) {
    if (s1 == vs[j]) {
     for (int i= 0;; s= mp(x, s)) {
      if (s == t) return n + i < N ? n + i : -1;
      if (++i == m) break;
     }
     if (failed) return -1;
     failed= true;
     break;
    }
   }
   if ((n+= m) >= N) break;
  }
  return -1;
 }
};