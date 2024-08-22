// competitive-verifier: STANDALONE

// メモリプールが汚れた状態で WeightBalancedTree を使っても正しく動作するかテストする。
#include <cstdlib>
#include <cassert>
#include <vector>
#include <tuple>
#include "src/DataStructure/WeightBalancedTree.hpp"
using namespace std;
namespace test1 {
struct Mono {
 using T= int;
 using E= int;
 static T op(T l, T r) { return min(l, r); }
 static void mp(T &a, E b, T) { a= max(a, b); }
 static void cp(E &a, E b) { a= max(a, b); }
 using commute= void;
};
vector<int> solve(const vector<int> &init, const vector<tuple<int, int, int, int, int>> &querys) {
 vector<int> ans;
 using WBT= WeightBalancedTree<Mono, true>;
 WBT wbt(init);
 for (auto [op, l, r, a, b]: querys) {
  if (op == 0) {
   ans.push_back(wbt.prod(l, r));
  } else if (op == 1) {
   wbt.apply(l, r, a);
  } else if (op == 2) {
   wbt.reverse(l, r);
  } else if (op == 3) {
   wbt.mul(l, a);
  } else if (op == 4) {
   wbt.set(l, a);
  } else if (op == 5) {
   ans.push_back(wbt.get(l));
  } else if (op == 6) {
   wbt.insert(l, a);
  } else if (op == 7) {
   wbt.erase(l);
  }
  if (WBT::pool_empty()) {
   auto dmp= wbt.dump();
   WBT::reset();
   wbt= WBT(dmp);
  }
 }
 WBT::reset();
 return ans;
}
void test() {
 int N= 100;
 constexpr int M= 100000;
 vector<int> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % M);
 int Q= 100;
 vector<tuple<int, int, int, int, int>> querys;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 8;
  if (N < 10) {
   while (op == 7) op= rand() % 9;
  }
  int l= 0, r= 0, a= 0, b= 0;
  if (op == 0) {
   l= rand() % N, r= rand() % N;
   if (l > r) swap(l, r);
   ++r;
  } else if (op == 1) {
   l= rand() % N, r= rand() % N, a= rand() % M;
   if (l > r) swap(l, r);
   ++r;
  } else if (op == 2) {
   l= rand() % N, r= rand() % N;
   if (l > r) swap(l, r);
   ++r;
  } else if (op == 3) {
   l= rand() % N, a= rand() % M;
  } else if (op == 4) {
   l= rand() % N, a= rand() % M;
  } else if (op == 5) {
   l= rand() % N;
  } else if (op == 6) {
   l= rand() % N, a= rand() % M;
   ++N;
  } else if (op == 7) {
   l= rand() % N;
   --N;
  }
  querys.emplace_back(op, l, r, a, b);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
namespace test1_persistent {
struct Mono {
 using T= int;
 using E= int;
 static T op(T l, T r) { return min(l, r); }
 static void mp(T &a, E b, T) { a= max(a, b); }
 static void cp(E &a, E b) { a= max(a, b); }
 using commute= void;
};
vector<int> solve(const vector<int> &init, const vector<tuple<int, int, int, int, int>> &querys) {
 vector<int> ans;
 using WBT= WeightBalancedTree<Mono, true, true>;
 WBT wbt(init);
 for (auto [op, l, r, a, b]: querys) {
  if (op == 0) {
   ans.push_back(wbt.prod(l, r));
  } else if (op == 1) {
   wbt.apply(l, r, a);
  } else if (op == 2) {
   wbt.reverse(l, r);
  } else if (op == 3) {
   wbt.mul(l, a);
  } else if (op == 4) {
   wbt.set(l, a);
  } else if (op == 5) {
   ans.push_back(wbt.get(l));
  } else if (op == 6) {
   wbt.insert(l, a);
  } else if (op == 7) {
   wbt.erase(l);
  } else {
   auto [x, y, z]= wbt.split3(l, r);
   auto [c, d, e]= wbt.split3(a, b);
   wbt= c + y + e;
  }
  if (WBT::pool_empty()) {
   auto dmp= wbt.dump();
   WBT::reset();
   wbt= WBT(dmp);
  }
 }
 WBT::reset();
 return ans;
}
void test() {
 int N= 100;
 constexpr int M= 100000;
 vector<int> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % M);
 int Q= 100;
 vector<tuple<int, int, int, int, int>> querys;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 9;
  if (N < 10) {
   while (op == 7) op= rand() % 9;
  }
  int l= 0, r= 0, a= 0, b= 0;
  if (op == 0) {
   l= rand() % N, r= rand() % N;
   if (l > r) swap(l, r);
   ++r;
  } else if (op == 1) {
   l= rand() % N, r= rand() % N, a= rand() % M;
   if (l > r) swap(l, r);
   ++r;
  } else if (op == 2) {
   l= rand() % N, r= rand() % N;
   if (l > r) swap(l, r);
   ++r;
  } else if (op == 3) {
   l= rand() % N, a= rand() % M;
  } else if (op == 4) {
   l= rand() % N, a= rand() % M;
  } else if (op == 5) {
   l= rand() % N;
  } else if (op == 6) {
   l= rand() % N, a= rand() % M;
   ++N;
  } else if (op == 7) {
   l= rand() % N;
   --N;
  } else {
   l= rand() % N, r= rand() % N, a= rand() % N, b= rand() % N;
   if (l > r) swap(l, r);
   if (a > b) swap(a, b);
   ++r, ++b;
  }
  querys.emplace_back(op, l, r, a, b);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
signed main() {
 for (int i= 20; i--;) test1::test();
 for (int i= 20; i--;) test1_persistent::test();
 return 0;
}