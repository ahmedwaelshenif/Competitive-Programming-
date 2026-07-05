#include <bits/stdc++.h>
using namespace std;
#define int long long

struct Node {
    int sum, pref, suff, ans;
    Node(int val = 0) {
        sum = val;
        pref = suff = ans = val;
    }
};

Node merge(const Node &L, const Node &R) {
    Node res;
    res.sum = L.sum + R.sum;
    res.pref = max(L.pref, L.sum + R.pref);
    res.suff = max(R.suff, R.sum + L.suff);
    res.ans = max({L.ans, R.ans, L.suff + R.pref});
    return res;
}

class SegmentTree {
private:
    int SZ;
    vector<Node> tree;

    void build(int node, int l, int r, vector<int> &arr) {
        if (l == r) {
            tree[node] = Node(arr[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, arr);
        build(node * 2 + 1, mid + 1, r, arr);
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    void update(int node, int l, int r, int idx, int val) {
        if (l == r) {
            tree[node] = Node(val);
            return;
        }
        int mid = (l + r) / 2;
        if (idx <= mid) update(node * 2, l, mid, idx, val);
        else update(node * 2 + 1, mid + 1, r, idx, val);
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    Node query(int node, int l, int r, int ql, int qr) {
        if (ql > r || qr < l) {
            // Neutral element for merging
            return Node(-1e18); 
        }
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        Node left = query(node * 2, l, mid, ql, qr);
        Node right = query(node * 2 + 1, mid + 1, r, ql, qr);
        if (left.ans == -1e18) return right;
        if (right.ans == -1e18) return left;
        return merge(left, right);
    }

public:
    SegmentTree(vector<int> &arr) {
        SZ = arr.size();
        tree.resize(4 * SZ);
        build(1, 0, SZ - 1, arr);
    }
    void update(int idx, int val) {
        update(1, 0, SZ - 1, idx, val);
    }
    int query(int l, int r) {
        return query(1, 0, SZ - 1, l, r).ans;
    }
};

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];

    SegmentTree st(arr);

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int idx, val;
            cin >> idx >> val;
            idx--; // 0-based
            st.update(idx, val);
        } else {
            int l, r;
            cin >> l >> r;
            l--, r--; // 0-based
            cout << st.query(l, r) << "\n";
        }
    }
}
