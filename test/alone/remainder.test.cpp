// competitive-verifier: STANDALONE
#include <iostream>
#include <random>
#include <cassert>
#include <cstdint>
#include "src/Internal/Remainder.hpp"
using namespace std;
template <class Rem> bool test(uint64_t mod) {
 Rem md(mod);
 mt19937 rng(0);
 uniform_int_distribution<uint64_t> dist(2, mod - 1);
 __uint128_t expect= dist(rng);
 uint64_t x= md.set(expect);
 if (md.get(x) != expect) return false;
 for (int i= 100; i--;) {
  {
   __uint128_t y= dist(rng);
   (expect+= y)%= mod;
   x= md.plus(x, md.set(y));
   if (md.get(x) != expect) return false;
  }
  {
   __uint128_t y= dist(rng);
   (expect+= mod - y)%= mod;
   x= md.diff(x, md.set(y));
   if (md.get(x) != expect) return false;
  }
  {
   __uint128_t y= dist(rng);
   (expect*= y)%= mod;
   x= md.mul(x, md.set(y));
   if (md.get(x) != expect) return false;
  }
 }
 return true;
}
signed main() {
 using namespace math_internal;
 assert(test<MP_Na>(998244353));
 assert(test<MP_Br>(998244353));
 assert(test<MP_Mo32>(998244353));

 assert(test<MP_Na>(1'000'000'007));
 assert(test<MP_Br>(1'000'000'007));
 assert(test<MP_Mo32>(1'000'000'007));

 assert(test<MP_Na>(1'000'000'009));
 assert(test<MP_Br>(1'000'000'009));
 assert(test<MP_Mo32>(1'000'000'009));

 assert(test<MP_D2B1_1>((1ull << 61) - 1));
 assert(test<MP_D2B1_2>((1ull << 61) - 1));
 assert(test<MP_Mo64>((1ull << 61) - 1));

 assert(test<MP_Na>(u32(-1)));
 assert(test<MP_Br>((1ull << 41) - 1));
 assert(test<MP_D2B1_1>((1ull << 63) - 1));
 assert(test<MP_D2B1_2>(u64(-1)));
 assert(test<MP_Mo64>((1ull << 62) - 1));
 {
  mt19937 rng(0);
  uniform_int_distribution<u32> dist(2, u32(-1));
  for (int i= 100; i--;) assert(test<MP_Na>(dist(rng)));
 }
 {
  mt19937_64 rng(0);
  uniform_int_distribution<u64> dist(1ull << 20, 1ull << 41);
  for (int i= 100; i--;) assert(test<MP_Br>(dist(rng)));
 }
 {
  mt19937_64 rng(0);
  uniform_int_distribution<u64> dist(2, (1ull << 63) - 1);
  for (int i= 100; i--;) assert(test<MP_D2B1_1>(dist(rng)));
 }
 {
  mt19937_64 rng(0);
  uniform_int_distribution<u64> dist((1ull << 63) - 1, u64(-1));
  for (int i= 100; i--;) assert(test<MP_D2B1_2>(dist(rng)));
 }
 {
  mt19937_64 rng(0);
  uniform_int_distribution<u64> dist(2, (1ull << 32) - 1);
  for (int i= 100; i--;) assert(test<MP_Mo32>(dist(rng) | 1));
 }
 {
  mt19937_64 rng(0);
  uniform_int_distribution<u64> dist(2, (1ull << 62) - 1);
  for (int i= 100; i--;) assert(test<MP_Mo64>(dist(rng) | 1));
 }
 return 0;
}
