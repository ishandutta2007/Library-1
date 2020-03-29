/**
 * @title KnuthMorrisPratt
 * @brief テーブル構築 O(|S|)
 * @brief pattern_match(T) Tの中からSと一致する部分を検索 O(|T|)
 */

#ifndef call_from_test
#include <bits/stdc++.h>
using namespace std;
#endif

struct KnuthMorrisPratt {
    vector<int> KMP;
    const string s;
    int n;
    KnuthMorrisPratt(const string &str) : s(str), n(s.length()) {
        KMP.resize(n + 1, -1);
        for(int i = 0, j = -1; i < n; i++) {
            while(j >= 0 && s[i] != s[j])
                j = KMP[j];
            j++;
            if((i + 1 == n ? '*' : s[i + 1]) == (j == n ? '*' : s[j]))
                KMP[i + 1] = KMP[j];
            else
                KMP[i + 1] = j;
        }
    }

    // positions for T that match s
    // O(|T|)
    vector<int> pattern_match(string T) {
        int m = T.length();
        vector<int> occur;
        for(int i = 0, k = 0; i < m; ++i) {
            while(k >= 0 && T[i] != s[k])
                k = KMP[k];
            if(++k == n) {
                /* match at T[i-n+1 ... i] */
                occur.push_back(i - n + 1);
            }
        }
        return occur;
    }
};
