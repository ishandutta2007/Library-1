// competitive-verifier: STANDALONE

// https://codeforces.com/contest/1140/problem/F
#include <sstream>
#include <string>
#include <cassert>
#include <unordered_set>
#include "src/DataStructure/OnlineDynamicConnectivity.hpp"
using namespace std;
bool test(int (*solve)(stringstream&, stringstream&), string in, string expected) {
 stringstream scin(in), scout;
 solve(scin, scout);
 return scout.str() == expected;
}
namespace TEST {
struct M {
 using T= pair<int, int>;
 static T op(T a, T b) { return {a.first + b.first, a.second + b.second}; }
 static T ti() { return {0, 0}; }
};
signed main(stringstream& scin, stringstream& scout) {
 int ofs= 3e5;
 OnlineDynamicConnectivity<M> dicon(ofs + ofs);
 for (int i= ofs + ofs; i-- > ofs;) dicon.set(i, {0, 1});
 for (int i= ofs; i--;) dicon.set(i, {1, 0});
 int q;
 scin >> q;
 unordered_set<long long> S;
 long long ans= 0;
 while (q--) {
  long long x, y;
  scin >> x >> y, --x, --y;
  auto it= S.find(x * ofs + y);
  if (it != S.end()) {
   S.erase(it);
   {
    auto [a, b]= dicon.prod(x);
    ans-= (long long)a * b;
   }
   dicon.cut(x, y + ofs);
   {
    auto [a, b]= dicon.prod(x);
    ans+= (long long)a * b;
   }
   if (!dicon.connected(x, y + ofs)) {
    auto [a, b]= dicon.prod(y + ofs);
    ans+= (long long)a * b;
   }
  } else {
   S.insert(x * ofs + y);
   {
    auto [a, b]= dicon.prod(x);
    ans-= (long long)a * b;
   }
   if (!dicon.connected(x, y + ofs)) {
    auto [a, b]= dicon.prod(y + ofs);
    ans-= (long long)a * b;
   }
   dicon.link(x, y + ofs);
   {
    auto [a, b]= dicon.prod(x);
    ans+= (long long)a * b;
   }
  }
  scout << ans << " \n"[q == 0];
 }
 return 0;
}
}
signed main() {
 assert(test(TEST::main, "7\n1 1\n1 2\n2 1\n2 2\n1 2\n1 3\n2 1\n", "1 2 4 4 4 6 3\n"));
 return 0;
}