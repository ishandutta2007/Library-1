/**
 * @title SuffixArray
 * @brief 構築 O(|S| log |S|)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct SuffixArray {
    vector<int> SA;
    const string s;

    SuffixArray(const string &str) : s(str) {
        SA.resize(s.size());
        iota(begin(SA), end(SA), 0);
        sort(begin(SA), end(SA),
             [&](int a, int b) { return s[a] == s[b] ? a > b : s[a] < s[b]; });
        vector<int> classes(s.size()), c(s.begin(), s.end()), cnt(s.size());
        for(int len = 1; len < s.size(); len <<= 1) {
            for(int i = 0; i < s.size(); i++) {
                if(i > 0 && c[SA[i - 1]] == c[SA[i]]
                   && SA[i - 1] + len < s.size()
                   && c[SA[i - 1] + len / 2] == c[SA[i] + len / 2]) {
                    classes[SA[i]] = classes[SA[i - 1]];
                } else {
                    classes[SA[i]] = i;
                }
            }
            iota(begin(cnt), end(cnt), 0);
            copy(begin(SA), end(SA), begin(c));
            for(int i = 0; i < s.size(); i++) {
                int s1 = c[i] - len;
                if(s1 >= 0)
                    SA[cnt[classes[s1]]++] = s1;
            }
            classes.swap(c);
        }
    }
    int operator[](int k) const { return (SA[k]); }
};
