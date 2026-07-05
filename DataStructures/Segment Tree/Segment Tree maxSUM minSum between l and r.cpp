#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long sum;       // total sum
    long long prefMax;   // max prefix sum
    long long suffMax;   // max suffix sum
    long long bestMax;   // max subarray sum
    long long prefMin;   // min prefix sum
    long long suffMin;   // min suffix sum
    long long bestMin;   // min subarray sum

    Node() {
        sum = prefMax = suffMax = bestMax = 0;
        prefMin = suffMin = bestMin = 0;
    }

    Node(long long val) {
        sum = val;
        prefMax = suffMax = bestMax = val;
        prefMin = suffMin = bestMin = val;
    }
};

struct SegmentTree {
    int n;
    vector<Node> tree;

    SegmentTree(vector<long long>& arr) {
        n = arr.size();
        tree.resize(4 * n);
        build(1, 0, n - 1, arr);
    }

    Node merge(const Node& L, const Node& R) {
        Node res;
        res.sum = L.sum + R.sum;

        // Max values
        res.prefMax = max(L.prefMax, L.sum + R.prefMax);
        res.suffMax = max(R.suffMax, R.sum + L.suffMax);
        res.bestMax = max({L.bestMax, R.bestMax, L.suffMax + R.prefMax});

        // Min values
        res.prefMin = min(L.prefMin, L.sum + R.prefMin);
        res.suffMin = min(R.suffMin, R.sum + L.suffMin);
        res.bestMin = min({L.bestMin, R.bestMin, L.suffMin + R.prefMin});

        return res;
    }

    void build(int node, int l, int r, vector<long long>& arr) {
        if (l == r) {
            tree[node] = Node(arr[l]);
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, arr);
        build(node * 2 + 1, mid + 1, r, arr);
        tree[node] = merge(tree[node * 2], tree[node * 2 + 1]);
    }

    Node query(int node, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return Node(0); // neutral node
        if (ql <= l && r <= qr) return tree[node];
        int mid = (l + r) / 2;
        Node left = query(node * 2, l, mid, ql, qr);
        Node right = query(node * 2 + 1, mid + 1, r, ql, qr);
        if (left.sum == 0 && left.bestMax == 0 && left.bestMin == 0) return right;
        if (right.sum == 0 && right.bestMax == 0 && right.bestMin == 0) return left;
        return merge(left, right);
    }

    long long getMaxSubarray(int l, int r) {
        return query(1, 0, n - 1, l, r).bestMax;
    }

    long long getMinSubarray(int l, int r) {
        return query(1, 0, n - 1, l, r).bestMin;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    cin >> n >> q;
    vector<long long> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];

    SegmentTree st(arr);

    while (q--) {
        int l, r;
        cin >> l >> r;
        l--, r--; // make 0-indexed
        cout << "MaxSubarray: " << st.getMaxSubarray(l, r)
             << " MinSubarray: " << st.getMinSubarray(l, r) << "\n";
    }
}
