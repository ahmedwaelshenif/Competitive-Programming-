////////////// 1-based   to change to 0 based change to 0,sz-1 and seg[0]
class segTree {
    struct Node {
        int sm, pref, suf, mx;
        int l, r;
        int prefL, prefR;
        int sufL, sufR;
        int mxL, mxR;

        Node(int val = 0, int pos = -1) {
            sm = val;

            l = r = pos;

            if (pos == -1) {
                pref = suf = mx = 0;

                prefL = prefR = -1;
                sufL = sufR = -1;
                mxL = mxR = -1;

                return;
            }

            pref = suf = mx = max(0LL, val);

            prefL = prefR = pos;
            sufL = sufR = pos;
            mxL = mxR = pos;
        }
    };

    int sz;
    vector<Node> seg;

    Node merge(const Node& L, const Node& R) {
        if (L.l == -1) return R;
        if (R.l == -1) return L;

        Node res;

        res.l = L.l;
        res.r = R.r;

        res.sm = L.sm + R.sm;

        res.pref = L.pref;
        res.prefL = L.prefL;
        res.prefR = L.prefR;

        if (L.sm + R.pref > res.pref) {
            res.pref = L.sm + R.pref;
            res.prefL = L.l;
            res.prefR = R.prefR;
        }

        res.suf = R.suf;
        res.sufL = R.sufL;
        res.sufR = R.sufR;

        if (L.suf + R.sm > res.suf) {
            res.suf = L.suf + R.sm;
            res.sufL = L.sufL;
            res.sufR = R.r;
        }

        res.mx = L.mx;
        res.mxL = L.mxL;
        res.mxR = L.mxR;

        if (R.mx > res.mx) {
            res.mx = R.mx;
            res.mxL = R.mxL;
            res.mxR = R.mxR;
        }

        if (L.suf + R.pref > res.mx) {
            res.mx = L.suf + R.pref;
            res.mxL = L.sufL;
            res.mxR = R.prefR;
        }

        return res;
    }

    void upd(int idx, int val, int st, int en, int p) {
        if (st == en) {
            seg[p] = Node(val, st);
            return;
        }

        int mid = (st + en) / 2;

        if (idx <= mid)
            upd(idx, val, st, mid, 2 * p);
        else
            upd(idx, val, mid + 1, en, 2 * p + 1);

        seg[p] = merge(seg[2 * p], seg[2 * p + 1]);
    }

    Node get(int l, int r, int st, int en, int p) {
        if (r < st || en < l)
            return Node();

        if (l <= st && en <= r)
            return seg[p];

        int mid = (st + en) / 2;

        Node L = get(l, r, st, mid, 2 * p);
        Node R = get(l, r, mid + 1, en, 2 * p + 1);

        return merge(L, R);
    }

public:
    segTree(int n) {
        sz = n;
        seg.resize(4 * n + 5);
    }

    void update(int idx, int val) {
        upd(idx, val, 1, sz, 1);
    }

    Node query(int l, int r) {
        return get(l, r, 1, sz, 1);
    }

    int queryMax(int l, int r) {
        return query(l, r).mx;
    }

    pair<int,int> queryRange(int l, int r) {
        Node ans = query(l, r);
        return {ans.mxL, ans.mxR};
    }

    int getMax() {
        return seg[1].mx;
    }

    pair<int,int> getRange() {
        return {seg[1].mxL, seg[1].mxR};
    }
};