#define PROBLEM "https://yukicoder.me/problems/no/728"
#include <iostream>
#include <vector>
#include <array>
#include "src/DataStructure/KDTree.hpp"
using namespace std;
struct RSQ {
 using T= long long;
 static T ti() { return 0; }
 static T op(T l, T r) { return l + r; }
};
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 using KDT= KDTree<int, 2, RSQ>;
 int N;
 cin >> N;
 int A[N], L[N], R[N];
 for (int i= 0; i < N; i++) cin >> A[i];
 for (int i= 0; i < N; i++) cin >> L[i] >> R[i];
 vector<array<int, 3>> v(N);
 for (int i= 0; i < N; i++) v[i]= {A[i], A[i] + R[i], 1};
 KDT kdt(v);
 long long ans= 0;
 for (int i= 0; i < N; i++)
  if (L[i] > 0) ans+= kdt.fold_cuboid(A[i] - L[i], A[i] - 1, A[i], 0x7fffffff);
 cout << ans << "\n";
 return 0;
}