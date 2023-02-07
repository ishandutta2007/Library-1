#pragma once
#include "src/Graph/Tree.hpp"
template <class T, class C> class RerootingData {
 Tree<C> &tree;
 std::vector<T> dp1, dp2, dp;
public:
RerootingData(Tree<C> &t,std::vector<T>& d1,std::vector<T>& d2,std::vector<T>& d):tree(t),dp1(d1),dp2(d2),dp(d){}
 T operator[](int v)const { return dp[v]; }
 auto begin() const { return dp.begin(); }
 auto end() const { return dp.end(); }
 const T& get(int root, int v)const{
  return root==v?dp[v]:tree.in_subtree(root,v)? dp2[tree.jump(v,root,1)]:dp1[v];
 }
};
template <class T, class U, class C,  class F1, class F2, class F3>
RerootingData<T,C> rerooting(Tree<C> &t, const F1 &f_ee, const F2 &f_ve, const F3 &f_ev, const U &unit){
  const int n= t.size();
  std::vector<T> dp1(n), dp2(n), dp(n);
  for (int i= n; i--;) {
   int v= t.to_node(i);
   U sum= unit;
   for (const auto &e: t[v])
    if (int u=e.to;u != t.parent(v)) sum= f_ee(sum, f_ve(dp1[u], v, e));
   dp1[v]= f_ev(sum, v);
  }
  for (int i= 0; i < n; ++i) {
   int v= t.to_node(i), deg= t[v].size();
   std::vector<U> f(deg + 1), b(deg + 1);
   for (int j= 0; j < deg; ++j) {
    const auto &e= t[v][j];
    int u= e.to;
    f[j + 1]= f_ve(u == t.parent(v) ? dp2[v] : dp1[u],v, e);
   }
   f[0]= b[deg]= unit;
   for (int j= deg; j--;) b[j]= f_ee(f[j + 1], b[j + 1]);
   for (int j= 0; j < deg; ++j) f[j + 1]= f_ee(f[j], f[j + 1]);
   for (int j= 0; j < deg; ++j) {
    const auto &e= t[v][j];
    if (int u= e.to;u != t.parent(v)) dp2[u]= f_ev(f_ee(f[j], b[j + 1]), v);
   }
   dp[v]= f_ev(f[deg], v);
  }
 return RerootingData<T,C>(t, dp1, dp2, dp);
}