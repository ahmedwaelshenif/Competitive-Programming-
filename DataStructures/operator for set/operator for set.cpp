struct Point {
    int x, y;
};

// Define operator< inside a comparator struct
struct Compare {
    bool operator()(const Point& a, const Point& b) const {
        if (a.x != b.x) return a.x < b.x;         // primary: sort by x ascending
        return a.y > b.y;                         // secondary: y descending
    }
};

int main() {
    set<Point, Compare> s;
    }