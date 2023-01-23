#pragma once
#include <array>
#include <tuple>
#include "src/FFT/Polynomial.hpp"
// ax + by = gcd(a, b)
template <class mod_t, std::size_t _Nm> Polynomial<mod_t, _Nm> extgcd(const Polynomial<mod_t, _Nm> &a, const Polynomial<mod_t, _Nm> &b, Polynomial<mod_t, _Nm> &x, Polynomial<mod_t, _Nm> &y) {
 using Poly= Polynomial<mod_t, _Nm>;
 using PolyMat= std::array<Poly, 4>;
 assert(a.deg() >= 0), assert(b.deg() >= 0);
#define SUF(f, bg, ed) Poly(f.begin() + bg, f.begin() + ed)
#define __COPY_HOGE \
 if (n= p3.deg(), k= 2 * m - n, o= qr.second.size(); o <= m) return R; \
 if (o <= (((n + 1) * 3 + k) >> 2)) return p3= SUF(p3, k, n + 1) / SUF(qr.second, k, o), PolyMat{R[2], R[3], R[0] - p3 * R[2], R[1] - p3 * R[3]}; \
 PolyMat A= self(self, SUF(p3, k, n + 1), SUF(qr.second, k, o)); \
 return {A[0] * R[0] + A[1] * R[2], A[0] * R[1] + A[1] * R[3], A[2] * R[0] + A[3] * R[2], A[2] * R[1] + A[3] * R[3]};
 auto hgcd= [&](auto self, const Poly &p0, const Poly &p1) -> PolyMat {
  int o= p0.deg(), m= (o + 1) / 2, n= p1.deg(), k= (o + m + 1) / 2;
  if (assert(o > n); n < (((o + 1) * 3 + m) >> 2)) {
   Poly tmp= SUF(p0, m, o + 1) / SUF(p1, m, n + 1), p3= p0 - tmp * p1;
   std::pair<Poly, Poly> qr= p1.quorem(p3);
   PolyMat R= {mod_t(1), -tmp, -qr.first, qr.first * tmp + mod_t(1)};
   __COPY_HOGE
  }
  PolyMat R= self(self, SUF(p0, m, o + 1), SUF(p1, m, n + 1));
  Poly p3= R[2] * p0 + R[3] * p1;
  std::pair<Poly, Poly> qr= (R[0] * p0 + R[1] * p1).quorem(p3);
  R[0].swap(R[2]), R[1].swap(R[3]), R[2]-= qr.first * R[0], R[3]-= qr.first * R[1];
  __COPY_HOGE
 };
#undef SUF
#undef __COPY_HOGE
 auto cogcd= [&](auto self, const Poly &p0, const Poly &p1) -> std::pair<Poly, Poly> {
  int o= p0.deg(), m= (o + 1) / 2, n= p1.deg(), k= (o + m + 1) / 2;
  if (assert(o > n); n < k) {
   std::pair<Poly, Poly> t= p0.quorem(p1);
   if (t.second.deg() == -1) return {Poly(), mod_t(1)};
   std::pair<Poly, Poly> qr= p1.quorem(t.second);
   if (qr.second.deg() == -1) return {mod_t(1), -t.first};
   auto A= self(self, t.second, qr.second);
   return A.first-= A.second * qr.first, std::make_pair(A.first, A.second - A.first * t.first);
  }
  PolyMat M= hgcd(hgcd, p0, p1);
  Poly p3= M[2] * p0 + M[3] * p1;
  if (p3.deg() == -1) return {M[0], M[1]};
  std::pair<Poly, Poly> qr= (M[0] * p0 + M[1] * p1).quorem(p3);
  if (qr.second.deg() == -1) return {M[2], M[3]};
  auto A= self(self, p3, qr.second);
  return A.first-= A.second * qr.first, std::make_pair(A.second * M[0] + A.first * M[2], A.second * M[1] + A.first * M[3]);
 };
 if (a.deg() <= b.deg()) {
  std::pair<Poly, Poly> qr= a.quorem(b);
  std::tie(y, x)= cogcd(cogcd, b, qr.second), y-= x * qr.first;
 } else std::tie(x, y)= cogcd(cogcd, a, b);
 return a * x + b * y;
}