#include "graph.h"

#include <stdlib.h>
#include <string.h>

static int find_vertex_index(const Graph *graph, const char *name) {
  int i;

  if (graph == NULL || name == NULL) {
    return -1;
  }

  for (i = 0; i < graph->numvtx; i++) {
    if (graph->vtx_list[i] != NULL && graph->vtx_list[i]->name != NULL &&
        strcmp(graph->vtx_list[i]->name, name) == 0) {
      return i;
    }
  }

  return -1;
}

static void free_edge_list(Edge *edge) {
  while (edge != NULL) {
    Edge *next = edge->next_edge;
    free(edge);
    edge = next;
  }
}

static char *copy_string(const char *src) {
  size_t len;
  char *copy;

  if (src == NULL) {
    return NULL;
  }

  len = strlen(src) + 1;
  copy = malloc(len);
  if (copy != NULL) {
    memcpy(copy, src, len);
  }

  return copy;
}

void init_graph(Graph *graph) {
  if (graph == NULL) {
    return;
  }

  graph->numvtx = 0;
  graph->vtx_list = NULL;
}

int add_vertex(Graph *graph, const char new_vertex[]) {
  Vertex **resized;
  Vertex *vertex;

  if (graph == NULL || new_vertex == NULL || has_vertex(*graph, new_vertex)) {
    return 0;
  }

  resized = realloc(graph->vtx_list, (size_t)(graph->numvtx + 1) * sizeof(*resized));
  if (resized == NULL) {
    return 0;
  }
  graph->vtx_list = resized;

  vertex = malloc(sizeof(*vertex));
  if (vertex == NULL) {
    return 0;
  }

  vertex->name = copy_string(new_vertex);
  if (vertex->name == NULL) {
    free(vertex);
    return 0;
  }

  vertex->numedg = 0;
  vertex->edge_list = NULL;

  graph->vtx_list[graph->numvtx] = vertex;
  graph->numvtx++;

  return 1;
}

int num_vertices(Graph graph) {
  return graph.numvtx;
}

int has_vertex(Graph graph, const char name[]) {
  return find_vertex_index(&graph, name) >= 0;
}

int add_edge(Graph *graph, const char source[], const char dest[], int cost) {
  int source_index;
  int dest_index;
  Edge *edge;
  Edge *current;

  if (graph == NULL || source == NULL || dest == NULL || cost < 0) {
    return 0;
  }

  source_index = find_vertex_index(graph, source);
  dest_index = find_vertex_index(graph, dest);
  if (source_index < 0 || dest_index < 0) {
    return 0;
  }

  current = graph->vtx_list[source_index]->edge_list;
  while (current != NULL) {
    if (strcmp(current->dest->name, dest) == 0) {
      return 0;
    }
    current = current->next_edge;
  }

  edge = malloc(sizeof(*edge));
  if (edge == NULL) {
    return 0;
  }

  edge->dest = graph->vtx_list[dest_index];
  edge->cost = cost;
  edge->next_edge = graph->vtx_list[source_index]->edge_list;
  graph->vtx_list[source_index]->edge_list = edge;
  graph->vtx_list[source_index]->numedg++;

  return 1;
}

int get_edge_cost(Graph graph, const char source[], const char dest[]) {
  int source_index;
  Edge *current;

  if (source == NULL || dest == NULL) {
    return -1;
  }

  source_index = find_vertex_index(&graph, source);
  if (source_index < 0 || find_vertex_index(&graph, dest) < 0) {
    return -1;
  }

  current = graph.vtx_list[source_index]->edge_list;
  while (current != NULL) {
    if (strcmp(current->dest->name, dest) == 0) {
      return current->cost;
    }
    current = current->next_edge;
  }

  return -1;
}

int change_edge_cost(Graph *graph, const char source[], const char dest[],
                     int new_cost) {
  int source_index;
  Edge *current;

  if (graph == NULL || source == NULL || dest == NULL || new_cost < 0) {
    return 0;
  }

  source_index = find_vertex_index(graph, source);
  if (source_index < 0 || find_vertex_index(graph, dest) < 0) {
    return 0;
  }

  current = graph->vtx_list[source_index]->edge_list;
  while (current != NULL) {
    if (strcmp(current->dest->name, dest) == 0) {
      current->cost = new_cost;
      return 1;
    }
    current = current->next_edge;
  }

  return 0;
}

int num_neighbors(Graph graph, const char vertex[]) {
  int vertex_index;

  if (vertex == NULL) {
    return -1;
  }

  vertex_index = find_vertex_index(&graph, vertex);
  if (vertex_index < 0) {
    return -1;
  }

  return graph.vtx_list[vertex_index]->numedg;
}

void clear_graph(Graph *graph) {
  int i;

  if (graph == NULL) {
    return;
  }

  for (i = 0; i < graph->numvtx; i++) {
    if (graph->vtx_list[i] != NULL) {
      free_edge_list(graph->vtx_list[i]->edge_list);
      free(graph->vtx_list[i]->name);
      free(graph->vtx_list[i]);
    }
  }

  free(graph->vtx_list);
  graph->vtx_list = NULL;
  graph->numvtx = 0;
}

char **get_vertices(Graph graph) {
  char **names;
  int i;
  int j;

  names = calloc((size_t)graph.numvtx + 1, sizeof(*names));
  if (names == NULL) {
    return NULL;
  }

  for (i = 0; i < graph.numvtx; i++) {
    names[i] = copy_string(graph.vtx_list[i]->name);
    if (names[i] == NULL) {
      free_vertex_name_list(names);
      return NULL;
    }
  }

  for (i = 1; i < graph.numvtx; i++) {
    char *key = names[i];
    j = i - 1;

    while (j >= 0 && strcmp(names[j], key) > 0) {
      names[j + 1] = names[j];
      j--;
    }
    names[j + 1] = key;
  }

  return names;
}

char **get_neighbors(Graph graph, const char vertex[]) {
  int vertex_index;
  int count;
  int i;
  int j;
  char **names;
  Edge *current;

  if (vertex == NULL) {
    return NULL;
  }

  vertex_index = find_vertex_index(&graph, vertex);
  if (vertex_index < 0) {
    return NULL;
  }

  count = graph.vtx_list[vertex_index]->numedg;
  names = calloc((size_t)count + 1, sizeof(*names));
  if (names == NULL) {
    return NULL;
  }

  current = graph.vtx_list[vertex_index]->edge_list;
  for (i = 0; i < count && current != NULL; i++) {
    names[i] = copy_string(current->dest->name);
    if (names[i] == NULL) {
      free_vertex_name_list(names);
      return NULL;
    }
    current = current->next_edge;
  }

  for (i = 1; i < count; i++) {
    char *key = names[i];
    j = i - 1;

    while (j >= 0 && strcmp(names[j], key) > 0) {
      names[j + 1] = names[j];
      j--;
    }
    names[j + 1] = key;
  }

  return names;
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
  int source_index;
  Edge *current;
  Edge *previous;

  if (graph == NULL || source == NULL || dest == NULL) {
    return 0;
  }

  source_index = find_vertex_index(graph, source);
  if (source_index < 0 || find_vertex_index(graph, dest) < 0) {
    return 0;
  }

  previous = NULL;
  current = graph->vtx_list[source_index]->edge_list;

  while (current != NULL) {
    if (strcmp(current->dest->name, dest) == 0) {
      if (previous == NULL) {
        graph->vtx_list[source_index]->edge_list = current->next_edge;
      } else {
        previous->next_edge = current->next_edge;
      }

      free(current);
      graph->vtx_list[source_index]->numedg--;
      return 1;
    }

    previous = current;
    current = current->next_edge;
  }

  return 0;
}

int remove_vertex(Graph *graph, const char vertex[]) {
  int vertex_index;
  int i;
  Vertex *removed;

  if (graph == NULL || vertex == NULL) {
    return 0;
  }

  vertex_index = find_vertex_index(graph, vertex);
  if (vertex_index < 0) {
    return 0;
  }

  for (i = 0; i < graph->numvtx; i++) {
    if (i != vertex_index) {
      remove_edge(graph, graph->vtx_list[i]->name, vertex);
    }
  }

  removed = graph->vtx_list[vertex_index];
  free_edge_list(removed->edge_list);
  free(removed->name);
  free(removed);

  for (i = vertex_index; i < graph->numvtx - 1; i++) {
    graph->vtx_list[i] = graph->vtx_list[i + 1];
  }

  graph->numvtx--;

  if (graph->numvtx == 0) {
    free(graph->vtx_list);
    graph->vtx_list = NULL;
  } else {
    Vertex **resized = realloc(graph->vtx_list, (size_t)graph->numvtx * sizeof(*resized));
    if (resized != NULL) {
      graph->vtx_list = resized;
    }
  }

  return 1;
}
