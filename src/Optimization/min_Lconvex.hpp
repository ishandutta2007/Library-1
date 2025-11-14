#pragma once
#include <vector>
template <typename TD, typename TR, class F> std::pair<TR, std::vector<TD>> min_Lconvex(const F &f, std::vector<TD> x, TD alpha) {
 TR f0= f(x), f1= f0, fS;
 for (int n= x.size(); alpha; f0 == f1 ? alpha>>= 1 : f0= f1) {
  std::vector<TD> x0{x};
  for (int S= 1; S < (1 << n) - 1; S++) {
   std::vector<TD> xS{x0};
   for (int i= 0; i < n; i++)
    if ((S >> i) & 1) xS[i]+= alpha;
   if ((fS= f(xS)) < f1) f1= fS, x= std::move(xS);
  }
 }
 return {f1, std::move(x)};
}