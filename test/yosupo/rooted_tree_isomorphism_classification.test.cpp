#define PROBLEM "https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification"
#include <iostream>
#include "src/Misc/compress.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Graph/rerooting.hpp"
#include "src/Misc/rng.hpp"
#include "src/Math/CartesianProduct.hpp"
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
 using K= CartesianProduct<Mint, Mint>;
 using Data= pair<int, K>;
 vector<K> hash(N);
 for (auto& x: hash) x= {rng(2, Mint::mod() - 1), rng(2, Mint::mod() - 1)};
 auto f_ee= [&](const Data& l, const Data& r) { return Data{max(l.first, r.first), l.second * r.second}; };
 auto f_ve= [&](const Data& d, int, auto) { return Data{d.first, d.second + hash[d.first]}; };
 auto f_ev= [&](const Data& d, int) { return Data{d.first + 1, d.second}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, K{1, 1}});
 vector<K> ans(N);
 for (int i= 0; i < N; ++i) ans[i]= dp.get(0, i).second;
 auto vec= ans;
 auto id= compress(vec);
 cout << vec.size() << '\n';
 for (int i= 0; i < N; ++i) cout << id(ans[i]) << " \n"[i == N - 1];
 return 0;
}