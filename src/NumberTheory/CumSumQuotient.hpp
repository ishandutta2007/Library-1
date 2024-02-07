#pragma once
#include <cstdint>
#include <valarray>
template <class T> struct CumSumQuotient {
 uint64_t N;
 size_t K;
 std::valarray<T> X;
 CumSumQuotient(uint64_t N): N(N), K(std::sqrt(N)), X(K + K + 1) {}
 T &operator[](uint64_t i) { return i > K ? X[K + double(N) / i] : X[i]; }
 const T &operator()(uint64_t i) const { return i > K ? X[K + double(N) / i] : X[i]; }
 CumSumQuotient &operator+=(const CumSumQuotient &r) { return X+= r.X, *this; }
 CumSumQuotient &operator-=(const CumSumQuotient &r) { return X-= r.X, *this; }
 CumSumQuotient &operator*=(T a) { return X*= a, *this; }
 CumSumQuotient operator-() const {
  CumSumQuotient ret= *this;
  return ret.X= -ret.X, ret;
 }
 CumSumQuotient operator+(const CumSumQuotient &r) const { return CumSumQuotient(*this)+= r; }
 CumSumQuotient operator-(const CumSumQuotient &r) const { return CumSumQuotient(*this)-= r; }
 CumSumQuotient operator*(T a) const { return CumSumQuotient(*this)*= a; }
 friend CumSumQuotient operator*(T a, const CumSumQuotient &x) { return x * a; }
 void add(uint64_t i, T v) {
  for (size_t j= std::min<uint64_t>(N / i, K) + K; j >= i; --j) X[j]+= v;
 }
 T sum() const { return X[K + 1]; }
};