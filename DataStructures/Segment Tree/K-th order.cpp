struct SegmentTree {
    struct Node {
        int sum;
        int left, right;
        Node(int s = 0, int l = 0, int r = 0) : sum(s), left(l), right(r) {}
    };
 
    std::vector<Node> pool;
    std::vector<int> root;
    std::vector<int> value;
    int M;
 
    int build(int tl, int tr) {
        int idx = (int)pool.size();
        pool.emplace_back(0, 0, 0);
        if (tl != tr) {
            int tm = (tl + tr) >> 1;
            pool[idx].left  = build(tl, tm);
            pool[idx].right = build(tm + 1, tr);
        }
        return idx;
    }
 
    int add(int prev, int tl, int tr, int pos) {
        int idx = (int)pool.size();
        pool.emplace_back(pool[prev]);
        ++pool[idx].sum;
        if (tl != tr) {
            int tm = (tl + tr) >> 1;
            if (pos <= tm)
                pool[idx].left  = add(pool[prev].left,  tl, tm, pos);
            else
                pool[idx].right = add(pool[prev].right, tm + 1, tr, pos);
        }
        return idx;
    }
 
 
    int kth(int uL, int uR, int tl, int tr, int k) const {
        if (tl == tr) return tl;
        int cntLeft = pool[ pool[uR].left ].sum - pool[ pool[uL].left ].sum;
        int tm = (tl + tr) >> 1;
        if (k <= cntLeft)
            return kth(pool[uL].left,  pool[uR].left,  tl, tm, k);
        return  kth(pool[uL].right, pool[uR].right, tm + 1, tr, k - cntLeft);
    }
 
public:
 
    explicit SegmentTree(const std::vector<int>& a) {
        int n = (int)a.size();
 
        value = a;
        std::sort(value.begin(), value.end());
        value.erase(std::unique(value.begin(), value.end()), value.end());
        M = (int)value.size();
 
        pool.reserve(n * 20);
        root.resize(n + 1);
        root[0] = build(0, M - 1);
 
        for (int i = 0; i < n; ++i) {
            int p = std::lower_bound(value.begin(), value.end(), a[i]) - value.begin();
            root[i + 1] = add(root[i], 0, M - 1, p);
        }
    }
 
    int kth_smallest(int l, int r, int k) const {
        int idx = kth(root[l], root[r + 1], 0, M - 1, k);
        return value[idx];
    }
};
