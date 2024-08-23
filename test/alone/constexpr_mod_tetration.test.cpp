// competitive-verifier: STANDALONE

#include "src/Math/mod_tetration.hpp"

constexpr auto f= mod_tetration(2, 3, 1000000007);
static_assert(f == 16);
constexpr auto g= mod_tetration(2, 10000, 998244353);
static_assert(g == 220050301);
signed main() { return 0; }