#ifndef PROJECT4_GRAPH_IMPLEMENTATION_H
#define PROJECT4_GRAPH_IMPLEMENTATION_H

/*
 * Adjacency-list graph representation.
 *
 * Vertices are identified by name. Each edge stores its destination vertex,
 * integer cost, and the next edge in the source vertex's adjacency list.
 */

typedef struct vertex {
  char *name;
  struct edge *edge_list;
} Vertex;

typedef struct edge {
  struct vertex *dest;
  int cost;
  struct edge *next_edge;
} Edge;

typedef struct graph {
  int numvtx;
  int numedg;
  struct vertex **vtx_list;
} Graph;

#endif /* PROJECT4_GRAPH_IMPLEMENTATION_H */
