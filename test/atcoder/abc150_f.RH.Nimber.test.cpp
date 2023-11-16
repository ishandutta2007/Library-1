#define PROBLEM "https://atcoder.jp/contests/abc150/tasks/abc150_f"
#include <iostream>
#include <vector>
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
#include "src/Math/Nimber.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 Nimber::init();
 using RH= RollingHash<Nimber, Nimber>;
 Nimber base= rng(2, 1ll << 60);
 RH::init(base);
 int N;
 cin >> N;
 vector<Nimber> a(N), b(N);
 for (int i= 0; i < N; ++i) cin >> a[i];
 for (int i= 0; i < N; ++i) cin >> b[i];
 RH ra(a), rb(b);
 auto X= (base.pow(N) - 1) / (base - 1);
 auto bh= rb.hash();
 for (int k= 0; k < N; ++k) {
  auto x= a[k] + b[0];
  if (concat_hash(ra.sub(k), ra.sub(0, k)) + x * X == bh) cout << k << " " << x << '\n';
 }
 return 0;
}