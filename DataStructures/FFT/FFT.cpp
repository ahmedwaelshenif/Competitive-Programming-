const double PI = acos(-1);
using cd = complex<double>;

struct FFT {
    // Performs the Cooley-Tukey FFT in O(n log n)
    void fft(vector<cd>& a, bool invert) {
        int n = a.size();
        for (int i = 1, j = 0; i < n; ++i) {
            int bit = n >> 1;
            for (; j & bit; bit >>= 1) j ^= bit;
            j ^= bit;
            if (i < j) swap(a[i], a[j]);
        }

        for (int len = 2; len <= n; len <<= 1) {
            double ang = 2 * PI / len * (invert ? -1 : 1);
            cd wlen = polar(1.0, ang);
            for (int i = 0; i < n; i += len) {
                cd w = cd(1.0, 0.0);
                for (int j = 0; j < len / 2; ++j) {
                    cd u = a[i + j];
                    cd v = a[i + j + len / 2] * w;
                    a[i + j] = u + v;
                    a[i + j + len / 2] = u - v;
                    w *= wlen;
                }
            }
        }

        if (invert) {
            for (cd& x : a) x /= n;
        }
    }

    // Multiply two polynomials a and b
    // Time Complexity: O(n log n)
    vector<int> multiply(const vector<int>& a, const vector<int>& b) {
        vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        fa.resize(n); fb.resize(n);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; ++i) fa[i] *= fb[i];
        fft(fa, true);

        vector<int> result(n);
        for (int i = 0; i < n; ++i) result[i] = round(fa[i].real());
        return result;
    }

    // Convolution under mod for large primes (mod must be < 1e9)
    // Time Complexity: O(n log n)
    vector<int> multiply_mod(const vector<int>& a, const vector<int>& b, int mod) {
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        vector<cd> A(n), B(n);
        for (int i = 0; i < a.size(); ++i)
            A[i] = cd(a[i] & ((1 << 15) - 1), a[i] >> 15);
        for (int i = 0; i < b.size(); ++i)
            B[i] = cd(b[i] & ((1 << 15) - 1), b[i] >> 15);

        fft(A, false); fft(B, false);

        vector<cd> aa(n), bb(n), cc(n);
        for (int i = 0; i < n; ++i) {
            int j = (n - i) & (n - 1);
            cd a1 = (A[i] + conj(A[j])) * cd(0.5, 0);
            cd a2 = (A[i] - conj(A[j])) * cd(0, -0.5);
            cd b1 = (B[i] + conj(B[j])) * cd(0.5, 0);
            cd b2 = (B[i] - conj(B[j])) * cd(0, -0.5);
            aa[i] = a1 * b1;
            bb[i] = a1 * b2;
            cc[i] = a2 * b2;
        }

        fft(aa, true); fft(bb, true); fft(cc, true);
        vector<int> result(n);
        for (int i = 0; i < n; ++i) {
            long long x = llround(aa[i].real());
            long long y = llround(bb[i].real());
            long long z = llround(cc[i].real());
            result[i] = (x + (y % mod << 15) + (z % mod << 30)) % mod;
        }
        return result;
    }

    // Multiply two polynomials (double coefficients)
    // Returns vector<cd>
    vector<cd> multiply_complex(const vector<cd>& a, const vector<cd>& b) {
        vector<cd> fa = a, fb = b;
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        fa.resize(n); fb.resize(n);

        fft(fa, false);
        fft(fb, false);
        for (int i = 0; i < n; ++i) fa[i] *= fb[i];
        fft(fa, true);
        return fa;
    }
    string multiply_bigints(const string& sa, const string& sb) {
        vector<int> a, b;
        for (int i = sa.size() - 1; i >= 0; --i) a.push_back(sa[i] - '0');
        for (int i = sb.size() - 1; i >= 0; --i) b.push_back(sb[i] - '0');

        vector<int> result = multiply(a, b);  // Use FFT multiply

        // Handle carry
        int carry = 0;
        for (int i = 0; i < result.size(); ++i) {
            result[i] += carry;
            carry = result[i] / 10;
            result[i] %= 10;
        }

        // Handle remaining carry
        while (carry) {
            result.push_back(carry % 10);
            carry /= 10;
        }

        // Remove leading zeros
        while (result.size() > 1 && result.back() == 0) result.pop_back();

        // Convert back to string
        string ans;
        for (int i = result.size() - 1; i >= 0; --i)
            ans += result[i] + '0';

        return ans;
    }
};