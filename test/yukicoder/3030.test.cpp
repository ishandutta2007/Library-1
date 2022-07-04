#define PROBLEM "https://yukicoder.me/problems/no/3030"
#include <bits/stdc++.h>
#include "src/Math/is_prime.hpp"

using namespace std;

signed main() {
  cin.tie(0);
  ios::sync_with_stdio(false);
  int n;
  cin >> n;
  while (n--) {
    long long x;
    cin >> x;
    cout << is_prime(x) << '\n';
  }
  return 0;
}