class FenwickTree {
private:
    vector<int> bit;
    int size;

public:
    FenwickTree(int n) {
        size = n + 2;
        bit.resize(size, 0);
    }

    void update(int i, int val) {
        while (i < size) {
            bit[i] += val;
            i += i & -i;
        }
    }

    int query(int i) {
        int sum = 0;
        while (i > 0) {
            sum += bit[i];
            i -= i & -i;
        }
        return sum;
    }
};
////////////////////////////////////////////////////////
vector<int> countSmallerRight(vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n);
    
    // Coordinate compression
    vector<int> sorted_nums(nums);
    sort(sorted_nums.begin(), sorted_nums.end());
    unordered_map<int, int> compressed;
    int rank = 1;
    for (int num : sorted_nums)
        if (compressed.find(num) == compressed.end())
            compressed[num] = rank++;

    FenwickTree bit(rank);

    // Process from right to left
    for (int i = n - 1; i >= 0; --i) {
        int idx = compressed[nums[i]];
        result[i] = bit.query(idx - 1);  // elements < nums[i]
        bit.update(idx, 1);              // add current element
    }

    return result;
}
/////////////////////////////////////////////
vector<int> countSmallerLeft(const vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n);
    vector<int> sorted_nums(nums);

    // Coordinate compression
    sort(sorted_nums.begin(), sorted_nums.end());
    unordered_map<int, int> compressed;
    int rank = 1;
    for (int num : sorted_nums)
        if (compressed.find(num) == compressed.end())
            compressed[num] = rank++;

    FenwickTree bit(rank);

    // Traverse from left to right
    for (int i = 0; i < n; ++i) {
        int idx = compressed[nums[i]];
        result[i] = bit.query(idx - 1); // Count of smaller elements seen so far
        bit.update(idx, 1);             // Mark this number as seen
    }

    return result;
}
///////////////////////////////////////////

vector<int> countGreaterLeft(const vector<int>& nums) {
    int n = nums.size();
    vector<int> result(n);
    vector<int> sorted_nums(nums);

    // Coordinate compression
    sort(sorted_nums.begin(), sorted_nums.end());
    unordered_map<int, int> compressed;
    int rank = 1;
    for (int num : sorted_nums)
        if (compressed.find(num) == compressed.end())
            compressed[num] = rank++;

    FenwickTree bit(rank);
    for (int i = 0; i < n; ++i) {
        int idx = compressed[nums[i]];
        // i elements have been seen so far
        result[i] = i - bit.query(idx);  // Greater = total - lessEqual
        bit.update(idx, 1);              // Mark current number as seen
    }

    return result;
}

