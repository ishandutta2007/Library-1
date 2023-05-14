#define PROBLEM "https://judge.yosupo.jp/problem/counting_primes"
#include <iostream>
#include "src/Math/prime_count.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(false);
 long long N;
 cin >> N;
 auto [_, pi]= prime_count_table(N);
 cout << pi.sum() << '\n';
 return 0;
}