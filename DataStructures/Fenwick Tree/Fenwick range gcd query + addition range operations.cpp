#include<bits/stdc++.h>
using namespace std;
#define int long long
#define endl "\n"

struct Fenwick {
    int n;
    vector<long long> bit;
    Fenwick(int n): n(n), bit(n + 1, 0) {}

    void add(int pos, long long val) {
        for (int idx = pos + 1; idx <= n; idx += idx & -idx)
            bit[idx] += val;
    }
    long long prefixSum(int pos) const {
        long long s = 0;
        for (int idx = pos + 1; idx > 0; idx -= idx & -idx)
            s += bit[idx];
        return s;
    }
    long long pointQuery(int pos) const { return prefixSum(pos) - (pos ? prefixSum(pos - 1) : 0); }
};
struct GCDSeg {
    int n;
    vector<long long> g;
    static long long gval(long long a, long long b) {
        return std::gcd(std::llabs(a), std::llabs(b));
    }

    GCDSeg(const vector<long long>& diff) {
        n = (int)diff.size();
        g.assign(4 * n, 0);
        build(1, 0, n - 1, diff);
    }

    void build(int v, int l, int r, const vector<long long>& d) {
        if (l == r) { g[v] = d[l]; return; }
        int m = (l + r) >> 1;
        build(v << 1, l, m, d);
        build(v << 1 | 1, m + 1, r, d);
        g[v] = gval(g[v << 1], g[v << 1 | 1]);
    }

    void pointAdd(int v, int l, int r, int pos, long long val) {
        if (l == r) { g[v] += val; return; }
        int m = (l + r) >> 1;
        if (pos <= m) pointAdd(v << 1, l, m, pos, val);
        else          pointAdd(v << 1 | 1, m + 1, r, pos, val);
        g[v] = gval(g[v << 1], g[v << 1 | 1]);
    }
    void add(int pos, long long val) { pointAdd(1, 0, n - 1, pos, val); }

    long long query(int v, int l, int r, int ql, int qr) const {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) return g[v];
        int m = (l + r) >> 1;
        return gval(query(v << 1, l, m, ql, qr),
                    query(v << 1 | 1, m + 1, r, ql, qr));
    }
    long long query(int l, int r) const {
        return l > r ? 0 : query(1, 0, n - 1, l, r);
    }
};
struct RangeAddRangeGCD {
    int n;
    Fenwick BIT;
    GCDSeg seg;
    static vector<long long> makeDiff(const vector<long long>& A) {
        int n = (int)A.size();
        vector<long long> d(n);
        d[0] = A[0];
        for (int i = 1; i < n; ++i) d[i] = A[i] - A[i - 1];
        return d;
    }
    RangeAddRangeGCD(const vector<long long>& A)
        : n((int)A.size()),
          BIT(n),
          seg(makeDiff(A))
    {
        vector<long long> d = makeDiff(A);
        for (int i = 0; i < n; ++i) BIT.add(i, d[i]);
    }
    void rangeAdd(int L, int R, long long x) {
        seg.add(L, x);                 BIT.add(L, x);
        if (R + 1 < n) { seg.add(R + 1, -x); BIT.add(R + 1, -x); }
    }

    long long rangeGCD(int L, int R) const {
        long long first = BIT.prefixSum(L);
        long long g     = seg.query(L + 1, R);
        return std::llabs(std::gcd(first, g));
    }

    long long at(int idx) const { return BIT.prefixSum(idx); }
};