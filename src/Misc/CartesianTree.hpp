#pragma once
#include <vector>
#include <array>
class CartesianTree {
 std::vector<std::array<int, 2>> rg, ch;
 std::vector<int> par;
 int rt;
public:
 template <class Vec> CartesianTree(const Vec &a, bool is_min= 1): rg(a.size()), ch(a.size(), std::array{-1, -1}), par(a.size(), -1) {
  const int n= a.size();
  auto comp= [&](int l, int r) { return (is_min ? a[l] < a[r] : a[l] > a[r]) || (a[l] == a[r] && l < r); };
  int st[n], t= 0;
  for (int i= n; i--; rg[i][1]= (t ? st[t - 1] : n), st[t++]= i)
   while (t && comp(i, st[t - 1])) ch[i][1]= st[--t];
  for (int i= t= 0; i < n; rg[i][0]= (t ? st[t - 1] + 1 : 0), st[t++]= i++)
   while (t && comp(i, st[t - 1])) ch[i][0]= st[--t];
  for (int i= 0; i < n; ++i)
   for (int b= 2; b--;)
    if (ch[i][b] != -1) par[ch[i][b]]= i;
  for (int i= 0; i < n; ++i)
   if (par[i] == -1) rt= i;
 }
 std::array<int, 2> children(int i) const { return ch[i]; }
 int parent(int i) const { return par[i]; }
 int root() const { return rt; }
 // [l,r)
 std::array<int, 2> range(int i) const { return rg[i]; }
};