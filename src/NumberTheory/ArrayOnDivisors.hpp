#pragma once
#include "src/NumberTheory/Factors.hpp"
template <class T> class ArrayOnDivisors {
 uint64_t n;
 uint8_t shift;
 std::vector<int> os, id;
 std::vector<std::pair<uint64_t, T>> dat;
 unsigned hash(uint64_t i) const { return (i * 11995408973635179863ULL) >> shift; }
#define _UP for (int j= k; j < a; ++j)
#define _DWN for (int j= a; j-- > k;)
#define _OP(J, K, op) dat[i + J].second op##= dat[i + K].second
#define _ZETA(op) \
 int k= 1; \
 for (auto [p, e]: factors) { \
  int a= k * (e + 1); \
  for (int i= 0, d= dat.size(); i < d; i+= a) op; \
  k= a; \
 }
public:
 Factors factors;
 template <class Uint> ArrayOnDivisors(uint64_t n, const Factors &factors, const std::vector<Uint> &divisors): n(n), shift(__builtin_clzll(divisors.size()) - 1), os((1 << (64 - shift)) + 1), id(divisors.size()), dat(divisors.size()), factors(factors) {
  for (int i= divisors.size(); i--;) dat[i].first= divisors[i];
  for (auto d: divisors) ++os[hash(d)];
  std::partial_sum(os.begin(), os.end(), os.begin());
  for (int i= divisors.size(); i--;) id[--os[hash(divisors[i])]]= i;
 }
 ArrayOnDivisors(uint64_t n, const Factors &factors): ArrayOnDivisors(n, factors, enumerate_divisors(factors)) {}
 ArrayOnDivisors(uint64_t n): ArrayOnDivisors(n, Factors(n)) {}
 T &operator[](uint64_t i) {
  assert(i && n % i == 0);
  for (unsigned a= hash(i), j= os[a]; j < os[a + 1]; ++j)
   if (auto &[d, v]= dat[id[j]]; d == i) return v;
  assert(0);
 }
 size_t size() const { return dat.size(); }
 auto begin() { return dat.begin(); }
 auto begin() const { return dat.begin(); }
 auto end() { return dat.begin() + os.back(); }
 auto end() const { return dat.begin() + os.back(); }
 /* f -> g s.t. g(n) = sum_{m|n} f(m) */
 void divisor_zeta() { _ZETA(_UP _OP(j, j - k, +)) }
 /* f -> h s.t. f(n) = sum_{m|n} h(m) */
 void divisor_mobius() { _ZETA(_DWN _OP(j, j - k, -)) }
 /* f -> g s.t. g(n) = sum_{n|m} f(m) */
 void multiple_zeta() { _ZETA(_DWN _OP(j - k, j, +)) }
 /* f -> h s.t. f(n) = sum_{n|m} h(m) */
 void multiple_mobius() { _ZETA(_UP _OP(j - k, j, -)) }
#undef _UP
#undef _DWN
#undef _OP
#undef _ZETA
};