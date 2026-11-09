/*
 * Q2 - Pink Panther Likes Ordinary (Running Median)
 *
 * Maintain a multiset of integers under two operations:
 *   1 x : insert x
 *   2 x : erase one copy of x (x is guaranteed to be in the set)
 * After every operation print the median of the set (nothing if it is empty).
 * For an even number of elements the median is the LOWER of the two middle
 * elements, which is what the sample output expects.
 *
 * Approach: keep the set split into two halves.
 *   lower - max-heap with the smaller half (its top is the median)
 *   upper - min-heap with the larger half
 * Invariants: |lower| == |upper| or |lower| == |upper| + 1,
 *             and every element of lower <= every element of upper.
 *
 * A binary heap cannot erase an arbitrary element efficiently, so erasing is
 * lazy: the value is only recorded as "erased" and is physically popped later,
 * when it reaches the top of its heap.
 *
 * The heap itself is implemented by hand, as the problem requires.
 *
 * Complexity: O(q log q) time, O(q) memory.
 */

#include <functional>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

using namespace std;

// Binary heap written by hand.
// before(a, b) == true means a belongs above b, so less<int> gives a
// min-heap and greater<int> gives a max-heap.
template <typename Before>
class Heap {
public:
    bool empty() const { return data_.empty(); }
    int top() const { return data_[0]; }

    void push(int x) {
        data_.push_back(x);
        siftUp(static_cast<int>(data_.size()) - 1);
    }

    void pop() {
        data_[0] = data_.back();
        data_.pop_back();
        if (!data_.empty()) siftDown(0);
    }

private:
    vector<int> data_;
    Before before_;

    void siftUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (!before_(data_[i], data_[parent])) break;
            swap(data_[i], data_[parent]);
            i = parent;
        }
    }

    void siftDown(int i) {
        int n = static_cast<int>(data_.size());
        while (true) {
            int best = i;
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            if (left < n && before_(data_[left], data_[best])) best = left;
            if (right < n && before_(data_[right], data_[best])) best = right;
            if (best == i) break;
            swap(data_[i], data_[best]);
            i = best;
        }
    }
};

// Heap with lazy deletion. size() counts only the live (not erased) elements.
template <typename Before>
class LazyHeap {
public:
    int size() const { return live_; }

    void push(int x) {
        heap_.push(x);
        live_++;
    }

    // Marks one copy of x as erased. x must be present in this heap.
    void erase(int x) {
        erased_[x]++;
        live_--;
    }

    // Best live element. The heap must contain at least one.
    int top() {
        prune();
        return heap_.top();
    }

    // Removes and returns the best live element.
    int extractTop() {
        prune();
        int x = heap_.top();
        heap_.pop();
        live_--;
        return x;
    }

private:
    Heap<Before> heap_;
    map<int, int> erased_;  // value -> copies waiting to be physically popped
    int live_ = 0;

    // Pops erased elements that are sitting on top of the heap.
    void prune() {
        while (!heap_.empty()) {
            auto it = erased_.find(heap_.top());
            if (it == erased_.end() || it->second == 0) break;
            it->second--;
            heap_.pop();
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int q;
    cin >> q;

    LazyHeap<greater<int>> lower;  // max-heap: smaller half, top is the median
    LazyHeap<less<int>> upper;     // min-heap: larger half

    while (q--) {
        int type, x;
        cin >> type >> x;

        if (type == 1) {
            if (lower.size() == 0 || x <= lower.top()) lower.push(x);
            else upper.push(x);
        } else {
            // Everything in upper is >= lower.top(). If x == lower.top() the
            // copy can be taken from either side, so lower is fine.
            if (x <= lower.top()) lower.erase(x);
            else upper.erase(x);
        }

        // Restore |lower| == |upper| or |lower| == |upper| + 1.
        // A single move is always enough, since one operation changes a size
        // by at most one.
        if (lower.size() > upper.size() + 1) upper.push(lower.extractTop());
        else if (lower.size() < upper.size()) lower.push(upper.extractTop());

        if (lower.size() > 0) cout << lower.top() << "\n";
    }

    return 0;
}
