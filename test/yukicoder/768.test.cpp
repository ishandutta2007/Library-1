#define PROBLEM "https://yukicoder.me/problems/no/768"
#include <iostream>
#include <vector>
#include "src/Graph/ReRooting.hpp"
using namespace std;
signed main() {
 cin.tie(0);
 ios::sync_with_stdio(0);
 int N;
 cin >> N;
 auto f= [](short vdp, short chdp) { return short(vdp & chdp); };
 auto g= [](short dp, int dat) { return short(!dp); };
 ReRooting<short> graph(N, f, 1, g);
 for (int i= 0; i < N - 1; i++) {
  int a, b;
  cin >> a >> b;
  a--, b--;
  graph.add_edge(a, b);
  graph.add_edge(b, a);
 }
 auto ret= graph.run();
 vector<int> ans;
 for (int i= 0; i < N; i++)
  if (ret[i]) ans.push_back(i + 1);
 cout << ans.size() << endl;
 for (int &a: ans) cout << a << "\n";
 return 0;
}