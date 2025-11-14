// competitive-verifier: STANDALONE

#include "src/Math/mod_sqrt.hpp"

constexpr auto f= mod_sqrt(4, 1000000007);
static_assert(f == 2);
constexpr auto g= mod_sqrt(3, 998244353);
static_assert(g == -1);
signed main() { return 0; }