#include <bits/stdc++.h>
using namespace std;
#define int long long

struct SegTree {
    struct Node {
        array<int, 31> cnt{};
        int lazy = 0;
    };
 
    int n;
    vector<Node> st;
    vector<int> len_cache;
 
    SegTree(const vector<int>& a) {
        n = a.size();
        st.resize(4 * n);
        len_cache.resize(4 * n);
        build(1, 0, n - 1, a);
        precompute_len(1, 0, n - 1);
    }
 
    void range_xor(int l, int r, int x) { update(1, 0, n - 1, l, r, x); }
    long long range_sum(int l, int r) { return query(1, 0, n - 1, l, r); }
 
private:
    void build(int p, int l, int r, const vector<int>& a) {
        if (l == r) {
            for (int b = 0; b < 31; ++b)
                if (a[l] & (1LL << b)) st[p].cnt[b] = 1;
            return;
        }
        int m = (l + r) / 2;
        build(p * 2, l, m, a);
        build(p * 2 + 1, m + 1, r, a);
        pull(p);
    }
 
    void precompute_len(int p, int l, int r) {
        len_cache[p] = r - l + 1;
        if (l == r) return;
        int m = (l + r) / 2;
        precompute_len(p * 2, l, m);
        precompute_len(p * 2 + 1, m + 1, r);
    }
 
    void apply(int p, int x) {
        for (int b = 0; b < 31; ++b)
            if (x & (1LL << b))
                st[p].cnt[b] = len_cache[p] - st[p].cnt[b];
        st[p].lazy ^= x;
    }
 
    void push(int p, int l, int r) {
        if (!st[p].lazy || l == r) return;
        apply(p * 2, st[p].lazy);
        apply(p * 2 + 1, st[p].lazy);
        st[p].lazy = 0;
    }
 
    void pull(int p) {
        for (int b = 0; b < 31; ++b)
            st[p].cnt[b] = st[p * 2].cnt[b] + st[p * 2 + 1].cnt[b];
    }
 
    void update(int p, int l, int r, int ql, int qr, int x) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) { apply(p, x); return; }
        push(p, l, r);
        int m = (l + r) / 2;
        update(p * 2, l, m, ql, qr, x);
        update(p * 2 + 1, m + 1, r, ql, qr, x);
        pull(p);
    }
 
    long long query(int p, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;
        if (ql <= l && r <= qr) return node_sum(st[p]);
        push(p, l, r);
        int m = (l + r) / 2;
        return query(p * 2, l, m, ql, qr) + query(p * 2 + 1, m + 1, r, ql, qr);
    }
 
    static long long node_sum(const Node& nd) {
        long long s = 0;
        for (int b = 0; b < 31; ++b) s += 1LL * nd.cnt[b] * (1LL << b);
        return s;
    }
};
 