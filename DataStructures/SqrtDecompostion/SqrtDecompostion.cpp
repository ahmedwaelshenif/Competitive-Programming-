class SqrtDecomposition {
private:
    int n;
    int SQ;
    vector<int> arr;
    vector<int> block;
    vector<vector<int>> adj;

public:
    SqrtDecomposition(const vector<int>& input_arr) {
        n = input_arr.size();
        SQ = ceil(sqrt(n));
        arr = input_arr;
        block.assign(SQ, 0);
        adj.assign(SQ, vector<int>());
        preprocess();
    }

    void preprocess() {
        for (int i = 0; i < n; i++) {
            adj[i / SQ].push_back(arr[i]);
            block[i / SQ] += arr[i];
        }
    }

    int query(int l, int r) {
        int ret = 0;
        while (l <= r) {
            if (l % SQ == 0 && l + SQ - 1 <= r) {
                ret += block[l / SQ];
                l += SQ;
            } else {
                ret += arr[l];
                l++;
            }
        }
        return ret;
    }

    void update(int idx, int val) {
        block[idx / SQ] -= arr[idx];
        block[idx / SQ] += val;
        arr[idx] = val;
        adj[idx / SQ][idx % SQ] = val;
    }
};