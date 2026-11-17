# Pink Panther Likes Ordinary (Running Median)

**پلنگ صورتی معمولی پسنده**

Algorithm Design - Homework 1, Problem 2 (Heap & Greedy algorithms)

- **Time limit:** 1 second
- **Memory limit:** 256 MB
- **Requirement:** the heap must be implemented by hand (not the STL `priority_queue`).

## Problem

The Pink Panther likes everything to be moderate: not too big, not too small.
For tonight's concert he has to pick a violin from a collection of violins of
different sizes, where each size is an integer. At every moment he wants to know
the **median** of the collection.

An annoying inspector keeps changing the collection: he either adds a new
violin, or removes an existing one. After every operation, report the median of
the collection to the Pink Panther.

### Input

- First line: an integer `q`, the number of operations.
- Next `q` lines, each one of:
  - `1 x` - the integer `x` is added to the set (a new violin size).
  - `2 x` - one copy of `x` is removed from the set. It is guaranteed that `x`
    is in the set.

Constraints:

```
1 <= q <= 200000
-10^9 <= x <= 10^9
```

Notes:

1. The set may contain duplicates.
2. Every removal is guaranteed to remove a value that exists at least once.
3. The set is initially empty.

### Output

After each operation, if the set is not empty, print its median on one line.
If the set is empty, print nothing.

For a set with an even number of elements, the median is the **lower** of the
two middle elements (this is what the sample output expects).

### Example

Input:

```
10
1 5
1 3
1 8
1 12
2 8
1 7
2 5
1 10
2 12
2 3
```

Output:

```
5
3
5
5
5
5
7
7
7
7
```

| Operation | Set after it    | Median |
|-----------|-----------------|--------|
| `1 5`     | {5}             | 5      |
| `1 3`     | {3, 5}          | 3      |
| `1 8`     | {3, 5, 8}       | 5      |
| `1 12`    | {3, 5, 8, 12}   | 5      |
| `2 8`     | {3, 5, 12}      | 5      |
| `1 7`     | {3, 5, 7, 12}   | 5      |
| `2 5`     | {3, 7, 12}      | 7      |
| `1 10`    | {3, 7, 10, 12}  | 7      |
| `2 12`    | {3, 7, 10}      | 7      |
| `2 3`     | {7, 10}         | 7      |

## Solution

This is the classic *running median* problem, solved with **two heaps**.

- `lower` is a max-heap holding the smaller half of the elements. Its top is
  the median.
- `upper` is a min-heap holding the larger half.
- Invariants: `|lower| == |upper|` or `|lower| == |upper| + 1`, and every
  element of `lower` is `<=` every element of `upper`.

**Insert `x`:** push it into `lower` if `x <= lower.top()` (or `lower` is
empty), otherwise into `upper`.

**Erase `x`:** if `x <= lower.top()` it belongs to `lower`, otherwise to
`upper`.

After each operation, at most one element has to move from one heap to the other
to restore the size invariant.

A binary heap cannot delete an arbitrary element in `O(log q)` directly, so
deletion is **lazy**: the value is only recorded in a map of "erased" counts and
is physically popped later, when it reaches the top of its heap. Each heap keeps
its own count of live elements, so the sizes used for balancing stay exact.

**Complexity:** `O(q log q)` time and `O(q)` memory.

## Build & Run

```bash
g++ -std=c++17 -O2 Q2.cpp -o Q2
./Q2 < input.txt
```
