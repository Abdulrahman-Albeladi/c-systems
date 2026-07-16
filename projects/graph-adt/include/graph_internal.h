#ifndef GRAPH_INTERNAL_H
#define GRAPH_INTERNAL_H

/*
 * Internal graph representation for an independently reconstructed
 * adjacency-list graph implementation.
 *
 * This header is intentionally limited to implementation details and should
 * not be included by external callers. Public APIs belong in a separate
 * interface header.
 */

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct graph_edge {
    struct graph_vertex *destination;
    int weight;
    struct graph_edge *next;
} graph_edge;

typedef struct graph_vertex {
    char *name;
    size_t edge_count;
    graph_edge *edges;
} graph_vertex;

typedef struct graph {
    size_t vertex_count;
    graph_vertex **vertices;
} graph;

#ifdef __cplusplus
}
#endif

#endif /* GRAPH_INTERNAL_H */
