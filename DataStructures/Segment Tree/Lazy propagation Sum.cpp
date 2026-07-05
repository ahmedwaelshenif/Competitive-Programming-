class SegmentTree {
#define L 2*node+1
#define R 2*node+2
#define mid (l+r)/2
private:
    int SZ;
    vector<int> tree, lazy;

    void build(int node, int l, int r, vector<int>& arr) {
        if (l == r) {
            tree[node] = arr[l];
            return;
        }
        build(L, l, mid, arr);
        build(R, mid+1, r, arr);
        tree[node] = tree[L] + tree[R];
    }

    void propagate(int node, int l, int r) {
        if (lazy[node] == 0) return;

        tree[node] += (r - l + 1) * lazy[node]; 
        if (l != r) {
            lazy[L] += lazy[node]; 
            lazy[R] += lazy[node]; 
        }
        lazy[node] = 0;
    }

    void update(int node, int l, int r, int lq, int rq, int val) {
        propagate(node, l, r);
        if (r < lq || l > rq) return;
        if (l >= lq && r <= rq) {
            lazy[node] += val;
            propagate(node, l, r);
            return;
        }
        update(L, l, mid, lq, rq, val);
        update(R, mid+1, r, lq, rq, val);
        tree[node] = tree[L] + tree[R];
    }

    int query(int node, int l, int r, int lq, int rq) {
        propagate(node, l, r);
        if (r < lq || l > rq) return 0; 
        if (l >= lq && r <= rq) return tree[node];
        return query(L, l, mid, lq, rq) + query(R, mid+1, r, lq, rq); 
    }

public:
    SegmentTree(vector<int>& arr) {
        SZ = arr.size();
        tree.assign(4 * SZ, 0);
        lazy.assign(4 * SZ, 0);
        build(0, 0, SZ - 1, arr);
    }

    void update(int l, int r, int val) {
        update(0, 0, SZ - 1, l, r, val);
    }

    int query(int l, int r) {
        return query(0, 0, SZ - 1, l, r);
    }

#undef L
#undef R
#undef mid
};