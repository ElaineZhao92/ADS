## 8-4 Red-black Tree (20 分)

There is a kind of binary tree named **red-black tree** in the data structure. It has the following 5 properties:

- (1) Every node is either red or black.
- (2) The root is black.
- (3) All the leaves are NULL nodes and are colored black.
- (4) Each red node must have 2 black descends (may be NULL).
- (5) All simple paths from any node x to a descendant leaf have the same number of black nodes.

We call a non-NULL node an **internal node**. From property (5) we can define the **black-height** of a red-black tree as the number of nodes on the simple path from the root (excluding the root itself) to any NULL leaf (including the NULL leaf). And we can derive that a red-black tree with black height H has at least 2*H*−1 internal nodes.

Here comes the question: given a positive *N*, how many distinct red-black trees are there that consist of exactly *N* internal nodes?

### Input Specification:

Each input file contains one test case which gives a positive integer *N* (≤500).

### Output Specification:

For each case, print in a line the number of distinct red-black tees with *N* internal nodes. Since the answer may be very large, output the remainder of it divided by 1000000007 please.

### Sample Input:

```c
5
```

### Sample Output:

```c
8
```

### Grading Policy:

**Programming:** Write the program (10 pts.) **with sufficient comments**.

**Testing:** Provide a set of test cases to fill in a test report (3 pts.). Write analysis and comments (3 pts.).

**Documentation:** Chapter 1 (1 pt.), Chapter 2 (2 pts.), and finally a complete report (1 point for overall style of documentation).