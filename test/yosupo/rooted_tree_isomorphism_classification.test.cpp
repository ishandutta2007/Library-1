#define PROBLEM "https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification"
#include <iostream>
#include <random>
#include "src/Misc/compress.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 1; i < N; ++i) {
  int p;
  cin >> p;
  tree.add_edge(p, i);
 }
 tree.build(0);
 using Mint= ModInt<(1ll << 61) - 1>;
 using Data= pair<int, Mint>;
 vector<Mint> hash(N);
 random_device rng;
 for (auto& x: hash) x= uniform_int_distribution<long long>(1, Mint::mod() - 1)(rng);
 auto f_ee= [&](const Data& l, const Data& r) { return Data{max(l.first, r.first), l.second * r.second}; };
 auto f_ve= [&](const Data& d, int, auto) { return Data{d.first, d.second + hash[d.first]}; };
 auto f_ev= [&](const Data& d, int) { return Data{d.first + 1, d.second}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, Mint(1)});
 vector<int> ans(N);
 for (int i= 0; i < N; ++i) ans[i]= dp.get(0, i).second.val();
 auto vec= ans;
 auto id= compress(vec);
 for (auto& x: ans) x= id(x);
 cout << *max_element(ans.begin(), ans.end()) + 1 << '\n';
 for (int i= 0; i < N; ++i) cout << ans[i] << " \n"[i == N - 1];
 return 0;
}