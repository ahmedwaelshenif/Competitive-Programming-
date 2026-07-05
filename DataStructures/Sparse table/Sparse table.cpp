struct SparseTable { // min and max sparse table
    int n, LOG;
    vector<int> lg;
    vector<vector<int>> mn, mx;

    SparseTable(vector<int> &a) {
        n = a.size();
        LOG = 32 - __builtin_clz(n);

        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;

        mn.assign(LOG, vector<int>(n));
        mx.assign(LOG, vector<int>(n));

        for (int i = 0; i < n; i++) {
            mn[0][i] = a[i];
            mx[0][i] = a[i];
        }

        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                mn[k][i] = min(mn[k-1][i], mn[k-1][i + (1 << (k-1))]);
                mx[k][i] = max(mx[k-1][i], mx[k-1][i + (1 << (k-1))]);
            }
        }
    }

    int getMin(int L, int R) {
        int len = R - L + 1;
        int k = lg[len];
        return min(mn[k][L], mn[k][R - (1 << k) + 1]);
    }

    int getMax(int L, int R) {
        int len = R - L + 1;
        int k = lg[len];
        return max(mx[k][L], mx[k][R - (1 << k) + 1]);
    }
};




struct GCDSparseTable {
    int n, LOG;
    vector<vector<int>> st;
    vector<int> lg;
 
    GCDSparseTable(const vector<int>& a) {
        n = a.size();
        LOG = 32 - __builtin_clz(n);
        st.assign(LOG, vector<int>(n));
        lg.assign(n + 1, 0);
 
        // Precompute logs
        for (int i = 2; i <= n; i++)
            lg[i] = lg[i / 2] + 1;
 
        // Base layer
        for (int i = 0; i < n; i++)
            st[0][i] = a[i];
 
        // Build table
        for (int k = 1; k < LOG; k++) {
            for (int i = 0; i + (1 << k) <= n; i++) {
                st[k][i] = gcd(
                    st[k - 1][i],
                    st[k - 1][i + (1 << (k - 1))]
                );
            }
        }
    }
 
    // Query gcd on range [l, r]
    int query(int l, int r) {
        int len = r - l + 1;
        int k = lg[len];
        return gcd(
            st[k][l],
            st[k][r - (1 << k) + 1]
        );
    }
};