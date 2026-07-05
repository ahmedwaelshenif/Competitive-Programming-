struct RollbackSegTree {
    using ll = long long;

    struct Node {
        ll sum = 0;
        ll lazy = 1;
    };

    struct Change {
        int idx;
        ll sum;
        ll lazy;
    };

    int n;
    vector<Node> seg;
    stack<Change> hist;

    // ===================== CONSTRUCTOR =====================
    RollbackSegTree(const vector<ll>& a) {
        init(a);
    }

    RollbackSegTree() {}

    // ===================== INIT =====================
    void init(const vector<ll>& a) {
        n = (int)a.size();
        seg.assign(4 * n + 5, Node());
        while (!hist.empty()) hist.pop();
        build(1, 0, n - 1, a);
    }

    // ===================== BUILD =====================
private:

    void build(int idx, int l, int r, const vector<ll>& a) {
        if (l == r) {
            seg[idx].sum = a[l];
            return;
        }

        int mid = (l + r) >> 1;

        build(idx * 2, l, mid, a);
        build(idx * 2 + 1, mid + 1, r, a);

        seg[idx].sum = seg[idx * 2].sum + seg[idx * 2 + 1].sum;
    }

    // ===================== ROLLBACK CORE =====================

    void save(int idx) {
        hist.push({idx, seg[idx].sum, seg[idx].lazy});
    }

    void apply(int idx, int l, int r, ll val) {
        save(idx);
        seg[idx].sum *= val;
        seg[idx].lazy *= val;
    }

    void push(int idx, int l, int r) {
        if (seg[idx].lazy == 1) return;

        int mid = (l + r) >> 1;

        apply(idx * 2, l, mid, seg[idx].lazy);
        apply(idx * 2 + 1, mid + 1, r, seg[idx].lazy);

        save(idx);
        seg[idx].lazy = 1;
    }

    void update(int idx, int l, int r, int ql, int qr, ll val) {
        if (qr < l || r < ql) return;

        if (ql <= l && r <= qr) {
            apply(idx, l, r, val);
            return;
        }

        push(idx, l, r);

        int mid = (l + r) >> 1;

        update(idx * 2, l, mid, ql, qr, val);
        update(idx * 2 + 1, mid + 1, r, ql, qr, val);

        save(idx);
        seg[idx].sum = seg[idx * 2].sum + seg[idx * 2 + 1].sum;
    }

    ll query(int idx, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return 0;

        if (ql <= l && r <= qr)
            return seg[idx].sum;

        push(idx, l, r);

        int mid = (l + r) >> 1;

        return query(idx * 2, l, mid, ql, qr) +
               query(idx * 2 + 1, mid + 1, r, ql, qr);
    }

    // ===================== PUBLIC INTERFACE =====================
public:

    // multiply range [l, r] by x (0-based)
    void multiply(int l, int r, ll x) {
        update(1, 0, n - 1, l, r, x);
    }

    // query sum in [l, r] (0-based)
    ll query(int l, int r) {
        return query(1, 0, n - 1, l, r);
    }

    // undo last modification (node-level undo)
    void undo() {
        if (hist.empty()) return;

        Change c = hist.top();
        hist.pop();

        seg[c.idx].sum = c.sum;
        seg[c.idx].lazy = c.lazy;
    }
};