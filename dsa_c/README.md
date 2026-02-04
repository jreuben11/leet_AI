# Data Structures and Algorithms in C

A comprehensive collection of classic data structures and algorithms implemented in C, with detailed explanations and complexity analysis.

## Table of Contents

- [Project Structure](#project-structure)
- [Building and Running](#building-and-running)
- [Data Structures & Algorithms](#data-structures--algorithms)
  - [1. Recursion](#1-recursion)
  - [2. Linked Lists](#2-linked-lists)
  - [3. Stacks and Queues](#3-stacks-and-queues)
  - [4. Skip Lists](#4-skip-lists)
  - [5. List Search Algorithms](#5-list-search-algorithms)
  - [6. Dynamic Programming](#6-dynamic-programming)
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
│   ├── skip_list.c
│   ├── list_search.c
│   └── dynamic_programming.c
├── makefiles/              # Build configurations
│   ├── recursion.mk
│   ├── linked_lists.mk
│   ├── stacks_and_queues.mk
│   ├── skip_list.mk
│   ├── list_search.mk
│   └── dynamic_programming.mk
├── out/                    # Compiled binaries
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
make skip_list

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
./out/skip_list
./out/list_search
./out/dynamic_programming
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

### 4. Skip Lists

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

### 5. List Search Algorithms

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

### 6. Dynamic Programming

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

## Complexity Summary

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
4. **Study Skip Lists** - Bridge to tree-like structures
5. **Practice Two-Pointer Techniques** - Essential interview skill
6. **Explore Dynamic Programming** - Advanced optimization

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
