#pragma once
#include <cstdint>
#include "src/FFT/fps_inv.hpp"
#include "src/FFT/convolve.hpp"
namespace div_at_internal {
template <class K> int deg(const std::vector<K> &p) {
 const K ZERO= 0;
 for (int n= p.size() - 1;; n--)
  if (n < 0 || p[n] != ZERO) return n;
}
template <size_t LM, class mod_t> void div_at_ntt(std::vector<mod_t> &p, std::vector<mod_t> &q, uint64_t &k) {
 static_assert(!is_nttfriend<mod_t, LM>());
 using GNA= GlobalNTTArray<mod_t, LM, 0>;
 using GNA1= GlobalNTTArray<mod_t, LM, 1>;
 using GNA2= GlobalNTTArray<mod_t, LM, 2>;
 const unsigned m= deg(q) + 1, offset= std::max<unsigned>(deg(p) + 1, m), len= pw2((offset + m) - 1);
 for (p.resize(len >> 1); k >= offset; k>>= 1) {
  GNA::bf.set(p.data(), 0, len >> 1), GNA::bf.zeros(len >> 1, len), GNA1::bf.set(q.data(), 0, m), GNA1::bf.zeros(m, len), GNA2::bf.zeros(m, len);
  for (int i= m; i--;) GNA2::bf.set(i, i & 1 ? -q[i] : q[i]);
  GNA::bf.dft(0, len), GNA1::bf.dft(0, len), GNA2::bf.dft(0, len), GNA::bf.mul(GNA2::bf, 0, len), GNA::bf.idft(0, len), GNA1::bf.mul(GNA2::bf, 0, len), GNA1::bf.idft(0, len);
  for (int i= k & 1; i < len; i+= 2) p[i >> 1]= GNA::bf.get(i);
  for (int i= m; i--;) q[i]= GNA1::bf.get(i << 1);
 }
}
template <size_t LM, class mod_t> void div_at_ntt_fast(std::vector<mod_t> &p, std::vector<mod_t> &q, uint64_t &k) {
 static_assert(is_nttfriend<mod_t, LM>());
 using ntt= NTT<mod_t>;
 const unsigned m= deg(q) + 1, offset= std::max<unsigned>(deg(p) + 1, m), len= pw2((offset + m) - 1), len2= len >> 1;
 p.resize(len), q.resize(len), ntt::dft(len, p.data()), ntt::dft(len, q.data());
 while (1) {
  for (int i= len; i--;) p[i]*= q[i ^ 1];
  k & 1 ? ntt::odd_dft(len, p.data()) : ntt::even_dft(len, p.data());
  for (int i= 0; i < len; i+= 2) q[i]= q[i + 1]= q[i] * q[i + 1];
  ntt::even_dft(len, q.data());
  if ((k>>= 1) < offset) break;
  ntt::dft_doubling(len2, p.data()), ntt::dft_doubling(len2, q.data());
 }
 ntt::idft(len2, p.data()), ntt::idft(len2, q.data());
}
}  // namespace div_at_internal
#define __FPS_DIVAT(Vec) \
 template <class mod_t, size_t LM= 1 << 22> mod_t div_at(Vec p, Vec q, uint64_t k) { \
  using namespace div_at_internal; \
  const int n= deg(p) + 1, m= deg(q) + 1; \
  assert(m != 0); \
  mod_t ret= 0; \
  if (n == 0) return ret; \
  if (m == 1) return k <= (uint64_t)n ? p[k] / q[0] : ret; \
  if (k >= m) { \
   if constexpr (is_nttfriend<mod_t, LM>()) div_at_ntt_fast<LM>(p, q, k); \
   else div_at_ntt<LM>(p, q, k); \
  } \
  p.resize(k + 1, ret), q.resize(k + 1, ret), q= inv<mod_t, LM>(q); \
  for (int i= k; i >= 0; i--) ret+= q[i] * p[k - i]; \
  return ret; \
 }

__FPS_DIVAT(std::vector<mod_t>)
#ifdef __POLYNOMIAL
__FPS_DIVAT(__POLYNOMIAL)
#endif
// a[n] = c[0] * a[n-1] + c[1] * a[n-2] + ... + c[d-1] * a[n-d]
// return a[k]
template <class mod_t, size_t LM= 1 << 22> mod_t linear_recurrence(std::vector<mod_t> c, std::vector<mod_t> a, uint64_t k) {
 const size_t d= c.size();
 assert(d <= a.size());
 for (auto &x: c) x= -x;
 c.insert(c.begin(), mod_t(1)), a.resize(d);
 auto p= convolve<mod_t, LM>(c, a);
 return p.resize(d), div_at<mod_t, LM>(p, c, k);
}
