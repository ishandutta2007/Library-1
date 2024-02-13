#define PROBLEM "https://judge.yosupo.jp/problem/rooted_tree_isomorphism_classification"
#include <iostream>
#include "src/Misc/compress.hpp"
#include "src/Math/ModInt.hpp"
#include "src/Graph/Graph.hpp"
#include "src/Graph/Rerooting.hpp"
#include "src/Misc/rng.hpp"
#include "src/Misc/Pointwise.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Graph g;
 for (int i= 1, p; i < N; ++i) cin >> p, g.emplace_back(p, i);
 g.build(N, 0);
 using Mint= ModInt<(1ll << 61) - 1>;
 using K= Pointwise<Mint, Mint>;
 using Data= pair<int, K>;
 vector<K> hash(N);
 for (auto& x: hash) x= {rng(2, Mint::mod() - 1), rng(2, Mint::mod() - 1)};
 auto put_edge= [&](int, int, const Data& d) { return Data{d.first, d.second + hash[d.first]}; };
 auto op= [&](const Data& l, const Data& r) { return Data{max(l.first, r.first), l.second * r.second}; };
 auto put_vertex= [&](int, const Data& d) { return Data{d.first + 1, d.second}; };
 Rerooting<Data> dp(g, put_edge, op, Data{0, K{1, 1}}, put_vertex);
 vector<K> ans(N);
 for (int i= 0; i < N; ++i) ans[i]= dp(0, i).second;
 auto vec= ans;
 auto id= compress(vec);
 cout << vec.size() << '\n';
 for (int i= 0; i < N; ++i) cout << id(ans[i]) << " \n"[i == N - 1];
 return 0;
}