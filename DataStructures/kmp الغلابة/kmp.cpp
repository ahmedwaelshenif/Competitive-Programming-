struct Kmp {
    vector<int>failure;

    void build_lps(const string &s) {
        int n=s.size();
        failure.assign(n, 0);
        int i=0;
        for (int j=1;j<n;j++) {
            while (i>0 && s[i]!=s[j]) {
                i=failure[i-1];
            }
            if (s[i]==s[j]) {
                failure[j]=++i;
            } else {
                failure[j]=0;
            }
        }
    }
    vector<int> KMPsearch(const string &text, const string &pattern) {
        vector<int> matches;
        if (pattern.empty()) return matches;
        build_lps(pattern);
        int n = text.size(), m = pattern.size();
        int i = 0;
        for (int j = 0; j < n; j++) {
            while (i > 0 && text[j] != pattern[i]) {
                i = failure[i - 1];
            }
            if (text[j] == pattern[i]) {
                i++;
            }
            if (i == m) {
                matches.push_back(j - m + 1);
                i = failure[i - 1];
            }
        }
        return matches;
    }
    vector<int> prefix_freq(string &pattern) {
        build_lps(pattern);
        int n = pattern.size();
        vector<int>freq(n+1,1);
        freq[0]=0;
        for(int i=n;i>0;i--) {
            freq[failure[i-1]]+=freq[i];
        }
        return freq;
    }
    string longest_prefix_palindrome(const string &s) {
        string rev = s;
        reverse(rev.begin(), rev.end());

        // Build combined string s + '#' + rev
        string t = s + "#" + rev;

        build_lps(t);

        // failure.back() gives length of longest prefix palindrome
        int len = failure.back();
        return s.substr(0, len);
    }
};
