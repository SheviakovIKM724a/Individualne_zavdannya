#include <iostream>
#include <vector>

using namespace std;

struct Interval {
    int low, high;
    Interval(int l, int h) : low(l), high(h) {}
    bool overlaps(const Interval& other) const {
        return low <= other.high && other.low <= high;
    }
};

int max(int a, int b) {
    return (a > b) ? a : b;
}

class IntervalTree {
private:
    struct Node {
        Interval interval;
        int maxVal;
        Node* left;
        Node* right;

        Node(const Interval& i)
            : interval(i), maxVal(i.high), left(nullptr), right(nullptr) {
        }
    };

    Node* root = nullptr;

    Node* insert(Node* node, const Interval& interval) {
        if (!node) return new Node(interval);

        if (interval.low < node->interval.low)
            node->left = insert(node->left, interval);
        else
            node->right = insert(node->right, interval);

        node->maxVal = max(node->maxVal, interval.high);
        return node;
    }

    Node* remove(Node* node, const Interval& interval) {
        if (!node) return nullptr;

        if (interval.low < node->interval.low)
            node->left = remove(node->left, interval);
        else if (interval.low > node->interval.low)
            node->right = remove(node->right, interval);
        else if (interval.high == node->interval.high) {
            if (!node->left) {
                Node* right = node->right;
                delete node;
                return right;
            }
            else if (!node->right) {
                Node* left = node->left;
                delete node;
                return left;
            }
            else {
                Node* minRight = node->right;
                while (minRight->left) minRight = minRight->left;
                node->interval = minRight->interval;
                node->right = remove(node->right, minRight->interval);
            }
        }
        else {
            node->right = remove(node->right, interval);
        }

        node->maxVal = node->interval.high;
        if (node->left) node->maxVal = max(node->maxVal, node->left->maxVal);
        if (node->right) node->maxVal = max(node->maxVal, node->right->maxVal);

        return node;
    }

    void query(Node* node, const Interval& range, vector<Interval>& result) const {
        if (!node) return;
        if (node->interval.overlaps(range)) result.push_back(node->interval);
        if (node->left && node->left->maxVal >= range.low)
            query(node->left, range, result);
        query(node->right, range, result);
    }

    void destroy(Node* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    ~IntervalTree() {
        destroy(root);
    }

    void insert(const Interval& interval) {
        root = insert(root, interval);
    }

    void remove(const Interval& interval) {
        root = remove(root, interval);
    }

    vector<Interval> query(const Interval& range) const {
        vector<Interval> result;
        query(root, range, result);
        return result;
    }
};

// Демонстрация
int main() {
    IntervalTree tree;
    tree.insert({ 15, 20 });
    tree.insert({ 10, 30 });
    tree.insert({ 17, 19 });
    tree.insert({ 5, 20 });
    tree.insert({ 12, 15 });
    tree.insert({ 30, 40 });

    vector<Interval> overlaps = tree.query({ 14, 16 });
    for (const auto& interval : overlaps) {
        cout << "[" << interval.low << ", " << interval.high << "] ";
    }
    cout << endl;

    return 0;
}
