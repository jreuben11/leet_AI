# Data Structures and Algorithms in C

A comprehensive collection of classic data structures and algorithms implemented in C, with detailed explanations and complexity analysis.

## Table of Contents

- [Project Structure](#project-structure)
- [Building and Running](#building-and-running)
- [Data Structures & Algorithms](#data-structures--algorithms)
  - [1. Recursion](#1-recursion)
  - [2. Linked Lists](#2-linked-lists)
  - [3. Stacks and Queues](#3-stacks-and-queues)
  - [4. Trees](#4-trees)
  - [5. Heaps](#5-heaps)
  - [6. Skip Lists](#6-skip-lists)
  - [7. List Search Algorithms](#7-list-search-algorithms)
  - [8. Dynamic Programming](#8-dynamic-programming)
  - [9. Graphs and Shortest Path Algorithms](#9-graphs-and-shortest-path-algorithms)
  - [10. Hash Tables](#10-hash-tables)
  - [11. Sorting Algorithms](#11-sorting-algorithms)
- [Complexity Summary](#complexity-summary)

---

## Project Structure

```
dsa_c/
├── src/                    # Source files
│   ├── 1_recursion.c
│   ├── 2_linked_lists.c
│   ├── 2_linked_lists.h
│   ├── 3_stacks_and_queues.c
│   ├── 4_trees.c
│   ├── 5_heap.c
│   ├── 6_skip_list.c
│   ├── 7_list_search.c
│   ├── 8_dynamic_programming.c
│   ├── 9_graphs.c
│   ├── 10_hash_tables.c
│   └── 11_sort.c
├── makefiles/              # Build configurations
│   ├── recursion.mk
│   ├── linked_lists.mk
│   ├── stacks_and_queues.mk
│   ├── trees.mk
│   ├── 5_heap.mk
│   ├── 6_skip_list.mk
│   ├── 7_list_search.mk
│   ├── 8_dynamic_programming.mk
│   ├── graphs.mk
│   ├── hash_tables.mk
│   └── sort.mk
├── out/                    # Compiled binaries and visualizations
├── Makefile               # Master build file
└── compile_and_run.sh     # Interactive build & run script
```

---

## Building and Running

### Quick Start

```bash
# Build all programs and run interactively
bash compile_and_run.sh

# Build specific program
make 6_skip_list

# Build all
make

# Clean build artifacts
make clean

# Rebuild everything
make rebuild
```

### Running Individual Programs

```bash
./out/1_recursion
./out/2_linked_lists
./out/3_stacks_and_queues
./out/4_trees
./out/5_heap
./out/6_skip_list
./out/7_list_search
./out/8_dynamic_programming
./out/9_graphs
./out/10_hash_tables
./out/11_sort
```

---

## Data Structures & Algorithms

### 1. Recursion

**File:** `src/1_recursion.c`

Classic recursive algorithms demonstrating recursion fundamentals.

#### Mechanisms:

**Factorial**
- Base case: `n == 0` returns 1
- Recursive case: `n * factorial(n-1)`
- Each call adds to the call stack until base case

**Fibonacci**
- Base cases: `fib(0) = 0`, `fib(1) = 1`
- Recursive case: `fib(n) = fib(n-1) + fib(n-2)`
- Creates a binary recursion tree

**Tower of Hanoi**
- Move n-1 disks to auxiliary peg
- Move largest disk to destination
- Move n-1 disks from auxiliary to destination
- Demonstrates divide-and-conquer strategy

#### Complexity:

| Algorithm | Time Complexity | Space Complexity | Notes |
|-----------|----------------|------------------|-------|
| Factorial | O(n) | O(n) | Linear recursion depth |
| Fibonacci | O(2ⁿ) | O(n) | Exponential without memoization |
| Tower of Hanoi | O(2ⁿ) | O(n) | 2ⁿ-1 moves required |

---

### 2. Linked Lists

**File:** `src/2_linked_lists.c`

Three variants of linked lists with comprehensive operations.

#### Singly Linked List (SLL)

**Mechanism:**
- Each node contains data and a pointer to the next node
- Head pointer tracks the first node
- NULL terminates the list
- Traversal moves forward only

**Operations:**
- Insert: Update pointers to include new node
- Delete: Update pointers to bypass removed node
- Search: Linear traversal from head

**Complexity:**

| Operation | Time | Space |
|-----------|------|-------|
| Insert at head | O(1) | O(1) |
| Insert at index | O(n) | O(1) |
| Delete | O(n) | O(1) |
| Search | O(n) | O(1) |
| Access by index | O(n) | O(1) |

#### Doubly Linked List (DLL)

**Mechanism:**
- Each node has pointers to both next AND previous nodes
- Allows bidirectional traversal
- More memory per node, but more flexible operations

**Advantages over SLL:**
- Delete in O(1) if node pointer is given
- Traverse backwards
- Easier insertion before a node

**Complexity:**

| Operation | Time | Space |
|-----------|------|-------|
| Insert at head/tail | O(1) | O(1) |
| Delete with node ref | O(1) | O(1) |
| Delete by value | O(n) | O(1) |
| Reverse traversal | O(n) | O(1) |

#### Circular Linked List (CLL)

**Mechanism:**
- Last node points back to head (instead of NULL)
- Forms a circle
- Useful for round-robin scheduling

**Special considerations:**
- Must track head carefully to avoid infinite loops
- Can traverse entire list from any node

**Complexity:**
Same as SLL, but with circular property benefits for specific use cases.

---

### 3. Stacks and Queues

**File:** `src/3_stacks_and_queues.c`

Two fundamental LIFO (Last In First Out) and FIFO (First In First Out) data structures with multiple implementations and classic algorithms.

#### Stack Implementations

##### Array-Based Stack

**Mechanism:**
- Dynamic array that grows when full
- Top pointer tracks index of top element (-1 when empty)
- When capacity reached, array is doubled in size
- Contiguous memory provides good cache locality

**Operations:**
```c
typedef struct {
    int* data;
    int top;
    int capacity;
} ArrayStack;
```

**Resize strategy:**
- When full, allocate new array with 2× capacity
- Copy all elements to new array
- Free old array

**Complexity:**

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| Push | O(1) amortized | O(1) | O(n) when resize needed |
| Pop | O(1) | O(1) | |
| Peek | O(1) | O(1) | |
| Size | O(1) | O(1) | |
| Total Space | O(n) | | May waste space if not full |

**Advantages:**
- Fast access (cache-friendly)
- O(1) size query
- No pointer overhead

**Disadvantages:**
- Occasional O(n) resize cost
- May waste allocated space
- Fixed maximum size (until resize)

##### Linked List-Based Stack

**Mechanism:**
- Uses singly linked list from `2_linked_lists.h`
- Push/pop operations at head for O(1) performance
- No resizing needed (dynamically grows)
- Each node has pointer overhead

**Operations:**
```c
typedef struct {
    struct SLL* list;
} ListStack;
```

**Push:** Insert at head
```
Before: head → 20 → 10 → NULL
Push 30
After:  head → 30 → 20 → 10 → NULL
```

**Pop:** Delete from head
```
Before: head → 30 → 20 → 10 → NULL
Pop
After:  head → 20 → 10 → NULL (returns 30)
```

**Complexity:**

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| Push | O(1) | O(1) | |
| Pop | O(1) | O(1) | |
| Peek | O(1) | O(1) | |
| Size | O(n) | O(1) | Must traverse list |
| Total Space | O(n) | | Plus pointer overhead per node |

**Advantages:**
- No wasted space
- No resize overhead
- Unbounded size

**Disadvantages:**
- Pointer overhead per node
- Worse cache locality
- O(n) size query

---

#### Queue Implementations

##### Linked List-Based Queue

**Mechanism:**
- Uses singly linked list with both front and rear pointers
- Enqueue at rear for O(1) operation
- Dequeue from front for O(1) operation
- Maintains FIFO (First In First Out) order

**Operations:**
```c
typedef struct {
    struct SLLNode* front;  // Points to first element
    struct SLLNode* rear;   // Points to last element
} ListQueue;
```

**Enqueue:** Add at rear
```
Before: front → 10 → 20 → 30 ← rear
Enqueue 40
After:  front → 10 → 20 → 30 → 40 ← rear
```

**Dequeue:** Remove from front
```
Before: front → 10 → 20 → 30 → 40 ← rear
Dequeue
After:  front → 20 → 30 → 40 ← rear (returns 10)
```

**Why we need both pointers:**
- Without rear pointer: Enqueue would be O(n) (need to traverse to end)
- Without front pointer: Dequeue would be O(n) (need to find second node for SLL)
- With both: Both operations are O(1)

**Complexity:**

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| Enqueue | O(1) | O(1) | Add at rear |
| Dequeue | O(1) | O(1) | Remove from front |
| Peek | O(1) | O(1) | View front |
| Size | O(n) | O(1) | Must traverse |
| Total Space | O(n) | | Plus pointer overhead |

**Advantages:**
- O(1) enqueue and dequeue
- No wasted space
- Unbounded size
- True queue behavior

**Disadvantages:**
- Pointer overhead per node
- O(n) size query
- Two pointers to maintain

---

#### Stack Algorithm Applications

##### 1. Balanced Symbols Checker

**Problem:** Check if parentheses (), brackets [], and braces {} are properly balanced.

**Mechanism:**
1. Scan expression left to right
2. When opening symbol `(`, `[`, or `{` found → push to stack
3. When closing symbol `)`, `]`, or `}` found → pop and check match
4. At end, stack must be empty

**Example:** Check `((A+B)+[C-D])`
```
Step 1: ( → push (
Step 2: ( → push (
Step 3: ) → pop ( → match ✓
Step 4: [ → push [
Step 5: ] → pop [ → match ✓
Step 6: ) → pop ( → match ✓
Stack empty → BALANCED ✓
```

**Example:** Check `((A+B)+[C-D]}`
```
Steps 1-5: same as above
Step 6: } → pop ( → NO MATCH ✗
Result: NOT BALANCED
```

**Complexity:**
- Time: O(n) - single pass through expression
- Space: O(n) - worst case all opening symbols

##### 2. Queue Using Two Stacks

**Problem:** Implement FIFO queue using two LIFO stacks.

**Mechanism:**
- **stack1:** For enqueue operations
- **stack2:** For dequeue operations
- Transfer elements from stack1 to stack2 when stack2 is empty

**Enqueue:** O(1)
```
Just push to stack1
```

**Dequeue:** O(1) amortized
```
If stack2 is empty:
    While stack1 not empty:
        Pop from stack1, push to stack2
Pop from stack2
```

**Why it works:**
- Transferring from stack1 to stack2 reverses the order
- Elements in stack2 are in correct FIFO order
- Each element is transferred at most once

**Example:** Enqueue 10, 20, 30, then dequeue
```
After enqueues:
  stack1: [30, 20, 10]  (top to bottom)
  stack2: []

First dequeue - transfer:
  stack1: []
  stack2: [10, 20, 30]  (top to bottom)
  Return: 10

Second dequeue:
  stack1: []
  stack2: [20, 30]
  Return: 20
```

**Complexity:**
- Enqueue: O(1)
- Dequeue: O(1) amortized, O(n) worst case
- Space: O(n)

##### 3. Stack Using Two Queues

**Problem:** Implement LIFO stack using two FIFO queues.

**Mechanism:**
- Keep all elements in queue1
- When pushing, use queue2 as temporary

**Push:** O(n)
```
1. Enqueue new element to queue2
2. Dequeue all from queue1, enqueue to queue2
3. Swap queue1 and queue2
```

**Pop:** O(1)
```
Just dequeue from queue1
```

**Why it works:**
- New element is placed at front of queue
- Maintains LIFO order

**Example:** Push 10, 20, 30
```
After push 10:
  queue1: [10]

After push 20:
  queue2: [20, 10]  (front to back)
  Swap → queue1: [20, 10]

After push 30:
  queue2: [30, 20, 10]
  Swap → queue1: [30, 20, 10]

Pop → returns 30 (LIFO order)
```

**Complexity:**
- Push: O(n) - must transfer all elements
- Pop: O(1)
- Space: O(n)

##### 4. Infix to Postfix Conversion

**Problem:** Convert infix expressions (A+B) to postfix (AB+) for evaluation.

**Mechanism:**
1. Scan infix expression left to right
2. If operand (A, B, C, etc.) → add to output
3. If operator (+, -, *, /, ^) → handle precedence
4. If `(` → push to stack
5. If `)` → pop until `(`

**Operator precedence:**
```
^ (power)       : 3
*, /            : 2
+, -            : 1
```

**Algorithm:**
```
For each character in infix:
    If operand:
        Add to output
    If operator op:
        While stack top has >= precedence:
            Pop to output
        Push op to stack
    If '(':
        Push to stack
    If ')':
        Pop to output until '('
        Discard '('

Pop all remaining operators to output
```

**Example:** Convert `A+B*C` to postfix
```
Input: A + B * C

Step 1: A → output: "A"
Step 2: + → stack: [+]
Step 3: B → output: "AB"
Step 4: * → higher precedence than +
        → stack: [+, *]
Step 5: C → output: "ABC"
End: Pop all → output: "ABC*+"
```

**Example:** Convert `(A+B)*C` to postfix
```
Input: ( A + B ) * C

Step 1: ( → stack: [(]
Step 2: A → output: "A"
Step 3: + → stack: [(, +]
Step 4: B → output: "AB"
Step 5: ) → pop until (
        → output: "AB+", stack: []
Step 6: * → stack: [*]
Step 7: C → output: "AB+C"
End: Pop all → output: "AB+C*"
```

**Complexity:**
- Time: O(n) - single pass
- Space: O(n) - operator stack

##### 5. Recursive Stack Reversal

**Problem:** Reverse a stack using only stack operations (push, pop, isEmpty) and recursion.

**Mechanism:**
- Use recursion call stack to hold elements
- Recursively pop all elements
- Insert each at bottom of stack

**Algorithm:**
```c
reverse_stack(stack):
    if stack is empty:
        return

    top = pop(stack)
    reverse_stack(stack)
    insert_at_bottom(stack, top)

insert_at_bottom(stack, value):
    if stack is empty:
        push(stack, value)
        return

    top = pop(stack)
    insert_at_bottom(stack, value)
    push(stack, top)
```

**Example:** Reverse stack [50, 40, 30, 20, 10] (top to bottom)
```
Initial: [50, 40, 30, 20, 10]

Reverse process:
  Pop 50, reverse rest
    Pop 40, reverse rest
      Pop 30, reverse rest
        Pop 20, reverse rest
          Pop 10, reverse rest
            Base case: empty
          Insert 10 at bottom: [10]
        Insert 20 at bottom: [10, 20]
      Insert 30 at bottom: [10, 20, 30]
    Insert 40 at bottom: [10, 20, 30, 40]
  Insert 50 at bottom: [10, 20, 30, 40, 50]

Result: [10, 20, 30, 40, 50]
```

**How insert_at_bottom works:**
```
Insert 50 at bottom of [10, 20, 30]:
  Pop 30
  Insert 50 at bottom of [10, 20]:
    Pop 20
    Insert 50 at bottom of [10]:
      Pop 10
      Insert 50 at bottom of []:
        Push 50 → [50]
      Push 10 → [50, 10]
    Push 20 → [50, 10, 20]
  Push 30 → [50, 10, 20, 30]

But we want [10, 20, 30, 50]!
Wait, the top is at the front in my notation.

Actually stack [10, 20, 30] means:
  top: 10
  bottom: 30

Insert 50 at bottom means:
  Final: [10, 20, 30, 50] where 50 is at bottom
```

**Complexity:**
- Time: O(n²) - n elements, each insert_at_bottom is O(n)
- Space: O(n) - recursion call stack depth

**Why O(n²):**
- Reversing n elements
- Each element needs insert_at_bottom
- insert_at_bottom for element i requires O(i) operations
- Total: 1 + 2 + 3 + ... + n = O(n²)

---

### 4. Trees

**File:** `src/4_trees.c`

Self-balancing binary search tree with guaranteed O(log n) operations.

#### Red-Black Tree Properties

A red-black tree is a binary search tree with additional color properties that ensure balance:

1. **Every node is either red or black**
2. **Root is always black**
3. **All leaves (NIL) are black**
4. **Red nodes cannot have red children** (no two red nodes in a row)
5. **All paths from root to leaves contain the same number of black nodes**

**Why these properties?**
- Property 4 prevents long red chains
- Property 5 ensures no path is more than twice as long as any other
- Result: Height is guaranteed to be ≤ 2 × log₂(n+1)

#### Data Structure

```c
typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int data;
    Color color;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;  // Needed for efficient rotations
} RBNode;

typedef struct {
    RBNode* root;
    RBNode* nil;  // Sentinel node (always black, represents NULL)
} RBTree;
```

**Why a sentinel NIL node?**
- Simplifies code (no NULL checks needed)
- All leaf pointers point to same NIL node
- NIL's color is always BLACK (satisfies property 3)

---

#### Insertion

**Mechanism:**
1. Insert like normal BST (find position)
2. Color new node RED (preserves property 5 - black height)
3. Fix violations with rotations and recoloring

**Why insert RED?**
- Inserting BLACK would violate property 5 (black height)
- Inserting RED only potentially violates property 4 (red-red)
- Easier to fix property 4 than property 5

**Insertion Fixup Cases:**

After inserting a RED node, we may have a red-red violation (red node with red parent).

**Case 1: Uncle is RED**
```
Action: Recolor parent, uncle, grandparent
        Grandparent becomes RED
        Parent and Uncle become BLACK
        Continue checking from grandparent
```

**Case 2: Uncle is BLACK (triangle configuration)**
```
Example: z is right child, parent is left child
Action: Rotate z's parent to convert to line case
        Then apply Case 3
```

**Case 3: Uncle is BLACK (line configuration)**
```
Example: z is left child, parent is left child
Action: Right rotate grandparent
        Recolor original parent and grandparent
```

**Example: Insert 10, 20, 30**

```
Insert 10:
  10(B)  (root is always black)

Insert 20:
  10(B)
    \
    20(R)  (new nodes are red)

Insert 30:
  10(B)
    \
    20(R)
      \
      30(R)  ← Red-red violation!

Fix (Case 3 - left rotation on 10):
    20(B)
   /  \
 10(R) 30(R)  ← Balanced!
```

**Complexity:**
- Time: O(log n) - height is O(log n), constant rotations
- Space: O(1) - only modifying pointers

---

#### Deletion

**Mechanism:**
1. Find node to delete (BST search)
2. Remove node (BST deletion)
3. If deleted node was BLACK, fix violations

**Why only fix if BLACK?**
- Deleting RED doesn't violate any properties
- Deleting BLACK violates property 5 (black height)

**Deletion Cases:**

When we remove a BLACK node, we have a "double black" problem.

**Case 1: Sibling is RED**
```
Action: Rotate parent, recolor
        Converts to Case 2, 3, or 4
```

**Case 2: Sibling and both nephews are BLACK**
```
Action: Recolor sibling to RED
        Move double-black up to parent
```

**Case 3: Sibling is BLACK, far nephew is BLACK**
```
Action: Rotate sibling, recolor
        Converts to Case 4
```

**Case 4: Sibling is BLACK, far nephew is RED**
```
Action: Rotate parent, recolor
        Fixes the double-black problem
```

**Complexity:**
- Time: O(log n) - height is O(log n), constant rotations
- Space: O(1) - only modifying pointers

---

#### Rotations

Rotations maintain BST property while restructuring the tree.

**Left Rotation:**
```
Before:        After:
   x             y
  / \           / \
 a   y    =>   x   c
    / \       / \
   b   c     a   b

Preserves: a < x < b < y < c
```

**Right Rotation:**
```
Before:        After:
    y            x
   / \          / \
  x   c   =>   a   y
 / \              / \
a   b            b   c

Preserves: a < x < b < y < c
```

**When used:**
- Insertion fixup: Convert triangle to line, then rotate
- Deletion fixup: Restore balance after removing black node

**Complexity:** O(1) - constant pointer updates

---

#### Recursive Traversals

##### Inorder (Left → Root → Right)

```c
void inorder(RBNode* x) {
    if (x != NIL) {
        inorder(x->left);
        visit(x);
        inorder(x->right);
    }
}
```

**Result:** Values in sorted order
- Example: `1(R) 5(B) 10(R) 15(B) 20(B) 25(R) 30(B)`
- **Use case:** Print sorted values, validate BST property

**Complexity:**
- Time: O(n) - visit each node once
- Space: O(h) - recursion stack, h = height = O(log n)

##### Preorder (Root → Left → Right)

```c
void preorder(RBNode* x) {
    if (x != NIL) {
        visit(x);
        preorder(x->left);
        preorder(x->right);
    }
}
```

**Result:** Root before subtrees
- Example: `20(B) 10(R) 5(B) 1(R) 15(B) 30(B) 25(R)`
- **Use case:** Copy tree structure, serialize tree

**Complexity:** Same as inorder

##### Postorder (Left → Right → Root)

```c
void postorder(RBNode* x) {
    if (x != NIL) {
        postorder(x->left);
        postorder(x->right);
        visit(x);
    }
}
```

**Result:** Root after subtrees
- Example: `1(R) 5(B) 15(B) 10(R) 25(R) 30(B) 20(B)`
- **Use case:** Delete tree (free children before parent)

**Complexity:** Same as inorder

---

#### Complexity Summary

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| Search | O(log n) | O(1) | Height guaranteed ≤ 2log₂(n+1) |
| Insert | O(log n) | O(1) | At most 2 rotations |
| Delete | O(log n) | O(1) | At most 3 rotations |
| Inorder | O(n) | O(log n) | Recursion stack |
| Preorder | O(n) | O(log n) | Recursion stack |
| Postorder | O(n) | O(log n) | Recursion stack |
| Height | O(n) | O(log n) | Must visit all nodes |

**Key advantage over regular BST:**
- Regular BST: O(n) worst case (degenerates to linked list)
- Red-Black Tree: O(log n) guaranteed (self-balancing)

---

#### Red-Black Tree vs Other Structures

| Feature | Red-Black Tree | AVL Tree | Skip List |
|---------|---------------|----------|-----------|
| Search | O(log n) | O(log n) | O(log n) avg |
| Insert | O(log n) | O(log n) | O(log n) avg |
| Delete | O(log n) | O(log n) | O(log n) avg |
| Balance | Less strict | More strict | Probabilistic |
| Rotations (insert) | ≤ 2 | ≤ 2 | 0 |
| Rotations (delete) | ≤ 3 | ≤ log n | 0 |
| Implementation | Complex | More complex | Simpler |
| Used in | Linux kernel, STL | Databases | MemSQL, Redis |

**When to use Red-Black Trees:**
- Need guaranteed O(log n) operations
- More inserts/deletes than AVL (fewer rotations)
- Industry standard for balanced BSTs

---

### 5. Heaps

**File:** `src/7_heap.c`

A binary max heap implemented using a dynamic array. Heaps are complete binary trees that maintain the heap property: each parent is greater than or equal to its children (max heap) or less than or equal to its children (min heap).

#### What is a Heap?

A **heap** is a specialized tree-based data structure that satisfies the **heap property**:
- **Max heap:** Parent ≥ Children (root is maximum)
- **Min heap:** Parent ≤ Children (root is minimum)

**Key properties:**
1. **Complete binary tree:** All levels filled except possibly the last, which fills left-to-right
2. **Efficient array representation:** No pointer overhead needed
3. **Height is O(log n):** Due to complete tree property

#### Array Representation

Heaps use arrays instead of node pointers. For any element at index `i`:

```
Parent index:      (i - 1) / 2
Left child index:  2 * i + 1
Right child index: 2 * i + 2
```

**Why use an array?**
- No pointer overhead (more memory efficient than linked nodes)
- Better cache locality (contiguous memory)
- Simple index arithmetic for navigation
- Complete tree property guarantees no wasted space

**Example:** Array `[40, 30, 15, 10, 20]` represents:
```
       40
      /  \
    30    15
   /  \
  10  20
```

#### Data Structure

```c
typedef struct {
    int* data;      // Dynamic array storing heap elements
    int size;       // Current number of elements
    int capacity;   // Maximum capacity
} MaxHeap;
```

---

#### Heap Operations

##### Percolate Up (Bubble Up)

**Purpose:** Restore heap property after insertion

**Mechanism:**
1. Insert new element at end (maintains complete tree)
2. Compare with parent
3. If child > parent, swap
4. Repeat until heap property restored or reach root

**Example:** Insert 50 into heap [40, 30, 15, 10, 20]
```
Step 1: Add at end
       40
      /  \
    30    15
   /  \   /
  10  20 50   ← Violates heap property!

Step 2: Compare 50 with parent 15
       40
      /  \
    30    50    ← Swap
   /  \   /
  10  20 15

Step 3: Compare 50 with parent 40
       50         ← Swap
      /  \
    30    40
   /  \   /
  10  20 15
```

**Complexity:**
- Time: O(log n) - at most height of tree
- Space: O(1) - only a few variables

##### Percolate Down (Heapify)

**Purpose:** Restore heap property after extraction or during heap build

**Mechanism:**
1. Compare node with both children
2. Find largest among node, left child, right child
3. If node is not largest, swap with largest child
4. Recursively heapify the affected subtree

**Example:** Array [10, 30, 20, 15, 25] (root violates property)
```
Initial:
       10         ← Too small!
      /  \
    30    20
   /  \
  15  25

Step 1: Find largest (10, 30, 20) → 30
       30
      /  \
    10    20
   /  \
  15  25

Step 2: Find largest (10, 15, 25) → 25
       30
      /  \
    25    20
   /  \
  15  10         ← Heap property restored!
```

**Complexity:**
- Time: O(log n) - at most height of tree
- Space: O(log n) - recursion depth

##### Insert

**Algorithm:**
1. Add element at end of array (maintains complete tree)
2. Increment size
3. Percolate up to restore heap property

**Complexity:** O(log n)

##### Extract Max

**Algorithm:**
1. Save root element (maximum value)
2. Move last element to root
3. Decrement size
4. Percolate down to restore heap property
5. Return saved maximum

**Why move last to root?**
- Maintains complete tree property
- Only creates one violation (at root)
- More efficient than other approaches

**Complexity:** O(log n)

##### Peek Max

Simply return root element without removing it.

**Complexity:** O(1)

---

#### Build Heap from Array

**Problem:** Given an unsorted array, convert it into a valid heap

**Naive approach:** Insert elements one by one → O(n log n)

**Optimal approach:** Heapify from bottom up → O(n)

**Algorithm:**
```c
MaxHeap* heap_build_from_array(int arr[], int size) {
    1. Copy all elements to heap array
    2. Start from last non-leaf node: size/2 - 1
    3. Heapify each node going backwards to root
}
```

**Why start from size/2 - 1?**
- Nodes from size/2 to size-1 are leaves (no children)
- Leaves are already valid heaps (trivially)
- We only need to heapify internal nodes

**Example:** Build heap from [4, 10, 3, 5, 1]
```
Initial array:
       4
      / \
    10   3
   /  \
  5    1

Heapify index 1 (node 10):
       4
      / \
    10   3      ← Already valid
   /  \
  5    1

Heapify index 0 (node 4):
       10
      / \
     5   3
    / \
   4   1        ← Valid max heap!
```

**Why O(n) and not O(n log n)?**
- Most nodes are near the bottom (few percolations needed)
- Mathematical proof: Sum of (nodes at level h) × (distance to move) = O(n)

**Complexity:**
- Time: O(n) - surprisingly linear!
- Space: O(1) - in-place heapification

---

#### Heap Sort

**Problem:** Sort array in ascending order using heap

**Algorithm:**
1. Build max heap from array - O(n)
2. Repeatedly:
   - Swap root (max) with last element
   - Reduce heap size by 1
   - Heapify root - O(log n)
3. Repeat n times

**Example:** Sort [12, 11, 13, 5, 6, 7]
```
Step 1: Build max heap
Array: [13, 11, 12, 5, 6, 7]
Heap:      13
          /  \
        11    12
       /  \   /
      5   6  7

Step 2: Swap 13 with 7, heapify
Array: [7, 11, 12, 5, 6 | 13]
After heapify: [12, 11, 7, 5, 6 | 13]

Step 3: Swap 12 with 6, heapify
Array: [6, 11, 7, 5 | 12, 13]
After heapify: [11, 6, 7, 5 | 12, 13]

...continue until sorted...

Final: [5, 6, 7, 11, 12, 13]
```

**Why it works:**
- Max heap has largest element at root
- Repeatedly extracting max gives sorted order (reversed)
- Result is ascending order

**Complexity:**
- Time: O(n log n) - n extractions, each O(log n)
- Space: O(1) - in-place sorting
- Not stable (relative order of equal elements not preserved)

**Comparison with other sorts:**
| Algorithm | Average | Worst | Space | Stable |
|-----------|---------|-------|-------|--------|
| Heap Sort | O(n log n) | O(n log n) | O(1) | No |
| Merge Sort | O(n log n) | O(n log n) | O(n) | Yes |
| Quick Sort | O(n log n) | O(n²) | O(log n) | No |

**Heap sort advantages:**
- Guaranteed O(n log n) worst case (unlike quicksort)
- In-place O(1) space (unlike merge sort)
- Good for memory-constrained systems

---

#### Priority Queue Implementation

**What is a Priority Queue?**
A priority queue is an abstract data type where each element has a priority. Elements are dequeued in order of priority, not insertion order.

**Operations:**
- `insert(value, priority)` - Add element with priority
- `extract_max()` - Remove and return highest priority element
- `peek()` - View highest priority without removing

**Heap as Priority Queue:**
Max heaps naturally implement priority queues:
- **Insert:** `heap_insert()` - O(log n)
- **Extract Max:** `heap_extract_max()` - O(log n)
- **Peek:** `heap_peek_max()` - O(1)

**You don't need separate priority queue code - just use the max heap directly!**

**Use cases:**
- Task scheduling (OS schedulers)
- Dijkstra's shortest path algorithm
- Huffman coding
- Event-driven simulation
- A* pathfinding
- Median maintenance

**Example usage:**
```c
MaxHeap* pq = heap_create(100);

// Insert tasks with priorities
heap_insert(pq, 10);  // Priority 10
heap_insert(pq, 50);  // Priority 50 (higher)
heap_insert(pq, 20);  // Priority 20

// Process highest priority first
int highest = heap_extract_max(pq);  // Returns 50
int next = heap_extract_max(pq);     // Returns 20
```

**Min heap for priority queue:**
Our implementation is a max heap (highest priority first). For a min heap (lowest priority first), simply negate priorities or modify comparison operators.

---

#### Complexity Summary

| Operation | Time | Space | Notes |
|-----------|------|-------|-------|
| Insert | O(log n) | O(1) | Percolate up |
| Extract Max | O(log n) | O(1) | Percolate down |
| Peek Max | O(1) | O(1) | Just read root |
| Build Heap | O(n) | O(1) | Bottom-up heapify |
| Heap Sort | O(n log n) | O(1) | In-place sorting |
| Search | O(n) | O(1) | No ordering within levels |

**Key insights:**
- O(log n) for priority queue operations (insert/extract)
- O(n) build heap (not O(n log n)!)
- O(1) space for all operations (in-place)
- Height always O(log n) (complete tree)

---

#### Heap Visualizations

The implementation provides three display functions:

1. **Array representation:** `heap_print_array()`
   ```
   [40, 30, 15, 10, 20]
   ```

2. **Level-order:** `heap_print_tree()`
   ```
   Level 0: 40
   Level 1: 30 15
   Level 2: 10 20
   ```

3. **Visual tree:** `heap_display_tree()`
   ```
    ┌──15
   40
        ┌──20
    └──30
        └──10
   ```

---

### 6. Skip Lists

**File:** `src/skip_list.c`

Two implementations: Key-Value (map/dictionary) and Simple (sorted list).

#### Mechanism:

**Structure:**
- Multiple levels of linked lists
- Level 0 contains all elements in sorted order
- Higher levels act as "express lanes" with fewer elements
- Each node has variable height (number of levels)

**How it works:**
1. Search starts at highest level
2. Move forward while next key < search key
3. Drop down one level
4. Repeat until found or reach level 0

**Level assignment:**
- Random level chosen probabilistically (P_FACTOR = 0.5)
- Average level ≈ log₂(n)
- Maintains probabilistic balance without rotations

**Insertion:**
1. Find position at all levels (store update pointers)
2. Generate random level for new node
3. Update forward pointers at each level

**Why it's efficient:**
- Higher levels skip over many elements
- Average search path length: log(n)
- No rebalancing needed (unlike AVL/Red-Black trees)

#### Complexity:

| Operation | Average | Worst Case | Space |
|-----------|---------|------------|-------|
| Search | O(log n) | O(n) | O(n log n) |
| Insert | O(log n) | O(n) | O(n log n) |
| Delete | O(log n) | O(n) | O(n log n) |

**Space:** Each node has on average log(n) forward pointers.

---

### 7. List Search Algorithms

**File:** `src/list_search.c`

Advanced linked list algorithms using two-pointer techniques.

#### Search nth from End (Two-Pointer/Runner Technique)

**Mechanism:**
1. Move `fast` pointer n steps ahead
2. Move `slow` and `fast` together until `fast` reaches NULL
3. `slow` is now at nth from end

**Why it works:**
- Gap between pointers is always n nodes
- When fast reaches end, slow is n from end
- Single pass solution!

**Example:** Find 2nd from end in `10→20→30→40→50`
```
Step 1: fast moves 2 ahead
  slow=10, fast=30

Step 2: Move both until fast=NULL
  slow=10, fast=30
  slow=20, fast=40
  slow=30, fast=50
  slow=40, fast=NULL ← Found!
```

**Complexity:**
- Time: O(n) - single pass
- Space: O(1) - only two pointers

#### Floyd's Cycle Detection (Tortoise and Hare)

**Mechanism:**
1. `slow` moves 1 step per iteration
2. `fast` moves 2 steps per iteration
3. If they meet, cycle exists
4. If fast reaches NULL, no cycle

**Why it works:**
- In a cycle, fast gains 1 position per iteration
- Must eventually catch slow (like a circular race track)
- Distance closes by 1 each step

**Finding cycle start:**
1. After detection, move one pointer to head
2. Move both pointers 1 step at a time
3. They meet at cycle start (mathematical proof)

**Cycle length:**
After detection, keep one pointer stationary, move other around cycle counting steps.

**Complexity:**
- Time: O(n) - fast traverses at most 2n nodes
- Space: O(1) - only two pointers

#### Recursive List Reversal

**Mechanism:**
1. Recursively reverse rest of list
2. Make next node point back to current
3. Set current→next to NULL

**Base case:** Single node or empty list

**Example:** Reverse `10→20→30`
```
reverse(10):
  reverse(20):
    reverse(30):
      return 30  (base case)
    30→next = 20, 20→next = NULL
    return 30
  20→next = 10, 10→next = NULL
  return 30
Result: 30→20→10
```

**Complexity:**
- Time: O(n) - visit each node once
- Space: O(n) - recursion stack depth

---

### 8. Dynamic Programming

**File:** `src/dynamic_programming.c`

Classic optimization problems using dynamic programming and backtracking.

#### 0/1 Knapsack Problem

**Problem:** Given items with weights and values, maximize value within capacity constraint. Each item can be taken or not (0/1 choice).

**Mechanism:**

**DP Table:** `dp[i][w]` = max value using first i items with capacity w

**Recurrence relation:**
```
dp[i][w] = max(
    dp[i-1][w],                      // Don't take item i
    value[i] + dp[i-1][w-weight[i]]  // Take item i (if fits)
)
```

**Base cases:**
- `dp[0][w] = 0` (no items = 0 value)
- `dp[i][0] = 0` (no capacity = 0 value)

**Why it works:**
- Builds solution bottom-up from smaller subproblems
- Each cell depends only on previous row
- Avoids recomputing same subproblems

**Example:** Items: [(w=2,v=12), (w=1,v=10)], capacity=3
```
    0  1  2  3
0:  0  0  0  0
1:  0  0 12 12   (item 1: w=2, v=12)
2:  0 10 12 22   (item 2: w=1, v=10, take both!)
```

**Optimization:** Can reduce space to O(capacity) by using 1D array.

**Complexity:**
- Time: O(n × capacity) - fill entire table
- Space: O(n × capacity) - 2D table
- Space (optimized): O(capacity) - 1D rolling array

#### N-Queens Problem

**Problem:** Place N queens on N×N chessboard so no two queens threaten each other.

**Mechanism (Backtracking):**

1. **Recursive approach:** Place queens row by row
2. **For each row:** Try each column
3. **Safety check:** No queen in same column or diagonal
4. **Backtrack:** If no safe position, undo previous placement

**Safety constraints:**
- **Column:** No other queen in same column
- **Diagonal 1:** No queen at (row-i, col-i)
- **Diagonal 2:** No queen at (row-i, col+i)

**Example:** 4-Queens solution
```
. Q . .    ← Queen in column 1
. . . Q    ← Queen in column 3
Q . . .    ← Queen in column 0
. . Q .    ← Queen in column 2
```

**Why backtracking:**
- Too many combinations to try all (N! arrangements)
- Backtracking prunes invalid branches early
- Only explores valid partial solutions

**Complexity:**
- Time: O(N!) - try different permutations, but pruned
- Space: O(N²) - chessboard storage
- Recursion depth: O(N)

**Optimization:** Can track attacked columns/diagonals with bit vectors for O(1) checks.

---

### 9. Graphs and Shortest Path Algorithms

**File:** `src/9_graphs.c`

Comprehensive graph implementation with multiple representations and shortest path algorithms.

#### Graph Representations

**When to use Adjacency Matrix:**
- **Dense graphs** where E ≈ V² (many edges)
- **O(1) edge lookup** - instant check if edge exists
- **Space: O(V²)** - always uses V×V array
- Best for: Complete graphs, dense connectivity

**When to use Adjacency List:**
- **Sparse graphs** where E << V² (few edges)
- **Space: O(V + E)** - only stores existing edges
- Better for graph traversals (DFS, BFS)
- Best for: DAGs, sparse networks, social graphs

#### Shortest Path Algorithms Comparison

The choice of shortest path algorithm depends on graph properties:

##### 1. BFS (Breadth-First Search)

**Use when:**
- Graph is **UNWEIGHTED** (all edges have weight = 1)
- Need guaranteed shortest path by number of edges

**How it works:**
```
1. Start from source, explore level by level
2. First time we reach destination = shortest path
3. Uses queue: FIFO order ensures shortest path first
```

**Why it works:**
- Explores nodes in order of distance from source
- All edges equal weight, so closest = fewest edges
- Layer-by-layer exploration guarantees optimality

**Complexity:**
- Time: **O(V + E)** - visit each vertex and edge once
- Space: **O(V)** - queue and visited arrays

**Example:** Social network - find shortest connection path
```
Person A → Friend → Friend → Person B  (3 hops)
```

**Key insight:** BFS explores layer-by-layer, guaranteeing shortest path in unweighted graphs.

---

##### 2. Dijkstra's Algorithm

**Use when:**
- Graph is **WEIGHTED** with **NON-NEGATIVE** edge weights
- Need single-source shortest paths

**How it works:**
```
1. Initialize: distance[source] = 0, all others = ∞
2. Repeat:
   a. Pick unvisited vertex u with minimum distance
   b. For each neighbor v of u:
      if distance[u] + weight(u,v) < distance[v]:
         distance[v] = distance[u] + weight(u,v)  (RELAXATION)
   c. Mark u as visited
3. Continue until all reachable vertices processed
```

**Why it works:**
- **Greedy approach**: Always pick closest unvisited vertex
- Safe assumption: Won't find shorter path later (with non-negative weights!)
- Each edge relaxed once per vertex visit

**Complexity:**
- Time: **O(V²)** with array, **O((V+E) log V)** with min-heap
- Space: **O(V)** - distance and visited arrays

**Example:** Road network with distances
```
City A → (5km) → City B → (3km) → City C
City A → (10km) → City C
Shortest: A → B → C = 8km (not direct 10km)
```

**Why NOT for negative edges?**
- Assumes greedy choice is safe
- Negative edge later could invalidate earlier decisions
- Example: A→B (5), B→C (-10), A→C (1)
  - Greedy picks A→C (1)
  - But A→B→C = -5 is shorter!

---

##### 3. Bellman-Ford Algorithm

**Use when:**
- Graph is **WEIGHTED** with **POSSIBLE NEGATIVE** edge weights
- Need to **detect negative cycles**
- Willing to trade speed for correctness

**How it works:**
```
1. Initialize: distance[source] = 0, all others = ∞
2. Relax ALL edges (V-1) times:
   For each edge (u, v):
      if distance[u] + weight(u,v) < distance[v]:
         distance[v] = distance[u] + weight(u,v)
3. Check for negative cycle:
   If ANY edge can still be relaxed, negative cycle exists
```

**Why V-1 iterations?**
- Shortest path has at most (V-1) edges
- Each iteration relaxes edges along shortest path
- After (V-1) iterations, all shortest paths found

**Why it handles negative weights:**
- Doesn't make greedy choices
- Explores all possible paths systematically
- Relaxes edges multiple times to find true shortest path

**Negative cycle detection:**
- If after V-1 iterations, edges can still be relaxed → negative cycle
- No shortest path exists (can loop forever decreasing distance)

**Complexity:**
- Time: **O(V × E)** - much slower than Dijkstra
- Space: **O(V)** - distance array

**Example:** Currency arbitrage detection
```
USD → EUR (rate: -log(1.2)) → GBP (rate: -log(0.9)) → USD (rate: -log(1.1))
Total: negative sum = arbitrage opportunity (negative cycle)
```

---

#### Algorithm Comparison Table

| Feature | BFS | Dijkstra | Bellman-Ford |
|---------|-----|----------|--------------|
| **Edge Weights** | Unweighted only | Non-negative | Any (including negative) |
| **Negative Edges** | N/A | ❌ Incorrect | ✅ Handles correctly |
| **Negative Cycles** | N/A | N/A | ✅ Detects them |
| **Time Complexity** | O(V + E) | O(V²) or O((V+E) log V) | O(V × E) |
| **Space Complexity** | O(V) | O(V) | O(V) |
| **Approach** | Layer-by-layer (queue) | Greedy (min distance) | Dynamic programming |
| **Guarantees** | Shortest path (unweighted) | Shortest path (non-neg) | Shortest path or cycle detect |
| **Best for** | Social networks, unweighted | Road networks, positive weights | Financial graphs, cycle detection |

---

#### Side-by-Side: BFS vs Dijkstra Algorithm Steps

**Core Difference:** BFS explores by hop count, Dijkstra explores by total distance.

| Step | BFS | Dijkstra |
|------|-----|----------|
| **1. Initialization** | `distance[src] = 0`<br>`All others = ∞`<br>`Queue = [src]` | `distance[src] = 0`<br>`All others = ∞`<br>`visited = all false` |
| **2. Data Structure** | **Queue (FIFO)**<br>- Add to rear<br>- Remove from front<br>- Order: insertion order | **Priority Queue (Min-Heap)**<br>- Extract minimum<br>- Update priorities<br>- Order: by distance |
| **3. Vertex Selection** | `u = queue.dequeue()`<br>**Take next in line**<br>(first added, first processed) | `u = vertex with min distance[u]`<br>**Take closest unvisited**<br>(greedy choice) |
| **4. Neighbor Processing** | For each neighbor `v` of `u`:<br>`if not visited[v]:`<br>`  distance[v] = distance[u] + 1`<br>`  queue.enqueue(v)` | For each neighbor `v` of `u`:<br>`if not visited[v]:`<br>`  new_dist = distance[u] + weight(u,v)`<br>`  if new_dist < distance[v]:`<br>`    distance[v] = new_dist` |
| **5. Distance Calculation** | **Always +1**<br>`distance[v] = distance[u] + 1`<br>(count edges, ignore weights) | **Add edge weight**<br>`distance[v] = distance[u] + weight(u,v)`<br>(accumulate actual weights) |
| **6. Relaxation** | **No relaxation**<br>Distance set once when discovered<br>Never updated | **Relaxation used**<br>`if distance[u] + w < distance[v]:`<br>`  distance[v] = distance[u] + w`<br>Can update multiple times |
| **7. Visit Order** | **Layer by layer**<br>- Visit all at distance 1<br>- Then all at distance 2<br>- Then all at distance 3 | **Closest first**<br>- Visit vertex with smallest distance<br>- Not necessarily in layers<br>- Can jump to distant vertex if closest |
| **8. Marking Visited** | When first discovered<br>`visited[v] = true` on enqueue | When processed<br>`visited[u] = true` after exploring neighbors |
| **9. Termination** | Queue becomes empty | All vertices visited OR<br>min distance = ∞ (unreachable) |
| **10. Correctness** | ✅ Unweighted graphs only<br>❌ Fails on weighted (ignores weights) | ✅ Non-negative weighted graphs<br>❌ Fails with negative weights |

**Example Walkthrough:**

Graph: `A--(1)--B--(1)--C` and `A--(10)--C`

```
BFS Execution:
1. Start: distance[A]=0, queue=[A]
2. Process A:
   - Discover B: distance[B]=0+1=1, queue=[B,C]
   - Discover C: distance[C]=0+1=1
3. Process B:
   - C already visited, skip
4. Result: distance[C]=1 ❌ (used edge with weight 10)

Dijkstra Execution:
1. Start: distance[A]=0
2. Pick A (min=0):
   - Relax A→B: distance[B]=0+1=1
   - Relax A→C: distance[C]=0+10=10
3. Pick B (min=1):
   - Relax B→C: distance[C]=min(10, 1+1)=2
4. Pick C (min=2)
5. Result: distance[C]=2 ✅ (A→B→C path)
```

**Key Insight:** BFS assumes all edges equal. Dijkstra weighs each step, always choosing the cheapest path explored so far.

---

#### Side-by-Side: Dijkstra vs Bellman-Ford Algorithm Steps

**Core Difference:** Dijkstra greedily picks best vertex, Bellman-Ford systematically relaxes all edges.

| Step | Dijkstra | Bellman-Ford |
|------|----------|--------------|
| **1. Initialization** | `distance[src] = 0`<br>`All others = ∞`<br>`visited = all false` | `distance[src] = 0`<br>`All others = ∞`<br>`No visited tracking` |
| **2. Main Loop** | **For each vertex:**<br>`while unvisited vertices exist:`<br>Process one vertex per iteration | **For (V-1) iterations:**<br>`for each iteration:`<br>Check ALL edges every iteration |
| **3. Vertex Selection** | **Greedy choice:**<br>`u = unvisited vertex with min distance[u]`<br>- Choose best option NOW<br>- Assumes choice is final | **No vertex selection:**<br>Process all edges regardless<br>- No assumptions<br>- Allows reconsideration |
| **4. Edge Processing** | **Outgoing edges from u only:**<br>`for each neighbor v of u:`<br>Only edges from current vertex | **ALL edges in graph:**<br>`for each edge (u,v) in E:`<br>Every edge, every iteration |
| **5. Relaxation** | `if distance[u] + weight(u,v) < distance[v]:`<br>`  distance[v] = distance[u] + weight(u,v)`<br>**Once per edge** (when u processed) | `if distance[u] + weight(u,v) < distance[v]:`<br>`  distance[v] = distance[u] + weight(u,v)`<br>**Up to (V-1) times per edge** |
| **6. Order Dependence** | **Order matters critically:**<br>Must process vertices in distance order<br>Wrong order = wrong answer | **Order doesn't matter:**<br>Can process edges in any order<br>Will converge to correct answer |
| **7. Finality** | **Once visited, never reconsidered:**<br>`visited[u] = true`<br>distance[u] is FINAL | **Can update any vertex any time:**<br>No finality until (V-1) iterations done<br>Allows fixing mistakes |
| **8. Negative Edges** | **Breaks algorithm:**<br>Greedy choice becomes wrong<br>Finalized vertices can't be updated | **Handles correctly:**<br>Multiple passes catch negative edges<br>Later iterations fix earlier estimates |
| **9. Termination** | When all reachable vertices visited | After exactly (V-1) iterations |
| **10. Cycle Detection** | **No cycle detection:**<br>Assumes no negative cycles exist | **V-th iteration check:**<br>`if any edge can still relax:`<br>`  NEGATIVE CYCLE EXISTS` |
| **11. Time Complexity** | **O((V+E) log V)**<br>- V vertex selections: O(V log V)<br>- E edge relaxations: O(E log V) | **O(V × E)**<br>- V-1 iterations<br>- E edges per iteration |

**Example: Why Dijkstra Fails with Negative Edges**

Graph: `A--(5)-->B--(-8)-->C` and `A--(10)-->C`

```
Dijkstra Execution (WRONG):
Iteration 1: Pick A (dist=0)
  - Relax A→B: distance[B]=5
  - Relax A→C: distance[C]=10
  - Mark A as VISITED (FINAL)

Iteration 2: Pick B (dist=5)
  - Relax B→C: distance[C]=min(10, 5-8)=-3
  - Mark B as VISITED (FINAL)

Iteration 3: Pick C (dist=-3)
  - Wait! We used distance[C]=10 when we picked B!
  - But distance[C] should have been -3!
  - C was discovered late, so wrong distance used

❌ Problem: Greedy choice assumed 10 was final for C
   Negative edge from B made it -3 instead

Bellman-Ford Execution (CORRECT):
Iteration 1 (pass through all edges):
  - Relax A→B: distance[B]=0+5=5
  - Relax A→C: distance[C]=0+10=10
  - Relax B→C: distance[C]=min(10, 5-8)=-3 ✅

Iteration 2:
  - No more updates (converged)

✅ Correct: All edges checked, found A→B→C = -3
```

**Example: Negative Cycle Detection**

Graph: `A--(1)-->B--(-3)-->C--(1)-->B`

```
Bellman-Ford Execution:
Initialization: distance[A]=0, distance[B]=∞, distance[C]=∞

Iteration 1:
  - Relax A→B: distance[B]=1
  - Relax B→C: distance[C]=1-3=-2
  - Relax C→B: distance[B]=min(1, -2+1)=-1 ✅ Updated!

Iteration 2:
  - Relax A→B: no change
  - Relax B→C: distance[C]=-1-3=-4 ✅ Updated again!
  - Relax C→B: distance[B]=min(-1, -4+1)=-3 ✅ Updated again!

After (V-1)=2 iterations, run V-th check:
  - Try relax B→C: -3-3=-6 < -4 ✅ CAN STILL RELAX!

Conclusion: ❌ NEGATIVE CYCLE: B→C→B (weight: -3+1=-2)
            No shortest path exists (can loop forever)
```

**Key Insight:** Dijkstra makes irreversible greedy choices (fast but fragile). Bellman-Ford exhaustively checks all possibilities (slow but robust).

---

#### When to Use Each Algorithm

**Use BFS when:**
- ✅ Unweighted graph (all edges equal)
- ✅ Finding shortest path by hop count
- ✅ Social networks, web crawling
- ✅ Need fastest algorithm
- Example: LinkedIn connections, six degrees of separation

**Use Dijkstra when:**
- ✅ Weighted graph with NON-NEGATIVE weights
- ✅ GPS navigation, network routing
- ✅ All weights positive (distances, costs, times)
- ✅ Need efficient solution
- Example: Google Maps, package delivery routes

**Use Bellman-Ford when:**
- ✅ Weighted graph with POSSIBLE NEGATIVE weights
- ✅ Need to detect negative cycles
- ✅ Financial applications (arbitrage detection)
- ✅ Correctness more important than speed
- Example: Currency exchange, difference constraints

**Summary decision tree:**
```
Unweighted graph? → BFS
    ↓ No
Negative edges? → Bellman-Ford
    ↓ No
Non-negative weighted → Dijkstra
```

---

### Minimum Spanning Tree (MST) Algorithms

#### What is a Minimum Spanning Tree?

A **Minimum Spanning Tree** of a weighted undirected graph is:
- A **spanning tree** (connects all vertices, no cycles)
- With **minimum total edge weight**
- Has exactly **(V-1) edges** where V = number of vertices

**Applications:**
- Network design (minimum cable/wire to connect all nodes)
- Road planning (connect cities with minimum road length)
- Clustering algorithms (group similar data)
- Approximation algorithms for NP-hard problems

---

#### Prim's Algorithm vs Kruskal's Algorithm

Both find the MST, but use different approaches:

| Aspect | Prim's Algorithm | Kruskal's Algorithm |
|--------|------------------|---------------------|
| **Core idea** | Grow tree from vertex | Merge forests by edges |
| **Selection** | Vertex-based | Edge-based |
| **Starts with** | Single vertex | All edges |
| **Each step** | Add cheapest edge to new vertex | Add cheapest non-cycle edge |
| **Data structure** | Priority queue (or array) | Union-Find (disjoint set) |
| **Maintains** | Always one connected tree | May have multiple trees |
| **Best for** | Dense graphs (E ≈ V²) | Sparse graphs (E << V²) |
| **Time** | O(V²) or O((V+E) log V) | O(E log E) |
| **Space** | O(V) | O(V) + O(E) for edge list |

---

#### Side-by-Side: Prim's vs Kruskal's Algorithm Steps

**Example Graph:**
```
    0 --2-- 1
    |  \    |
    6   4   3
    |    \  |
    3 --5-- 2

Edges: 0-1(2), 0-2(4), 0-3(6), 1-2(3), 2-3(5)
```

| Step | Prim's Algorithm | Kruskal's Algorithm |
|------|------------------|---------------------|
| **1. Initialization** | Pick starting vertex (e.g., 0)<br>`MST = {0}`<br>`key[0] = 0, others = ∞` | Sort ALL edges by weight<br>`Sorted: [0-1(2), 1-2(3), 0-2(4), 2-3(5), 0-3(6)]`<br>Each vertex in own set |
| **2. Iteration 1** | Find cheapest edge from MST to non-MST<br>`0→1 (weight 2)` is minimum<br>Add vertex 1<br>`MST = {0, 1}` | Process edge 0-1 (weight 2)<br>`find(0) ≠ find(1)` ✓<br>Add edge 0-1<br>`union(0, 1)` |
| **3. Iteration 2** | Edges from {0,1} to non-MST:<br>`0→2(4), 1→2(3), 0→3(6)`<br>Minimum: `1→2(3)`<br>Add vertex 2<br>`MST = {0, 1, 2}` | Process edge 1-2 (weight 3)<br>`find(1) ≠ find(2)` ✓<br>Add edge 1-2<br>`union(1, 2)` |
| **4. Iteration 3** | Edges from {0,1,2} to non-MST:<br>`0→3(6), 2→3(5)`<br>Minimum: `2→3(5)`<br>Add vertex 3<br>`MST = {0, 1, 2, 3}` | Process edge 0-2 (weight 4)<br>`find(0) = find(2)` ✗ SKIP<br>(would create cycle) |
| **5. Iteration 4** | All vertices in MST<br>DONE! | Process edge 2-3 (weight 5)<br>`find(2) ≠ find(3)` ✓<br>Add edge 2-3<br>`union(2, 3)` |
| **6. Completion** | MST edges: 0-1, 1-2, 2-3<br>Total weight: 2+3+5 = **10** | Process edge 0-3 (weight 6)<br>`find(0) = find(3)` ✗ SKIP<br>4 edges added, DONE!<br>Total: **10** |

**Result:** Both find MST with weight 10, but in different order!

---

#### Detailed Algorithm Comparisons

##### Prim's Algorithm - Vertex-Centric Growth

```
Algorithm:
1. Start: Pick any vertex, add to MST
2. Repeat until all vertices in MST:
   - Find MINIMUM weight edge where:
     * One endpoint IN MST
     * Other endpoint NOT in MST
   - Add this edge and new vertex to MST
3. Result: MST with (V-1) edges

Key data:
- key[v]: Minimum weight edge to connect v to MST
- parent[v]: Which MST vertex connects to v
- in_mst[v]: Boolean - is v in MST yet?
```

**Step-by-step example:**
```
Graph:     0--2--1        Start at 0
           |\ /  |
           6 4  3        Step 1: MST = {0}
           |  \/ |               Available: 0-1(2), 0-2(4), 0-3(6)
           3--5--2               Choose: 0-1(2) ✓

                         Step 2: MST = {0,1}
                                 Available: 1-2(3), 0-2(4), 0-3(6)
                                 Choose: 1-2(3) ✓

                         Step 3: MST = {0,1,2}
                                 Available: 0-2(4)✗skip, 2-3(5), 0-3(6)
                                 Choose: 2-3(5) ✓

                         Done! Total = 2+3+5 = 10
```

**Why it works:**
- Greedy choice: always pick minimum weight edge to grow tree
- Cut property: minimum edge crossing MST/non-MST boundary must be in MST
- Maintains one connected component throughout

---

##### Kruskal's Algorithm - Edge-Centric Merging

```
Algorithm:
1. Sort ALL edges by weight (ascending)
2. Initialize Union-Find: each vertex separate component
3. For each edge (u,v) in sorted order:
   - If find(u) ≠ find(v):  // Different components
     * Add edge to MST
     * union(u, v)           // Merge components
   - Else: skip (would create cycle)
4. Stop when (V-1) edges added

Key data structure: Union-Find
- find(v): Which component is v in?
- union(u,v): Merge components of u and v
```

**Step-by-step example:**
```
Graph:     0--2--1        Sorted edges: [0-1(2), 1-2(3), 0-2(4), 2-3(5), 0-3(6)]
           |\ /  |
           6 4  3        Components: {0} {1} {2} {3}
           |  \/ |
           3--5--2        Edge 0-1(2): find(0)≠find(1) ✓ Add, union(0,1)
                          Components: {0,1} {2} {3}

                          Edge 1-2(3): find(1)≠find(2) ✓ Add, union(1,2)
                          Components: {0,1,2} {3}

                          Edge 0-2(4): find(0)=find(2) ✗ Skip (cycle!)
                          Components: {0,1,2} {3}

                          Edge 2-3(5): find(2)≠find(3) ✓ Add, union(2,3)
                          Components: {0,1,2,3}

                          3 edges added, DONE! Total = 2+3+5 = 10
```

**Why it works:**
- Cycle property: maximum weight edge in any cycle cannot be in MST
- Union-Find efficiently detects if adding edge creates cycle
- Sorting ensures we always try cheapest edges first

---

#### Union-Find (Disjoint Set) - Required for Kruskal's

Kruskal's algorithm requires efficient cycle detection using Union-Find:

**Operations:**
```c
find(x):      Which set does x belong to?
              Returns representative/root of set

union(x, y):  Merge sets containing x and y
              Returns true if merged, false if already same set
```

**Optimizations:**

| Optimization | Technique | Benefit |
|-------------|-----------|---------|
| **Path Compression** | During find(), point all nodes directly to root | Makes tree flat, O(1) future finds |
| **Union by Rank** | Attach shorter tree under taller tree | Keeps trees balanced |

**Without optimizations:** O(n) per operation
**With both optimizations:** O(α(n)) per operation where α is inverse Ackermann

**Example:**
```
Initial: Each node its own set
  0   1   2   3

union(0, 1):
  0       2   3
 /
1

union(2, 3):
  0       2
 /       /
1       3

union(1, 2): find(1)=0, find(2)=2, merge
  0
 /|\
1 2 3

find(3): Traverse 3→2→0, compress path: 3→0
  0
 /|\
1 2 3  ← All point directly to root
```

---

#### When to Use Which MST Algorithm

**Use Prim's when:**
- ✅ Graph is **dense** (E ≈ V²)
- ✅ Using **adjacency matrix**
- ✅ Want to see tree **grow from a point**
- ✅ Can use optimized priority queue
- Example: Dense city network with many connections

**Use Kruskal's when:**
- ✅ Graph is **sparse** (E << V²)
- ✅ Have **edge list** representation
- ✅ Edges are **pre-sorted** or sortable
- ✅ Can implement Union-Find efficiently
- Example: Large sparse network, parallel processing

**Performance comparison:**

```
For 1000 vertices:

Dense (E = 100,000):
  Prim's (array):    O(V²) = 1,000,000        ✓ Better
  Kruskal's:         O(E log E) ≈ 1,700,000   Slower

Sparse (E = 5,000):
  Prim's (heap):     O((V+E) log V) ≈ 70,000  Slower
  Kruskal's:         O(E log E) ≈ 62,000      ✓ Better
```

---

#### MST Properties and Guarantees

**Both algorithms guarantee:**
- ✅ Find **optimal** MST (minimum total weight)
- ✅ Same **total weight** (though edges may differ if weights tied)
- ✅ Work only on **undirected** graphs
- ✅ Require **connected** graph (or find MST forest)
- ✅ **Greedy algorithms** (locally optimal → globally optimal)

**Why greedy works for MST (but not always):**
- **Cut Property**: For any cut, minimum edge crossing cut is in some MST
- **Cycle Property**: Maximum edge in any cycle not in any MST
- These properties guarantee greedy choices are safe

---

#### Advanced Graph Algorithms

##### Topological Sort - Kahn's Algorithm

**What is Topological Sort?**

A linear ordering of vertices in a DAG (Directed Acyclic Graph) where:
- For every edge u → v, vertex u comes before v in the ordering
- Only works on DAGs (no cycles!)

**Real-world applications:**
- **Course prerequisites** - take Data Structures before Algorithms
- **Build systems** - compile dependencies before main program
- **Task scheduling** - finish prerequisite tasks first
- **Package managers** - install dependencies before package

**Example:**
```
Graph:  0 → 1 → 3
        ↓       ↑
        2 ------┘

Valid orderings: [0, 1, 2, 3] or [0, 2, 1, 3]
Invalid: [1, 0, 2, 3] (1 before 0, but 0→1 exists)
```

**Kahn's Algorithm (BFS-based):**

```
1. Calculate in-degree for each vertex
2. Add vertices with in-degree 0 to queue (no dependencies)
3. While queue not empty:
   a. Remove vertex, add to result
   b. Reduce in-degree of neighbors
   c. If neighbor's in-degree becomes 0, add to queue
4. If processed all vertices → success
   Else → cycle detected
```

**Why it works:**
- Vertices with in-degree 0 have no dependencies → safe to process
- Removing a vertex = satisfying its dependency for neighbors
- If can't process all vertices → cycle blocks progress

**Time Complexity:** O(V + E)
- Calculate in-degrees: O(V + E)
- Process each vertex once: O(V)
- Process each edge once: O(E)

**Space Complexity:** O(V)

**Cycle Detection:**
- If result contains all V vertices → valid topological order
- Otherwise → cycle exists, no valid ordering

---

##### Floyd-Warshall - All-Pairs Shortest Paths

**What is Floyd-Warshall?**

Finds shortest paths between **all pairs** of vertices.

Unlike Dijkstra (single-source) or BFS (single-source, unweighted), Floyd-Warshall computes distances from every vertex to every other vertex in one go.

**When to use:**
- ✅ Need distances between **all pairs**
- ✅ Dense graphs (many edges)
- ✅ **Negative weights** allowed
- ✅ Graph fits in memory (O(V²) space)

**When NOT to use:**
- ❌ Only need single-source paths (use Dijkstra/Bellman-Ford instead)
- ❌ Sparse graphs with single query (Dijkstra is faster)
- ❌ Very large graphs (O(V³) time prohibitive)

**Algorithm Intuition:**

Dynamic Programming with intermediate vertices:

```
dist[i][j][k] = shortest path from i to j using {0,1,...,k}

Recurrence:
dist[i][j][k] = min(
    dist[i][j][k-1],                    // Don't use k
    dist[i][k][k-1] + dist[k][j][k-1]  // Go through k
)

Space optimization: update 2D array in-place
dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
```

**Algorithm Steps:**

```
1. Initialize distance matrix:
   - dist[i][i] = 0 (distance to self)
   - dist[i][j] = weight(i,j) if edge exists
   - dist[i][j] = ∞ otherwise

2. For each intermediate vertex k (0 to V-1):
     For each pair (i, j):
       Try path i → k → j
       If shorter than current dist[i][j], update

3. Check diagonal for negative cycles:
   If dist[i][i] < 0 → negative cycle exists
```

**Example:**
```
Initial (direct edges):
     0   1   2   3
0 [  0   5  ∞  10 ]
1 [ ∞   0   3  ∞  ]
2 [ ∞  ∞   0   1  ]
3 [ ∞  ∞  ∞   0  ]

After k=1 (paths through vertex 1):
dist[0][2] = min(∞, dist[0][1] + dist[1][2])
           = min(∞, 5 + 3) = 8  ✓ Update!

After k=2 (paths through vertex 2):
dist[0][3] = min(10, dist[0][2] + dist[2][3])
           = min(10, 8 + 1) = 9  ✓ Better!

Final (all-pairs shortest):
     0   1   2   3
0 [  0   5   8   9 ]
1 [ ∞   0   3   4 ]
2 [ ∞  ∞   0   1 ]
3 [ ∞  ∞  ∞   0 ]
```

**Time Complexity:** O(V³)
- Three nested loops over all vertices
- Simple but can be slow for large graphs

**Space Complexity:** O(V²)
- Distance matrix: V × V

**Comparison:**
```
Single-source queries:
  Dijkstra: O((V+E) log V)  ← Better for single query

All-pairs queries:
  Run Dijkstra V times: O(V² log V + VE log V) ≈ O(V³ log V) dense
  Floyd-Warshall:       O(V³)                  ← Simpler, handles negatives
```

**Key Features:**
- ✅ Handles **negative weights** (unlike Dijkstra)
- ✅ **Detects negative cycles**
- ✅ Simple implementation (3 nested loops)
- ✅ Returns complete distance matrix
- ❌ Slower than repeated Dijkstra for sparse graphs
- ❌ High memory usage O(V²)

---

**Graph types supported:**
- ✅ Directed and Undirected
- ✅ Weighted and Unweighted
- ✅ Complete graphs (dense - uses matrix)
- ✅ Sparse graphs (uses adjacency list)
- ✅ DAGs (Directed Acyclic Graphs)
- ✅ Bipartite graphs

**Visualization:**
- ✅ Exports to Graphviz DOT format
- ✅ Automatic rendering to PNG
- ✅ Terminal display with chafa
- ✅ Force-directed layouts for clarity

**Shortest path algorithms:**
- `graph_bfs_shortest_path()` - BFS for unweighted graphs (guarantees shortest path)
- `graph_dijkstra()` - For non-negative weighted graphs (greedy, optimal)
- `graph_bellman_ford()` - For graphs with negative weights + cycle detection
- `graph_floyd_warshall()` - All-pairs shortest paths (handles negatives, detects cycles)

**Minimum Spanning Tree (MST) algorithms:**
- `graph_prim_mst()` - Vertex-based MST (best for dense graphs)
- `graph_kruskal_mst()` - Edge-based MST with Union-Find (best for sparse)
- Full Union-Find implementation with path compression and union by rank

**Advanced graph algorithms:**
- `graph_topological_sort_kahn()` - Topological ordering for DAGs (Kahn's algorithm)

**Graph properties:**
- `graph_is_bipartite()` - 2-coloring algorithm, O(V+E)
- `graph_is_dag()` - Cycle detection using DFS, O(V+E)

**Algorithm complexity:**
- BFS: Time O(V + E), Space O(V) - fastest, unweighted only
- Dijkstra: Time O((V+E) log V), Space O(V) - fast, non-negative weights
- Bellman-Ford: Time O(V × E), Space O(V) - slower, handles negatives
- Floyd-Warshall: Time O(V³), Space O(V²) - all-pairs, handles negatives
- Prim's: Time O(V²) or O((V+E) log V), Space O(V) - dense graphs
- Kruskal's: Time O(E log E), Space O(V + E) - sparse graphs
- Topological Sort: Time O(V + E), Space O(V) - DAGs only

---

### 10. Hash Tables

**File:** `src/10_hash_tables.c`

Hash tables provide O(1) average-case lookups using hash functions and collision resolution.

#### What is a Hash Table?

A hash table maps keys to values using a hash function:
1. **Hash function** converts key → array index
2. Store key-value pair at that index
3. Handle **collisions** when different keys hash to same index

**Time Complexity:**
- Average case: **O(1)** for insert, search, delete
- Worst case: **O(n)** if all keys collide (becomes linked list)

**Space Complexity:** O(n + m) where n = entries, m = table size

#### Hash Functions Implemented

**1. Additive Hash (Poor)**
```c
hash = sum of all character values
```
- ❌ Anagrams collide ("cat" == "act" == "tac")
- ❌ Poor distribution
- Used to demonstrate why hash quality matters

**2. Multiplicative Hash (Better)**
```c
hash = (hash * 31) + char
```
- ✅ Position matters ("abc" ≠ "bca")
- ✅ Prime multiplier (31) improves distribution
- Common in practice

**3. DJB2 Hash (Excellent)**
```c
hash = ((hash << 5) + hash) + char  // hash * 33 + char
```
- ✅ Created by Dan Bernstein
- ✅ Magic constant 33 = 32 + 1 (fast bit shift)
- ✅ Starting value 5381 empirically chosen
- ✅ Very good distribution for strings

**4. FNV-1a Hash (Excellent)**
```c
hash = (hash XOR char) * FNV_PRIME
```
- ✅ Fowler-Noll-Vo algorithm
- ✅ Excellent avalanche effect
- ✅ Industry standard
- ✅ Good for hash tables

#### Collision Resolution: Chaining

**Implementation:** Array of linked lists
- Each bucket is a linked list
- Multiple keys at same index → stored in same list
- Simple and handles unlimited collisions

**Performance Impact:**
```
Load Factor = entries / table_size

< 0.5:  Fast but wastes space
~0.75:  Optimal balance (recommended)
> 1.0:  Slower, many collisions
```

#### Features

**Interactive Menu:**
1. Insert key-value pair
2. Search for key
3. Delete key
4. Display table (shows all buckets and chains)
5. Show statistics (collisions, load factor, chain lengths)
6. Change hash function (compare all 4 on-the-fly)
7. Clear table

**Demonstrations:**
1. **Hash Function Comparison** - Shows anagrams with all 4 hash functions
2. **Collision Demo** - Uses poor hash to force collisions
3. **Good Distribution** - Shows optimal spreading with DJB2

#### Key Concepts

**Hash Function Quality:**
```
Poor Hash (Additive):
  "listen" → 5
  "silent" → 5  ← COLLISION!
  "enlist" → 5  ← COLLISION!

Good Hash (DJB2):
  "listen" → 6
  "silent" → 6  ← Different internal paths
  "enlist" → 6  ← All hash differently
```

**Chaining Visualization:**
```
Bucket 0: [key1=val1] → [key2=val2] → NULL
Bucket 1: (empty)
Bucket 2: [key3=val3] → NULL
Bucket 3: [key4=val4] → [key5=val5] → [key6=val6] → NULL
```

**Statistics Tracked:**
- Total collisions
- Load factor
- Max chain length
- Average chain length
- Empty vs used buckets

#### When to Use Hash Tables

**Excellent for:**
- ✅ Fast lookups (O(1) average)
- ✅ Caching (URL → content)
- ✅ Frequency counting (word → count)
- ✅ Database indexing (key → record)
- ✅ Symbol tables in compilers
- ✅ Removing duplicates

**Not ideal for:**
- ❌ Ordered data (use trees)
- ❌ Range queries (need sorted structure)
- ❌ Guaranteed O(1) worst-case (can degrade to O(n))

#### Implementation Details

- Uses linked list chaining (leverages `2_linked_lists.h` concepts)
- Tracks collision statistics for analysis
- Supports dynamic hash function switching
- Real-time performance visualization
- Educational anagram tests

**Key Observations:**
1. Hash function quality dramatically affects performance
2. Load factor should stay below 0.75
3. Chaining is simple but chains should be short
4. Prime table sizes improve distribution

---

### 11. Sorting Algorithms

**File:** `src/11_sort.c`

Comprehensive implementation of four different sorting approaches, each optimized for different scenarios.

#### Algorithms Implemented

##### 1. Merge Sort (Linked List)

**Perfect for linked lists** - no random access needed!

**How it works:**
```
DIVIDE:  Split list in half (slow/fast pointer)
CONQUER: Recursively sort both halves
COMBINE: Merge two sorted lists
```

**Example:**
```
[4 → 2 → 1 → 3]

Split:     [4 → 2]  [1 → 3]
Split:     [4] [2]  [1] [3]
Merge:     [2 → 4]  [1 → 3]
Merge:     [1 → 2 → 3 → 4]
```

**Complexity:**
- Time: **O(n log n)** - guaranteed, no worst case
- Space: **O(log n)** - recursion stack only
- Stable: **YES**

**Why for linked lists:**
- ✅ No random access needed (unlike Quicksort)
- ✅ Sequential access perfect for linked structures
- ✅ In-place merging (no extra arrays)
- ✅ Guaranteed O(n log n) performance

---

##### 2. Quicksort (Array)

**Fast in-place array sorting** - industry standard.

**How it works:**
```
PARTITION: Choose pivot, rearrange so:
           [< pivot] pivot [> pivot]
RECURSE:   Sort left and right subarrays
```

**Partitioning (Lomuto scheme):**
```
[3, 7, 8, 5, 2, 1, 9]  pivot=9

i tracks position for small elements
When arr[j] < pivot: swap(arr[++i], arr[j])
Finally: place pivot at i+1
```

**Complexity:**
- Time: **O(n log n)** average, **O(n²)** worst
- Space: **O(log n)** - recursion stack
- Stable: **NO**

**Why for arrays:**
- ✅ In-place sorting (minimal extra space)
- ✅ Cache-friendly (good locality)
- ✅ Fast average case
- ✅ Widely used (stdlib qsort)

**When worst case O(n²):**
- Already sorted array
- Pivot always min/max
- Solution: randomized pivot or median-of-three

---

##### 3. Bucket Sort (Integers 0-k)

**Linear time for bounded integers** - distribution sort.

**How it works:**
```
1. CREATE: Array of k+1 buckets
2. COUNT:  For each value x: bucket[x]++
3. GATHER: Output each value bucket[i] times
```

**Example (range 0-9):**
```
Input: [3, 1, 4, 1, 5, 9, 2, 6]

Buckets:
  0:0  1:2  2:1  3:1  4:1  5:1  6:1  7:0  8:0  9:1
       ↑ appears twice

Output: [1, 1, 2, 3, 4, 5, 6, 9]
```

**Complexity:**
- Time: **O(n + k)** - linear when k = O(n)!
- Space: **O(k)** - bucket array
- Stable: **YES**

**When to use:**
- ✅ Know range [0, k] in advance
- ✅ Uniform distribution
- ✅ k = O(n) (not too large)
- Example: Sorting exam scores (0-100), ages (0-150)

**When NOT to use:**
- ❌ Unknown range
- ❌ k >> n (wastes space)
- ❌ Non-integer keys

---

##### 4. Bitonic Sort (Parallel)

**Hardware sorting network** - designed for GPUs and parallel architectures.

**What is Bitonic Sequence?**
```
First increases, then decreases:
[1, 3, 5, 7, 6, 4, 2]  ← bitonic!

NOT bitonic (multiple peaks):
[1, 3, 2, 4, 3, 5]
```

**How it works:**
```
BUILD:  Create bitonic sequences from pairs
        [3,7] [4,8] [6,2] [1,5]  ← sort pairs
        [3,4,7,8↓] [6,5,2,1↑]    ← merge to bitonic

MERGE:  Bitonic merge operation
        Compare/swap at distance d
        Creates sorted sequence
```

**Complexity:**
- Time: **O(n log² n)** sequential (worse than merge sort!)
- Time: **O(log² n)** parallel with n processors
- Space: **O(log n)**
- Requires: **n = power of 2**

**Why Bitonic Sort:**
- ✅ Highly parallelizable
- ✅ Data-oblivious (predictable comparisons)
- ✅ Perfect for GPUs and hardware networks
- ❌ NOT for sequential CPU (use quicksort/mergesort)
- ❌ Requires power-of-2 size

**Use cases:**
- GPU sorting (CUDA, OpenCL)
- Hardware sorting circuits
- Parallel sorting on multi-core systems
- NOT for general sequential sorting

---

#### Algorithm Comparison

| Algorithm | Best | Average | Worst | Space | Stable | Best For |
|-----------|------|---------|-------|-------|--------|----------|
| **Merge Sort** | O(n log n) | O(n log n) | O(n log n) | O(log n) | YES | Linked lists, guaranteed |
| **Quicksort** | O(n log n) | O(n log n) | O(n²) | O(log n) | NO | Arrays, in-place |
| **Bucket Sort** | O(n+k) | O(n+k) | O(n+k) | O(k) | YES | Bounded integers |
| **Bitonic Sort** | O(n log² n) | O(n log² n) | O(n log² n) | O(log n) | NO | Parallel hardware |

**When to use which:**

```
Linked list?              → Merge Sort
Array, general-purpose?   → Quicksort
Bounded integers [0, k]?  → Bucket Sort
GPU/parallel hardware?    → Bitonic Sort
Need stability?           → Merge or Bucket
Need O(n log n) guarantee?→ Merge Sort
Need in-place array sort? → Quicksort
```

---

#### Key Concepts

**Stability:**
- Stable: Equal elements maintain relative order
- Merge Sort, Bucket Sort: stable
- Quicksort, Bitonic: not stable

**In-place:**
- Quicksort: in-place (O(log n) stack)
- Merge Sort (array): NOT in-place, needs temp
- Merge Sort (list): in-place!
- Bucket Sort: NOT in-place (needs buckets)

**Divide-and-Conquer:**
- Merge Sort: split, recurse, merge
- Quicksort: partition, recurse
- Bitonic: build bitonic sequences, merge

**Distribution Sort:**
- Bucket Sort: distribute into buckets
- Linear time possible!
- Requires knowledge of data range

---

### Quick Reference Table

| Data Structure/Algorithm | Search | Insert | Delete | Space | Notes |
|-------------------------|--------|---------|---------|-------|-------|
| **Linked Lists** | | | | | |
| Singly Linked List | O(n) | O(1)* | O(n) | O(n) | *at head |
| Doubly Linked List | O(n) | O(1)* | O(1)* | O(n) | *with node ref |
| Circular Linked List | O(n) | O(1)* | O(n) | O(n) | *at head |
| **Stacks** | | | | | |
| Array-based Stack | - | O(1)† | O(1) | O(n) | †amortized |
| List-based Stack | - | O(1) | O(1) | O(n) | Plus pointer overhead |
| **Queues** | | | | | |
| List-based Queue | - | O(1) | O(1) | O(n) | Front/rear pointers |
| Queue (2 stacks) | - | O(1) | O(1)† | O(n) | †amortized |
| Stack (2 queues) | - | O(n) | O(1) | O(n) | Inefficient push |
| **Trees** | | | | | |
| Red-Black Tree | O(log n) | O(log n) | O(log n) | O(n) | Self-balancing BST |
| **Heaps** | | | | | |
| Max/Min Heap | O(n) | O(log n) | O(log n) | O(n) | Priority queue |
| Build Heap | - | - | - | O(n) | Linear time! |
| Heap Sort | - | - | - | O(n log n) | In-place O(1) |
| **Skip List** | O(log n) | O(log n) | O(log n) | O(n log n) | Probabilistic |
| **Stack Algorithms** | | | | | |
| Balanced symbols | O(n) | - | - | O(n) | Single pass |
| Infix to postfix | O(n) | - | - | O(n) | Operator precedence |
| Reverse stack (recursive) | - | - | - | O(n²) | Time O(n²) |
| **Algorithms** | | | | | |
| Factorial | - | - | - | O(n) | Recursion depth |
| Fibonacci (naive) | - | - | - | O(n) | Recursion depth |
| Tower of Hanoi | - | - | - | O(n) | 2ⁿ-1 moves |
| Nth from end | O(n) | - | - | O(1) | Single pass |
| Cycle detection | O(n) | - | - | O(1) | Two pointers |
| List reversal (recursive) | - | - | - | O(n) | Stack depth |
| Knapsack (DP) | - | - | - | O(n×W) | W=capacity |
| N-Queens | - | - | - | O(N²) | Backtracking |
| **Graphs** | | | | | |
| Adjacency Matrix | O(1) | O(1) | O(1) | O(V²) | Dense graphs |
| Adjacency List | O(V) | O(1) | O(V) | O(V+E) | Sparse graphs |
| BFS (unweighted) | O(V+E) | - | - | O(V) | Shortest path |
| Dijkstra | O((V+E)logV) | - | - | O(V) | Non-negative weights |
| Bellman-Ford | O(V×E) | - | - | O(V) | Handles negative weights |
| Floyd-Warshall | O(V³) | - | - | O(V²) | All-pairs shortest paths |
| Topological Sort | O(V+E) | - | - | O(V) | DAGs only, Kahn's algorithm |
| Prim's MST | O(V²) or O((V+E)logV) | - | - | O(V) | Dense graphs |
| Kruskal's MST | O(E log E) | - | - | O(V+E) | Sparse graphs |
| Union-Find | O(α(V)) | - | - | O(V) | α ≈ constant |
| **Hash Tables** | | | | | |
| Hash Table (chaining) | O(1) avg, O(n) worst | O(1) avg | O(1) avg | O(n+m) | m = table size |
| Hash Function | O(k) | - | - | O(1) | k = key length |
| **Sorting** | | | | | |
| Merge Sort (list) | - | - | - | O(log n) | O(n log n) time, stable |
| Quicksort (array) | - | - | - | O(log n) | O(n log n) avg, in-place |
| Bucket Sort | - | - | - | O(k) | O(n+k) time, k=range |
| Bitonic Sort | - | - | - | O(log n) | O(n log² n), parallel |

### Space Complexity Notes:

- **O(1):** Constant - only a few variables
- **O(n):** Linear - proportional to input size
- **O(log n):** Logarithmic - recursion depth for balanced structures
- **O(n²):** Quadratic - 2D arrays, nested structures

### Time Complexity Classes:

- **O(1):** Constant - hash table lookup, array access
- **O(log n):** Logarithmic - binary search, balanced trees, skip lists
- **O(n):** Linear - array traversal, linked list operations
- **O(n log n):** Linearithmic - efficient sorting (merge sort)
- **O(n²):** Quadratic - nested loops, DP tables
- **O(2ⁿ):** Exponential - recursive fibonacci, subset generation
- **O(n!):** Factorial - permutations, N-Queens

---

## Implementation Highlights

### Design Patterns

- **Separation of Concerns:** Source, build files, and outputs in separate directories
- **Modular Makefiles:** Each program has its own `.mk` file with LSP support
- **Consistent Naming:** Prefix functions by data structure (e.g., `sll_insert`, `skip_create`)
- **Memory Management:** All allocations have corresponding free operations
- **Error Handling:** Null checks and bounds validation

### Code Quality

- **Compiler Warnings:** All programs compile with `-Wall -Wextra`
- **No Memory Leaks:** Proper cleanup in all test functions
- **Well Commented:** Algorithm explanations and complexity notes
- **Interactive Tests:** Menu-driven programs for hands-on exploration

---

## Learning Path

### Recommended Order:

1. **Start with Recursion** - Foundation for many algorithms
2. **Master Linked Lists** - Core dynamic data structure
3. **Learn Stacks and Queues** - Essential for algorithm design
4. **Understand Trees** - Self-balancing structures and rotations
5. **Master Heaps** - Priority queues and efficient sorting
6. **Study Skip Lists** - Probabilistic alternative to balanced trees
7. **Practice Two-Pointer Techniques** - Essential interview skill
8. **Explore Dynamic Programming** - Advanced optimization
9. **Master Graphs** - Shortest paths, DFS/BFS, real-world networks
10. **Understand Hash Tables** - O(1) lookups, collision resolution, hash functions
11. **Study Sorting Algorithms** - Compare approaches, understand trade-offs

### Key Concepts to Understand:

- **Pointers & Memory:** How linked structures work
- **Recursion:** Base cases, call stack, divide-and-conquer
- **Time/Space Tradeoffs:** When to optimize for speed vs memory
- **Probabilistic Structures:** How randomization provides balance
- **Algorithmic Paradigms:** DP, backtracking, greedy, divide-and-conquer

---

## Contributing

Feel free to add new data structures and algorithms! Follow the existing patterns:

1. Create `src/your_structure.c`
2. Create `makefiles/your_structure.mk`
3. Update `Makefile` and `compile_and_run.sh`
4. Add documentation to this README

---

## License

Educational code for learning data structures and algorithms.

---

## References

- Cormen, T. H., et al. *Introduction to Algorithms* (CLRS)
- Sedgewick, R. *Algorithms in C*
- Skiena, S. *The Algorithm Design Manual*
- Pugh, W. *Skip Lists: A Probabilistic Alternative to Balanced Trees*

---

*Built with standard C, compiled with GCC, organized for clarity and learning.*
