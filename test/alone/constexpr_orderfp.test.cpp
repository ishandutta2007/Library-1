// competitive-verifier: STANDALONE

#include "src/NumberTheory/OrderFp.hpp"

constexpr auto ord998= OrderFp(998244353);
constexpr int g= ord998.primitive_root();
static_assert(g == 3);
constexpr int o1= ord998(1);
constexpr int o2= ord998(2);
constexpr int o3= ord998(3);
constexpr int o4= ord998(4);
constexpr int o0= ord998(998244353);
static_assert(o1 == 1);
static_assert(o2 == 499122176);
static_assert(o3 == 998244352);
static_assert(o4 == 249561088);
static_assert(o0 == 0);
signed main() { return 0; }