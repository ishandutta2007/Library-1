#pragma once
#include "src/NumberTheory/Factors.hpp"
template <class Int, class T> struct ArrayOnDivisors {
 using Hint= std::conditional_t<sizeof(Int) == 64, unsigned, uint16_t>;
 Int n;
 uint8_t shift;
 std::vector<Hint> os, id;
 std::vector<std::pair<Int, T>> dat;
 Hint hash(uint64_t i) const { return (i * 11995408973635179863ULL) >> shift; }
#define _UP for (int j= k; j < a; ++j)
#define _DWN for (int j= a; j-- > k;)
#define _OP(J, K, op) dat[i + J].second op##= dat[i + K].second
#define _FUN(J, K, name) name(dat[i + J].second, dat[i + K].second)
#define _ZETA(op) \
 int k= 1; \
 for (auto [p, e]: factors) { \
  int a= k * (e + 1); \
  for (int i= 0, d= dat.size(); i < d; i+= a) op; \
  k= a; \
 }
public:
 Factors factors;
 ArrayOnDivisors() {}
 template <class Uint> ArrayOnDivisors(Int N, const Factors &factors, const std::vector<Uint> &divisors): n(N), shift(__builtin_clzll(divisors.size()) - 1), os((1 << (64 - shift)) + 1), id(divisors.size()), dat(divisors.size()), factors(factors) {
  static_assert(std::is_integral_v<Uint>, "Uint must be integral");
  for (int i= divisors.size(); i--;) dat[i].first= divisors[i];
  for (auto d: divisors) ++os[hash(d)];
  std::partial_sum(os.begin(), os.end(), os.begin());
  for (int i= divisors.size(); i--;) id[--os[hash(divisors[i])]]= i;
 }
 ArrayOnDivisors(Int N, const Factors &factors): ArrayOnDivisors(N, factors, enumerate_divisors(factors)) {}
 ArrayOnDivisors(Int N): ArrayOnDivisors(N, Factors(N)) {}
 T &operator[](Int i) {
  assert(i && n % i == 0);
  for (unsigned a= hash(i), j= os[a]; j < os[a + 1]; ++j)
   if (auto &[d, v]= dat[id[j]]; d == i) return v;
  assert(0);
 }
 const T &operator[](Int i) const {
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
 /* f -> g s.t. g(n) = sum_{m|n} f(m), add(T& a, T b): a+=b */
 template <class F> void divisor_zeta(const F &add) { _ZETA(_UP _FUN(j, j - k, add)) }
 /* f -> h s.t. f(n) = sum_{m|n} h(m), sub(T& a, T b): a-=b */
 template <class F> void divisor_mobius(const F &sub) { _ZETA(_UP _FUN(j, j - k, sub)) }
 /* f -> g s.t. g(n) = sum_{n|m} f(m), add(T& a, T b): a+=b */
 template <class F> void multiple_zeta(const F &add) { _ZETA(_UP _FUN(j - k, j, add)) }
 /* f -> h s.t. f(n) = sum_{n|m} h(m), sub(T& a, T b): a-=b */
 template <class F> void multiple_mobius(const F &sub) { _ZETA(_UP _FUN(j - k, j, sub)) }
#undef _UP
#undef _DWN
#undef _OP
#undef _ZETA
 // f(p,e): multiplicative function of p^e
 template <typename F> void set_multiplicative(const F &f) {
  int k= 1;
  dat[0].second= 1;
  for (auto [p, e]: factors)
   for (int m= k, d= 1; d <= e; ++d)
    for (int i= 0; i < m;) dat[k++].second= dat[i++].second * f(p, d);
 }
 void set_totient() {
  int k= 1;
  dat[0].second= 1;
  for (auto [p, e]: factors) {
   Int b= p - 1;
   for (int m= k; e--; b*= p)
    for (int i= 0; i < m;) dat[k++].second= dat[i++].second * b;
  }
 }
 void set_mobius() {
  set_multiplicative([](auto, auto e) { return e == 1 ? -1 : 0; });
 }
};