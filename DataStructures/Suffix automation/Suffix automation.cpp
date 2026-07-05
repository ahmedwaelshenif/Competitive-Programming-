struct SuffixAutomaton {
    struct State {
        int len;                      // Max length of substring in this state
        int link;                     // Suffix link (longest proper suffix)
        unordered_map<char, int> next; // Transitions (char → state)
        int occ = 0;                  // Number of times substring ends here
        long long dp_distinct = -1;  // Memo for counting distinct substrings
    };

    vector<State> st;
    int last; // Index of the state representing the full string

    // Constructor — O(1)
    SuffixAutomaton(int maxlen = 1e6) {
        st.reserve(2 * maxlen);
        st.push_back({0, -1});
        last = 0;
    }

    // Extend automaton with character c — O(1) amortized per character
    void extend(char c) {
        int cur = st.size();
        st.push_back({st[last].len + 1, 0});
        int p = last;

        while (p != -1 && !st[p].next.count(c)) {
            st[p].next[c] = cur;
            p = st[p].link;
        }

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[c];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = st.size();
                st.push_back({st[p].len + 1, st[q].link, st[q].next, 0});
                while (p != -1 && st[p].next[c] == q) {
                    st[p].next[c] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }

        last = cur;
        st[cur].occ = 1; // mark as end of new substring
    }

    // Build automaton from string — O(n)
    void build(const string &s) {
        for (char c : s) extend(c);
    }

    // Count number of distinct substrings — O(n)
    long long count_distinct(int v = 0) {
        if (st[v].dp_distinct != -1) return st[v].dp_distinct;
        long long res = 1; // empty string
        for (auto [c, u] : st[v].next) {
            res += count_distinct(u);
        }
        return st[v].dp_distinct = res;
    }

    // Wrapper: count distinct substrings (excluding empty) — O(n)
    long long count_distinct_substrings() {
        return count_distinct() - 1;
    }

    // Count occurrences of all substrings — O(n)
    vector<int> count_occurrences() {
        int N = st.size();
        vector<int> order(N), indeg(N);
        for (int i = 1; i < N; ++i)
            indeg[st[i].link]++;
        queue<int> q;
        for (int i = 1; i < N; ++i)
            if (indeg[i] == 0) q.push(i);

        vector<vector<int>> tree(N);
        for (int i = 1; i < N; ++i)
            tree[st[i].link].push_back(i);

        // reverse topological sort
        vector<int> topo;
        function<void(int)> dfs = [&](int u) {
            for (int v : tree[u]) dfs(v);
            topo.push_back(u);
        };
        dfs(0);

        // Accumulate counts from leaves up
        for (int u : topo) {
            if (st[u].link != -1)
                st[st[u].link].occ += st[u].occ;
        }

        vector<int> occs(N);
        for (int i = 0; i < N; ++i) occs[i] = st[i].occ;
        return occs;
    }

    // Check if a string is a substring — O(|t|)
    bool is_substring(const string &t) {
        int v = 0;
        for (char c : t) {
            if (!st[v].next.count(c)) return false;
            v = st[v].next[c];
        }
        return true;
    }

    // Count total substrings including repetitions — O(n)
    long long total_substrings() {
        vector<int> occ = count_occurrences();
        long long total = 0;
        for (int i = 1; i < st.size(); ++i) {
            int len_v = st[i].len;
            int len_link = st[st[i].link].len;
            total += 1LL * occ[i] * (len_v - len_link);
        }
        return total;
    }

    // O(|s2|) — finds and returns the longest common substring between the automaton's string and s2
    string longest_common_substring(const string& s2) const {
        int v = 0;         // Start at the initial state
        int l = 0;         // Current match length
        int best = 0;      // Best match length so far
        int best_pos = 0;  // End position of best match in s2

        for (int i = 0; i < (int)s2.size(); i++) {
            char c = s2[i];
            if (st[v].next.count(c)) {
                v = st[v].next.at(c);
                l++;
            } else {
                while (v != -1 && !st[v].next.count(c))
                    v = st[v].link;
                if (v == -1) {
                    v = 0;
                    l = 0;
                } else {
                    l = st[v].len + 1;
                    v = st[v].next.at(c);
                }
            }
            if (l > best) {
                best = l;
                best_pos = i;
            }
        }
        return s2.substr(best_pos - best + 1, best);
    }
};
