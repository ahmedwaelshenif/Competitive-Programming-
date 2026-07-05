// Build suffix array in O(n log² n)
vector<int> buildSuffixArray(const string &s) {
    int n = s.size();
    vector<int> sa(n), rank(n), temp(n);
    iota(sa.begin(), sa.end(), 0);
    for (int i = 0; i < n; i++) rank[i] = s[i];

    for (int k = 1; k < n; k <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rank[a] != rank[b]) return rank[a] < rank[b];
            return (a + k < n ? rank[a + k] : -1) < (b + k < n ? rank[b + k] : -1);
        };
        sort(sa.begin(), sa.end(), cmp);
        temp[sa[0]] = 0;
        for (int i = 1; i < n; i++)
            temp[sa[i]] = temp[sa[i - 1]] + cmp(sa[i - 1], sa[i]);
        rank = temp;
    }
    return sa;
}

// Build LCP in O(n)
vector<int> buildLCP(const string &s, const vector<int> &sa) {
    int n = s.size();
    vector<int> lcp(n), rank(n);
    for (int i = 0; i < n; i++) rank[sa[i]] = i;

    int k = 0;
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) { k = 0; continue; }
        int j = sa[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rank[i]] = k;
        if (k) k--;
    }
    return lcp;
}


// Count occurrences of a pattern in O(m log n)
int countOccurrences(const string &s, const vector<int> &sa, const string &pat) {
    int n = s.size(), m = pat.size();
    int l = 0, r = n, left = -1, right = -1;

    // Lower bound
    while (l < r) {
        int mid = (l + r) / 2;
        string sub = s.substr(sa[mid], min(m, n - sa[mid]));
        if (sub < pat) l = mid + 1;
        else r = mid;
    }
    left = l;

    // Upper bound
    l = 0, r = n;
    while (l < r) {
        int mid = (l + r) / 2;
        string sub = s.substr(sa[mid], min(m, n - sa[mid]));
        if (sub <= pat) l = mid + 1;
        else r = mid;
    }
    right = l;

    return max(0, right - left);
}

// Get borders (prefixes that are also suffixes) in O(n)
vector<string> getBorders(const string &s) {
    int n = s.size();
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) j = pi[j - 1];
        if (s[i] == s[j]) j++;
        pi[i] = j;
    }
    vector<string> res;
    int len = pi[n - 1];
    while (len > 0) {
        res.push_back(s.substr(0, len));
        len = pi[len - 1];
    }
    return res;
}

// Get all substrings sorted lexicographically in O(n^2 log n)
vector<string> getAllSubstringsLex(const string &s, const vector<int> &sa) {
    int n = s.size();
    vector<string> result;
    for (int i = 0; i < n; i++) {
        for (int len = 1; len <= n - sa[i]; len++) {
            result.push_back(s.substr(sa[i], len));
        }
    }
    sort(result.begin(), result.end());
    return result;
}

// Count distinct substrings in O(n)
long long countDistinctSubstrings(const string &s, const vector<int> &sa, const vector<int> &lcp) {
    int n = s.size();
    long long total = 0;
    for (int i = 0; i < n; i++) {
        int suffix_len = n - sa[i];
        int lcp_len = (i == 0 ? 0 : lcp[i]);
        total += suffix_len - lcp_len;
    }
    return total;
}

// Get k-th lexicographical substring in O(n^2) worst case
string getKthSubstring(const string &s, const vector<int> &sa, const vector<int> &lcp, long long k) {
    int n = s.size();
    for (int i = 0; i < n; i++) {
        int suffix_len = n - sa[i];
        int prev_lcp = (i == 0 ? 0 : lcp[i]);
        long long new_subs = suffix_len - prev_lcp;
        if (k <= new_subs) {
            return s.substr(sa[i], prev_lcp + k);
        }
        k -= new_subs;
    }
    return ""; // k is too large
}
//n log(n) log(n);
string longestCommonSubstringMultiple(const vector<string>& strings) {
    int k = strings.size();
    string concat;
    vector<int> owner; // owner[i] = index of string for character i in concat
    char sep = 127; // Start from DEL ASCII

    for (int idx = 0; idx < k; idx++) {
        concat += strings[idx] + sep;
        for (int i = 0; i < strings[idx].size(); ++i)
            owner.push_back(idx);
        owner.push_back(-1); // separator
        sep--; // next unique separator
    }

    auto sa = buildSuffixArray(concat);
    auto lcp = buildLCP(concat, sa);
    int n = concat.size();

    vector<int> count(k, 0);
    int total = 0, left = 0;
    int maxLen = 0, pos = -1;
    map<int, int> freq;

    auto getOwner = [&](int idx) -> int {
        if (idx >= owner.size()) return -1;
        return owner[idx];
    };

    for (int right = 0; right < n; ++right) {
        int ow = getOwner(sa[right]);
        if (ow != -1) {
            freq[ow]++;
            if (freq[ow] == 1) total++;
        }

        while (total == k) {
            // LCP between sa[left] and sa[right]
            if (right > left) {
                int minLCP = INT_MAX;
                for (int i = left + 1; i <= right; ++i)
                    minLCP = min(minLCP, lcp[i - 1]);

                if (minLCP > maxLen) {
                    maxLen = minLCP;
                    pos = sa[right];
                }
            }

            int low = getOwner(sa[left]);
            if (low != -1) {
                freq[low]--;
                if (freq[low] == 0) total--;
            }
            left++;
        }
    }

    return (pos != -1 ? concat.substr(pos, maxLen) : "");
}