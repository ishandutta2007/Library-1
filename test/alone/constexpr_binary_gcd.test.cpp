// competitive-verifier: STANDALONE

#include "src/Math/binary_gcd.hpp"

constexpr auto f= binary_gcd(2, 4);
static_assert(f == 2);
constexpr auto g= binary_gcd(1000000007, 1000000009);
static_assert(g == 1);
signed main() { return 0; }