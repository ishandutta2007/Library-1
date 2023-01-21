#define PROBLEM "https://atcoder.jp/contests/abc172/tasks/abc172_d"
// O(N^(3/4)/log N)
#include <iostream>
#include "src/Math/prime_count.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 auto f= [](long long p, short e) {
  long long ret= e + 1;
  while (e--) ret*= p;
  return ret;
 };
 long long N;
 cin >> N;
 cout << (long long)multiplicative_sum<>(N, f, {0, 2}) << '\n';
 return 0;
}