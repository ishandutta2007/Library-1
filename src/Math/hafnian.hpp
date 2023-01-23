#pragma once
#include <vector>
#include <array>
#include <cassert>
template <typename T, unsigned short MAX_N= 38> T hafnian(const std::vector<std::vector<T>> &mat) {
 using Poly= std::array<T, MAX_N / 2 + 1>;
 const int n= mat.size(), n2= n / 2;
 assert(!(n & 1));
 for (int i= n; i--;)
  for (int j= i; j--;) assert(mat[i][j] == mat[j][i]);
 std::vector<std::vector<Poly>> a(n);
 for (int i= n, j; i--;)
  for (a[j= i].resize(i); j--;) a[i][j][0]= mat[i][j];
 auto rec= [&](auto self, const auto &b) -> Poly {
  const int m= b.size() - 2;
  if (m < 0) return Poly{1};
  auto c= b;
  c.resize(m);
  Poly r= self(self, c);
  for (int i= m; i--;)
   for (int j= i; j--;)
    for (int k= n2 - m / 2; k--;)
     for (int l= k; l >= 0; l--) c[i][j][k + 1]+= b[m][i][l] * b[m + 1][j][k - l] + b[m + 1][i][l] * b[m][j][k - l];
  Poly t= self(self, c);
  for (int i= n2, j; i >= 0; i--)
   for (r[i]= t[j= i] - r[i]; j--;) r[i]+= t[j] * b[m + 1][m][i - j - 1];
  return r;
 };
 return rec(rec, a)[n2];
}