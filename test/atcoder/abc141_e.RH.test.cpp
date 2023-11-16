#define PROBLEM "https://atcoder.jp/contests/abc141/tasks/abc141_e"
#include <iostream>
#include <string>
#include <set>
#include "src/Math/CartesianProduct.hpp"
#include "src/String/RollingHash.hpp"
#include "src/Misc/rng.hpp"
#include "src/Math/ModInt.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 using K= CartesianProduct<Mint, Mint>;
 using RH= RollingHash<K>;
 RH::init({rng(), rng()});
 int N;
 cin >> N;
 string S;
 cin >> S;
 RH rh(S);
 multiset<K> st;
 int ok= 0, ng= N / 2 + 1;
 while (ng - ok > 1) {
  int x= (ok + ng) / 2;
  st.clear();
  bool isok= false;
  for (int i= x; i + x <= N; ++i) {
   st.insert(rh.sub(i - x, x).hash());
   isok|= st.count(rh.sub(i, x).hash());
  }
  (isok ? ok : ng)= x;
 }
 cout << ok << '\n';
 return 0;
}