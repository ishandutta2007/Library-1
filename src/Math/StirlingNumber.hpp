#pragma once
#include <bits/stdc++.h>
#include "src/Math/is_prime.hpp"
class StirlingNumber {
 const std::uint16_t p;
 std::vector<std::vector<std::uint16_t>> c, s1, s2;
 void buildS1() {
  s1.resize(p), s1[0]= {1};
  for (int i= 1, j, t; i < p; s1[i][i]= 1, i++)
   for (s1[i].resize(i + 1, 0), j= 1, t= p - i + 1; j < i; j++) s1[i][j]= (t * s1[i - 1][j] + s1[i - 1][j - 1]) % p;
 }
 void buildS2() {
  s2.resize(p), s2[0]= {1};
  for (int i= 1, j; i < p; s2[i][i]= 1, i++)
   for (s2[i].resize(i + 1, 0), j= 1; j < i; j++) s2[i][j]= (j * s2[i - 1][j] + s2[i - 1][j - 1]) % p;
 }
public:
 StirlingNumber(std::uint32_t p_, bool first= true, bool second= true): p(p_), c(p) {
  assert(is_prime(p_)), assert(p_ < (1 << 15)), c[0]= {1};
  for (int i= 1, j; i < p; i++)
   for (c[i]= c[i - 1], c[i].emplace_back(0), j= 1; j <= i; j++) c[i][j]-= p & -((c[i][j]+= c[i - 1][j - 1]) >= p);
  if (first) buildS1();
  if (second) buildS2();
 }
 int nCk(std::uint64_t n, std::uint64_t k) {
  if (k > n) return 0;
  int ret= 1, i, j;
  for (k= std::min(k, n - k); k; ret= ret * c[i][j] % p, n/= p, k/= p)
   if (i= n % p, j= k % p; j > i) return 0;
  return ret;
 }
 int S1(std::uint64_t n, std::uint64_t k) {
  if (k > n) return 0;
  std::uint64_t i= n / p;
  if (i > k) return 0;
  std::int64_t a= (k - i) / (p - 1);
  std::uint16_t j= n % p, b= (k - i) % (p - 1);
  if (!b && j) b+= (p - 1), a-= 1;
  if (a < 0 || i < a || b > j) return 0;
  return (j= nCk(i, a) * s1[j][b] % p) && ((i + a) & 1) ? p - j : j;
 }
 int S2(std::uint64_t n, std::uint64_t k) {
  if (k > n) return 0;
  if (!n) return 1;
  std::uint64_t i= k / p;
  if (n <= i) return 0;
  std::uint64_t a= (n - i - 1) / (p - 1);
  std::uint16_t j= k % p, b= (n - i) - a * (p - 1);
  if (j > b) return 0;
  return b == p - 1 && !j ? nCk(a, i - 1) : nCk(a, i) * s2[b][j] % p;
 }
};