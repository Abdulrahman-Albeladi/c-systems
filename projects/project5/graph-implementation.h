#ifndef GRAPH_IMPLEMENTATION_H
#define GRAPH_IMPLEMENTATION_H

/*
 * Adjacency-list graph data structures.
 *
 * Each vertex owns a linked list of outgoing edges. Edge destinations refer
 * to vertices stored by the containing graph.
 */

typedef struct edge Edge;
typedef struct vertex Vertex;
typedef struct graph Graph;

struct vertex {
    char *name;
    int numedg;
    Edge *edge_list;
};

struct edge {
    Vertex *dest;
    int cost;
    Edge *next_edge;
};

struct graph {
    int numvtx;
    Vertex **vtx_list;
};

#endif /* GRAPH_IMPLEMENTATION_H */
