// competitive-verifier: STANDALONE

#include "src/NumberTheory/is_prime.hpp"

constexpr auto f= is_prime(1000000007);
static_assert(f == true);
constexpr auto g= is_prime(1729);
static_assert(g == false);
signed main() { return 0; }