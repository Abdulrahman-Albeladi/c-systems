#include "graph.h"

#include <stdlib.h>
#include <string.h>

static int vertex_no(Graph graph, const char vertex[]) {
  int i;

  if (vertex == NULL || graph.vtx_list == NULL)
    return -1;

  for (i = 0; i < graph.numvtx; i++) {
    if (strcmp(graph.vtx_list[i]->name, vertex) == 0)
      return i;
  }

  return -1;
}

void init_graph(Graph *graph) {
  if (graph != NULL)
    memset(graph, 0, sizeof(*graph));
}

int add_vertex(Graph *graph, const char new_vertex[]) {
  Vertex *vertex;
  Vertex **vertices;
  size_t name_length;

  if (graph == NULL || new_vertex == NULL || has_vertex(*graph, new_vertex))
    return 0;

  vertex = malloc(sizeof(*vertex));
  if (vertex == NULL)
    return 0;

  name_length = strlen(new_vertex) + 1;
  vertex->name = malloc(name_length);
  if (vertex->name == NULL) {
    free(vertex);
    return 0;
  }

  strcpy(vertex->name, new_vertex);
  vertex->edge_list = NULL;

  vertices = realloc(graph->vtx_list,
                     (size_t)(graph->numvtx + 1) * sizeof(*vertices));
  if (vertices == NULL) {
    free(vertex->name);
    free(vertex);
    return 0;
  }

  graph->vtx_list = vertices;
  graph->vtx_list[graph->numvtx] = vertex;
  graph->numvtx++;

  return 1;
}

int num_vertices(Graph graph) {
  return graph.numvtx;
}

int has_vertex(Graph graph, const char name[]) {
  return vertex_no(graph, name) != -1;
}

int add_edge(Graph *graph, const char source[], const char dest[], int cost) {
  Edge **edge;
  Edge *new_edge;
  int source_id;
  int dest_id;

  if (graph == NULL || source == NULL || dest == NULL || cost < 0 ||
      !has_vertex(*graph, source) || !has_vertex(*graph, dest))
    return 0;

  source_id = vertex_no(*graph, source);
  dest_id = vertex_no(*graph, dest);
  edge = &graph->vtx_list[source_id]->edge_list;

  while (*edge != NULL) {
    if (strcmp((*edge)->dest->name, dest) == 0)
      return 0;
    edge = &(*edge)->next_edge;
  }

  new_edge = malloc(sizeof(*new_edge));
  if (new_edge == NULL)
    return 0;

  new_edge->dest = graph->vtx_list[dest_id];
  new_edge->cost = cost;
  new_edge->next_edge = NULL;
  *edge = new_edge;

  return 1;
}

int get_edge_cost(Graph graph, const char source[], const char dest[]) {
  Edge *edge;
  int source_id;

  if (source == NULL || dest == NULL || !has_vertex(graph, source) ||
      !has_vertex(graph, dest))
    return -1;

  source_id = vertex_no(graph, source);
  edge = graph.vtx_list[source_id]->edge_list;

  while (edge != NULL) {
    if (strcmp(edge->dest->name, dest) == 0)
      return edge->cost;
    edge = edge->next_edge;
  }

  return -1;
}

int change_edge_cost(Graph *graph, const char source[], const char dest[],
                     int new_cost) {
  Edge *edge;
  int source_id;

  if (graph == NULL || source == NULL || dest == NULL || new_cost < 0 ||
      !has_vertex(*graph, source) || !has_vertex(*graph, dest))
    return 0;

  source_id = vertex_no(*graph, source);
  edge = graph->vtx_list[source_id]->edge_list;

  while (edge != NULL) {
    if (strcmp(edge->dest->name, dest) == 0) {
      edge->cost = new_cost;
      return 1;
    }
    edge = edge->next_edge;
  }

  return 0;
}

int num_neighbors(Graph graph, const char vertex[]) {
  Edge *edge;
  int count;
  int vertex_id;

  if (vertex == NULL || !has_vertex(graph, vertex))
    return -1;

  vertex_id = vertex_no(graph, vertex);
  edge = graph.vtx_list[vertex_id]->edge_list;
  count = 0;

  while (edge != NULL) {
    count++;
    edge = edge->next_edge;
  }

  return count;
}
