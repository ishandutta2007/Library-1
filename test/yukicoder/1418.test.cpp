#define PROBLEM "https://yukicoder.me/problems/no/1418"
#include <iostream>
#include <array>
#include "src/Graph/rerooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 Tree tree(N);
 for (int i= 0; i < N - 1; ++i) {
  int A, B;
  cin >> A >> B;
  tree.add_edge(--A, --B);
 }
 tree.build();
 using Data= array<long long, 2>;
 auto f_ee= [&](const Data &l, const Data &r) { return Data{l[0] + r[0], l[1] + r[1]}; };
 auto f_ve= [&](const Data &d, int, auto) { return d; };
 auto f_ev= [&](Data d, int) {
  ++d[0], d[1]+= d[0];
  return d;
 };
 auto dp= rerooting<Data>(tree, f_ee, f_ve, f_ev, Data{0, 0});
 long long ans= 0;
 for (const auto &x: dp) ans+= x[1];
 cout << ans << '\n';
 return 0;
}