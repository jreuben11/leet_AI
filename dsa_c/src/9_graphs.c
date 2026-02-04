/*
 * Graph Data Structure Implementation
 *
 * Supports:
 * - Directed and Undirected graphs
 * - Weighted and Unweighted graphs
 * - Both Adjacency Matrix and Adjacency List representations
 * - Special graph types: Complete, Sparse, DAG, Bipartite
 *
 * When to use which representation:
 * - Adjacency Matrix: Dense graphs (E ≈ V²), O(1) edge lookup, O(V²) space
 * - Adjacency List: Sparse graphs (E << V²), O(V+E) space, better for traversals
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <math.h>

// ============================================================
// ENUMS AND CONSTANTS
// ============================================================

typedef enum {
    DIRECTED,
    UNDIRECTED
} GraphType;

typedef enum {
    WEIGHTED,
    UNWEIGHTED
} WeightType;

typedef enum {
    ADJACENCY_MATRIX,
    ADJACENCY_LIST
} RepType;

#define INF INT_MAX
#define NO_EDGE 0

// ============================================================
// DATA STRUCTURES
// ============================================================

/**
 * Node in adjacency list
 */
typedef struct AdjListNode {
    int dest;                    // Destination vertex
    int weight;                  // Edge weight (1 if unweighted)
    struct AdjListNode* next;    // Next node in list
} AdjListNode;

/**
 * Graph structure supporting both representations
 */
typedef struct {
    GraphType type;              // DIRECTED or UNDIRECTED
    WeightType weight_type;      // WEIGHTED or UNWEIGHTED
    RepType representation;      // ADJACENCY_MATRIX or ADJACENCY_LIST

    int num_vertices;            // Number of vertices
    int num_edges;               // Number of edges

    // Adjacency Matrix (if representation == ADJACENCY_MATRIX)
    int** adj_matrix;            // 2D array: adj_matrix[i][j] = weight of edge i->j

    // Adjacency List (if representation == ADJACENCY_LIST)
    AdjListNode** adj_list;      // Array of linked lists
} Graph;

// ============================================================
// HELPER FUNCTIONS - ADJACENCY LIST
// ============================================================

/**
 * Create a new adjacency list node
 */
AdjListNode* create_adj_list_node(int dest, int weight) {
    AdjListNode* node = (AdjListNode*)malloc(sizeof(AdjListNode));
    node->dest = dest;
    node->weight = weight;
    node->next = NULL;
    return node;
}

/**
 * Add edge to adjacency list
 */
void add_to_adj_list(AdjListNode** head, int dest, int weight) {
    AdjListNode* node = create_adj_list_node(dest, weight);
    node->next = *head;
    *head = node;
}

/**
 * Check if edge exists in adjacency list
 */
bool has_edge_in_list(AdjListNode* head, int dest) {
    AdjListNode* current = head;
    while (current != NULL) {
        if (current->dest == dest) {
            return true;
        }
        current = current->next;
    }
    return false;
}

/**
 * Free adjacency list
 */
void free_adj_list(AdjListNode* head) {
    AdjListNode* current = head;
    while (current != NULL) {
        AdjListNode* temp = current;
        current = current->next;
        free(temp);
    }
}

// ============================================================
// GRAPH CREATION AND MANAGEMENT
// ============================================================

/**
 * Create a new graph
 *
 * @param num_vertices  Number of vertices in graph
 * @param type          DIRECTED or UNDIRECTED
 * @param weight_type   WEIGHTED or UNWEIGHTED
 * @param rep           ADJACENCY_MATRIX or ADJACENCY_LIST
 * @return              Pointer to newly created graph
 */
Graph* graph_create(int num_vertices, GraphType type, WeightType weight_type, RepType rep) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->num_edges = 0;
    graph->type = type;
    graph->weight_type = weight_type;
    graph->representation = rep;

    if (rep == ADJACENCY_MATRIX) {
        // Allocate 2D matrix
        graph->adj_matrix = (int**)malloc(num_vertices * sizeof(int*));
        for (int i = 0; i < num_vertices; i++) {
            graph->adj_matrix[i] = (int*)calloc(num_vertices, sizeof(int));
        }
        graph->adj_list = NULL;
    } else {
        // Allocate array of linked lists
        graph->adj_list = (AdjListNode**)calloc(num_vertices, sizeof(AdjListNode*));
        graph->adj_matrix = NULL;
    }

    return graph;
}

/**
 * Destroy graph and free memory
 */
void graph_destroy(Graph* graph) {
    if (graph->representation == ADJACENCY_MATRIX) {
        for (int i = 0; i < graph->num_vertices; i++) {
            free(graph->adj_matrix[i]);
        }
        free(graph->adj_matrix);
    } else {
        for (int i = 0; i < graph->num_vertices; i++) {
            free_adj_list(graph->adj_list[i]);
        }
        free(graph->adj_list);
    }
    free(graph);
}

// ============================================================
// EDGE OPERATIONS
// ============================================================

/**
 * Add edge to graph
 *
 * @param graph   Pointer to graph
 * @param src     Source vertex
 * @param dest    Destination vertex
 * @param weight  Edge weight (1 if unweighted)
 */
void graph_add_edge(Graph* graph, int src, int dest, int weight) {
    if (src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        printf("Invalid vertex indices\n");
        return;
    }

    // Default weight to 1 if unweighted
    if (graph->weight_type == UNWEIGHTED) {
        weight = 1;
    }

    if (graph->representation == ADJACENCY_MATRIX) {
        // Add to matrix
        if (graph->adj_matrix[src][dest] == NO_EDGE) {
            graph->adj_matrix[src][dest] = weight;
            graph->num_edges++;

            // If undirected, add reverse edge
            if (graph->type == UNDIRECTED && src != dest) {
                graph->adj_matrix[dest][src] = weight;
            }
        }
    } else {
        // Add to list
        if (!has_edge_in_list(graph->adj_list[src], dest)) {
            add_to_adj_list(&graph->adj_list[src], dest, weight);
            graph->num_edges++;

            // If undirected, add reverse edge
            if (graph->type == UNDIRECTED && src != dest) {
                add_to_adj_list(&graph->adj_list[dest], src, weight);
            }
        }
    }
}

/**
 * Check if edge exists
 */
bool graph_has_edge(Graph* graph, int src, int dest) {
    if (src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        return false;
    }

    if (graph->representation == ADJACENCY_MATRIX) {
        return graph->adj_matrix[src][dest] != NO_EDGE;
    } else {
        return has_edge_in_list(graph->adj_list[src], dest);
    }
}

// ============================================================
// DISPLAY FUNCTIONS
// ============================================================

/**
 * Display graph information
 */
void graph_display_info(Graph* graph) {
    printf("Graph Information:\n");
    printf("  Type: %s\n", graph->type == DIRECTED ? "Directed" : "Undirected");
    printf("  Weight: %s\n", graph->weight_type == WEIGHTED ? "Weighted" : "Unweighted");
    printf("  Representation: %s\n",
           graph->representation == ADJACENCY_MATRIX ? "Adjacency Matrix" : "Adjacency List");
    printf("  Vertices: %d\n", graph->num_vertices);
    printf("  Edges: %d\n", graph->num_edges);

    // Calculate density
    int max_edges = graph->type == DIRECTED ?
                    graph->num_vertices * (graph->num_vertices - 1) :
                    graph->num_vertices * (graph->num_vertices - 1) / 2;
    double density = max_edges > 0 ? (double)graph->num_edges / max_edges : 0.0;
    printf("  Density: %.2f%%\n", density * 100);
}

/**
 * Display adjacency matrix
 */
void graph_display_matrix(Graph* graph) {
    printf("\nAdjacency Matrix:\n");

    if (graph->representation != ADJACENCY_MATRIX) {
        printf("  (Graph uses adjacency list representation, building matrix view...)\n");
        // Build temporary matrix for display
        int** temp_matrix = (int**)malloc(graph->num_vertices * sizeof(int*));
        for (int i = 0; i < graph->num_vertices; i++) {
            temp_matrix[i] = (int*)calloc(graph->num_vertices, sizeof(int));
        }

        // Fill from adjacency list
        for (int i = 0; i < graph->num_vertices; i++) {
            AdjListNode* node = graph->adj_list[i];
            while (node != NULL) {
                temp_matrix[i][node->dest] = node->weight;
                node = node->next;
            }
        }

        // Print header
        printf("    ");
        for (int i = 0; i < graph->num_vertices; i++) {
            printf("%3d ", i);
        }
        printf("\n");

        // Print matrix
        for (int i = 0; i < graph->num_vertices; i++) {
            printf("%3d ", i);
            for (int j = 0; j < graph->num_vertices; j++) {
                if (temp_matrix[i][j] == NO_EDGE) {
                    printf("  . ");
                } else {
                    printf("%3d ", temp_matrix[i][j]);
                }
            }
            printf("\n");
        }

        // Free temporary matrix
        for (int i = 0; i < graph->num_vertices; i++) {
            free(temp_matrix[i]);
        }
        free(temp_matrix);
    } else {
        // Print header
        printf("    ");
        for (int i = 0; i < graph->num_vertices; i++) {
            printf("%3d ", i);
        }
        printf("\n");

        // Print matrix
        for (int i = 0; i < graph->num_vertices; i++) {
            printf("%3d ", i);
            for (int j = 0; j < graph->num_vertices; j++) {
                if (graph->adj_matrix[i][j] == NO_EDGE) {
                    printf("  . ");
                } else {
                    printf("%3d ", graph->adj_matrix[i][j]);
                }
            }
            printf("\n");
        }
    }
}

/**
 * Display adjacency list
 */
void graph_display_list(Graph* graph) {
    printf("\nAdjacency List:\n");

    if (graph->representation != ADJACENCY_LIST) {
        printf("  (Graph uses adjacency matrix representation, building list view...)\n");
        // Display from matrix
        for (int i = 0; i < graph->num_vertices; i++) {
            printf("  [%d]: ", i);
            bool first = true;
            for (int j = 0; j < graph->num_vertices; j++) {
                if (graph->adj_matrix[i][j] != NO_EDGE) {
                    if (!first) printf(" -> ");
                    if (graph->weight_type == WEIGHTED) {
                        printf("%d(w=%d)", j, graph->adj_matrix[i][j]);
                    } else {
                        printf("%d", j);
                    }
                    first = false;
                }
            }
            if (first) printf("(empty)");
            printf("\n");
        }
    } else {
        // Display from list
        for (int i = 0; i < graph->num_vertices; i++) {
            printf("  [%d]: ", i);
            AdjListNode* node = graph->adj_list[i];
            if (node == NULL) {
                printf("(empty)");
            } else {
                while (node != NULL) {
                    if (graph->weight_type == WEIGHTED) {
                        printf("%d(w=%d)", node->dest, node->weight);
                    } else {
                        printf("%d", node->dest);
                    }
                    if (node->next != NULL) printf(" -> ");
                    node = node->next;
                }
            }
            printf("\n");
        }
    }
}

/**
 * Helper: Get edge weight between two vertices
 */
int get_edge_weight(Graph* graph, int src, int dest) {
    if (graph->representation == ADJACENCY_MATRIX) {
        return graph->adj_matrix[src][dest];
    } else {
        AdjListNode* node = graph->adj_list[src];
        while (node != NULL) {
            if (node->dest == dest) {
                return node->weight;
            }
            node = node->next;
        }
    }
    return NO_EDGE;
}

/**
 * Check if graph is bipartite and get the two sets
 */
bool is_bipartite_with_sets(Graph* graph, int* set, int* set_sizes) {
    int* color = (int*)malloc(graph->num_vertices * sizeof(int));
    for (int i = 0; i < graph->num_vertices; i++) {
        color[i] = -1;
        set[i] = -1;
    }

    int* queue = (int*)malloc(graph->num_vertices * sizeof(int));
    set_sizes[0] = set_sizes[1] = 0;

    for (int start = 0; start < graph->num_vertices; start++) {
        if (color[start] != -1) continue;

        color[start] = 0;
        set[start] = 0;
        set_sizes[0]++;

        int front = 0, rear = 0;
        queue[rear++] = start;

        while (front < rear) {
            int u = queue[front++];

            if (graph->representation == ADJACENCY_LIST) {
                AdjListNode* node = graph->adj_list[u];
                while (node != NULL) {
                    int v = node->dest;
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];
                        set[v] = color[v];
                        set_sizes[color[v]]++;
                        queue[rear++] = v;
                    } else if (color[v] == color[u]) {
                        free(color);
                        free(queue);
                        return false;
                    }
                    node = node->next;
                }
            } else {
                for (int v = 0; v < graph->num_vertices; v++) {
                    if (graph->adj_matrix[u][v] != NO_EDGE) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            set[v] = color[v];
                            set_sizes[color[v]]++;
                            queue[rear++] = v;
                        } else if (color[v] == color[u]) {
                            free(color);
                            free(queue);
                            return false;
                        }
                    }
                }
            }
        }
    }

    free(color);
    free(queue);
    return true;
}

/**
 * Display graph using layout optimized to minimize crossings
 */
/**
 * Export graph to DOT format (Graphviz)
 */
void graph_export_dot(Graph* graph, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Error: Could not open %s for writing\n", filename);
        return;
    }

    // Write graph header
    if (graph->type == DIRECTED) {
        fprintf(fp, "digraph G {\n");
    } else {
        fprintf(fp, "graph G {\n");
    }

    // Graph attributes for better layout
    fprintf(fp, "  layout=neato;\n");
    fprintf(fp, "  overlap=false;\n");
    fprintf(fp, "  splines=true;\n");
    fprintf(fp, "  node [shape=circle, style=filled, fillcolor=lightblue];\n");
    fprintf(fp, "\n");

    // Write edges
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->representation == ADJACENCY_MATRIX) {
            for (int j = 0; j < graph->num_vertices; j++) {
                if (graph->adj_matrix[i][j] != NO_EDGE) {
                    // For undirected, only write each edge once
                    if (graph->type == UNDIRECTED && i > j) continue;

                    fprintf(fp, "  %d %s %d", i,
                            graph->type == DIRECTED ? "->" : "--", j);

                    // Add weight label if weighted
                    if (graph->weight_type == WEIGHTED) {
                        fprintf(fp, " [label=\"%d\"]", graph->adj_matrix[i][j]);
                    }
                    fprintf(fp, ";\n");
                }
            }
        } else {
            AdjListNode* node = graph->adj_list[i];
            while (node != NULL) {
                int j = node->dest;

                // For undirected, only write each edge once
                if (graph->type == UNDIRECTED && i > j) {
                    node = node->next;
                    continue;
                }

                fprintf(fp, "  %d %s %d", i,
                        graph->type == DIRECTED ? "->" : "--", j);

                // Add weight label if weighted
                if (graph->weight_type == WEIGHTED) {
                    fprintf(fp, " [label=\"%d\"]", node->weight);
                }
                fprintf(fp, ";\n");

                node = node->next;
            }
        }
    }

    fprintf(fp, "}\n");
    fclose(fp);

    printf("\nGraph exported to: %s\n", filename);
    printf("\nTo visualize with Graphviz:\n");
    printf("  neato -Tpng %s -o graph.png     # Force-directed layout\n", filename);
    printf("  dot -Tpng %s -o graph.png       # Hierarchical layout (DAGs)\n", filename);
    printf("  circo -Tpng %s -o graph.png     # Circular layout\n", filename);
    printf("  fdp -Tpng %s -o graph.png       # Force-directed planar\n", filename);
    printf("  sfdp -Tpng %s -o graph.png      # Scalable force-directed\n", filename);
    printf("\nThen open graph.png to view the proper planar layout\n");
}

/**
 * Display graph using Graphviz and terminal image viewer
 */
void graph_display_visual(Graph* graph) {
    char dot_file[256];
    char png_file[256];
    snprintf(dot_file, sizeof(dot_file), "out/graph_%dv_%de.dot",
             graph->num_vertices, graph->num_edges);
    snprintf(png_file, sizeof(png_file), "out/graph_%dv_%de.png",
             graph->num_vertices, graph->num_edges);

    printf("\n");
    graph_export_dot(graph, dot_file);

    // Check if graphviz is installed
    if (system("which dot > /dev/null 2>&1") != 0) {
        printf("\n  Graphviz not installed. Install with:\n");
        printf("    Ubuntu/Debian: sudo apt-get install graphviz\n");
        printf("    macOS: brew install graphviz\n");
        printf("    Fedora: sudo dnf install graphviz\n");
        return;
    }

    // Render with Graphviz
    printf("\nRendering graph...\n");
    char render_cmd[768];

    // Choose layout engine based on graph type
    const char* layout = "fdp";  // force-directed planar (default)
    if (graph->type == DIRECTED) {
        layout = "dot";  // hierarchical layout for directed graphs/DAGs
    }

    snprintf(render_cmd, sizeof(render_cmd),
             "%s -Tpng %s -o %s 2>/dev/null",
             layout, dot_file, png_file);

    if (system(render_cmd) != 0) {
        printf("✗ Render failed\n");
        return;
    }

    printf("✓ Graph rendered to: %s\n\n", png_file);

    // Display in terminal using chafa (best quality)
    if (system("which chafa > /dev/null 2>&1") == 0) {
        printf("Displaying graph in terminal:\n\n");
        char display_cmd[768];
        snprintf(display_cmd, sizeof(display_cmd),
                 "chafa --size 80x40 --symbols block+border %s",
                 png_file);
        system(display_cmd);
        printf("\n");
    }
    // Fallback to viu
    else if (system("which viu > /dev/null 2>&1") == 0) {
        printf("Displaying graph in terminal:\n\n");
        char display_cmd[768];
        snprintf(display_cmd, sizeof(display_cmd),
                 "viu -w 80 %s",
                 png_file);
        system(display_cmd);
        printf("\n");
    }
    else {
        printf("Terminal image viewer not found.\n");
        printf("Install chafa for best results:\n");
        printf("  Ubuntu/Debian: sudo apt-get install chafa\n");
        printf("  macOS: brew install chafa\n");
        printf("  Fedora: sudo dnf install chafa\n");
        printf("\nImage saved to: %s (open externally)\n", png_file);
    }
}

/**
 * Display complete graph visualization
 */
void graph_display(Graph* graph) {
    printf("\n");
    printf("========================================\n");
    graph_display_info(graph);
    graph_display_visual(graph);
    graph_display_matrix(graph);
    graph_display_list(graph);
    printf("========================================\n");
}

// ============================================================
// GRAPH BUILDERS - SPECIAL TYPES
// ============================================================

/**
 * Create a complete graph (every vertex connected to every other)
 * Uses ADJACENCY_MATRIX (dense graph)
 *
 * Time: O(V²)
 * Space: O(V²)
 */
Graph* graph_create_complete(int num_vertices, GraphType type, WeightType weight_type) {
    Graph* graph = graph_create(num_vertices, type, weight_type, ADJACENCY_MATRIX);

    printf("Building complete graph with %d vertices...\n", num_vertices);

    // Add all possible edges
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (i != j) {
                int weight = weight_type == WEIGHTED ? (i + j + 1) : 1;
                graph_add_edge(graph, i, j, weight);
            }
        }
    }

    // Adjust edge count for undirected (we counted each edge twice)
    if (type == UNDIRECTED) {
        graph->num_edges /= 2;
    }

    return graph;
}

/**
 * Create a sparse graph (few edges)
 * Uses ADJACENCY_LIST (sparse graph)
 *
 * Creates a graph with approximately 2*V edges (linear in vertices)
 */
Graph* graph_create_sparse(int num_vertices, GraphType type, WeightType weight_type, int num_edges) {
    Graph* graph = graph_create(num_vertices, type, weight_type, ADJACENCY_LIST);

    printf("Building sparse graph with %d vertices and ~%d edges...\n", num_vertices, num_edges);

    // Add edges to make it sparse but connected
    // First create a path to ensure connectivity
    for (int i = 0; i < num_vertices - 1; i++) {
        int weight = weight_type == WEIGHTED ? (i + 1) : 1;
        graph_add_edge(graph, i, i + 1, weight);
    }

    // Add random additional edges
    int edges_added = num_vertices - 1;
    while (edges_added < num_edges && edges_added < num_vertices * (num_vertices - 1) / 2) {
        int src = rand() % num_vertices;
        int dest = rand() % num_vertices;
        if (src != dest && !graph_has_edge(graph, src, dest)) {
            int weight = weight_type == WEIGHTED ? (rand() % 20 + 1) : 1;
            graph_add_edge(graph, src, dest, weight);
            edges_added++;
        }
    }

    return graph;
}

/**
 * Create a DAG (Directed Acyclic Graph)
 * Uses ADJACENCY_LIST (typically sparse)
 *
 * Ensures no cycles by only adding edges from lower to higher numbered vertices
 */
Graph* graph_create_dag(int num_vertices, int num_edges, WeightType weight_type) {
    Graph* graph = graph_create(num_vertices, DIRECTED, weight_type, ADJACENCY_LIST);

    printf("Building DAG with %d vertices and ~%d edges...\n", num_vertices, num_edges);

    int edges_added = 0;
    while (edges_added < num_edges) {
        int src = rand() % num_vertices;
        int dest = rand() % num_vertices;

        // Only add edge if src < dest (ensures acyclic)
        if (src < dest && !graph_has_edge(graph, src, dest)) {
            int weight = weight_type == WEIGHTED ? (rand() % 20 + 1) : 1;
            graph_add_edge(graph, src, dest, weight);
            edges_added++;
        }
    }

    return graph;
}

/**
 * Create a bipartite graph
 * Uses ADJACENCY_LIST (can be sparse)
 *
 * Vertices split into two sets: [0, split) and [split, num_vertices)
 * Edges only between sets, not within sets
 */
Graph* graph_create_bipartite(int num_vertices, GraphType type, WeightType weight_type, int num_edges) {
    Graph* graph = graph_create(num_vertices, type, weight_type, ADJACENCY_LIST);

    int split = num_vertices / 2;
    printf("Building bipartite graph: Set1=[0,%d) Set2=[%d,%d) with ~%d edges...\n",
           split, split, num_vertices, num_edges);

    int edges_added = 0;
    while (edges_added < num_edges) {
        // Pick one vertex from each set
        int src = rand() % split;                    // From first set
        int dest = split + (rand() % (num_vertices - split));  // From second set

        if (!graph_has_edge(graph, src, dest)) {
            int weight = weight_type == WEIGHTED ? (rand() % 20 + 1) : 1;
            graph_add_edge(graph, src, dest, weight);
            edges_added++;
        }
    }

    return graph;
}

// ============================================================
// GRAPH PROPERTIES - VERIFICATION
// ============================================================

/**
 * Check if graph is bipartite using BFS coloring
 *
 * Algorithm: Try to 2-color the graph
 * - If successful, graph is bipartite
 * - If we find adjacent vertices with same color, not bipartite
 *
 * Time: O(V + E)
 */
bool graph_is_bipartite(Graph* graph) {
    int* color = (int*)malloc(graph->num_vertices * sizeof(int));
    for (int i = 0; i < graph->num_vertices; i++) {
        color[i] = -1;  // -1 = uncolored, 0 = color 1, 1 = color 2
    }

    // BFS queue (simple array implementation)
    int* queue = (int*)malloc(graph->num_vertices * sizeof(int));
    int front = 0, rear = 0;

    // Check each component (graph might be disconnected)
    for (int start = 0; start < graph->num_vertices; start++) {
        if (color[start] != -1) continue;

        // Start BFS from this vertex
        color[start] = 0;
        queue[rear++] = start;

        while (front < rear) {
            int u = queue[front++];

            // Check all neighbors
            if (graph->representation == ADJACENCY_LIST) {
                AdjListNode* node = graph->adj_list[u];
                while (node != NULL) {
                    int v = node->dest;
                    if (color[v] == -1) {
                        color[v] = 1 - color[u];  // Opposite color
                        queue[rear++] = v;
                    } else if (color[v] == color[u]) {
                        // Adjacent vertices have same color - not bipartite!
                        free(color);
                        free(queue);
                        return false;
                    }
                    node = node->next;
                }
            } else {
                for (int v = 0; v < graph->num_vertices; v++) {
                    if (graph->adj_matrix[u][v] != NO_EDGE) {
                        if (color[v] == -1) {
                            color[v] = 1 - color[u];
                            queue[rear++] = v;
                        } else if (color[v] == color[u]) {
                            free(color);
                            free(queue);
                            return false;
                        }
                    }
                }
            }
        }
    }

    free(color);
    free(queue);
    return true;
}

/**
 * Check if directed graph is a DAG (has no cycles) using DFS
 *
 * Algorithm: Detect back edges during DFS
 * - White (0): unvisited
 * - Gray (1): currently being processed (in DFS stack)
 * - Black (2): finished processing
 * - If we find edge to gray vertex, we have a cycle (back edge)
 *
 * Time: O(V + E)
 */
bool has_cycle_helper(Graph* graph, int v, int* color) {
    color[v] = 1;  // Gray - currently processing

    if (graph->representation == ADJACENCY_LIST) {
        AdjListNode* node = graph->adj_list[v];
        while (node != NULL) {
            int u = node->dest;
            if (color[u] == 1) {
                // Back edge found - cycle exists
                return true;
            }
            if (color[u] == 0 && has_cycle_helper(graph, u, color)) {
                return true;
            }
            node = node->next;
        }
    } else {
        for (int u = 0; u < graph->num_vertices; u++) {
            if (graph->adj_matrix[v][u] != NO_EDGE) {
                if (color[u] == 1) {
                    return true;
                }
                if (color[u] == 0 && has_cycle_helper(graph, u, color)) {
                    return true;
                }
            }
        }
    }

    color[v] = 2;  // Black - finished
    return false;
}

bool graph_is_dag(Graph* graph) {
    if (graph->type == UNDIRECTED) {
        printf("DAG check only applies to directed graphs\n");
        return false;
    }

    int* color = (int*)calloc(graph->num_vertices, sizeof(int));

    // Check each component
    for (int i = 0; i < graph->num_vertices; i++) {
        if (color[i] == 0) {
            if (has_cycle_helper(graph, i, color)) {
                free(color);
                return false;
            }
        }
    }

    free(color);
    return true;
}

// ============================================================
// TEST FUNCTIONS
// ============================================================

void test_complete_graph() {
    printf("\n=== Test 1: Complete Graph ===\n");
    printf("Using ADJACENCY_MATRIX (dense graph)\n\n");

    Graph* graph = graph_create_complete(5, UNDIRECTED, UNWEIGHTED);
    graph_display(graph);

    printf("\nBipartite check: %s\n", graph_is_bipartite(graph) ? "YES" : "NO");
    printf("(Complete graphs with >2 vertices are not bipartite)\n");

    graph_destroy(graph);
}

void test_sparse_graph() {
    printf("\n=== Test 2: Sparse Graph ===\n");
    printf("Using ADJACENCY_LIST (sparse graph)\n\n");

    Graph* graph = graph_create_sparse(6, UNDIRECTED, WEIGHTED, 8);
    graph_display(graph);

    printf("\nBipartite check: %s\n", graph_is_bipartite(graph) ? "YES" : "NO");

    graph_destroy(graph);
}

void test_dag() {
    printf("\n=== Test 3: DAG (Directed Acyclic Graph) ===\n");
    printf("Using ADJACENCY_LIST (typically sparse)\n\n");

    Graph* graph = graph_create_dag(6, 8, WEIGHTED);
    graph_display(graph);

    printf("\nDAG check: %s\n", graph_is_dag(graph) ? "YES (no cycles)" : "NO (has cycles)");
    printf("(Should be YES - construction ensures acyclic)\n");

    graph_destroy(graph);
}

void test_bipartite_graph() {
    printf("\n=== Test 4: Bipartite Graph ===\n");
    printf("Using ADJACENCY_LIST\n\n");

    Graph* graph = graph_create_bipartite(8, UNDIRECTED, UNWEIGHTED, 10);
    graph_display(graph);

    printf("\nBipartite check: %s\n", graph_is_bipartite(graph) ? "YES" : "NO");
    printf("(Should be YES - construction ensures bipartite)\n");

    graph_destroy(graph);
}

void test_directed_weighted() {
    printf("\n=== Test 5: Directed Weighted Graph ===\n");
    printf("Using ADJACENCY_LIST\n\n");

    Graph* graph = graph_create(5, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    // Manually add edges to create interesting structure
    graph_add_edge(graph, 0, 1, 5);
    graph_add_edge(graph, 0, 2, 3);
    graph_add_edge(graph, 1, 2, 2);
    graph_add_edge(graph, 1, 3, 6);
    graph_add_edge(graph, 2, 3, 7);
    graph_add_edge(graph, 2, 4, 4);
    graph_add_edge(graph, 3, 4, 1);

    graph_display(graph);

    printf("\nDAG check: %s\n", graph_is_dag(graph) ? "YES" : "NO");

    graph_destroy(graph);
}

void test_comparison_matrix_vs_list() {
    printf("\n=== Test 6: Comparison - Matrix vs List ===\n");

    printf("\nSame graph, two representations:\n");

    // Create with matrix
    printf("\n--- Using ADJACENCY_MATRIX ---\n");
    Graph* g_matrix = graph_create(4, UNDIRECTED, UNWEIGHTED, ADJACENCY_MATRIX);
    graph_add_edge(g_matrix, 0, 1, 1);
    graph_add_edge(g_matrix, 0, 2, 1);
    graph_add_edge(g_matrix, 1, 3, 1);
    graph_add_edge(g_matrix, 2, 3, 1);
    graph_display(g_matrix);

    // Create with list
    printf("\n--- Using ADJACENCY_LIST ---\n");
    Graph* g_list = graph_create(4, UNDIRECTED, UNWEIGHTED, ADJACENCY_LIST);
    graph_add_edge(g_list, 0, 1, 1);
    graph_add_edge(g_list, 0, 2, 1);
    graph_add_edge(g_list, 1, 3, 1);
    graph_add_edge(g_list, 2, 3, 1);
    graph_display(g_list);

    printf("\nSpace comparison:\n");
    printf("  Matrix: O(V²) = O(16) = 16 integers\n");
    printf("  List:   O(V+E) = O(4+4) = 8 integers + pointer overhead\n");
    printf("  For this sparse graph (density=50%%), list is more efficient\n");

    graph_destroy(g_matrix);
    graph_destroy(g_list);
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n=== Graph Data Structure Menu ===\n");
        printf("1. Complete Graph (Matrix representation)\n");
        printf("2. Sparse Graph (List representation)\n");
        printf("3. DAG - Directed Acyclic Graph (List)\n");
        printf("4. Bipartite Graph (List)\n");
        printf("5. Directed Weighted Graph (List)\n");
        printf("6. Comparison: Matrix vs List\n");
        printf("x. Exit\n");
        printf("Enter choice: ");
        scanf(" %c", &choice);

        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        if (choice == 'x') {
            break;
        } else if (choice == '1') {
            test_complete_graph();
        } else if (choice == '2') {
            test_sparse_graph();
        } else if (choice == '3') {
            test_dag();
        } else if (choice == '4') {
            test_bipartite_graph();
        } else if (choice == '5') {
            test_directed_weighted();
        } else if (choice == '6') {
            test_comparison_matrix_vs_list();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
