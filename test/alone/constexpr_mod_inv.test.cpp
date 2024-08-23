// competitive-verifier: STANDALONE

#include "src/Math/mod_inv.hpp"

constexpr auto f= mod_inv(2, 1000000007);
static_assert(f == 500000004);
constexpr auto g= mod_inv(3, 998244353);
static_assert(g == 332748118);
signed main() { return 0; }