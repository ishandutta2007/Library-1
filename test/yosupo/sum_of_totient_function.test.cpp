#define PROBLEM "https://judge.yosupo.jp/problem/sum_of_totient_function"
#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/dujiao_sieve.hpp"
#include "src/Math/NumberTheory.hpp"
#include "src/Math/multiplicative_and_additive.hpp"
using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  using Mint = ModInt<998244353>;
  using NT = NumberTheory;
  using namespace multiplicative_functions;
  const int M = 1 << (200 / 9);
  auto phi = NT::multiplicative_table<Mint>(M, Totient<Mint>::f);
  for (int i = 2; i < M; i++) phi[i] += phi[i - 1];
  auto hsum = [](long long N) { return Mint(N) * Mint(N + 1) / Mint(2); };
  auto gsum = [](long long N) { return Mint(N); };
  unordered_map<unsigned long long, Mint> memo;
  for (int i = 1; i < M; i++) memo[i] = phi[i];
  long long N;
  cin >> N;
  cout << dirichlet_div_sum<Mint>(N, gsum, hsum, memo) << '\n';
  return 0;
}