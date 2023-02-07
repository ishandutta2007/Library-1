#define PROBLEM "https://yukicoder.me/problems/no/1124"
#include <iostream>
#include <array>
#include "src/Math/ModInt.hpp"
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int A, B;
  cin >> A >> B;
  tree.add_edge(--A, --B);
 }
 tree.build();
 using Data= array<Mint, 2>;
 static constexpr Mint iv2= Mint(1) / 2;
 auto f_ee= [&](const Data &l, const Data &r) { return Data{l[0] + r[0], l[1] + l[0] * r[0] * 2 + r[1]}; };
 auto f_ve= [&](const Data &d, int, auto) { return Data{d[0] * iv2, d[1] * iv2}; };
 auto f_ev= [&](const Data &d, int) { return Data{d[0] + 1, d[1] + d[0] * 2 + 1}; };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 Mint ans= 0;
 for (const auto &x: dp) ans+= x[1];
 ans*= Mint(2).pow(N - 1);
 cout << ans << '\n';
 return 0;
}