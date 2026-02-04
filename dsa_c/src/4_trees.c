/*
 * Red-Black Tree Implementation
 *
 * A self-balancing binary search tree with the following properties:
 * 1. Every node is either red or black
 * 2. Root is always black
 * 3. All leaves (NIL) are black
 * 4. Red nodes cannot have red children
 * 5. All paths from root to leaves contain the same number of black nodes
 *
 * These properties guarantee O(log n) height and thus O(log n) operations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stacks_queues.h"

// ============================================================
// DATA STRUCTURES
// ============================================================

// Color enumeration for nodes
typedef enum { RED, BLACK } Color;

// Red-Black Tree Node
// Contains data, color, and pointers to left, right, and parent nodes
typedef struct RBNode {
    int data;                   // Value stored in the node
    Color color;                // RED or BLACK
    struct RBNode* left;        // Left child
    struct RBNode* right;       // Right child
    struct RBNode* parent;      // Parent (needed for efficient rotations)
} RBNode;

// Red-Black Tree
// Contains root and sentinel NIL node
typedef struct {
    RBNode* root;               // Root of the tree
    RBNode* nil;                // Sentinel node (always black, represents NULL)
} RBTree;

// ============================================================
// TREE CREATION AND NODE MANAGEMENT
// ============================================================

/**
 * Create a new red-black tree node
 *
 * @param tree  Pointer to the tree (needed for NIL sentinel)
 * @param data  Value to store in the node
 * @return      Pointer to newly created node
 *
 * Note: New nodes are always colored RED to preserve black height property.
 *       This may temporarily violate the red-red property, which is fixed
 *       during insertion fixup.
 */
RBNode* rb_create_node(RBTree* tree, int data) {
    RBNode* node = (RBNode*)malloc(sizeof(RBNode));
    if (!node) {
        printf("Memory allocation error\n");
        exit(1);
    }

    node->data = data;
    node->color = RED;              // New nodes start RED (preserves black height)
    node->left = tree->nil;         // Children point to sentinel NIL
    node->right = tree->nil;
    node->parent = tree->nil;       // Parent initially NIL
    return node;
}

/**
 * Create a new empty red-black tree
 *
 * @return  Pointer to newly created tree
 *
 * Creates a sentinel NIL node that represents all NULL pointers in the tree.
 * This simplifies code by eliminating NULL checks.
 * The NIL node is always BLACK (satisfies RB property 3).
 */
RBTree* rb_create_tree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    if (!tree) {
        printf("Memory allocation error\n");
        exit(1);
    }

    // Create sentinel NIL node (shared by all NULL pointers)
    tree->nil = (RBNode*)malloc(sizeof(RBNode));
    tree->nil->color = BLACK;       // NIL is always BLACK
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->parent = NULL;
    tree->nil->data = 0;            // Data irrelevant for sentinel

    tree->root = tree->nil;         // Empty tree: root points to NIL
    return tree;
}

// ============================================================
// ROTATION OPERATIONS
// ============================================================

/**
 * Perform left rotation on node x
 *
 * @param tree  Pointer to the tree
 * @param x     Node to rotate (becomes left child of its right child)
 *
 * Before:         After:
 *     x             y
 *    a  y    =>    x  c
 *      b c        a b
 *
 * Operation:
 * 1. y (x's right child) becomes the new parent
 * 2. x becomes y's left child
 * 3. y's left subtree (b) becomes x's right subtree
 *
 * Time: O(1) - only pointer updates
 * Preserves: BST property (a < x < b < y < c)
 */
void rb_rotate_left(RBTree* tree, RBNode* x) {
    RBNode* y = x->right;           // y will become new parent

    // Step 1: Move y's left subtree to x's right
    x->right = y->left;
    if (y->left != tree->nil) {
        y->left->parent = x;        // Update parent pointer
    }

    // Step 2: Link y to x's parent
    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;             // x was root, now y is root
    } else if (x == x->parent->left) {
        x->parent->left = y;        // x was left child
    } else {
        x->parent->right = y;       // x was right child
    }

    // Step 3: Put x as y's left child
    y->left = x;
    x->parent = y;
}

/**
 * Perform right rotation on node y
 *
 * @param tree  Pointer to the tree
 * @param y     Node to rotate (becomes right child of its left child)
 *
 * Before:         After:
 *      y            x
 *     x c    =>    a y
 *    a b            b c
 *
 * Operation:
 * 1. x (y's left child) becomes the new parent
 * 2. y becomes x's right child
 * 3. x's right subtree (b) becomes y's left subtree
 *
 * Time: O(1) - only pointer updates
 * Preserves: BST property (a < x < b < y < c)
 */
void rb_rotate_right(RBTree* tree, RBNode* y) {
    RBNode* x = y->left;            // x will become new parent

    // Step 1: Move x's right subtree to y's left
    y->left = x->right;
    if (x->right != tree->nil) {
        x->right->parent = y;       // Update parent pointer
    }

    // Step 2: Link x to y's parent
    x->parent = y->parent;
    if (y->parent == tree->nil) {
        tree->root = x;             // y was root, now x is root
    } else if (y == y->parent->right) {
        y->parent->right = x;       // y was right child
    } else {
        y->parent->left = x;        // y was left child
    }

    // Step 3: Put y as x's right child
    x->right = y;
    y->parent = x;
}

// ============================================================
// INSERTION OPERATIONS
// ============================================================

/**
 * Fix red-black tree properties after insertion
 *
 * @param tree  Pointer to the tree
 * @param z     Newly inserted node (starts as RED)
 *
 * Problem: Inserting a RED node may create red-red violation (red parent + red child)
 *
 * Solution: Three cases based on uncle's color:
 *
 * Case 1: Uncle is RED
 *   - Action: Recolor parent, uncle (to BLACK), grandparent (to RED)
 *   - Result: Pushes problem up to grandparent
 *   - Repeat: Continue checking from grandparent
 *
 * Case 2: Uncle is BLACK, z is "inside" child (triangle)
 *   - Action: Rotate z's parent to convert to "outside" (line) configuration
 *   - Result: Converts to Case 3
 *
 * Case 3: Uncle is BLACK, z is "outside" child (line)
 *   - Action: Rotate grandparent, recolor parent and grandparent
 *   - Result: Fixes violation, done
 *
 * Time: O(log n) - may travel up the tree
 * Rotations: At most 2
 */
void rb_fix_insert_violations(RBTree* tree, RBNode* z) {
    // Loop while there's a red-red violation (red node with red parent)
    while (z->parent->color == RED) {
        // Check if parent is LEFT child of grandparent
        if (z->parent == z->parent->parent->left) {
            RBNode* uncle = z->parent->parent->right;  // Uncle is on the right

            if (uncle->color == RED) {
                // CASE 1: Uncle is RED
                // Recolor: parent BLACK, uncle BLACK, grandparent RED
                // This preserves black height but may create violation at grandparent
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;  // Move up and continue checking
            } else {
                // Uncle is BLACK
                if (z == z->parent->right) {
                    // CASE 2: Triangle configuration (z is right child, parent is left child)
                    // Left rotate parent to convert to line configuration (Case 3)
                    z = z->parent;
                    rb_rotate_left(tree, z);
                }
                // CASE 3: Line configuration (z is left child, parent is left child)
                // Recolor and right rotate grandparent
                z->parent->color = BLACK;           // Parent becomes BLACK
                z->parent->parent->color = RED;     // Grandparent becomes RED
                rb_rotate_right(tree, z->parent->parent);  // Rotate grandparent
            }
        } else {
            // MIRROR CASES: Parent is RIGHT child of grandparent
            RBNode* uncle = z->parent->parent->left;  // Uncle is on the left

            if (uncle->color == RED) {
                // CASE 1 (mirror): Uncle is RED - recolor
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                // Uncle is BLACK
                if (z == z->parent->left) {
                    // CASE 2 (mirror): Triangle - right rotate to convert to line
                    z = z->parent;
                    rb_rotate_right(tree, z);
                }
                // CASE 3 (mirror): Line - recolor and left rotate
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rb_rotate_left(tree, z->parent->parent);
            }
        }
    }
    // Ensure root is always BLACK (may have been recolored RED in Case 1)
    tree->root->color = BLACK;
}

/**
 * Insert a value into the red-black tree
 *
 * @param tree  Pointer to the tree
 * @param data  Value to insert
 *
 * Algorithm:
 * 1. Create new RED node
 * 2. Insert like normal BST (find position, link to parent)
 * 3. Fix any red-black violations caused by insertion
 *
 * Time: O(log n) - O(log n) to find position + O(log n) to fix
 * Space: O(1) - only uses constant extra space
 */
void rb_insert_node(RBTree* tree, int data) {
    RBNode* new_node = rb_create_node(tree, data);  // Create RED node
    RBNode* parent = tree->nil;                     // Will track parent
    RBNode* current = tree->root;                   // Start from root

    // Standard BST insertion: find correct position
    while (current != tree->nil) {
        parent = current;                           // Track parent as we descend
        if (new_node->data < current->data) {
            current = current->left;                // Go left if smaller
        } else {
            current = current->right;               // Go right if larger/equal
        }
    }

    // Link new node to its parent
    new_node->parent = parent;

    if (parent == tree->nil) {
        tree->root = new_node;                      // Tree was empty, new node is root
    } else if (new_node->data < parent->data) {
        parent->left = new_node;                    // Attach as left child
    } else {
        parent->right = new_node;                   // Attach as right child
    }

    // Fix any red-black property violations
    rb_fix_insert_violations(tree, new_node);
}

// ============================================================
// RED-BLACK TREE DELETION
// ============================================================

// Find minimum node in subtree
RBNode* rb_minimum(RBTree* tree, RBNode* x) {
    while (x->left != tree->nil) {
        x = x->left;
    }
    return x;
}

// Transplant subtree
void rb_transplant(RBTree* tree, RBNode* u, RBNode* v) {
    if (u->parent == tree->nil) {
        tree->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}

// Fix red-black tree properties after deletion
void rb_delete_fixup(RBTree* tree, RBNode* x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode* w = x->parent->right;  // Sibling

            if (w->color == RED) {
                // Case 1: Sibling is red
                w->color = BLACK;
                x->parent->color = RED;
                rb_rotate_left(tree, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == BLACK && w->right->color == BLACK) {
                // Case 2: Sibling's children are both black
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    // Case 3: Sibling's right child is black
                    w->left->color = BLACK;
                    w->color = RED;
                    rb_rotate_right(tree, w);
                    w = x->parent->right;
                }
                // Case 4: Sibling's right child is red
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rb_rotate_left(tree, x->parent);
                x = tree->root;
            }
        } else {
            // Mirror cases (x is right child)
            RBNode* w = x->parent->left;  // Sibling

            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_rotate_right(tree, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    rb_rotate_left(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rb_rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

// Delete a node from the red-black tree
// Time Complexity: O(log n)
void rb_delete(RBTree* tree, RBNode* z) {
    RBNode* y = z;
    RBNode* x;
    Color y_original_color = y->color;

    if (z->left == tree->nil) {
        x = z->right;
        rb_transplant(tree, z, z->right);
    } else if (z->right == tree->nil) {
        x = z->left;
        rb_transplant(tree, z, z->left);
    } else {
        y = rb_minimum(tree, z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        } else {
            rb_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rb_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == BLACK) {
        rb_delete_fixup(tree, x);
    }

    free(z);
}

// Search for a node with given value
RBNode* rb_search(RBTree* tree, RBNode* x, int data) {
    if (x == tree->nil || data == x->data) {
        return x;
    }

    if (data < x->data) {
        return rb_search(tree, x->left, data);
    } else {
        return rb_search(tree, x->right, data);
    }
}

// ============================================================
// TREE TRAVERSAL OPERATIONS
// ============================================================

/**
 * Inorder traversal: Left subtree -> Root -> Right subtree
 *
 * @param tree  Pointer to the tree
 * @param x     Current node being visited
 *
 * Order: Visit left subtree, then current node, then right subtree
 * Result: Values printed in SORTED order (ascending)
 *
 * Example tree:     20
 *                  /  \
 *                10    30
 * Output: 10 20 30
 *
 * Time: O(n) - visits each node exactly once
 * Space: O(h) - recursion stack depth, h = height
 *
 * Use cases:
 * - Print all values in sorted order
 * - Validate BST property (should be ascending)
 * - Get sorted list of elements
 */
void rb_traverse_inorder(RBTree* tree, RBNode* x) {
    if (x != tree->nil) {
        rb_traverse_inorder(tree, x->left);        // 1. Visit left subtree
        printf("%d(%s) ", x->data, x->color == RED ? "R" : "B");  // 2. Visit root
        rb_traverse_inorder(tree, x->right);       // 3. Visit right subtree
    }
}

/**
 * Preorder traversal: Root -> Left subtree -> Right subtree
 *
 * @param tree  Pointer to the tree
 * @param x     Current node being visited
 *
 * Order: Visit current node first, then left subtree, then right subtree
 * Result: Root appears before its children
 *
 * Example tree:     20
 *                  /  \
 *                10    30
 * Output: 20 10 30
 *
 * Time: O(n) - visits each node exactly once
 * Space: O(h) - recursion stack depth
 *
 * Use cases:
 * - Copy/clone tree structure
 * - Serialize tree (save to file)
 * - Create prefix expression from expression tree
 */
void rb_traverse_preorder(RBTree* tree, RBNode* x) {
    if (x != tree->nil) {
        printf("%d(%s) ", x->data, x->color == RED ? "R" : "B");  // 1. Visit root
        rb_traverse_preorder(tree, x->left);       // 2. Visit left subtree
        rb_traverse_preorder(tree, x->right);      // 3. Visit right subtree
    }
}

/**
 * Postorder traversal: Left subtree -> Right subtree -> Root
 *
 * @param tree  Pointer to the tree
 * @param x     Current node being visited
 *
 * Order: Visit left subtree, then right subtree, then current node
 * Result: Children appear before their parent
 *
 * Example tree:     20
 *                  /  \
 *                10    30
 * Output: 10 30 20
 *
 * Time: O(n) - visits each node exactly once
 * Space: O(h) - recursion stack depth
 *
 * Use cases:
 * - Delete tree safely (free children before parent)
 * - Evaluate expression tree (compute children before parent)
 * - Get postfix expression from expression tree
 */
void rb_traverse_postorder(RBTree* tree, RBNode* x) {
    if (x != tree->nil) {
        rb_traverse_postorder(tree, x->left);      // 1. Visit left subtree
        rb_traverse_postorder(tree, x->right);     // 2. Visit right subtree
        printf("%d(%s) ", x->data, x->color == RED ? "R" : "B");  // 3. Visit root
    }
}

// ============================================================
// DEPTH-FIRST SEARCH (DFS) AND BREADTH-FIRST SEARCH (BFS)
// ============================================================

/**
 * Iterative Depth-First Search using a stack
 *
 * @param tree          Pointer to the tree
 * @param root          Starting node for DFS
 * @param search_value  Value to search for (-1 to just traverse and print)
 * @return              Pointer to node if found, NULL otherwise
 *
 * Algorithm:
 * 1. Push root onto stack
 * 2. While stack not empty:
 *    a. Pop node from stack
 *    b. Visit node (if searching, check if found)
 *    c. Push right child (will be visited later)
 *    d. Push left child (will be visited sooner)
 *
 * Result: Preorder traversal (Root -> Left -> Right)
 * Note: We push right before left so left is visited first (stack is LIFO)
 *
 * Time: O(n) - visits each node once (or until found if searching)
 * Space: O(h) - stack depth equals height
 *
 * Use cases:
 * - Tree traversal without recursion
 * - Search for a value in the tree
 * - Detect cycles in graphs
 * - Topological sorting
 * - Path finding
 */
RBNode* rb_traverse_dfs_iterative(RBTree* tree, RBNode* root, int search_value) {
    if (root == tree->nil) {
        return NULL;
    }

    // Create stack to hold nodes
    ListStack* stack = list_stack_create();

    // Push root to start
    list_stack_push(stack, (long)root);  // Store pointer as long

    bool is_search = (search_value != -1);
    if (!is_search) {
        printf("DFS (Iterative): ");
    }

    RBNode* found = NULL;

    while (!list_stack_is_empty(stack)) {
        // Pop node from stack
        long node_long = list_stack_pop(stack);
        RBNode* node = (RBNode*)node_long;

        // Check if this is the node we're searching for
        if (is_search && node->data == search_value) {
            found = node;
            break;  // Found it, exit early
        }

        // Visit node (only print if traversing, not searching)
        if (!is_search) {
            printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
        }

        // Push right child first (LIFO - will be visited later)
        if (node->right != tree->nil) {
            list_stack_push(stack, (long)node->right);
        }

        // Push left child second (will be visited sooner)
        if (node->left != tree->nil) {
            list_stack_push(stack, (long)node->left);
        }
    }

    if (!is_search) {
        printf("\n");
    }
    list_stack_destroy(stack);

    return found;
}

/**
 * Recursive Depth-First Search (general form)
 *
 * @param tree          Pointer to the tree
 * @param node          Current node being visited
 * @param search_value  Value to search for (-1 to just traverse and print)
 * @return              Pointer to node if found, NULL otherwise
 *
 * This is the general recursive DFS pattern.
 * The specific order (preorder, inorder, postorder) depends on when you visit the node.
 *
 * This implementation does preorder (Root -> Left -> Right)
 *
 * Time: O(n) - visits each node once (or until found if searching)
 * Space: O(h) - recursion stack depth equals height
 */
RBNode* rb_traverse_dfs_recursive(RBTree* tree, RBNode* node, int search_value) {
    if (node == tree->nil) {
        return NULL;
    }

    bool is_search = (search_value != -1);

    // Check if this is the node we're searching for
    if (is_search && node->data == search_value) {
        return node;  // Found it
    }

    // Visit node (preorder position - only print if traversing)
    if (!is_search) {
        printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
    }

    // Recurse on left subtree
    RBNode* found = rb_traverse_dfs_recursive(tree, node->left, search_value);
    if (found != NULL) {
        return found;  // Found in left subtree
    }

    // Recurse on right subtree
    return rb_traverse_dfs_recursive(tree, node->right, search_value);
}

/**
 * Breadth-First Search (Level-Order Traversal) using a queue
 *
 * @param tree          Pointer to the tree
 * @param root          Starting node for BFS
 * @param search_value  Value to search for (-1 to just traverse and print)
 * @return              Pointer to node if found, NULL otherwise
 *
 * Algorithm:
 * 1. Enqueue root
 * 2. While queue not empty:
 *    a. Dequeue node
 *    b. Visit node (if searching, check if found)
 *    c. Enqueue left child
 *    d. Enqueue right child
 *
 * Result: Level-order traversal (visits all nodes at depth d before depth d+1)
 *
 * Example tree:      20
 *                   /  \
 *                 10    30
 *                / \    / \
 *               5  15  25  35
 *
 * BFS output: 20 10 30 5 15 25 35 (level by level)
 *
 * Time: O(n) - visits each node once (or until found if searching)
 * Space: O(w) - queue size equals maximum width of tree
 *
 * Use cases:
 * - Level-order tree printing
 * - Search for a value in the tree
 * - Find shortest path in unweighted graph
 * - Find all nodes at distance k from root
 * - Serialize tree level-by-level
 */
RBNode* rb_traverse_bfs(RBTree* tree, RBNode* root, int search_value) {
    if (root == tree->nil) {
        return NULL;
    }

    // Create queue to hold nodes
    ListQueue* queue = list_queue_create();

    // Enqueue root to start
    list_queue_enqueue(queue, (long)root);  // Store pointer as long

    bool is_search = (search_value != -1);
    if (!is_search) {
        printf("BFS (Level-order): ");
    }

    RBNode* found = NULL;

    while (!list_queue_is_empty(queue)) {
        // Dequeue node from front
        RBNode* node = (RBNode*)(long)list_queue_dequeue(queue);

        // Check if this is the node we're searching for
        if (is_search && node->data == search_value) {
            found = node;
            break;  // Found it, exit early
        }

        // Visit node (only print if traversing, not searching)
        if (!is_search) {
            printf("%d(%s) ", node->data, node->color == RED ? "R" : "B");
        }

        // Enqueue left child (will be visited before right due to FIFO)
        if (node->left != tree->nil) {
            list_queue_enqueue(queue, (long)node->left);
        }

        // Enqueue right child
        if (node->right != tree->nil) {
            list_queue_enqueue(queue, (long)node->right);
        }
    }

    if (!is_search) {
        printf("\n");
    }
    list_queue_destroy(queue);

    return found;
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================

// Get height of tree
int rb_height(RBTree* tree, RBNode* x) {
    if (x == tree->nil) {
        return 0;
    }

    int left_height = rb_height(tree, x->left);
    int right_height = rb_height(tree, x->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

// Count black nodes on path to leaf
int rb_black_height(RBTree* tree, RBNode* x) {
    if (x == tree->nil) {
        return 1;
    }

    int left_bh = rb_black_height(tree, x->left);
    return left_bh + (x->color == BLACK ? 1 : 0);
}

// Display tree helper function
void rb_display_tree_helper(RBTree* tree, RBNode* node, int space, RBNode* highlight) {
    if (node == tree->nil) {
        return;
    }

    space += 5;

    // Display right subtree first (appears at top)
    rb_display_tree_helper(tree, node->right, space, highlight);

    // Print current node with proper spacing
    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }

    // Highlight current node if specified
    if (node == highlight) {
        printf(">>> ");
    }

    printf("%d(%c)", node->data, node->color == RED ? 'R' : 'B');

    if (node == highlight) {
        printf(" <<<");
    }

    // Display left subtree (appears at bottom)
    rb_display_tree_helper(tree, node->left, space, highlight);
}

// Display tree structure
// highlight: pointer to node to highlight (or NULL for no highlight)
void rb_display_tree(RBTree* tree, RBNode* highlight, const char* message) {
    if (message != NULL) {
        printf("\n%s\n", message);
    }
    if (tree->root == tree->nil) {
        printf("(empty tree)\n");
        return;
    }
    rb_display_tree_helper(tree, tree->root, 0, highlight);
    printf("\n");
}

// Verify red-black tree properties
bool rb_verify_properties(RBTree* tree, RBNode* x, int black_count, int* path_black_count) {
    if (x == tree->nil) {
        if (*path_black_count == -1) {
            *path_black_count = black_count;
        }
        return black_count == *path_black_count;
    }

    // Property 4: Red node cannot have red children
    if (x->color == RED) {
        if (x->left->color == RED || x->right->color == RED) {
            printf("Red violation: Red node %d has red child\n", x->data);
            return false;
        }
    }

    int new_black_count = black_count + (x->color == BLACK ? 1 : 0);

    return rb_verify_properties(tree, x->left, new_black_count, path_black_count) &&
           rb_verify_properties(tree, x->right, new_black_count, path_black_count);
}

// Destroy tree (postorder deletion)
void rb_destroy_helper(RBTree* tree, RBNode* x) {
    if (x != tree->nil) {
        rb_destroy_helper(tree, x->left);
        rb_destroy_helper(tree, x->right);
        free(x);
    }
}

void rb_destroy(RBTree* tree) {
    if (tree) {
        rb_destroy_helper(tree, tree->root);
        free(tree->nil);
        free(tree);
    }
}

// ============================================================
// TEST FUNCTIONS
// ============================================================

void test_rb_tree_basic() {
    printf("\n=== Testing Red-Black Tree - Basic Operations ===\n");
    RBTree* tree = rb_create_tree();

    printf("\nInserting: 10, 20, 30, 15, 25, 5, 1\n");
    rb_insert_node(tree, 10);
    rb_insert_node(tree, 20);
    rb_insert_node(tree, 30);
    rb_insert_node(tree, 15);
    rb_insert_node(tree, 25);
    rb_insert_node(tree, 5);
    rb_insert_node(tree, 1);

    printf("\nInorder traversal (should be sorted):\n");
    rb_traverse_inorder(tree, tree->root);
    printf("\n");

    printf("\nPreorder traversal:\n");
    rb_traverse_preorder(tree, tree->root);
    printf("\n");

    printf("\nPostorder traversal:\n");
    rb_traverse_postorder(tree, tree->root);
    printf("\n");

    printf("\nTree height: %d\n", rb_height(tree, tree->root));
    printf("Black height: %d\n", rb_black_height(tree, tree->root));

    // Verify properties
    int path_black_count = -1;
    bool valid = rb_verify_properties(tree, tree->root, 0, &path_black_count);
    printf("Tree is %s\n", valid ? "VALID" : "INVALID");

    rb_destroy(tree);
}

void test_rb_tree_search() {
    printf("\n=== Testing Red-Black Tree - Search ===\n");
    RBTree* tree = rb_create_tree();

    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(values) / sizeof(values[0]);

    printf("\nInserting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", values[i]);
        rb_insert_node(tree, values[i]);
    }
    printf("\n");

    printf("\nSearching for values:\n");
    int search_values[] = {20, 40, 60, 100, 25};
    for (int i = 0; i < 5; i++) {
        RBNode* result = rb_search(tree, tree->root, search_values[i]);
        if (result != tree->nil) {
            printf("Found: %d (color: %s)\n", result->data,
                   result->color == RED ? "RED" : "BLACK");
        } else {
            printf("Not found: %d\n", search_values[i]);
        }
    }

    rb_destroy(tree);
}

void test_rb_tree_deletion() {
    printf("\n=== Testing Red-Black Tree - Deletion ===\n");
    RBTree* tree = rb_create_tree();

    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    int n = sizeof(values) / sizeof(values[0]);

    printf("\nInserting: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", values[i]);
        rb_insert_node(tree, values[i]);
    }
    printf("\n");

    printf("\nInorder before deletion:\n");
    rb_traverse_inorder(tree, tree->root);
    printf("\n");

    // Delete some values
    int delete_values[] = {20, 30, 50};
    for (int i = 0; i < 3; i++) {
        printf("\nDeleting: %d\n", delete_values[i]);
        RBNode* node = rb_search(tree, tree->root, delete_values[i]);
        if (node != tree->nil) {
            rb_delete(tree, node);
            printf("Inorder after deletion:\n");
            rb_traverse_inorder(tree, tree->root);
            printf("\n");

            // Verify properties
            int path_black_count = -1;
            bool valid = rb_verify_properties(tree, tree->root, 0, &path_black_count);
            printf("Tree is %s\n", valid ? "VALID" : "INVALID");
        }
    }

    rb_destroy(tree);
}

void test_rb_tree_dfs_bfs() {
    printf("\n=== Testing DFS and BFS Traversals ===\n");
    RBTree* tree = rb_create_tree();

    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45};
    int n = sizeof(values) / sizeof(values[0]);

    printf("\nBuilding tree with values: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", values[i]);
        rb_insert_node(tree, values[i]);
    }
    printf("\n");

    rb_display_tree(tree, NULL, "\nTree structure:");

    printf("\n=== Recursive Traversals (for comparison) ===\n");
    printf("Inorder (L-Root-R):   ");
    rb_traverse_inorder(tree, tree->root);
    printf("\n");

    printf("Preorder (Root-L-R):  ");
    rb_traverse_preorder(tree, tree->root);
    printf("\n");

    printf("Postorder (L-R-Root): ");
    rb_traverse_postorder(tree, tree->root);
    printf("\n");

    printf("\n=== Depth-First Search ===\n");
    printf("DFS Recursive:        ");
    rb_traverse_dfs_recursive(tree, tree->root, -1);  // -1 = traverse mode
    printf("\n");

    rb_traverse_dfs_iterative(tree, tree->root, -1);  // -1 = traverse mode

    printf("\n=== Breadth-First Search ===\n");
    rb_traverse_bfs(tree, tree->root, -1);  // -1 = traverse mode

    printf("\n=== Comparison ===\n");
    printf("Notice:\n");
    printf("- DFS Recursive = Preorder (Root -> Left -> Right)\n");
    printf("- DFS Iterative = Preorder (using stack instead of recursion)\n");
    printf("- BFS = Level-order (visits all nodes level by level)\n");
    printf("- Inorder = Sorted order (for BST)\n");

    printf("\n=== Search Tests ===\n");
    int search_vals[] = {35, 80, 99};  // Two exist, one doesn't
    for (int i = 0; i < 3; i++) {
        int val = search_vals[i];
        printf("\nSearching for %d:\n", val);

        RBNode* found = rb_traverse_dfs_iterative(tree, tree->root, val);
        printf("  DFS Iterative: %s\n", found ? "Found" : "Not found");

        found = rb_traverse_dfs_recursive(tree, tree->root, val);
        printf("  DFS Recursive: %s\n", found ? "Found" : "Not found");

        found = rb_traverse_bfs(tree, tree->root, val);
        printf("  BFS:           %s\n", found ? "Found" : "Not found");
    }

    rb_destroy(tree);
}

void test_rb_tree_visual() {
    printf("\n=== Visual Red-Black Tree - Step-by-Step Insertion ===\n");
    RBTree* tree = rb_create_tree();

    int values[] = {10, 20, 30, 15, 25, 5, 1};
    int n = sizeof(values) / sizeof(values[0]);

    printf("\nInserting values step-by-step: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", values[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("\n========================================\n");
        printf("Inserting: %d\n", values[i]);
        printf("========================================\n");

        rb_insert_node(tree, values[i]);

        // Find the inserted node to highlight it
        RBNode* inserted = rb_search(tree, tree->root, values[i]);

        rb_display_tree(tree, inserted, "Tree after insertion:");

        printf("\nPress Enter to continue...");
        getchar();
    }

    printf("\n\n=== Final Tree ===\n");
    rb_display_tree(tree, NULL, "Complete tree:");

    printf("\nTree properties:\n");
    printf("Height: %d\n", rb_height(tree, tree->root));
    printf("Black height: %d\n", rb_black_height(tree, tree->root));

    int path_black_count = -1;
    bool valid = rb_verify_properties(tree, tree->root, 0, &path_black_count);
    printf("Tree is %s\n", valid ? "VALID" : "INVALID");

    rb_destroy(tree);
}

void test_rb_tree_traversal_visual() {
    printf("\n=== Visual Tree Traversal ===\n");
    RBTree* tree = rb_create_tree();

    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(values) / sizeof(values[0]);

    printf("\nBuilding tree with values: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", values[i]);
        rb_insert_node(tree, values[i]);
    }
    printf("\n");

    rb_display_tree(tree, NULL, "\nComplete tree:");

    // Inorder traversal with highlighting
    printf("\n\n=== Inorder Traversal (Left -> Root -> Right) ===\n");
    printf("Expected order: 20 30 40 50 60 70 80 (sorted)\n");
    printf("\nPress Enter to start traversal...");
    getchar();

    // Manual inorder to highlight each step
    RBNode* nodes[7];
    int idx = 0;

    // Collect nodes in inorder
    void collect_inorder(RBTree* t, RBNode* x, RBNode** arr, int* index) {
        if (x != t->nil) {
            collect_inorder(t, x->left, arr, index);
            arr[(*index)++] = x;
            collect_inorder(t, x->right, arr, index);
        }
    }

    collect_inorder(tree, tree->root, nodes, &idx);

    for (int i = 0; i < idx; i++) {
        char msg[100];
        sprintf(msg, "Step %d: Visiting node %d", i + 1, nodes[i]->data);
        rb_display_tree(tree, nodes[i], msg);
        printf("Press Enter for next node...");
        getchar();
    }

    rb_destroy(tree);
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n=== Red-Black Trees Menu ===\n");
        printf("1. Test Basic Operations (Insert, Traversals)\n");
        printf("2. Test Search\n");
        printf("3. Test Deletion\n");
        printf("4. Test DFS and BFS Traversals\n");
        printf("5. Visual Step-by-Step Insertion\n");
        printf("6. Visual Tree Traversal\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_rb_tree_basic();
        } else if (choice == '2') {
            test_rb_tree_search();
        } else if (choice == '3') {
            test_rb_tree_deletion();
        } else if (choice == '4') {
            test_rb_tree_dfs_bfs();
        } else if (choice == '5') {
            test_rb_tree_visual();
        } else if (choice == '6') {
            test_rb_tree_traversal_visual();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
