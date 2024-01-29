#pragma once
#include <cstdint>
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/Math/mod_sqrt.hpp"
namespace sfps {
namespace sfps_internal {
using namespace std;
template <class K> using Dat= vector<pair<int, K>>;
template <class K> Dat<K> to_sfps(const vector<K>& f, int b, int N) {
 Dat<K> p;
 for (int e= min<int>(N + 1, f.size()); b < e; ++b)
  if (f[b] != K(0)) p.emplace_back(b, f[b]);
 return p;
}
// f/g, O(N S_g)
template <class K> vector<K> div(vector<K> f, const vector<K>& g, int N) {
 assert(g[0] != K(0)), f.resize(N + 1);
 auto p= to_sfps(g, 1, N);
 const K iv= K(1) / g[0];
 for (int i= 0; i <= N; f[i++]*= iv)
  for (auto&& [j, v]: p) {
   if (i < j) break;
   f[i]-= f[i - j] * v;
  }
 return f;
}
template <class mod_t> void pw_mul_pw(mod_t* f, const Dat<mod_t>& p, const Dat<mod_t>& q, mod_t a, mod_t b, int N) {
 static_assert(is_modint_v<mod_t>);
 vector<mod_t> x(N + 1), y(N);
 f[0]= 1;
 for (int i= 0, k= 1; i < N; i= k++, f[i]*= FactorialPrecalculation<mod_t>::inv(i)) {
  for (auto&& [j, v]: q) {
   if (i < j) break;
   y[i]+= f[i - j] * v;
  }
  (y[i]+= f[i])*= a;
  for (auto&& [j, v]: p) {
   if (k < j) break;
   x[k]+= v * (y[k - j] * j - x[k - j]);
  }
  f[k]= x[k];
  for (auto&& [j, v]: q) {
   if (k < j) break;
   f[k]+= v * (b * j - k) * f[k - j];
  }
 }
}
template <class K> int mn_dg(const vector<K>& f) {
 for (int z= 0, e= f.size(); z < e; ++z)
  if (f[z] != K(0)) return z;
 return -1;
}
template <class mod_t> mod_t pw(mod_t a, int64_t k) { return k < 0 ? mod_t(1) / a.pow(-k) : a.pow(k); }
// f^k (k can be negative), O(N S_f)
template <class mod_t> vector<mod_t> pow(const vector<mod_t>& f, int64_t k, int N) {
 vector<mod_t> F(N + 1);
 if (!k) return F[0]= 1, F;
 int x= mn_dg(f);
 assert(!(x < 0 && k < 0));
 if (x < 0) return F;
 auto o= __int128_t(x) * k;
 if (o > N) return F;
 mod_t p0= f[x], ip= mod_t(1) / p0, a= pw(p0, k);
 auto p= to_sfps(f, x + 1, N - o + x);
 for (auto& [j, v]: p) v*= ip, j-= x;
 pw_mul_pw<mod_t>(F.data() + o, p, {}, k, 0, N - o);
 for (int i= o; i <= N; ++i) F[i]*= a;
 return F;
}
// f^k g^l (k, l can be negative), O(N(S_f+S_g))
template <class mod_t> vector<mod_t> pow_mul_pow(const vector<mod_t>& f, int64_t k, const vector<mod_t>& g, int64_t l, int N) {
 if (!k) return pow(g, l, N);
 if (!l) return pow(f, k, N);
 int x= mn_dg(f), y= mn_dg(g);
 vector<mod_t> F(N + 1);
 assert(!(x < 0 && k < 0)), assert(!(y < 0 && l < 0));
 if (x < 0 || y < 0) return F;
 auto o= __int128_t(x) * k + __int128_t(y) * l;
 if (assert(o >= 0); o > N) return F;
 mod_t p0= f[x], q0= g[y], ip= mod_t(1) / p0, iq= mod_t(1) / q0, a= pw(p0, k) * pw(q0, l);
 auto p= to_sfps(f, x + 1, N - o + x), q= to_sfps(g, y + 1, N - o + y);
 for (auto& [j, v]: p) v*= ip, j-= x;
 for (auto& [j, v]: q) v*= iq, j-= y;
 pw_mul_pw<mod_t>(F.data() + o, p, q, k, l + 1, N - o);
 for (int i= o; i <= N; ++i) F[i]*= a;
 return F;
}
// √(f/g), O(N(S_f+S_g))
template <class mod_t> vector<mod_t> sqrt_of_div(const vector<mod_t>& f, const vector<mod_t>& g, int N) {
 int x= mn_dg(f), y= mn_dg(g), o= (x - y) >> 1;
 vector<mod_t> F(N + 1);
 if (assert(y >= 0); x < 0) return F;
 if (assert(x >= y); (x - y) & 1) return {};  // no solution
 mod_t p0= f[x], ip= mod_t(1) / p0, iq= mod_t(1) / g[y], a= mod_sqrt(p0 * iq, mod_t::mod()), i2= mod_t(1) / 2;
 auto p= to_sfps(f, x + 1, N - o + x), q= to_sfps(g, y + 1, N - o + y);
 for (auto& [j, v]: p) v*= ip, j-= x;
 for (auto& [j, v]: q) v*= iq, j-= y;
 pw_mul_pw<mod_t>(F.data() + o, p, q, i2, -i2 + 1, N - o);
 for (int i= o; i <= N; ++i) F[i]*= a;
 return F;
}
// log(f), O(N S_f)
template <class mod_t> vector<mod_t> log(vector<mod_t> f, int N) {
 assert(f[0] == mod_t(1));
 auto p= to_sfps(f, 1, N);
 f.resize(N + 1);
 for (int i= 1; i <= N; ++i, f[i]*= i)
  for (auto&& [j, v]: p) {
   if (i <= j) break;
   f[i]-= f[i - j] * v;
  }
 for (int i= 2; i <= N; ++i) f[i]*= FactorialPrecalculation<mod_t>::inv(i);
 return f[0]= 0, f;
}
// exp(f/g), O(N(S_f+S_g))
template <class mod_t> vector<mod_t> exp_of_div(const vector<mod_t>& f, const vector<mod_t>& g, int N) {
 int x= mn_dg(f), y= mn_dg(g);
 assert(y >= 0), assert(x < 0 || x > y);
 auto p= to_sfps(f, 0, N + y), q= to_sfps(g, y + 1, N + y);
 mod_t iv= mod_t(1) / g[y];
 for (auto& [j, v]: p) v*= iv, j-= y;
 for (auto& [j, v]: q) v*= iv, j-= y;
 vector<mod_t> F(N + 1), dF(N), a(N), b(N);
 F[0]= 1;
 for (int i= 0; i < N; ++i) {
  for (auto&& [j, v]: p) {
   if (i < j) break;
   b[i]+= v * F[i - j];
  }
  for (auto&& [j, v]: q) {
   if (i < j) break;
   a[i]-= v * (a[i - j] + b[i + 1 - j] * j);
   dF[i]-= v * dF[i - j];
  }
  for (auto&& [j, v]: p) {
   if (i + 1 < j) break;
   dF[i]+= v * j * F[i + 1 - j];
  }
  F[i + 1]= (dF[i]+= a[i]) * FactorialPrecalculation<mod_t>::inv(i + 1);
 }
 return F;
}
}
using sfps_internal::div, sfps_internal::pow, sfps_internal::pow_mul_pow, sfps_internal::pow_mul_pow, sfps_internal::sqrt_of_div, sfps_internal::log, sfps_internal::exp_of_div;
// 1/f, O(N S_f)
template <class K> std::vector<K> inv(const std::vector<K>& f, int N) { return div<K>({1}, f, N); }
// (f/g)^k (k can be negative), O(N(S_f+S_g))
template <class mod_t> std::vector<mod_t> pow_of_div(const std::vector<mod_t>& f, const std::vector<mod_t>& g, int64_t k, int N) { return pow_mul_pow(f, k, g, -k, N); }
// √f, O(N S_f)
template <class mod_t> std::vector<mod_t> sqrt(const std::vector<mod_t>& f, int N) { return sqrt_of_div<mod_t>(f, {1}, N); }
// exp(f), O(N S_f)
template <class mod_t> std::vector<mod_t> exp(const std::vector<mod_t>& f, int N) { return exp_of_div<mod_t>(f, {1}, N); }
}