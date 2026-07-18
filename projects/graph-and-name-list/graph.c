#include "graph.h"

#include <stdlib.h>
#include <string.h>

static int vertex_no(Graph graph, const char vertex[]) {
  int i;

  if (vertex == NULL) {
    return -1;
  }

  for (i = 0; i < graph.numvtx; i++) {
    if (strcmp(graph.vtx_list[i]->name, vertex) == 0) {
      return i;
    }
  }

  return -1;
}

static char *copy_name(const char name[]) {
  char *copy;

  copy = malloc(strlen(name) + 1);
  if (copy != NULL) {
    strcpy(copy, name);
  }

  return copy;
}

static void free_edges(Edge *edge_list) {
  Edge *next;

  while (edge_list != NULL) {
    next = edge_list->next_edge;
    free(edge_list);
    edge_list = next;
  }
}

void init_graph(Graph *graph) {
  if (graph != NULL) {
    memset(graph, 0, sizeof(*graph));
  }
}

int add_vertex(Graph *graph, const char new_vertex[]) {
  Vertex *vertex;
  Vertex **vertices;
  int num_vtx;

  if (graph == NULL || new_vertex == NULL || has_vertex(*graph, new_vertex)) {
    return 0;
  }

  vertex = malloc(sizeof(*vertex));
  if (vertex == NULL) {
    return 0;
  }

  vertex->name = copy_name(new_vertex);
  if (vertex->name == NULL) {
    free(vertex);
    return 0;
  }

  vertex->numedg = 0;
  vertex->edge_list = NULL;
  num_vtx = graph->numvtx;

  vertices = realloc(graph->vtx_list, (num_vtx + 1) * sizeof(*vertices));
  if (vertices == NULL) {
    free(vertex->name);
    free(vertex);
    return 0;
  }

  graph->vtx_list = vertices;
  graph->vtx_list[num_vtx] = vertex;
  graph->numvtx++;

  return 1;
}

int num_vertices(Graph graph) {
  return graph.numvtx;
}

int has_vertex(Graph graph, const char name[]) {
  return vertex_no(graph, name) >= 0;
}

int add_edge(Graph *graph, const char source[], const char dest[], int cost) {
  Edge **edge;
  Edge *new_edge;
  int source_id;
  int dest_id;

  if (graph == NULL || source == NULL || dest == NULL || cost < 0 ||
      !has_vertex(*graph, source) || !has_vertex(*graph, dest)) {
    return 0;
  }

  source_id = vertex_no(*graph, source);
  dest_id = vertex_no(*graph, dest);
  edge = &graph->vtx_list[source_id]->edge_list;

  while (*edge != NULL) {
    if (strcmp((*edge)->dest->name, dest) == 0) {
      return 0;
    }
    edge = &(*edge)->next_edge;
  }

  new_edge = malloc(sizeof(*new_edge));
  if (new_edge == NULL) {
    return 0;
  }

  new_edge->dest = graph->vtx_list[dest_id];
  new_edge->cost = cost;
  new_edge->next_edge = NULL;
  *edge = new_edge;
  graph->vtx_list[source_id]->numedg++;

  return 1;
}

int get_edge_cost(Graph graph, const char source[], const char dest[]) {
  Edge *edge;
  int source_id;

  if (source == NULL || dest == NULL || !has_vertex(graph, source) ||
      !has_vertex(graph, dest)) {
    return -1;
  }

  source_id = vertex_no(graph, source);
  edge = graph.vtx_list[source_id]->edge_list;

  while (edge != NULL) {
    if (strcmp(edge->dest->name, dest) == 0) {
      return edge->cost;
    }
    edge = edge->next_edge;
  }

  return -1;
}

int change_edge_cost(Graph *graph, const char source[], const char dest[],
                     int new_cost) {
  Edge *edge;
  int source_id;

  if (graph == NULL || source == NULL || dest == NULL || new_cost < 0 ||
      !has_vertex(*graph, source) || !has_vertex(*graph, dest)) {
    return 0;
  }

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
  int vertex_id;

  if (vertex == NULL || !has_vertex(graph, vertex)) {
    return -1;
  }

  vertex_id = vertex_no(graph, vertex);
  return graph.vtx_list[vertex_id]->numedg;
}

void clear_graph(Graph *graph) {
  int i;

  if (graph == NULL) {
    return;
  }

  for (i = 0; i < graph->numvtx; i++) {
    free_edges(graph->vtx_list[i]->edge_list);
    free(graph->vtx_list[i]->name);
    free(graph->vtx_list[i]);
  }

  free(graph->vtx_list);
  memset(graph, 0, sizeof(*graph));
}

char **get_vertices(Graph graph) {
  char **vertices;
  int i;
  int j;

  vertices = calloc(graph.numvtx + 1, sizeof(*vertices));
  if (vertices == NULL) {
    return NULL;
  }

  for (i = 0; i < graph.numvtx; i++) {
    vertices[i] = copy_name(graph.vtx_list[i]->name);
    if (vertices[i] == NULL) {
      free_vertex_name_list(vertices);
      return NULL;
    }
  }

  for (i = 1; i < graph.numvtx; i++) {
    char *current = vertices[i];

    for (j = i; j > 0 && strcmp(vertices[j - 1], current) > 0; j--) {
      vertices[j] = vertices[j - 1];
    }
    vertices[j] = current;
  }

  return vertices;
}

char **get_neighbors(Graph graph, const char vertex[]) {
  char **neighbors;
  Edge *edge;
  int vertex_id;
  int count;
  int i;
  int j;

  if (vertex == NULL || !has_vertex(graph, vertex)) {
    return NULL;
  }

  vertex_id = vertex_no(graph, vertex);
  count = graph.vtx_list[vertex_id]->numedg;
  neighbors = calloc(count + 1, sizeof(*neighbors));
  if (neighbors == NULL) {
    return NULL;
  }

  edge = graph.vtx_list[vertex_id]->edge_list;
  for (i = 0; i < count; i++) {
    neighbors[i] = copy_name(edge->dest->name);
    if (neighbors[i] == NULL) {
      free_vertex_name_list(neighbors);
      return NULL;
    }
    edge = edge->next_edge;
  }

  for (i = 1; i < count; i++) {
    char *current = neighbors[i];

    for (j = i; j > 0 && strcmp(neighbors[j - 1], current) > 0; j--) {
      neighbors[j] = neighbors[j - 1];
    }
    neighbors[j] = current;
  }

  return neighbors;
}

void free_vertex_name_list(char **vertex_names) {
  int i;

  if (vertex_names == NULL) {
    return;
  }

  for (i = 0; vertex_names[i] != NULL; i++) {
    free(vertex_names[i]);
  }

  free(vertex_names);
}

int remove_edge(Graph *graph, const char source[], const char dest[]) {
  Edge **edge;
  Edge *removed;
  int source_id;

  if (graph == NULL || source == NULL || dest == NULL ||
      !has_vertex(*graph, source) || !has_vertex(*graph, dest)) {
    return 0;
  }

  source_id = vertex_no(*graph, source);
  edge = &graph->vtx_list[source_id]->edge_list;

  while (*edge != NULL) {
    if (strcmp((*edge)->dest->name, dest) == 0) {
      removed = *edge;
      *edge = removed->next_edge;
      free(removed);
      graph->vtx_list[source_id]->numedg--;
      return 1;
    }
    edge = &(*edge)->next_edge;
  }

  return 0;
}

int remove_vertex(Graph *graph, const char vertex[]) {
  Vertex **vertices;
  int vertex_id;
  int i;

  if (graph == NULL || vertex == NULL || !has_vertex(*graph, vertex)) {
    return 0;
  }

  vertex_id = vertex_no(*graph, vertex);

  for (i = 0; i < graph->numvtx; i++) {
    if (i != vertex_id) {
      remove_edge(graph, graph->vtx_list[i]->name, vertex);
    }
  }

  free_edges(graph->vtx_list[vertex_id]->edge_list);
  free(graph->vtx_list[vertex_id]->name);
  free(graph->vtx_list[vertex_id]);

  for (i = vertex_id; i < graph->numvtx - 1; i++) {
    graph->vtx_list[i] = graph->vtx_list[i + 1];
  }

  graph->numvtx--;
  if (graph->numvtx == 0) {
    free(graph->vtx_list);
    graph->vtx_list = NULL;
    return 1;
  }

  vertices = realloc(graph->vtx_list, graph->numvtx * sizeof(*vertices));
  if (vertices != NULL) {
    graph->vtx_list = vertices;
  }

  return 1;
}
