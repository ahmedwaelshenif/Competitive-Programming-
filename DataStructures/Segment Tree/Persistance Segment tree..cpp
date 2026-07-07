class PersistentSegTree {
private:
    static const long long INF = 2e18;

    struct Node {
        long long sum, mn, mx, g, xr;
        int lc, rc;

        Node() { sum=0; mn=INF; mx=-INF; g=0; xr=0; lc=rc=0; }
        Node(long long v) { sum=v; mn=v; mx=v; g=abs(v); xr=v; lc=rc=0; }
    };

    vector<Node> tree;
    vector<int> roots;
    int n;

    Node merge(const Node& l, const Node& r) {
        Node res;
        res.sum = l.sum + r.sum;
        res.mn = min(l.mn, r.mn);
        res.mx = max(l.mx, r.mx);
        res.g = std::gcd(l.g, r.g);
        res.xr = l.xr ^ r.xr;
        return res;
    }

    int build(int l, int r, const vector<long long>& a) {
        int u = tree.size();
        tree.emplace_back();
        if (l == r) { tree[u] = Node(a[l]); return u; }
        int mid = l + (r - l) / 2;
        tree[u].lc = build(l, mid, a);
        tree[u].rc = build(mid + 1, r, a);
        Node merged = merge(tree[tree[u].lc], tree[tree[u].rc]);
        merged.lc = tree[u].lc; merged.rc = tree[u].rc;
        tree[u] = merged;
        return u;
    }

    int update(int prev_u, int l, int r, int pos, long long val) {
        int u = tree.size();
        tree.push_back(tree[prev_u]);
        if (l == r) { tree[u] = Node(val); return u; }
        int mid = l + (r - l) / 2;
        if (pos <= mid) tree[u].lc = update(tree[prev_u].lc, l, mid, pos, val);
        else            tree[u].rc = update(tree[prev_u].rc, mid + 1, r, pos, val);
        Node merged = merge(tree[tree[u].lc], tree[tree[u].rc]);
        merged.lc = tree[u].lc; merged.rc = tree[u].rc;
        tree[u] = merged;
        return u;
    }

    Node query(int u, int l, int r, int ql, int qr) {
        if (ql > qr) return Node();
        if (ql <= l && r <= qr) return tree[u];
        int mid = l + (r - l) / 2;
        if (qr <= mid) return query(tree[u].lc, l, mid, ql, qr);
        if (ql > mid) return query(tree[u].rc, mid + 1, r, ql, qr);
        return merge(query(tree[u].lc, l, mid, ql, qr), query(tree[u].rc, mid + 1, r, ql, qr));
    }

public:
    PersistentSegTree(const vector<long long>& initial_arr) {
        n = initial_arr.size();
        tree.push_back(Node());
        roots.push_back(build(0, n - 1, initial_arr));
    }

    PersistentSegTree(int size) {
        n = size;
        tree.push_back(Node());
        roots.push_back(build(0, n - 1, vector<long long>(size, 0)));
    }

    // CALL THIS ONCE right after construction, before doing updates.
    // expected_updates = total number of pst.update(...) calls you'll make (e.g. Q, or Q*num_fields).
    void reserve(long long expected_updates) {
        long long cap = 2LL * n + expected_updates * ((long long)log2(max(n,1)) + 2) + 10;
        tree.reserve(cap);
    }

    int update(int version, int pos, long long val) {
        int new_root = update(roots[version], 0, n - 1, pos, val);
        roots.push_back(new_root);
        return roots.size() - 1;
    }

    int versions_count() { return roots.size(); }

    long long get_sum(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).sum; }
    long long get_min(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).mn; }
    long long get_max(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).mx; }
    long long get_gcd(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).g; }
    long long get_xor(int version, int l, int r) { return query(roots[version], 0, n - 1, l, r).xr; }
};
// Example usage:
PersistentSegTree pst(a);
pst.reserve(q);          // <-- new line, add this everywhere you use the template
int v1 = pst.update(0, 2, 12);