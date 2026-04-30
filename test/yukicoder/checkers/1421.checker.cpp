// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/1421
// yuki 1421 国勢調査 (Hard): self-validating sp judge.
//   - expected の最初のトークンが -1 か否かで「解の有無」を判別
//   - 解ありケース: user の N 個の値が各 query の XOR 制約を満たし、各値が 2^30 未満
//   - 不能ケース: user も -1
// Args (run-lib.sh testlib convention): input user_output expected
#include <cstdio>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
 if(argc < 4) { std::fprintf(stderr, "usage: checker input user_output expected\n"); return 2; }
 std::ifstream in(argv[1]);
 std::ifstream out(argv[2]);
 std::ifstream exp(argv[3]);
 if(!in || !out || !exp) { std::fprintf(stderr, "open failed\n"); return 2; }

 long long expected_first;
 if(!(exp >> expected_first)) { std::fprintf(stderr, "expected parse\n"); return 2; }

 long long user_first;
 if(!(out >> user_first)) { std::fprintf(stderr, "user output empty\n"); return 1; }

 if((expected_first == -1) != (user_first == -1)) {
  std::fprintf(stderr, "expected first=%lld but user=%lld\n", expected_first, user_first);
  return 1;
 }

 if(user_first == -1) {
  long long extra;
  if(out >> extra) { std::fprintf(stderr, "-1 case but extra output present\n"); return 1; }
  return 0;
 }

 int n, m;
 if(!(in >> n >> m)) { std::fprintf(stderr, "input parse: n m\n"); return 2; }

 std::vector<unsigned long long> x(n);
 x[0]= (unsigned long long)user_first;
 for(int i= 1; i < n; ++i) {
  if(!(out >> x[i])) { std::fprintf(stderr, "user output truncated at i=%d\n", i); return 1; }
 }
 for(int i= 0; i < n; ++i) {
  if(x[i] >= (1ull << 30)) {
   std::fprintf(stderr, "X[%d]=%llu out of range\n", i, x[i]);
   return 1;
  }
 }

 for(int j= 0; j < m; ++j) {
  int a;
  if(!(in >> a)) { std::fprintf(stderr, "input parse: query %d a\n", j); return 2; }
  unsigned long long ans= 0;
  for(int t= 0; t < a; ++t) {
   int b;
   if(!(in >> b)) { std::fprintf(stderr, "input parse: query %d b\n", j); return 2; }
   ans^= x[b - 1];
  }
  unsigned long long y;
  if(!(in >> y)) { std::fprintf(stderr, "input parse: query %d y\n", j); return 2; }
  if(ans != y) {
   std::fprintf(stderr, "query %d: XOR=%llu expected %llu\n", j, ans, y);
   return 1;
  }
 }

 long long extra;
 if(out >> extra) { std::fprintf(stderr, "extra output present\n"); return 1; }
 return 0;
}
