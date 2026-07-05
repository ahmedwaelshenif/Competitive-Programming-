#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long val = 0, lazy = 0;
    Node *left = nullptr, *right = nullptr;

    void apply(int l, int r, long long delta) {
        val += delta * (r - l + 1);
        lazy += delta;
    }
};

	class DynamicSegmentTree {
		private:
    Node* root;
    int L, R;

    void push(Node* node, int l, int r) {
        if (node->lazy != 0) {
            int mid = (l + r) / 2;
            if (!node->left) node->left = new Node();
            if (!node->right) node->right = new Node();
            node->left->apply(l, mid, node->lazy);
            node->right->apply(mid + 1, r, node->lazy);
            node->lazy = 0;
        }
    }

    void update(Node* node, int l, int r, int ql, int qr, long long delta) {
        if (!node || r < ql || l > qr) return;
        if (ql <= l && r <= qr) {
            node->apply(l, r, delta);
            return;
        }
        push(node, l, r);
        int mid = (l + r) / 2;
        if (!node->left) node->left = new Node();
        if (!node->right) node->right = new Node();
        update(node->left, l, mid, ql, qr, delta);
        update(node->right, mid + 1, r, ql, qr, delta);
        node->val = (node->left ? node->left->val : 0) + (node->right ? node->right->val : 0);
    }

    long long query(Node* node, int l, int r, int ql, int qr) {
        if (!node || r < ql || l > qr) return 0;
        if (ql <= l && r <= qr) return node->val;
        push(node, l, r);
        int mid = (l + r) / 2;
        return query(node->left, l, mid, ql, qr) + query(node->right, mid + 1, r, ql, qr);
    }

public:
    DynamicSegmentTree(int L = 1, int R = 1e9) : L(L), R(R) {
        root = new Node();
    }

    void rangeUpdate(int ql, int qr, long long delta) {
        update(root, L, R, ql, qr, delta);
    }

    long long rangeQuery(int ql, int qr) {
        return query(root, L, R, ql, qr);
    }

    void pointUpdate(int idx, long long delta) {
        rangeUpdate(idx, idx, delta);
    }

    long long pointQuery(int idx) {
        return rangeQuery(idx, idx);
    }
};



// update point and query range





struct Node {
    long long val = 1;            // neutral for multiplication
    Node *left = nullptr, *right = nullptr;
};

class DynamicSegmentTree {
private:
    Node* root;
    int L, R;

    void update(Node* node, int l, int r, int pos, long long value) {
        if (l == r) {
            node->val = value % MOD;
            return;
        }

        int mid = (l + r) / 2;

        if (pos <= mid) {
            if (!node->left) node->left = new Node();
            update(node->left, l, mid, pos, value);
        } else {
            if (!node->right) node->right = new Node();
            update(node->right, mid + 1, r, pos, value);
        }

        long long leftVal  = node->left  ? node->left->val  : 1;
        long long rightVal = node->right ? node->right->val : 1;

        node->val = mul(leftVal , rightVal);
    }

    long long query(Node* node, int l, int r, int ql, int qr) {
        if (r < ql || l > qr) return 1;
        if (!node) return 0;
        if (ql <= l && r <= qr) return node->val;
        int mid = (l + r) / 2;
        return mul(query(node->left, l, mid, ql, qr), query(node->right, mid + 1, r, ql, qr));
    }

public:
    DynamicSegmentTree(int L = 1, int R = 1e9) : L(L), R(R) {
        root = new Node();
    }

    // SET value at index
    void pointUpdate(int idx, long long value) {
        update(root, L, R, idx, value);
    }

    // MULTIPLY range
    long long rangeQuery(int l, int r) {
        return query(root, L, R, l, r);
    }
};







