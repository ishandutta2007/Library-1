// competitive-verifier: PROBLEM https://yukicoder.me/problems/no/382
// yuki 382 シャイな人たち (2): self-validating sp judge.
//   - 入力 (S) からグラフを生成 (公式判定と同じ LCG)
//   - 期待 K と user K の一致を確認 (max independent set + 1 は一意)
//   - K-1 個の頂点が distinct でかつ独立集合であることを確認
// Args (run-lib.sh testlib convention): input user_output expected
#include <cstdio>
#include <fstream>

static long long S;
static int gen() { return (int)(S= S * 12345 % 1000003); }

int main(int argc, char** argv) {
 if(argc < 4) { std::fprintf(stderr, "usage: checker input user_output expected\n"); return 2; }
 std::ifstream in(argv[1]);
 std::ifstream out(argv[2]);
 std::ifstream exp(argv[3]);
 if(!in || !out || !exp) { std::fprintf(stderr, "open failed\n"); return 2; }

 if(!(in >> S)) { std::fprintf(stderr, "input parse: S\n"); return 2; }

 int N= gen() % 120 + 2;
 int P= gen();
 static int mat[130][130]= {{0}};
 for(int i= 0; i < N; ++i)
  for(int j= i + 1; j < N; ++j) {
   int k= gen();
   if(k >= P) mat[i][j]= mat[j][i]= 1;
  }

 int K_expected, K_user;
 if(!(exp >> K_expected)) { std::fprintf(stderr, "expected parse\n"); return 2; }
 if(!(out >> K_user)) { std::fprintf(stderr, "user output empty\n"); return 1; }

 if(K_user != K_expected) {
  std::fprintf(stderr, "K mismatch: user=%d expected=%d\n", K_user, K_expected);
  return 1;
 }
 if(K_user == -1) return 0;

 int Km= K_user - 1;
 int arr[130];
 for(int i= 0; i < Km; ++i) {
  if(!(out >> arr[i])) {
   std::fprintf(stderr, "user output truncated at index %d\n", i);
   return 1;
  }
  if(arr[i] < 1 || arr[i] > N) {
   std::fprintf(stderr, "vertex %d out of [1,%d]\n", arr[i], N);
   return 1;
  }
 }
 for(int i= 0; i < Km; ++i)
  for(int j= i + 1; j < Km; ++j) {
   if(arr[i] == arr[j]) {
    std::fprintf(stderr, "duplicate vertex %d\n", arr[i]);
    return 1;
   }
   if(mat[arr[i] - 1][arr[j] - 1] != 0) {
    std::fprintf(stderr, "edge between %d and %d (not indep set)\n", arr[i], arr[j]);
    return 1;
   }
  }
 return 0;
}
