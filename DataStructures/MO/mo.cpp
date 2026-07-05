/*
====================== MO'S ALGORITHM FRAMEWORK ======================

MO STRUCT:
- addQuery(l, r, idx)          -> store queries (0-based inclusive)
- run(DS)                      -> executes Mo using a DS module
- DS must implement:
      void add(int x)
      void remove(int x)
      int get(int k=0)

----------------------------------------------------------------------
COMPLEXITY:
Let:
    N = array size
    Q = number of queries
    F = complexity of DS add/remove

Sorting queries:
    O(Q log Q)

Processing:
    O((N + Q) * sqrt(N) * F)

Total:
    O((N + Q) * sqrt(N) * F)

----------------------------------------------------------------------
SUPPORTED DS TYPES (examples in this file):

1) Distinct Count
   - add/remove: O(1)
   - get: O(1)
   - problem: number of distinct elements in range

2) Max Frequency
   - add/remove: O(1)
   - get: O(1)
   - problem: highest frequency in range

3) AtLeastK Frequency
   - add/remove: O(1)
   - get: O(1)
   - problem: count values with freq ≥ k

4) Mex
   - add/remove: O(log N)
   - get: O(log N)
   - problem:MEX in range

5) Mode (value)
   - add/remove: O(1)
   - get: O(1)
   - problem: most frequent element in range

6) FreqAtLeastK (dynamic freq tracking)
   - add/remove: O(1)
   - get: O(1)
   - problem: count numbers whose frequency ≥ k
   -Total: O(Nsqrt(Q) + Qsqrt(N))

7) Count pairs (i < j) such that:
     a[i] XOR a[j] = K
 Maintains frequency of values in current Mo window
 and counts valid pair contributions incrementally.
 add/remove: O(1)
 get: O(1)
 total: O(N sqrt Q)
 
 
 8) Count number of subarrays inside [L,R] whose XOR = K
	 IMPORTANT:
   - Mo array must be prefix XOR array (NOT original array)
	 add/remove: O(1)
	 get: O(1)
	 total: O(N sqrt Q)
 

----------------------------------------------------------------------
*/
struct Mo {
    struct Query {
        int l, r, k, idx;
    };

    int n;
    vector<int> a;
    vector<Query> q;

    Mo(vector<int> arr) : a(arr) {
        n = a.size();
    }

    void addQuery(int l, int r, int idx, int k = 0) {
        q.push_back({l, r, k, idx});
    }

    vector<int> run(auto &DS) {
        int Q = q.size();
        if (Q == 0) return {};
        
        // Calculate block size here, once we know how many queries we have
        int block_sz = max(1LL, (int)(n / sqrt(max(1LL, Q))));

        sort(q.begin(), q.end(), [&](const Query &x, const Query &y) {
            int bx = x.l / block_sz;
            int by = y.l / block_sz;
            if (bx != by) return bx < by;
            return (bx & 1) ? (x.r < y.r) : (x.r > y.r);
        });

        vector<int> ans(Q);
        int L = 0, R = -1;
        for (auto &qu : q) {
            while (L > qu.l) DS.add(a[--L]);
            while (R < qu.r) DS.add(a[++R]);
            while (L < qu.l) DS.remove(a[L++]);
            while (R > qu.r) DS.remove(a[R--]);

            ans[qu.idx] = DS.get(qu.k); 
        }
        return ans;
    }
};

/////Now everything depends on the DS (data structure module)./////

//“How many distinct elements in [L, R]?”
struct Distinct {
    vector<int> freq;   //OR USE MAP
    int cur = 0;

    Distinct(int maxVal = 1e6) {
        freq.assign(maxVal + 5, 0);
    }

    void add(int x) {
        if (freq[x] == 0) cur++;
        freq[x]++;
    }

    void remove(int x) {
        freq[x]--;
        if (freq[x] == 0) cur--;
    }

    int get(int k=0) {
        return cur;
    }
};



///Return frequency of most frequent element in range
struct MaxFreq {
    vector<int> freq;
    vector<int> bucket;
    int best = 0;

    MaxFreq(int maxVal = 1e6) {
        freq.assign(maxVal + 5, 0);
        bucket.assign(maxVal + 5, 0);
    }

    void add(int x) {
        int f = freq[x];
        bucket[f]--;
        freq[x]++;
        bucket[freq[x]]++;
        best = max(best, freq[x]);
    }

    void remove(int x) {
        int f = freq[x];
        bucket[f]--;
        freq[x]--;
        bucket[freq[x]]++;
        while (best > 0 && bucket[best] == 0) best--;
    }

    int get(int k=0) {
        return best;
    }
};


//“Count elements appearing ≥ k times”
struct AtLeastK {
    vector<int> freq;  /// OR USE MAP
    int k, cnt = 0;

    AtLeastK(int K, int maxVal = 1e6) {
        k = K;
        freq.assign(maxVal + 5, 0);
    }

    void add(int x) {
        freq[x]++;
        if (freq[x] == k) cnt++;
    }

    void remove(int x) {
        if (freq[x] == k) cnt--;
        freq[x]--;
    }

    int get(int k=0) {
        return cnt;
    }
};

// Mex in range
struct Mex {
    vector<int> freq;
    set<int> missing;

    Mex(int maxVal = 1e6) {
        freq.assign(maxVal + 5, 0);
        for(int i = 0; i <= maxVal; i++) missing.insert(i);
    }

    void add(int x) {
        if (++freq[x] == 1)
            missing.erase(x);
    }

    void remove(int x) {
        if (--freq[x] == 0)
            missing.insert(x);
    }

    int get(int k=0) {
        return *missing.begin();
    }
};
        
    //actual element with max frequency
struct Mode {
    vector<int> freq;
    vector<vector<int>> valPerFreq; // Elements that have a specific frequency
    int best = 0;

    Mode(int maxVal = 1e6) {
        freq.assign(maxVal + 5, 0);
        valPerFreq.resize(maxVal + 5);
    }

    void add(int x) {
        int f = freq[x];
        freq[x]++;
        valPerFreq[freq[x]].push_back(x);
        if (freq[x] > best) best = freq[x];
    }

    void remove(int x) {
        freq[x]--;
        while (best > 0 && (valPerFreq[best].empty() || freq[valPerFreq[best].back()] != best)) {
            if (valPerFreq[best].empty()) {
                best--;
            } else {
                valPerFreq[best].pop_back(); // Clean up stale values
            }
        }
    }

    int get(int k = 0) { // k is ignored here
        return (best == 0) ? -1 : valPerFreq[best].back();
    }
};


/////“count values with freq ≥ k” (dynamic k)
struct MoDynamicK {
    vector<int> freq;      // How many times value 'x' appears
    vector<int> cntFreq;   // How many values have frequency 'f'
    vector<int> blockSum;  // SQRT decomposition to query cntFreq ranges
    int n, blockSize;

    MoDynamicK(int maxVal, int arraySize) {
        n = arraySize + 1;
        blockSize = sqrt(n) + 1;
        freq.assign(maxVal + 5, 0);
        cntFreq.assign(n + 5, 0);
        blockSum.assign(blockSize + 5, 0);
        
        // Initial state: all values have frequency 0
        // (Though we usually only track active elements)
    }

    void add(int x) {
        int oldF = freq[x];
        // Remove old frequency from tracking
        if (oldF > 0) {
            cntFreq[oldF]--;
            blockSum[oldF / blockSize]--;
        }
        
        // Add new frequency
        freq[x]++;
        int newF = freq[x];
        cntFreq[newF]++;
        blockSum[newF / blockSize]++;
    }

    void remove(int x) {
        int oldF = freq[x];
        cntFreq[oldF]--;
        blockSum[oldF / blockSize]--;

        freq[x]--;
        int newF = freq[x];
        if (newF > 0) {
            cntFreq[newF]++;
            blockSum[newF / blockSize]++;
        }
    }

    // New get function that accepts the specific K for this query
    int get(int k) {
        if (k <= 0) return 0; // Or total distinct elements
        if (k >= n) return 0;

        int res = 0;
        int startBlock = k / blockSize;
        int endBlock = (n - 1) / blockSize;

        if (startBlock == endBlock) {
            for (int i = k; i < n; i++) res += cntFreq[i];
        } else {
            // Elements in the first partial block
            for (int i = k; i < (startBlock + 1) * blockSize; i++) res += cntFreq[i];
            // Full blocks
            for (int b = startBlock + 1; b < endBlock; b++) res += blockSum[b];
            // Elements in the last block
            for (int i = endBlock * blockSize; i < n; i++) res += cntFreq[i];
        }
        return res;
    }
};


// For each query [L, R], count pairs (i < j) such that:
struct XorPairK {
    unordered_map<int, int> freq;
    int K;
    long long cnt = 0;

    XorPairK(int k) : K(k) {}

    void add(int x) {
        cnt += freq[x ^ K]; // pairs formed with previous elements
        freq[x]++;
    }

    void remove(int x) {
        freq[x]--;
        cnt -= freq[x ^ K]; // undo contribution
    }

    long long get(int k = 0) {
        return cnt;
    }
};



///Count number of subarrays inside current Mo window whose XOR = K
//MO takes prefix XOR 1-based!!!
// query:int l,r; cin>>l>>r;
       
        //mo.addQuery(l-1,r,i);
struct SubarrayXorK {
    unordered_map<long long, long long> freq;
    long long ans = 0;
    long long K;

    SubarrayXorK(long long k) : K(k) {}

    void add(long long x) {
        ans += freq[x ^ K];
        freq[x]++;
    }

    void remove(long long x) {
        freq[x]--;
        ans -= freq[x ^ K];
    }

    long long get(long long k = 0) {
        return ans;
    }
};

////////////////////////////// How to use in main
	  Mo mo(a);

    for (int i = 0; i < q; i++) {
        int l, r;
        cin >> l >> r;
        --l; --r; // convert to 0-based
        mo.addQuery(l, r, i);
    }

    Distinct ds(1e6); // adjust max value if needed

    vector<int> ans = mo.run(ds);

    for (auto x : ans) {
        cout << x << "\n";
    }