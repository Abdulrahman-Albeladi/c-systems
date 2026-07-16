#ifndef GRAPH_H
#define GRAPH_H

/*
 * Independent graph abstract data type.
 *
 * This interface is intentionally small and assignment-agnostic. It models a
 * mutable directed graph with string-labeled vertices and integer edge weights.
 * Callers interact through an opaque handle so the representation can change
 * without affecting users of the API.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque graph handle. */
typedef struct Graph Graph;

/* Status codes returned by mutating and lookup operations. */
typedef enum GraphStatus {
  GRAPH_OK = 0,
  GRAPH_ERR_INVALID_ARGUMENT = 1,
  GRAPH_ERR_NOT_FOUND = 2,
  GRAPH_ERR_ALREADY_EXISTS = 3,
  GRAPH_ERR_ALLOCATION_FAILED = 4
} GraphStatus;

/*
 * A heap-allocated list of borrowed string pointers.
 *
 * The array itself is owned by the caller after a successful query and must be
 * released with graph_string_list_destroy(). Each string pointer refers to
 * memory owned by the graph and remains valid until the graph is modified or
 * destroyed.
 */
typedef struct GraphStringList {
  const char **items;
  size_t count;
} GraphStringList;

/* Lifecycle */
Graph *graph_create(void);
void graph_destroy(Graph *graph);
void graph_clear(Graph *graph);

/* Vertex operations */
GraphStatus graph_add_vertex(Graph *graph, const char *name);
GraphStatus graph_remove_vertex(Graph *graph, const char *name);
int graph_has_vertex(const Graph *graph, const char *name);
size_t graph_vertex_count(const Graph *graph);

/* Edge operations for a directed weighted graph. */
GraphStatus graph_add_edge(Graph *graph,
                           const char *source,
                           const char *destination,
                           int weight);
GraphStatus graph_remove_edge(Graph *graph,
                              const char *source,
                              const char *destination);
GraphStatus graph_set_edge_weight(Graph *graph,
                                  const char *source,
                                  const char *destination,
                                  int weight);
GraphStatus graph_get_edge_weight(const Graph *graph,
                                  const char *source,
                                  const char *destination,
                                  int *out_weight);

/* Neighborhood and enumeration queries. */
size_t graph_neighbor_count(const Graph *graph, const char *vertex);
GraphStatus graph_get_vertices(const Graph *graph, GraphStringList *out_list);
GraphStatus graph_get_neighbors(const Graph *graph,
                                const char *vertex,
                                GraphStringList *out_list);
void graph_string_list_destroy(GraphStringList *list);

#ifdef __cplusplus
}
#endif

#endif /* GRAPH_H */
