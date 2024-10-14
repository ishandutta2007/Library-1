#pragma once
#include "src/Math/FactorialPrecalculation.hpp"
// given: f(0),f(1),...,f(n-1), c   output: f(c)    O(n)
template <class mod_t> mod_t sample_points_shift(std::vector<mod_t> y, mod_t c) {
 using F= FactorialPrecalculation<mod_t>;
 int n= y.size();
 if ((int)c.val() < n) return y[c.val()];
 for (int i= n; i--;) y[i]*= F::finv(i) * F::finv(n - i - 1);
 for (int i= 1; i < n; i+= 2) y[n - i - 1]= -y[n - i - 1];
 mod_t t= 1, ret= 0;
 for (int i= n; i--;) y[i]*= t, t*= c - i;
 t= 1;
 for (int i= 0; i < n; ++i) ret+= y[i] * t, t*= c - i;
 return ret;
}