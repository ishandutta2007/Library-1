// competitive-verifier: STANDALONE

#include "src/NumberTheory/Factors.hpp"

constexpr auto f= Factors(2 * 2 * 3 * 5);
static_assert(f.size() == 3);
static_assert(f[0].first == 2);
static_assert(f[0].second == 2);
static_assert(f[1].first == 3);
static_assert(f[1].second == 1);
static_assert(f[2].first == 5);
static_assert(f[2].second == 1);
constexpr int n= totient(100);
static_assert(n == 40);
signed main() { return 0; }