#pragma once
#include <vector>
#include <cstdint>
namespace famous_arithmetic_functions {
namespace mul {
template <class T> struct Totient {
 static constexpr T f(uint64_t p, short e) {
  T ret= p - 1;
  while (--e) ret*= p;
  return ret;
 }
 static std::vector<T> poly() { return {-1, 1}; }
};
template <class T> struct Moebius {
 static constexpr T f(uint64_t, short e) { return (e == 0) - (e == 1); }
 static std::vector<T> poly() { return {-1}; }
};
template <class T> struct Liouville {
 static constexpr T f(uint64_t, short e) { return e & 1 ? -1 : 1; }
 static std::vector<T> poly() { return {-1}; }
};
template <class T, uint64_t k> struct Divisor {
 static constexpr T f(uint64_t p, short e) {
  T ret= 0, pk= 1, pkpw= 1, b= p;
  for (uint64_t kk= k; kk; kk>>= 1, b*= b)
   if (kk & 1) pk*= b;
  for (short i= 0; i <= e; i++, pkpw*= pk) ret+= pkpw;
  return ret;
 }
 static std::vector<T> poly() {
  std::vector<T> ret(k + 1, 0);
  return ret[0]+= 1, ret[k]+= 1, ret;
 }
};
template <class T> struct Dedekind {
 static constexpr T f(uint64_t p, short e) {
  T ret= p + 1;
  while (e-- > 1) ret*= p;
  return ret;
 }
 static std::vector<T> poly() { return {1, 1}; }
};
}  // namespace mul
namespace add {
template <class T> struct BigOmega {  // the total number of prime factors of n
 static constexpr T f(uint64_t, short e) { return e; }
 static std::vector<T> poly() { return {1}; }
};
template <class T> struct LittleOmega {  // the total number of different prime factors of n
 static constexpr T f(uint64_t, short) { return 1; }
 static std::vector<T> poly() { return {1}; }
};
template <class T> struct Sopfr {  // the sum of primes dividing n counting multiplicity
 static constexpr T f(uint64_t p, short e) { return p * e; }
 static std::vector<T> poly() { return {0, 1}; }
};
template <class T> struct Sopf {  // the sum of the distinct primes dividing n
 static constexpr T f(uint64_t p, short) { return p; }
 static std::vector<T> poly() { return {0, 1}; }
};
}  // namespace add
}