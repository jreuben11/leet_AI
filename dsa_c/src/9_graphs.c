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
// SHORTEST PATH ALGORITHMS
// ============================================================

/**
 * BFS Shortest Path for Unweighted Graphs
 *
 * BFS guarantees shortest path in unweighted graphs because:
 * - It explores level by level (layer by layer)
 * - First time we reach destination is via shortest path
 * - All edges have equal weight (1)
 *
 * When to use:
 * - Unweighted graphs (all edges weight = 1)
 * - Need guaranteed shortest path
 * - Both directed and undirected graphs
 *
 * Time: O(V + E)
 * Space: O(V) for queue and arrays
 *
 * @param graph  Pointer to graph
 * @param src    Source vertex
 * @param dest   Destination vertex
 */
void graph_bfs_shortest_path(Graph* graph, int src, int dest) {
    printf("\n=== BFS Shortest Path (Unweighted Graphs) ===\n");
    printf("From vertex %d to vertex %d\n\n", src, dest);

    if (src < 0 || src >= graph->num_vertices || dest < 0 || dest >= graph->num_vertices) {
        printf("Invalid source or destination\n");
        return;
    }

    // Arrays for BFS
    bool* visited = (bool*)calloc(graph->num_vertices, sizeof(bool));
    int* distance = (int*)malloc(graph->num_vertices * sizeof(int));
    int* parent = (int*)malloc(graph->num_vertices * sizeof(int));
    int* queue = (int*)malloc(graph->num_vertices * sizeof(int));

    // Initialize
    for (int i = 0; i < graph->num_vertices; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }

    // BFS initialization
    distance[src] = 0;
    visited[src] = true;
    int front = 0, rear = 0;
    queue[rear++] = src;

    // BFS traversal
    while (front < rear) {
        int u = queue[front++];

        // Early exit if we reached destination
        if (u == dest) break;

        // Explore neighbors
        if (graph->representation == ADJACENCY_LIST) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                int v = node->dest;
                if (!visited[v]) {
                    visited[v] = true;
                    distance[v] = distance[u] + 1;  // All edges weight 1
                    parent[v] = u;
                    queue[rear++] = v;
                }
                node = node->next;
            }
        } else {
            for (int v = 0; v < graph->num_vertices; v++) {
                if (graph->adj_matrix[u][v] != NO_EDGE && !visited[v]) {
                    visited[v] = true;
                    distance[v] = distance[u] + 1;
                    parent[v] = u;
                    queue[rear++] = v;
                }
            }
        }
    }

    // Print result
    if (distance[dest] == INF) {
        printf("No path found\n");
    } else {
        printf("Shortest path found!\n");
        printf("Distance: %d edges\n\n", distance[dest]);

        // Reconstruct path
        int* path = (int*)malloc(graph->num_vertices * sizeof(int));
        int path_len = 0;
        int current = dest;
        while (current != -1) {
            path[path_len++] = current;
            current = parent[current];
        }

        // Print path (reverse order)
        printf("Path: ");
        for (int i = path_len - 1; i >= 0; i--) {
            printf("%d", path[i]);
            if (i > 0) printf(" -> ");
        }
        printf("\n");

        free(path);
    }

    free(visited);
    free(distance);
    free(parent);
    free(queue);
}

/**
 * Dijkstra's Shortest Path Algorithm
 *
 * Dijkstra finds shortest path in weighted graphs with NON-NEGATIVE edges.
 * Uses greedy approach: always expand the closest unvisited vertex.
 *
 * Algorithm:
 * 1. Initialize distances: source = 0, all others = infinity
 * 2. Repeat until all vertices processed:
 *    a. Pick unvisited vertex with minimum distance
 *    b. For each neighbor, try to relax (improve) its distance
 *    c. Mark vertex as visited
 *
 * When to use:
 * - Weighted graphs with NON-NEGATIVE edge weights
 * - Need single-source shortest paths
 * - Works on both directed and undirected graphs
 *
 * Why not for negative edges?
 * - Greedy choice assumes we won't find shorter path later
 * - Negative edges can invalidate this assumption
 * - Use Bellman-Ford for negative edges
 *
 * Time: O(V²) with array, O((V+E) log V) with min-heap
 * Space: O(V)
 *
 * NOTE: This implementation uses simple array (O(V²)).
 *       For better performance on sparse graphs, use priority queue.
 *
 * @param graph  Pointer to graph
 * @param src    Source vertex
 * @param dest   Destination vertex (or -1 for all paths)
 */
void graph_dijkstra(Graph* graph, int src, int dest) {
    printf("\n=== Dijkstra's Algorithm (Non-negative Weighted Graphs) ===\n");
    if (dest >= 0) {
        printf("From vertex %d to vertex %d\n\n", src, dest);
    } else {
        printf("From vertex %d to all vertices\n\n", src);
    }

    if (src < 0 || src >= graph->num_vertices ||
        (dest >= 0 && dest >= graph->num_vertices)) {
        printf("Invalid source or destination\n");
        return;
    }

    // Arrays for Dijkstra
    int* distance = (int*)malloc(graph->num_vertices * sizeof(int));
    int* parent = (int*)malloc(graph->num_vertices * sizeof(int));
    bool* visited = (bool*)calloc(graph->num_vertices, sizeof(bool));

    // Initialize: all distances = infinity, source = 0
    for (int i = 0; i < graph->num_vertices; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[src] = 0;

    // Main Dijkstra loop: process each vertex
    for (int count = 0; count < graph->num_vertices; count++) {
        // Find unvisited vertex with minimum distance
        int min_dist = INF;
        int u = -1;
        for (int i = 0; i < graph->num_vertices; i++) {
            if (!visited[i] && distance[i] < min_dist) {
                min_dist = distance[i];
                u = i;
            }
        }

        // If no reachable unvisited vertex, done
        if (u == -1 || min_dist == INF) break;

        // Mark as visited
        visited[u] = true;

        // Early exit if we reached destination
        if (dest >= 0 && u == dest) break;

        // Relax edges: try to improve distance to neighbors
        if (graph->representation == ADJACENCY_LIST) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                int v = node->dest;
                int weight = node->weight;

                // Relaxation: if path through u is shorter, update
                if (!visited[v] && distance[u] != INF &&
                    distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                }
                node = node->next;
            }
        } else {
            for (int v = 0; v < graph->num_vertices; v++) {
                int weight = graph->adj_matrix[u][v];
                if (weight != NO_EDGE && !visited[v] && distance[u] != INF &&
                    distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                }
            }
        }
    }

    // Print results
    if (dest >= 0) {
        // Single destination
        if (distance[dest] == INF) {
            printf("No path found\n");
        } else {
            printf("Shortest path found!\n");
            printf("Total weight: %d\n\n", distance[dest]);

            // Reconstruct path
            int* path = (int*)malloc(graph->num_vertices * sizeof(int));
            int path_len = 0;
            int current = dest;
            while (current != -1) {
                path[path_len++] = current;
                current = parent[current];
            }

            // Print path with weights
            printf("Path: ");
            int total_weight = 0;
            for (int i = path_len - 1; i >= 0; i--) {
                printf("%d", path[i]);
                if (i > 0) {
                    int u = path[i];
                    int v = path[i-1];
                    int weight = get_edge_weight(graph, u, v);
                    printf(" -(%d)-> ", weight);
                    total_weight += weight;
                }
            }
            printf("\n");
            printf("Verification: Total weight = %d\n", total_weight);

            free(path);
        }
    } else {
        // All destinations
        printf("Shortest paths from vertex %d:\n\n", src);
        printf("Dest | Distance | Path\n");
        printf("-----|----------|---------------------\n");

        for (int i = 0; i < graph->num_vertices; i++) {
            if (i == src) continue;

            printf(" %2d  | ", i);

            if (distance[i] == INF) {
                printf("   INF   | No path\n");
            } else {
                printf("%6d   | ", distance[i]);

                // Reconstruct path
                int* path = (int*)malloc(graph->num_vertices * sizeof(int));
                int path_len = 0;
                int current = i;
                while (current != -1) {
                    path[path_len++] = current;
                    current = parent[current];
                }

                // Print path
                for (int j = path_len - 1; j >= 0; j--) {
                    printf("%d", path[j]);
                    if (j > 0) printf("->");
                }
                printf("\n");

                free(path);
            }
        }
    }

    free(distance);
    free(parent);
    free(visited);
}

/**
 * Bellman-Ford Shortest Path Algorithm
 *
 * Bellman-Ford finds shortest path even with NEGATIVE edge weights.
 * Unlike Dijkstra, it can handle negative weights and detect negative cycles.
 *
 * Algorithm:
 * 1. Initialize distances: source = 0, all others = infinity
 * 2. Relax ALL edges (V-1) times:
 *    - For each edge (u,v): if dist[u] + weight < dist[v], update dist[v]
 *    - Why V-1 times? Shortest path has at most V-1 edges
 * 3. Check for negative cycles:
 *    - If we can still relax any edge, negative cycle exists
 *
 * When to use:
 * - Weighted graphs that MAY have NEGATIVE edge weights
 * - Need to detect negative cycles
 * - Works only on directed graphs (for undirected, negative edge = negative cycle)
 *
 * Why slower than Dijkstra?
 * - Dijkstra: O((V+E) log V) with heap, processes each vertex once
 * - Bellman-Ford: O(V*E), must relax all edges V-1 times
 * - Trade-off: Bellman-Ford handles negative weights
 *
 * Negative cycle:
 * - Cycle whose total weight is negative
 * - No shortest path exists (can keep going around cycle)
 * - Bellman-Ford can detect this
 *
 * Time: O(V * E)
 * Space: O(V)
 *
 * @param graph  Pointer to graph (should be DIRECTED)
 * @param src    Source vertex
 * @param dest   Destination vertex (or -1 for all paths)
 */
void graph_bellman_ford(Graph* graph, int src, int dest) {
    printf("\n=== Bellman-Ford Algorithm (Handles Negative Weights) ===\n");
    if (dest >= 0) {
        printf("From vertex %d to vertex %d\n\n", src, dest);
    } else {
        printf("From vertex %d to all vertices\n\n", src);
    }

    if (graph->type == UNDIRECTED) {
        printf("WARNING: Bellman-Ford typically used on DIRECTED graphs.\n");
        printf("         For undirected graphs, negative edge = negative cycle!\n\n");
    }

    if (src < 0 || src >= graph->num_vertices ||
        (dest >= 0 && dest >= graph->num_vertices)) {
        printf("Invalid source or destination\n");
        return;
    }

    // Arrays for Bellman-Ford
    int* distance = (int*)malloc(graph->num_vertices * sizeof(int));
    int* parent = (int*)malloc(graph->num_vertices * sizeof(int));

    // Initialize: all distances = infinity, source = 0
    for (int i = 0; i < graph->num_vertices; i++) {
        distance[i] = INF;
        parent[i] = -1;
    }
    distance[src] = 0;

    // Main Bellman-Ford: Relax all edges (V-1) times
    // Why V-1? Shortest path has at most V-1 edges
    for (int iter = 0; iter < graph->num_vertices - 1; iter++) {
        bool updated = false;

        // Try to relax every edge in the graph
        for (int u = 0; u < graph->num_vertices; u++) {
            if (distance[u] == INF) continue;  // Skip unreachable vertices

            if (graph->representation == ADJACENCY_LIST) {
                AdjListNode* node = graph->adj_list[u];
                while (node != NULL) {
                    int v = node->dest;
                    int weight = node->weight;

                    // Relaxation: if path through u is shorter, update
                    if (distance[u] + weight < distance[v]) {
                        distance[v] = distance[u] + weight;
                        parent[v] = u;
                        updated = true;
                    }
                    node = node->next;
                }
            } else {
                for (int v = 0; v < graph->num_vertices; v++) {
                    int weight = graph->adj_matrix[u][v];
                    if (weight != NO_EDGE && distance[u] + weight < distance[v]) {
                        distance[v] = distance[u] + weight;
                        parent[v] = u;
                        updated = true;
                    }
                }
            }
        }

        // Optimization: if no updates in this iteration, done early
        if (!updated) {
            printf("Converged after %d iterations (early exit)\n\n", iter + 1);
            break;
        }
    }

    // Negative cycle detection
    // If we can still relax edges, negative cycle exists
    bool has_negative_cycle = false;
    for (int u = 0; u < graph->num_vertices; u++) {
        if (distance[u] == INF) continue;

        if (graph->representation == ADJACENCY_LIST) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                int v = node->dest;
                int weight = node->weight;
                if (distance[u] + weight < distance[v]) {
                    has_negative_cycle = true;
                    break;
                }
                node = node->next;
            }
        } else {
            for (int v = 0; v < graph->num_vertices; v++) {
                int weight = graph->adj_matrix[u][v];
                if (weight != NO_EDGE && distance[u] + weight < distance[v]) {
                    has_negative_cycle = true;
                    break;
                }
            }
        }
        if (has_negative_cycle) break;
    }

    if (has_negative_cycle) {
        printf("❌ NEGATIVE CYCLE DETECTED!\n");
        printf("   No shortest path exists (can keep decreasing distance)\n");
        free(distance);
        free(parent);
        return;
    }

    // Print results
    if (dest >= 0) {
        // Single destination
        if (distance[dest] == INF) {
            printf("No path found\n");
        } else {
            printf("Shortest path found!\n");
            printf("Total weight: %d\n\n", distance[dest]);

            // Reconstruct path
            int* path = (int*)malloc(graph->num_vertices * sizeof(int));
            int path_len = 0;
            int current = dest;
            while (current != -1) {
                path[path_len++] = current;
                current = parent[current];
            }

            // Print path with weights
            printf("Path: ");
            int total_weight = 0;
            for (int i = path_len - 1; i >= 0; i--) {
                printf("%d", path[i]);
                if (i > 0) {
                    int u = path[i];
                    int v = path[i-1];
                    int weight = get_edge_weight(graph, u, v);
                    printf(" -(%d)-> ", weight);
                    total_weight += weight;
                }
            }
            printf("\n");
            printf("Verification: Total weight = %d\n", total_weight);

            free(path);
        }
    } else {
        // All destinations
        printf("Shortest paths from vertex %d:\n\n", src);
        printf("Dest | Distance | Path\n");
        printf("-----|----------|---------------------\n");

        for (int i = 0; i < graph->num_vertices; i++) {
            if (i == src) continue;

            printf(" %2d  | ", i);

            if (distance[i] == INF) {
                printf("   INF   | No path\n");
            } else {
                printf("%6d   | ", distance[i]);

                // Reconstruct path
                int* path = (int*)malloc(graph->num_vertices * sizeof(int));
                int path_len = 0;
                int current = i;
                while (current != -1) {
                    path[path_len++] = current;
                    current = parent[current];
                }

                // Print path
                for (int j = path_len - 1; j >= 0; j--) {
                    printf("%d", path[j]);
                    if (j > 0) printf("->");
                }
                printf("\n");

                free(path);
            }
        }
    }

    free(distance);
    free(parent);
}

// ============================================================
// TOPOLOGICAL SORT - Kahn's Algorithm
// ============================================================

/**
 * Topological Sort using Kahn's Algorithm (BFS-based)
 *
 * What is Topological Sort?
 * --------------------------
 * A linear ordering of vertices in a Directed Acyclic Graph (DAG) such that:
 * For every directed edge u → v, vertex u comes before v in the ordering.
 *
 * Think of it as: "Ordering tasks so dependencies come first"
 *
 * Real-world applications:
 * - Course prerequisites (take Data Structures before Algorithms)
 * - Build systems (compile dependencies before main program)
 * - Task scheduling (finish prerequisite tasks first)
 * - Package managers (install dependencies before package)
 *
 * Important: Only works on DAGs (Directed Acyclic Graphs)
 * If graph has cycle → no valid topological ordering exists!
 *
 * Example:
 *   Graph:  0 → 1 → 3
 *           ↓       ↑
 *           2 ------┘
 *
 *   Valid orderings: [0, 1, 2, 3] or [0, 2, 1, 3]
 *   Invalid: [1, 0, 2, 3] (1 before 0, but 0→1 edge exists)
 *
 * Kahn's Algorithm Approach (BFS-based):
 * --------------------------------------
 * 1. Calculate in-degree (number of incoming edges) for each vertex
 * 2. Add all vertices with in-degree 0 to queue (no dependencies)
 * 3. While queue not empty:
 *    a. Remove vertex from queue, add to result
 *    b. For each neighbor, decrease its in-degree
 *    c. If neighbor's in-degree becomes 0, add to queue
 * 4. If result has all vertices → success, else cycle exists
 *
 * Why it works:
 * - Vertices with in-degree 0 have no dependencies → safe to process
 * - Removing a vertex = satisfying its dependency for neighbors
 * - If we can't process all vertices → cycle blocks progress
 *
 * Time Complexity: O(V + E)
 * - Calculate in-degrees: O(V + E)
 * - Process each vertex once: O(V)
 * - Process each edge once: O(E)
 *
 * Space Complexity: O(V)
 * - In-degree array: O(V)
 * - Queue: O(V) worst case
 * - Result array: O(V)
 *
 * Alternative: DFS-based topological sort (post-order reversal)
 *
 * @param graph  Pointer to directed graph (must be DAG)
 * @return       Array of vertices in topological order, or NULL if cycle detected
 */
int* graph_topological_sort_kahn(Graph* graph, int* result_size) {
    if (graph->type == UNDIRECTED) {
        printf("Error: Topological sort requires DIRECTED graph\n");
        return NULL;
    }

    printf("\n=== Kahn's Algorithm - Topological Sort ===\n\n");

    int V = graph->num_vertices;

    // Step 1: Calculate in-degree for each vertex
    printf("Step 1: Calculate in-degrees\n");
    int* in_degree = (int*)calloc(V, sizeof(int));

    if (graph->representation == ADJACENCY_LIST) {
        for (int u = 0; u < V; u++) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                in_degree[node->dest]++;
                node = node->next;
            }
        }
    } else {
        for (int u = 0; u < V; u++) {
            for (int v = 0; v < V; v++) {
                if (graph->adj_matrix[u][v] != NO_EDGE) {
                    in_degree[v]++;
                }
            }
        }
    }

    printf("In-degrees: ");
    for (int i = 0; i < V; i++) {
        printf("v%d=%d ", i, in_degree[i]);
    }
    printf("\n\n");

    // Step 2: Initialize queue with vertices having in-degree 0
    printf("Step 2: Find vertices with no dependencies (in-degree = 0)\n");
    int* queue = (int*)malloc(V * sizeof(int));
    int front = 0, rear = 0;

    for (int i = 0; i < V; i++) {
        if (in_degree[i] == 0) {
            queue[rear++] = i;
            printf("        Added vertex %d (no dependencies)\n", i);
        }
    }
    printf("\n");

    // Step 3: Process vertices in topological order
    printf("Step 3: Process vertices and reduce neighbors' in-degrees\n\n");
    int* result = (int*)malloc(V * sizeof(int));
    int count = 0;
    int step = 1;

    while (front < rear) {
        // Remove vertex with in-degree 0
        int u = queue[front++];
        result[count++] = u;

        printf("Step %d: Process vertex %d\n", step++, u);
        printf("        Current order: [");
        for (int i = 0; i < count; i++) {
            printf("%d", result[i]);
            if (i < count - 1) printf(", ");
        }
        printf("]\n");

        // Reduce in-degree of neighbors
        if (graph->representation == ADJACENCY_LIST) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                int v = node->dest;
                in_degree[v]--;
                printf("        Neighbor %d: in-degree %d → %d\n",
                       v, in_degree[v] + 1, in_degree[v]);

                // If in-degree becomes 0, add to queue
                if (in_degree[v] == 0) {
                    queue[rear++] = v;
                    printf("        → Vertex %d ready (in-degree = 0)\n", v);
                }
                node = node->next;
            }
        } else {
            for (int v = 0; v < V; v++) {
                if (graph->adj_matrix[u][v] != NO_EDGE) {
                    in_degree[v]--;
                    printf("        Neighbor %d: in-degree %d → %d\n",
                           v, in_degree[v] + 1, in_degree[v]);

                    if (in_degree[v] == 0) {
                        queue[rear++] = v;
                        printf("        → Vertex %d ready (in-degree = 0)\n", v);
                    }
                }
            }
        }
        printf("\n");
    }

    // Step 4: Check if all vertices were processed
    if (count != V) {
        printf("❌ CYCLE DETECTED!\n");
        printf("   Only processed %d/%d vertices\n", count, V);
        printf("   Remaining vertices are part of a cycle\n");
        printf("   → No valid topological ordering exists\n");

        free(in_degree);
        free(queue);
        free(result);
        *result_size = 0;
        return NULL;
    }

    printf("✓ Topological Sort Complete!\n");
    printf("  Valid ordering: [");
    for (int i = 0; i < count; i++) {
        printf("%d", result[i]);
        if (i < count - 1) printf(", ");
    }
    printf("]\n");

    free(in_degree);
    free(queue);
    *result_size = count;
    return result;
}

// ============================================================
// ALL-PAIRS SHORTEST PATH - Floyd-Warshall Algorithm
// ============================================================

/**
 * Floyd-Warshall Algorithm - All-Pairs Shortest Paths
 *
 * What is Floyd-Warshall?
 * -----------------------
 * Finds shortest paths between ALL pairs of vertices in a weighted graph.
 * Unlike Dijkstra (single-source) or BFS (single-source, unweighted),
 * Floyd-Warshall computes distances from every vertex to every other vertex.
 *
 * Key Features:
 * - Works with negative edge weights (unlike Dijkstra)
 * - Detects negative cycles
 * - Simple to implement (3 nested loops)
 * - Returns complete distance matrix
 *
 * When to use:
 * ✅ Need distances between all pairs
 * ✅ Dense graphs (many edges)
 * ✅ Negative weights allowed
 * ✅ Graph fits in memory (O(V²) space)
 *
 * When NOT to use:
 * ❌ Only need single-source paths (use Dijkstra/Bellman-Ford)
 * ❌ Sparse graphs with single-source query (Dijkstra is faster)
 * ❌ Very large graphs (O(V³) time, O(V²) space)
 *
 * Algorithm Intuition:
 * --------------------
 * Dynamic Programming approach with intermediate vertices:
 *
 * dist[i][j][k] = shortest path from i to j using vertices {0,1,...,k}
 *
 * Recurrence:
 * dist[i][j][k] = min(
 *     dist[i][j][k-1],           // Don't use vertex k
 *     dist[i][k][k-1] + dist[k][j][k-1]  // Use vertex k as intermediate
 * )
 *
 * We can optimize space by using 2D array (update in-place):
 * dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])
 *
 * Example:
 *   Graph:  0 --5--> 1
 *           |        |
 *           2        3
 *           |        |
 *           v        v
 *           2 <-4--- 3
 *
 *   Initially: direct edges only
 *   After k=0: paths through vertex 0
 *   After k=1: paths through vertices 0,1
 *   After k=2: paths through vertices 0,1,2
 *   Final: shortest paths using all vertices
 *
 * Algorithm Steps:
 * ----------------
 * 1. Initialize distance matrix:
 *    - dist[i][i] = 0 (distance to self)
 *    - dist[i][j] = weight(i,j) if edge exists
 *    - dist[i][j] = ∞ otherwise
 *
 * 2. For each intermediate vertex k:
 *    For each pair (i, j):
 *       Try path i → k → j
 *       If shorter than current dist[i][j], update
 *
 * 3. Check diagonal for negative cycles:
 *    If dist[i][i] < 0 for any i → negative cycle exists
 *
 * Time Complexity: O(V³)
 * - Three nested loops over all vertices
 * - Simple but can be slow for large graphs
 *
 * Space Complexity: O(V²)
 * - Distance matrix: V × V
 * - Next matrix for path reconstruction: V × V (optional)
 *
 * Comparison:
 * - Dijkstra (all pairs): O(V × (V+E) log V) ≈ O(V³ log V) for dense graphs
 * - Floyd-Warshall: O(V³) but simpler, handles negative weights
 *
 * @param graph  Pointer to weighted graph
 * @return       Distance matrix (V×V), or NULL on error
 */
int** graph_floyd_warshall(Graph* graph) {
    printf("\n=== Floyd-Warshall Algorithm - All-Pairs Shortest Paths ===\n\n");

    int V = graph->num_vertices;

    // Step 1: Initialize distance matrix
    printf("Step 1: Initialize distance matrix\n");
    int** dist = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        dist[i] = (int*)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;  // Distance to self = 0
            } else if (graph->representation == ADJACENCY_MATRIX) {
                dist[i][j] = graph->adj_matrix[i][j];
            } else {
                // For adjacency list, check if edge exists
                dist[i][j] = INF;
                AdjListNode* node = graph->adj_list[i];
                while (node != NULL) {
                    if (node->dest == j) {
                        dist[i][j] = node->weight;
                        break;
                    }
                    node = node->next;
                }
            }
        }
    }

    printf("Initial distance matrix (direct edges only):\n");
    printf("     ");
    for (int j = 0; j < V; j++) {
        printf("%5d ", j);
    }
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("%2d:  ", i);
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                printf("  INF ");
            } else {
                printf("%5d ", dist[i][j]);
            }
        }
        printf("\n");
    }
    printf("\n");

    // Step 2: Dynamic Programming - consider each vertex as intermediate
    printf("Step 2: Try all intermediate vertices\n\n");

    for (int k = 0; k < V; k++) {
        printf("Iteration k=%d: Consider paths through vertex %d\n", k, k);

        int updates = 0;
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                // Can we improve path i→j by going through k?
                if (dist[i][k] != INF && dist[k][j] != INF) {
                    int new_dist = dist[i][k] + dist[k][j];
                    if (new_dist < dist[i][j]) {
                        printf("  Update dist[%d][%d]: %d → %d (via %d)\n",
                               i, j, dist[i][j], new_dist, k);
                        dist[i][j] = new_dist;
                        updates++;
                    }
                }
            }
        }

        if (updates == 0) {
            printf("  (no improvements)\n");
        }
        printf("\n");
    }

    // Step 3: Check for negative cycles
    printf("Step 3: Check for negative cycles\n");
    bool has_negative_cycle = false;
    for (int i = 0; i < V; i++) {
        if (dist[i][i] < 0) {
            printf("❌ Negative cycle detected! (dist[%d][%d] = %d)\n",
                   i, i, dist[i][i]);
            has_negative_cycle = true;
        }
    }

    if (!has_negative_cycle) {
        printf("✓ No negative cycles\n");
    }
    printf("\n");

    // Display final distance matrix
    printf("Final All-Pairs Shortest Paths:\n");
    printf("     ");
    for (int j = 0; j < V; j++) {
        printf("%5d ", j);
    }
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("%2d:  ", i);
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) {
                printf("  INF ");
            } else {
                printf("%5d ", dist[i][j]);
            }
        }
        printf("\n");
    }

    if (has_negative_cycle) {
        printf("\n⚠ Warning: Results invalid due to negative cycle\n");
    }

    return dist;
}

/**
 * Free Floyd-Warshall distance matrix
 */
void floyd_warshall_free(int** dist, int V) {
    for (int i = 0; i < V; i++) {
        free(dist[i]);
    }
    free(dist);
}

// ============================================================
// MINIMUM SPANNING TREE (MST) ALGORITHMS
// ============================================================

// ------------------------------------------------------------
// UNION-FIND (Disjoint Set) Data Structure
// Required for Kruskal's algorithm
// ------------------------------------------------------------

/**
 * Union-Find (Disjoint Set) Data Structure
 *
 * Used to track connected components efficiently.
 * Essential for Kruskal's algorithm to detect cycles.
 *
 * Operations:
 * - find(x): Which set does x belong to? (with path compression)
 * - union(x, y): Merge sets containing x and y (with union by rank)
 *
 * Optimizations:
 * - Path compression: Make tree flat during find
 * - Union by rank: Attach smaller tree under larger tree
 *
 * Time complexity: O(α(n)) per operation, where α is inverse Ackermann
 * (effectively constant for all practical purposes)
 */
typedef struct {
    int* parent;  // parent[i] = parent of vertex i
    int* rank;    // rank[i] = approximate height of tree rooted at i
    int size;     // number of elements
} UnionFind;

/**
 * Create Union-Find structure
 * Initially, each element is in its own set
 */
UnionFind* uf_create(int n) {
    UnionFind* uf = (UnionFind*)malloc(sizeof(UnionFind));
    uf->size = n;
    uf->parent = (int*)malloc(n * sizeof(int));
    uf->rank = (int*)calloc(n, sizeof(int));

    // Initially, each vertex is its own parent (separate component)
    for (int i = 0; i < n; i++) {
        uf->parent[i] = i;
    }

    return uf;
}

/**
 * Find the root (representative) of the set containing x
 * Uses path compression: makes tree flat for faster future queries
 */
int uf_find(UnionFind* uf, int x) {
    if (uf->parent[x] != x) {
        // Path compression: point directly to root
        uf->parent[x] = uf_find(uf, uf->parent[x]);
    }
    return uf->parent[x];
}

/**
 * Merge (union) the sets containing x and y
 * Uses union by rank: attach smaller tree under larger tree
 * Returns true if sets were different (union performed)
 * Returns false if already in same set (would create cycle)
 */
bool uf_union(UnionFind* uf, int x, int y) {
    int root_x = uf_find(uf, x);
    int root_y = uf_find(uf, y);

    // Already in same set
    if (root_x == root_y) {
        return false;
    }

    // Union by rank: attach smaller tree under larger tree
    if (uf->rank[root_x] < uf->rank[root_y]) {
        uf->parent[root_x] = root_y;
    } else if (uf->rank[root_x] > uf->rank[root_y]) {
        uf->parent[root_y] = root_x;
    } else {
        // Equal rank: choose one as root, increment rank
        uf->parent[root_y] = root_x;
        uf->rank[root_x]++;
    }

    return true;
}

/**
 * Free Union-Find structure
 */
void uf_destroy(UnionFind* uf) {
    free(uf->parent);
    free(uf->rank);
    free(uf);
}

// ------------------------------------------------------------
// Edge structure for MST algorithms
// ------------------------------------------------------------

typedef struct {
    int u, v;      // Edge endpoints
    int weight;    // Edge weight
} Edge;

/**
 * Comparison function for sorting edges by weight (for Kruskal's)
 */
int compare_edges(const void* a, const void* b) {
    Edge* edge_a = (Edge*)a;
    Edge* edge_b = (Edge*)b;
    return edge_a->weight - edge_b->weight;
}

// ------------------------------------------------------------
// Prim's Algorithm - Minimum Spanning Tree
// ------------------------------------------------------------

/**
 * Prim's Algorithm for Minimum Spanning Tree
 *
 * Approach: Grow MST one vertex at a time
 * - Start with arbitrary vertex
 * - Repeatedly add cheapest edge from MST to non-MST vertex
 * - Continue until all vertices included
 *
 * Think of it as: Growing a connected tree from a single point
 *
 * Algorithm steps:
 * 1. Start with any vertex in MST
 * 2. Find minimum weight edge connecting:
 *    - A vertex IN the MST
 *    - To a vertex NOT in the MST
 * 3. Add this edge and vertex to MST
 * 4. Repeat until all vertices in MST
 *
 * When to use:
 * - Dense graphs (many edges)
 * - Adjacency matrix representation
 * - Want to see MST grow from a point
 *
 * Time: O(V²) with array, O((V+E) log V) with min-heap
 * Space: O(V)
 *
 * NOTE: This implementation uses simple array (O(V²))
 *       For better performance, use priority queue
 *
 * @param graph  Pointer to graph (must be UNDIRECTED and WEIGHTED)
 * @return       Array of edges in MST, or NULL on error
 */
Edge* graph_prim_mst(Graph* graph, int* mst_size) {
    if (graph->type == DIRECTED) {
        printf("Error: Prim's requires UNDIRECTED graph\n");
        return NULL;
    }

    printf("\n=== Prim's Algorithm - Minimum Spanning Tree ===\n\n");

    int V = graph->num_vertices;

    // Arrays for Prim's algorithm
    int* key = (int*)malloc(V * sizeof(int));        // Minimum weight to include vertex
    int* parent = (int*)malloc(V * sizeof(int));     // Parent vertex in MST
    bool* in_mst = (bool*)calloc(V, sizeof(bool));   // Whether vertex in MST

    // Initialize: all keys = infinity, no parents
    for (int i = 0; i < V; i++) {
        key[i] = INF;
        parent[i] = -1;
    }

    // Start from vertex 0
    key[0] = 0;  // Start vertex has key 0 so it's picked first

    printf("Step-by-step construction:\n\n");
    int step = 0;

    // Build MST with V-1 edges
    for (int count = 0; count < V; count++) {
        // Find vertex with minimum key that's not in MST
        int min_key = INF;
        int u = -1;
        for (int v = 0; v < V; v++) {
            if (!in_mst[v] && key[v] < min_key) {
                min_key = key[v];
                u = v;
            }
        }

        if (u == -1) break;  // No more reachable vertices

        // Add vertex to MST
        in_mst[u] = true;

        // Print step
        if (parent[u] != -1) {
            printf("Step %d: Add edge %d-%d (weight: %d)\n",
                   ++step, parent[u], u, key[u]);
            printf("        MST now includes vertices: ");
            for (int i = 0; i < V; i++) {
                if (in_mst[i]) printf("%d ", i);
            }
            printf("\n\n");
        } else {
            printf("Step %d: Start at vertex %d\n\n", ++step, u);
        }

        // Update keys of adjacent vertices
        if (graph->representation == ADJACENCY_LIST) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                int v = node->dest;
                int weight = node->weight;

                // If v not in MST and weight is smaller than current key
                if (!in_mst[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                }
                node = node->next;
            }
        } else {
            for (int v = 0; v < V; v++) {
                int weight = graph->adj_matrix[u][v];
                if (weight != NO_EDGE && !in_mst[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                }
            }
        }
    }

    // Build MST edge array
    Edge* mst = (Edge*)malloc((V - 1) * sizeof(Edge));
    *mst_size = 0;
    int total_weight = 0;

    for (int v = 1; v < V; v++) {
        if (parent[v] != -1) {
            mst[*mst_size].u = parent[v];
            mst[*mst_size].v = v;
            mst[*mst_size].weight = key[v];
            total_weight += key[v];
            (*mst_size)++;
        }
    }

    printf("Prim's MST Complete!\n");
    printf("Total MST weight: %d\n", total_weight);
    printf("Edges in MST: %d\n", *mst_size);

    free(key);
    free(parent);
    free(in_mst);

    return mst;
}

// ------------------------------------------------------------
// Kruskal's Algorithm - Minimum Spanning Tree
// ------------------------------------------------------------

/**
 * Kruskal's Algorithm for Minimum Spanning Tree
 *
 * Approach: Sort all edges, add cheapest edges that don't create cycles
 * - Sort all edges by weight (ascending)
 * - Use Union-Find to track connected components
 * - For each edge (cheapest first):
 *   - If endpoints in different components: add edge, merge components
 *   - If endpoints in same component: skip (would create cycle)
 *
 * Think of it as: Merging forests into a tree by choosing cheapest connections
 *
 * Algorithm steps:
 * 1. Sort ALL edges by weight
 * 2. Initialize Union-Find (each vertex is separate component)
 * 3. For each edge (u, v) in sorted order:
 *    a. If find(u) ≠ find(v):  // Different components
 *       - Add edge to MST
 *       - union(u, v)           // Merge components
 *    b. Else: skip (would create cycle)
 * 4. Stop when V-1 edges added
 *
 * When to use:
 * - Sparse graphs (few edges)
 * - Edge list representation
 * - Edges already sorted
 * - Parallel processing possible
 *
 * Time: O(E log E) for sorting + O(E α(V)) for Union-Find ≈ O(E log E)
 * Space: O(E) for edge list + O(V) for Union-Find
 *
 * @param graph  Pointer to graph (must be UNDIRECTED and WEIGHTED)
 * @return       Array of edges in MST, or NULL on error
 */
Edge* graph_kruskal_mst(Graph* graph, int* mst_size) {
    if (graph->type == DIRECTED) {
        printf("Error: Kruskal's requires UNDIRECTED graph\n");
        return NULL;
    }

    printf("\n=== Kruskal's Algorithm - Minimum Spanning Tree ===\n\n");

    int V = graph->num_vertices;
    int E = graph->num_edges;

    // Step 1: Collect all edges
    printf("Step 1: Collecting all edges...\n");
    Edge* edges = (Edge*)malloc(E * sizeof(Edge));
    int edge_count = 0;

    if (graph->representation == ADJACENCY_LIST) {
        for (int u = 0; u < V; u++) {
            AdjListNode* node = graph->adj_list[u];
            while (node != NULL) {
                int v = node->dest;
                // For undirected graph, only add edge once (u < v)
                if (u < v) {
                    edges[edge_count].u = u;
                    edges[edge_count].v = v;
                    edges[edge_count].weight = node->weight;
                    edge_count++;
                }
                node = node->next;
            }
        }
    } else {
        for (int u = 0; u < V; u++) {
            for (int v = u + 1; v < V; v++) {  // u < v for undirected
                if (graph->adj_matrix[u][v] != NO_EDGE) {
                    edges[edge_count].u = u;
                    edges[edge_count].v = v;
                    edges[edge_count].weight = graph->adj_matrix[u][v];
                    edge_count++;
                }
            }
        }
    }

    printf("        Found %d edges\n\n", edge_count);

    // Step 2: Sort edges by weight
    printf("Step 2: Sorting edges by weight...\n");
    qsort(edges, edge_count, sizeof(Edge), compare_edges);

    printf("        Sorted edge list:\n");
    for (int i = 0; i < edge_count; i++) {
        printf("        %d-%d (weight: %d)\n",
               edges[i].u, edges[i].v, edges[i].weight);
    }
    printf("\n");

    // Step 3: Initialize Union-Find
    printf("Step 3: Initializing Union-Find...\n");
    UnionFind* uf = uf_create(V);
    printf("        Each vertex starts in its own component\n\n");

    // Step 4: Process edges in sorted order
    printf("Step 4: Processing edges (adding if no cycle):\n\n");
    Edge* mst = (Edge*)malloc((V - 1) * sizeof(Edge));
    *mst_size = 0;
    int total_weight = 0;
    int step = 0;

    for (int i = 0; i < edge_count && *mst_size < V - 1; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int weight = edges[i].weight;

        // Check if u and v are in different components
        int root_u = uf_find(uf, u);
        int root_v = uf_find(uf, v);

        printf("Edge %d-%d (weight: %d): ", u, v, weight);

        if (root_u != root_v) {
            // Different components - add edge to MST
            printf("✓ ADDED (connects components %d and %d)\n", root_u, root_v);
            mst[*mst_size] = edges[i];
            (*mst_size)++;
            total_weight += weight;
            uf_union(uf, u, v);
            step++;
        } else {
            // Same component - would create cycle
            printf("✗ SKIPPED (would create cycle, both in component %d)\n", root_u);
        }
    }

    printf("\nKruskal's MST Complete!\n");
    printf("Total MST weight: %d\n", total_weight);
    printf("Edges in MST: %d\n", *mst_size);

    free(edges);
    uf_destroy(uf);

    return mst;
}

/**
 * Display MST edges
 */
void display_mst(Edge* mst, int mst_size) {
    printf("\nMST Edges:\n");
    int total = 0;
    for (int i = 0; i < mst_size; i++) {
        printf("  %d -- %d  (weight: %d)\n", mst[i].u, mst[i].v, mst[i].weight);
        total += mst[i].weight;
    }
    printf("\nTotal weight: %d\n", total);
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

void test_bfs_shortest_path() {
    printf("\n=== Test 7: BFS Shortest Path (Unweighted Graph) ===\n\n");

    // Create unweighted graph
    Graph* graph = graph_create(6, UNDIRECTED, UNWEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph, 0, 1, 1);
    graph_add_edge(graph, 0, 2, 1);
    graph_add_edge(graph, 1, 3, 1);
    graph_add_edge(graph, 2, 3, 1);
    graph_add_edge(graph, 2, 4, 1);
    graph_add_edge(graph, 3, 5, 1);
    graph_add_edge(graph, 4, 5, 1);

    graph_display_info(graph);
    graph_display_list(graph);

    // Test BFS shortest path
    graph_bfs_shortest_path(graph, 0, 5);

    graph_destroy(graph);
}

void test_dijkstra() {
    printf("\n=== Test 8: Dijkstra's Algorithm (Weighted Graph) ===\n\n");

    // Create weighted directed graph
    Graph* graph = graph_create(6, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    // Add edges to create interesting paths
    graph_add_edge(graph, 0, 1, 4);
    graph_add_edge(graph, 0, 2, 2);
    graph_add_edge(graph, 1, 2, 1);
    graph_add_edge(graph, 1, 3, 5);
    graph_add_edge(graph, 2, 3, 8);
    graph_add_edge(graph, 2, 4, 10);
    graph_add_edge(graph, 3, 4, 2);
    graph_add_edge(graph, 3, 5, 6);
    graph_add_edge(graph, 4, 5, 3);

    graph_display_info(graph);
    graph_display_list(graph);

    // Test single destination
    printf("\n--- Single destination shortest path ---\n");
    graph_dijkstra(graph, 0, 5);

    // Test all destinations
    printf("\n\n--- All destinations from source ---\n");
    graph_dijkstra(graph, 0, -1);

    graph_destroy(graph);
}

void test_bellman_ford() {
    printf("\n=== Test 9: Bellman-Ford Algorithm ===\n\n");

    // Test 1: Graph with negative weights (but no negative cycle)
    printf("--- Test 9a: Graph with Negative Weights ---\n\n");
    Graph* graph1 = graph_create(5, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph1, 0, 1, 4);
    graph_add_edge(graph1, 0, 2, 2);
    graph_add_edge(graph1, 1, 3, 3);
    graph_add_edge(graph1, 2, 1, -5);  // Negative weight!
    graph_add_edge(graph1, 2, 3, 6);
    graph_add_edge(graph1, 3, 4, 2);

    graph_display_info(graph1);
    graph_display_list(graph1);

    graph_bellman_ford(graph1, 0, 4);

    graph_destroy(graph1);

    // Test 2: Graph with negative cycle
    printf("\n\n--- Test 9b: Negative Cycle Detection ---\n\n");
    Graph* graph2 = graph_create(4, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph2, 0, 1, 1);
    graph_add_edge(graph2, 1, 2, -3);  // Negative weight
    graph_add_edge(graph2, 2, 3, 2);
    graph_add_edge(graph2, 3, 1, -2);  // Creates negative cycle: 1->2->3->1 = -3+2-2 = -3

    graph_display_info(graph2);
    graph_display_list(graph2);

    printf("\nNegative cycle: 1 -> 2 -> 3 -> 1\n");
    printf("Weight: -3 + 2 + (-2) = -3 (NEGATIVE!)\n");

    graph_bellman_ford(graph2, 0, 3);

    graph_destroy(graph2);
}

void test_mst_comparison() {
    printf("\n=== Test 10: MST Algorithms Comparison ===\n\n");

    // Create a weighted undirected graph
    printf("Creating test graph for MST:\n\n");
    Graph* graph = graph_create(6, UNDIRECTED, WEIGHTED, ADJACENCY_LIST);

    // Add edges - classic MST example
    graph_add_edge(graph, 0, 1, 4);
    graph_add_edge(graph, 0, 2, 3);
    graph_add_edge(graph, 1, 2, 1);
    graph_add_edge(graph, 1, 3, 2);
    graph_add_edge(graph, 2, 3, 4);
    graph_add_edge(graph, 2, 4, 5);
    graph_add_edge(graph, 3, 4, 1);
    graph_add_edge(graph, 3, 5, 6);
    graph_add_edge(graph, 4, 5, 3);

    graph_display_info(graph);
    graph_display_list(graph);
    graph_display_visual(graph);

    // Run Prim's algorithm
    printf("\n");
    printf("========================================\n");
    int prim_size;
    Edge* prim_mst = graph_prim_mst(graph, &prim_size);
    printf("========================================\n");

    if (prim_mst) {
        display_mst(prim_mst, prim_size);
    }

    // Run Kruskal's algorithm
    printf("\n\n");
    printf("========================================\n");
    int kruskal_size;
    Edge* kruskal_mst = graph_kruskal_mst(graph, &kruskal_size);
    printf("========================================\n");

    if (kruskal_mst) {
        display_mst(kruskal_mst, kruskal_size);
    }

    // Compare results
    printf("\n\n========================================\n");
    printf("COMPARISON:\n");
    printf("========================================\n\n");

    if (prim_mst && kruskal_mst) {
        int prim_total = 0, kruskal_total = 0;

        for (int i = 0; i < prim_size; i++) {
            prim_total += prim_mst[i].weight;
        }
        for (int i = 0; i < kruskal_size; i++) {
            kruskal_total += kruskal_mst[i].weight;
        }

        printf("Prim's MST weight:     %d\n", prim_total);
        printf("Kruskal's MST weight:  %d\n", kruskal_total);
        printf("\n");

        if (prim_total == kruskal_total) {
            printf("✓ Both algorithms found MST with same total weight!\n");
            printf("  (The actual edges may differ, but weight is optimal)\n");
        } else {
            printf("⚠ Different weights - this shouldn't happen!\n");
        }
    }

    // Cleanup
    if (prim_mst) free(prim_mst);
    if (kruskal_mst) free(kruskal_mst);
    graph_destroy(graph);
}

void test_mst_prim() {
    printf("\n=== Test 11: Prim's Algorithm ===\n\n");

    // Create a smaller graph for clearer visualization
    Graph* graph = graph_create(5, UNDIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph, 0, 1, 2);
    graph_add_edge(graph, 0, 3, 6);
    graph_add_edge(graph, 1, 2, 3);
    graph_add_edge(graph, 1, 3, 8);
    graph_add_edge(graph, 1, 4, 5);
    graph_add_edge(graph, 2, 4, 7);
    graph_add_edge(graph, 3, 4, 9);

    graph_display_info(graph);
    graph_display_list(graph);
    graph_display_visual(graph);

    int mst_size;
    Edge* mst = graph_prim_mst(graph, &mst_size);

    if (mst) {
        display_mst(mst, mst_size);
        free(mst);
    }

    graph_destroy(graph);
}

void test_mst_kruskal() {
    printf("\n=== Test 12: Kruskal's Algorithm ===\n\n");

    // Create same graph as Prim's test for comparison
    Graph* graph = graph_create(5, UNDIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph, 0, 1, 2);
    graph_add_edge(graph, 0, 3, 6);
    graph_add_edge(graph, 1, 2, 3);
    graph_add_edge(graph, 1, 3, 8);
    graph_add_edge(graph, 1, 4, 5);
    graph_add_edge(graph, 2, 4, 7);
    graph_add_edge(graph, 3, 4, 9);

    graph_display_info(graph);
    graph_display_list(graph);
    graph_display_visual(graph);

    int mst_size;
    Edge* mst = graph_kruskal_mst(graph, &mst_size);

    if (mst) {
        display_mst(mst, mst_size);
        free(mst);
    }

    graph_destroy(graph);
}

void test_topological_sort() {
    printf("\n=== Test 13: Topological Sort (Kahn's Algorithm) ===\n\n");

    // Test 1: Valid DAG (course prerequisites)
    printf("--- Test 13a: Valid DAG (Course Prerequisites) ---\n\n");
    printf("Courses: 0=Intro, 1=DataStruct, 2=Algorithms, 3=Systems, 4=ML, 5=AI\n");
    printf("Prerequisites:\n");
    printf("  Algorithms (2) requires DataStruct (1) and Intro (0)\n");
    printf("  Systems (3) requires DataStruct (1)\n");
    printf("  ML (4) requires Algorithms (2) and Systems (3)\n");
    printf("  AI (5) requires ML (4)\n\n");

    Graph* dag = graph_create(6, DIRECTED, UNWEIGHTED, ADJACENCY_LIST);

    // Course dependencies (edge from prerequisite to course)
    graph_add_edge(dag, 0, 2, 1);  // Intro → Algorithms
    graph_add_edge(dag, 1, 2, 1);  // DataStruct → Algorithms
    graph_add_edge(dag, 1, 3, 1);  // DataStruct → Systems
    graph_add_edge(dag, 2, 4, 1);  // Algorithms → ML
    graph_add_edge(dag, 3, 4, 1);  // Systems → ML
    graph_add_edge(dag, 4, 5, 1);  // ML → AI

    graph_display_info(dag);
    graph_display_list(dag);

    int result_size;
    int* topo_order = graph_topological_sort_kahn(dag, &result_size);

    if (topo_order) {
        printf("\n✓ Valid course ordering: ");
        const char* courses[] = {"Intro", "DataStruct", "Algorithms", "Systems", "ML", "AI"};
        for (int i = 0; i < result_size; i++) {
            printf("%s", courses[topo_order[i]]);
            if (i < result_size - 1) printf(" → ");
        }
        printf("\n");
        free(topo_order);
    }

    graph_destroy(dag);

    // Test 2: Graph with cycle
    printf("\n\n--- Test 13b: Graph with Cycle (No Valid Ordering) ---\n\n");
    Graph* cyclic = graph_create(4, DIRECTED, UNWEIGHTED, ADJACENCY_LIST);

    graph_add_edge(cyclic, 0, 1, 1);
    graph_add_edge(cyclic, 1, 2, 1);
    graph_add_edge(cyclic, 2, 3, 1);
    graph_add_edge(cyclic, 3, 1, 1);  // Creates cycle: 1 → 2 → 3 → 1

    graph_display_info(cyclic);
    graph_display_list(cyclic);

    printf("\nCycle: 1 → 2 → 3 → 1\n");

    topo_order = graph_topological_sort_kahn(cyclic, &result_size);

    graph_destroy(cyclic);
}

void test_floyd_warshall() {
    printf("\n=== Test 14: Floyd-Warshall All-Pairs Shortest Paths ===\n\n");

    // Test 1: Small weighted graph
    printf("--- Test 14a: Weighted Graph ---\n\n");
    Graph* graph = graph_create(4, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph, 0, 1, 5);
    graph_add_edge(graph, 0, 3, 10);
    graph_add_edge(graph, 1, 2, 3);
    graph_add_edge(graph, 2, 3, 1);

    graph_display_info(graph);
    graph_display_list(graph);

    int** dist = graph_floyd_warshall(graph);

    printf("\nExamples:\n");
    printf("  Shortest path 0→3: %d (via 0→1→2→3)\n", dist[0][3]);
    printf("  Shortest path 0→2: %d (via 0→1→2)\n", dist[0][2]);
    printf("  No path 3→0: %s\n", dist[3][0] == INF ? "INF" : "exists");

    floyd_warshall_free(dist, graph->num_vertices);
    graph_destroy(graph);

    // Test 2: Graph with negative weights
    printf("\n\n--- Test 14b: Graph with Negative Weights ---\n\n");
    Graph* graph2 = graph_create(4, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph2, 0, 1, 3);
    graph_add_edge(graph2, 1, 2, -2);  // Negative weight
    graph_add_edge(graph2, 2, 3, 2);
    graph_add_edge(graph2, 0, 3, 7);

    graph_display_info(graph2);
    graph_display_list(graph2);

    printf("\nNote: Edge 1→2 has negative weight (-2)\n");

    int** dist2 = graph_floyd_warshall(graph2);

    printf("\nObservation:\n");
    printf("  Direct path 0→3: 7\n");
    printf("  Via negative edge 0→1→2→3: %d (shorter!)\n", dist2[0][3]);

    floyd_warshall_free(dist2, graph2->num_vertices);
    graph_destroy(graph2);

    // Test 3: Graph with negative cycle
    printf("\n\n--- Test 14c: Negative Cycle Detection ---\n\n");
    Graph* graph3 = graph_create(3, DIRECTED, WEIGHTED, ADJACENCY_LIST);

    graph_add_edge(graph3, 0, 1, 1);
    graph_add_edge(graph3, 1, 2, -3);
    graph_add_edge(graph3, 2, 0, 1);  // Cycle: 0→1→2→0 = 1-3+1 = -1

    graph_display_info(graph3);
    graph_display_list(graph3);

    printf("\nNegative cycle: 0 → 1 → 2 → 0\n");
    printf("Weight: 1 + (-3) + 1 = -1 (NEGATIVE!)\n");

    int** dist3 = graph_floyd_warshall(graph3);

    floyd_warshall_free(dist3, graph3->num_vertices);
    graph_destroy(graph3);
}

// ============================================================
// MAIN
// ============================================================

int main() {
    char choice;

    while (1) {
        printf("\n=== Graph Data Structure Menu ===\n");
        printf("\nGraph Types:\n");
        printf("1. Complete Graph (Matrix representation)\n");
        printf("2. Sparse Graph (List representation)\n");
        printf("3. DAG - Directed Acyclic Graph (List)\n");
        printf("4. Bipartite Graph (List)\n");
        printf("5. Directed Weighted Graph (List)\n");
        printf("6. Comparison: Matrix vs List\n");
        printf("\nShortest Path Algorithms:\n");
        printf("7. BFS Shortest Path (Unweighted)\n");
        printf("8. Dijkstra's Algorithm (Non-negative Weighted)\n");
        printf("9. Bellman-Ford Algorithm (Negative Weights & Cycle Detection)\n");
        printf("\nMinimum Spanning Tree (MST):\n");
        printf("a. MST Comparison (Prim's vs Kruskal's)\n");
        printf("b. Prim's Algorithm\n");
        printf("c. Kruskal's Algorithm\n");
        printf("\nAdvanced Graph Algorithms:\n");
        printf("d. Topological Sort (Kahn's Algorithm)\n");
        printf("e. Floyd-Warshall (All-Pairs Shortest Paths)\n");
        printf("\nx. Exit\n");
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
        } else if (choice == '7') {
            test_bfs_shortest_path();
        } else if (choice == '8') {
            test_dijkstra();
        } else if (choice == '9') {
            test_bellman_ford();
        } else if (choice == 'a') {
            test_mst_comparison();
        } else if (choice == 'b') {
            test_mst_prim();
        } else if (choice == 'c') {
            test_mst_kruskal();
        } else if (choice == 'd') {
            test_topological_sort();
        } else if (choice == 'e') {
            test_floyd_warshall();
        } else {
            printf("Invalid choice\n");
        }
    }

    return 0;
}
