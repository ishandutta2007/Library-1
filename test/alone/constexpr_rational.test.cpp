// competitive-verifier: STANDALONE

#include "src/Math/Rational.hpp"

using Q= Rational<__int128_t>;
constexpr auto x= Q(2, 3), y= Q(4, 3);
constexpr auto a= x + y;
static_assert(a == 2);
constexpr auto b= x - y;
static_assert(b == Q(-2, 3));
constexpr auto c= x * y;
static_assert(c == Q(8, 9));
constexpr auto d= x / y;
static_assert(d == Q(1, 2));
constexpr auto e= -x;
static_assert(e == Q(-2, 3));
constexpr auto f= y.floor();
static_assert(f == 1);
constexpr auto g= b.floor();
static_assert(g == -1);
static_assert(x < y == true);
signed main() { return 0; }