#define PROBLEM "https://atcoder.jp/contests/arc116/tasks/arc116_c"

#include <bits/stdc++.h>
#include "src/Math/ModInt.hpp"
#include "src/Math/DirichletConvSumTable.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<998244353>;
 long long N, M;
 cin >> N >> M;
 auto zeta= get_1<Mint>(M, pow(M, 2. / 3));
 cout << zeta.pow2(N).sum() << '\n';
 return 0;
}