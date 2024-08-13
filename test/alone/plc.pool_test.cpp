// competitive-verifier: STANDALONE

// メモリプールが汚れた状態で PiecewiseLinearConvex を使っても正しく動作するかテストする。
// その他，関数に仕込んだ assert が落ちないことも確認する。
#include <iostream>
#include "src/Optimization/PiecewiseLinearConvex.hpp"
using namespace std;
namespace test1 {
vector<long long> solve(const vector<pair<int, int>> &init, const vector<tuple<int, int, int>> &querys) {
 vector<long long> ans;
 using PLC= PiecewiseLinearConvex<int>;
 PLC f(init);
 for (auto [op, a, b]: querys) {
  if (op == 0) {
   f.shift(a);
  } else if (op == 1) {
   f.add_ramp(a, b);
  } else if (op == 2) {
   f.add_abs(a, b);
  } else if (op == 3) {
   f.add_const(a);
  } else if (op == 4) {
   f.chmin_cum();
  } else if (op == 5) {
   f.chmin_cum(true);
  } else if (op == 6) {
   f.chmin_slide_win(a, b);
  }
  ans.push_back(f.min());
 }
 PLC::reset();
 return ans;
}
void test() {
 int N= 10;
 vector<pair<int, int>> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % 100 - 50, rand() % 100 - 50);
 int Q= 100;
 vector<tuple<int, int, int>> querys;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 7;
  int a= 0, b= 0;
  if (op == 0) {
   a= rand() % 100 - 50;
  } else if (op == 1) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
  } else if (op == 2) {
   a= rand() % 50, b= rand() % 100 - 50;
  } else if (op == 3) {
   a= rand() % 100 - 50;
  } else if (op == 4) {
  } else if (op == 5) {
  } else if (op == 6) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
   if (a > b) swap(a, b);
  }
  querys.emplace_back(op, a, b);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
namespace test2 {
vector<long long> solve(const std::vector<pair<int, int>> &init, const vector<tuple<int, int, int, int>> &querys) {
 vector<long long> ans;
 using PLC= PiecewiseLinearConvex<int>;
 PLC f(init);
 for (auto [op, a, b, c]: querys) {
  if (op == 0) {
   f.add_max(a, b, c);
  } else if (op == 1) {
   f.add_ramp(a, b);
  } else if (op == 2) {
   f.add_abs(a, b);
  } else if (op == 3) {
   f.add_const(a);
  } else if (op == 4) {
   f.chmin_slide_win(a, b);
  } else if (op == 5) {
   f.add_inf(true, a);
  } else if (op == 6) {
   f.add_inf(false, a);
  } else if (op == 7) {
   f.add_linear(a);
  }
  ans.push_back(f(0));
 }
 PLC::reset();
 return ans;
}
void test() {
 int N= 10;
 vector<pair<int, int>> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % 100 - 50, rand() % 100 - 50);
 int Q= 100;
 vector<tuple<int, int, int, int>> querys;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 8;
  int a= 0, b= 0, c= 0;
  if (op == 0) {
   a= rand() % 100 - 50, b= rand() % 100 - 50, c= rand() % 100 - 50;
   if (a > b) swap(a, b);
   if (a == b) ++b;
  } else if (op == 1) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
  } else if (op == 2) {
   a= rand() % 50, b= rand() % 100 - 50;
  } else if (op == 3) {
   a= rand() % 100 - 50;
  } else if (op == 4) {
   a= -(rand() % 50), b= rand() % 50;
  } else if (op == 5) {
   a= rand() % 50;
  } else if (op == 6) {
   a= -(rand() % 50);
  } else if (op == 7) {
   a= rand() % 100 - 50;
  }
  querys.emplace_back(op, a, b, c);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
namespace test3 {
vector<long long> solve(const vector<pair<int, int>> &init, const vector<tuple<int, int, int>> &querys) {
 vector<long long> ans;
 using PLC= PiecewiseLinearConvex<int>;
 PLC f(init);
 f.add_inf(), f.add_inf(true);
 for (auto [op, a, b]: querys) {
  if (op == 0) {
   f.shift(a);
  } else if (op == 1) {
   f.add_ramp(a, b);
  } else if (op == 2) {
   f.add_abs(a, b);
  } else if (op == 3) {
   f.add_const(a);
  } else if (op == 4) {
   f.chmin_slide_win(a, b);
  }
  auto [l, r]= f.argmin();
  ans.push_back(l);
  ans.push_back(r);
  ans.push_back(f.min());
 }
 PLC::reset();
 return ans;
}
void test() {
 int N= 10;
 vector<pair<int, int>> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % 100 - 50, rand() % 100 - 50);
 int Q= 100;
 vector<tuple<int, int, int>> querys;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 5;
  int a= 0, b= 0;
  if (op == 0) {
   a= rand() % 100 - 50;
  } else if (op == 1) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
  } else if (op == 2) {
   a= rand() % 50, b= rand() % 100 - 50;
  } else if (op == 3) {
   a= rand() % 100 - 50;
  } else if (op == 4) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
   if (a > b) swap(a, b);
  }
  querys.emplace_back(op, a, b);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
namespace test4 {
vector<long long> solve(const vector<pair<int, int>> &init, const vector<tuple<int, int, int>> &querys) {
 vector<long long> ans;
 using PLC= PiecewiseLinearConvex<int>;
 PLC f(init);
 f.add_inf(), f.add_inf(true);
 for (auto [op, a, b]: querys) {
  if (op == 0) {
   f.add_ramp(a, b);
  } else if (op == 1) {
   f.add_abs(a, b);
  } else if (op == 2) {
   f.add_const(a);
  } else if (op == 3) {
   f.chmin_slide_win(a, b);
  } else if (op == 4) {
   f.chmin_cum();
  } else if (op == 5) {
   f.chmin_cum(true);
  } else if (op == 6) {
   f.add_inf(true, a);
  } else if (op == 7) {
   f.add_inf(false, a);
  }
  ans.push_back(f.min());
 }
 PLC::reset();
 return ans;
}
void test() {
 int N= 10;
 vector<pair<int, int>> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % 100 - 50, rand() % 100 - 50);
 int Q= 100;
 vector<tuple<int, int, int>> querys;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 8;
  int a= 0, b= 0;
  if (op == 0) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
  } else if (op == 1) {
   a= rand() % 50, b= rand() % 100 - 50;
  } else if (op == 2) {
   a= rand() % 100 - 50;
  } else if (op == 3) {
   a= -(rand() % 50), b= rand() % 50;
  } else if (op == 4) {
  } else if (op == 5) {
  } else if (op == 6) {
   a= rand() % 50;
  } else if (op == 7) {
   a= -(rand() % 50);
  }
  querys.emplace_back(op, a, b);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
namespace test_persistent_1 {
vector<long long> solve(const vector<pair<int, int>> &init, const vector<tuple<int, int, int, int>> &querys) {
 vector<long long> ans;
 using PLC= PiecewiseLinearConvex<int, true>;
 vector<PLC> fs;
 fs.emplace_back(init);
 for (auto [op, s, a, b]: querys) {
  PLC f= fs[s];
  if (op == 0) {
   f.shift(a);
  } else if (op == 1) {
   f.add_ramp(a, b);
  } else if (op == 2) {
   f.add_abs(a, b);
  } else if (op == 3) {
   f.add_const(a);
  } else if (op == 4) {
   f.chmin_cum();
  } else if (op == 5) {
   f.chmin_cum(true);
  } else if (op == 6) {
   f.chmin_slide_win(a, b);
  } else if (op == 7) {
   f+= fs[a];
  }
  ans.push_back(f.min());
  fs.push_back(f);
 }
 PLC::reset();
 return ans;
}
void test() {
 int N= 10;
 vector<pair<int, int>> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % 100 - 50, rand() % 100 - 50);
 int Q= 100;
 vector<tuple<int, int, int, int>> querys;
 int cnt= 10;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 8;
  int s= rand() % (i + 1);
  int a= 0, b= 0;
  if (op == 0) {
   a= rand() % 100 - 50;
  } else if (op == 1) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
  } else if (op == 2) {
   a= rand() % 50, b= rand() % 100 - 50;
  } else if (op == 3) {
   a= rand() % 100 - 50;
  } else if (op == 4) {
  } else if (op == 5) {
  } else if (op == 6) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
   if (a > b) swap(a, b);
  } else if (op == 7) {
   if (cnt) {
    --cnt;
    a= rand() % (i + 1);
   } else {
    op= 1;
    a= rand() % 100 - 50, b= rand() % 100 - 50;
   }
  }
  querys.emplace_back(op, s, a, b);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
namespace test_persistent_2 {
vector<long long> solve(const std::vector<pair<int, int>> &init, const vector<tuple<int, int, int, int, int>> &querys) {
 vector<long long> ans;
 using PLC= PiecewiseLinearConvex<int, true>;
 vector<PLC> fs;
 fs.emplace_back(init);
 for (auto [op, s, a, b, c]: querys) {
  PLC f= fs[s];
  if (op == 0) {
   f.add_max(a, b, c);
  } else if (op == 1) {
   f.add_ramp(a, b);
  } else if (op == 2) {
   f.add_abs(a, b);
  } else if (op == 3) {
   f.add_const(a);
  } else if (op == 4) {
   f.chmin_slide_win(a, b);
  } else if (op == 5) {
   f.add_inf(true, a);
  } else if (op == 6) {
   f.add_inf(false, a);
  } else if (op == 7) {
   f.add_linear(a);
  } else if (op == 8) {
   f+= fs[a];
  }
  ans.push_back(f(0));
  fs.push_back(f);
 }
 PLC::reset();
 return ans;
}
void test() {
 int N= 10;
 vector<pair<int, int>> init;
 for (int i= 0; i < N; ++i) init.emplace_back(rand() % 100 - 50, rand() % 100 - 50);
 int Q= 100;
 vector<tuple<int, int, int, int, int>> querys;
 int cnt= 10;
 for (int i= 0; i < Q; ++i) {
  int op= rand() % 9;
  int s= rand() % (i + 1);
  int a= 0, b= 0, c= 0;
  if (op == 0) {
   a= rand() % 100 - 50, b= rand() % 100 - 50, c= rand() % 100 - 50;
   if (a > b) swap(a, b);
   if (a == b) ++b;
  } else if (op == 1) {
   a= rand() % 100 - 50, b= rand() % 100 - 50;
  } else if (op == 2) {
   a= rand() % 50, b= rand() % 100 - 50;
  } else if (op == 3) {
   a= rand() % 100 - 50;
  } else if (op == 4) {
   a= -(rand() % 50), b= rand() % 50;
  } else if (op == 5) {
   a= rand() % 50;
  } else if (op == 6) {
   a= -(rand() % 50);
  } else if (op == 7) {
   a= rand() % 100 - 50;
  } else if (op == 8) {
   if (cnt) {
    --cnt;
    a= rand() % (i + 1);
   } else {
    op= 1;
    a= rand() % 100 - 50, b= rand() % 100 - 50;
   }
  }
  querys.emplace_back(op, s, a, b, c);
 }
 auto ans1= solve(init, querys);
 auto ans2= solve(init, querys);
 assert(ans1 == ans2);
}
}
signed main() {
 for (int i= 20; i--;) test1::test();
 for (int i= 20; i--;) test2::test();
 for (int i= 20; i--;) test3::test();
 for (int i= 20; i--;) test4::test();
 for (int i= 20; i--;) test_persistent_1::test();
 for (int i= 20; i--;) test_persistent_2::test();
 return 0;
}