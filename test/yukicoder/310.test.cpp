// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/310
// competitive-verifier: TLE 0.5
#include <iostream>
#include <algorithm>
#include "src/Math/ModInt.hpp"
#include "src/Math/FactorialPrecalculation.hpp"
#include "src/LinearAlgebra/MinimalPolynomial.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using Mint= ModInt<int(1e9 + 7)>;
 using F= FactorialPrecalculation<Mint>;
 using Vec= Vector<Mint>;
 int N, M;
 cin >> N >> M;
 int in[N], out[N], A[M], B[M];
 fill_n(in, N, N), fill_n(out, N, N);
 for (int i= 0; i < M; ++i) cin >> A[i] >> B[i], --out[--A[i]], --in[--B[i]];
 vector<int> V;
 for (int i= 0; i < N; ++i)
  if (in[i] > 0 || out[i] > 0) V.push_back(i);
 int n= V.size();
 if (n == 0) return cout << 1 << '\n', 0;
 for (int i= 0; i < M; ++i) {
  A[i]= lower_bound(V.begin(), V.end(), A[i]) - V.begin();
  B[i]= lower_bound(V.begin(), V.end(), B[i]) - V.begin();
 }
 int s= -1, t= -1;
 for (int i= 0; i < n; ++i) {
  int v= V[i];
  int tmp= in[v] - out[v];
  if (abs(tmp) > 1) {
   s= -2;
  } else if (tmp == 1) {
   if (s == -1) s= i;
   else s= -2;
  } else if (tmp == -1) {
   if (t == -1) t= i;
   else s= -2;
  }
 }
 if (s == -2) return cout << 0 << '\n', 0;
 Mint ans= s == -1 ? N * N - M : 1;
 if (s != -1) ++in[V[t]], ++out[V[s]];
 auto f= [&](const Vec &v) {
  Mint sum= 0;
  for (int i= n - 1; i--;) sum+= v[i];
  Vec ret(n - 1, -sum);
  for (int i= n - 1; i--;) ret[i]+= v[i] * n;
  for (int i= 0; i < M; ++i)
   if (B[i] < n - 1) {
    ret[B[i]]-= v[B[i]];
    if (A[i] < n - 1) ret[B[i]]+= v[A[i]];
   }
  if (s != -1 && t < n - 1) {
   ret[t]+= v[t];
   if (s < n - 1) ret[t]-= v[s];
  }
  return ret;
 };
 ans*= linear_map_det<Mint>(f, n - 1);
 for (int i= 0; i < n; ++i) ans*= F::fact(in[V[i]] - 1);
 cout << ans << '\n';
 return 0;
}
