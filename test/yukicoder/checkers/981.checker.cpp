// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/981
// yuki 981 一般冪乗根: self-validating checker.
// Reproduces the official Java judge's logic in C++.
// Args (run-lib.sh testlib convention): input user_output expected
//   - expected is unused; the checker self-verifies via x^k == a (mod p).
#include <cstdio>
#include <cstdint>
#include <fstream>
#include <utility>
using i128 = __int128;

static long long mulmod(long long a, long long b, long long p) {
 return (long long)((i128)a * b % p);
}
static long long powmod(long long a, long long n, long long p) {
 long long r = 1 % p;
 a %= p; if(a < 0) a += p;
 while(n > 0) {
  if(n & 1) r = mulmod(r, a, p);
  a = mulmod(a, a, p);
  n >>= 1;
 }
 return r;
}
static long long gcdll(long long a, long long b) {
 while(b) { a %= b; std::swap(a, b); }
 return a;
}

int main(int argc, char** argv) {
 if(argc < 3) { std::fprintf(stderr, "usage: checker input user_output [expected]\n"); return 2; }
 std::ifstream in(argv[1]);
 std::ifstream out(argv[2]);
 if(!in || !out) { std::fprintf(stderr, "open failed\n"); return 2; }

 int T;
 if(!(in >> T)) { std::fprintf(stderr, "input parse: T\n"); return 2; }
 for(int t = 0; t < T; ++t) {
  long long p, k, a;
  if(!(in >> p >> k >> a)) { std::fprintf(stderr, "input parse: case %d\n", t); return 2; }
  long long x;
  if(!(out >> x)) { std::fprintf(stderr, "case %d: missing user output\n", t); return 1; }

  long long g = gcdll(k, p - 1);
  if(powmod(a, (p - 1) / g, p) != 1) {
   if(x != -1) { std::fprintf(stderr, "case %d: no solution but user gave %lld\n", t, x); return 1; }
  } else {
   if(x == -1) { std::fprintf(stderr, "case %d: solution exists but user said -1\n", t); return 1; }
   if(x < 0 || x >= p) { std::fprintf(stderr, "case %d: x=%lld out of [0,%lld)\n", t, x, p); return 1; }
   if(powmod(x, k, p) != a) { std::fprintf(stderr, "case %d: %lld^%lld mod %lld != %lld\n", t, x, k, p, a); return 1; }
  }
 }
 long long extra;
 if(out >> extra) { std::fprintf(stderr, "extra output present\n"); return 1; }
 return 0;
}
