/*****************************************************************************

 This file is part of QSS Solver.

 QSS Solver is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 QSS Solver is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with QSS Solver.  If not, see <http://www.gnu.org/licenses/>.

 ******************************************************************************/

#include "qss_graph.h"

#include <stdlib.h>

#include <common/data.h>
#include <common/utils.h>

typedef char Conn_t;

#define NO_ART_CONN 0
#define FORWARD_CONN 1
#define BACKWARD_CONN 2
#define BOTH_CONN 3

Conn_t GRP_updateConnections(Conn_t current_conn, Conn_t new_conn)
{
  if (current_conn == NO_ART_CONN) {
    return new_conn;
  }
  if (current_conn != new_conn && current_conn != BOTH_CONN) {
    return BOTH_CONN;
  }
  return current_conn;
}

grp_t GRP_getWeight(Conn_t conn, int index, int node, bool is_state)
{
  if ((conn == BOTH_CONN) && (index == node + 1 || index == node - 1)) {
    return 1;
  } else if ((conn == FORWARD_CONN) && (index == node + 1)) {
    return 1;
  } else if ((conn == BACKWARD_CONN) && (index == node - 1)) {
    return 1;
  } else if (is_state == TRUE) {
    return 10;
  }
  return 20;
}

int GRP_nodeWeight(int node, QSS_data data, int *node_inputs)
{
  int weight = node_inputs[node] + 1;
  if (node > data->states) {
    int event_id = node - data->states;
    weight += data->event[event_id].nLHSSt + data->event[event_id].nLHSDsc;
  }
  return weight;
}

int GRP_addEdges(vMatrix adjMatrix, int from, int to, int *node_inputs, bool *add_edges)
{
  int edges = 0;
  if (from != to) {
    insert(adjMatrix[from], to);
    insert(adjMatrix[to], from);
    node_inputs[to]++;
    edges += 2;
  }
  if (to == from + 1 && add_edges != NULL) {
    add_edges = FALSE;
  }
  return edges;
}

int GRP_connectNodesIfNeeded(vMatrix adjMatrix, int node, int *node_inputs, bool add_edges, Conn_t *artificial_edge, int limit)
{
  int edges = 0;
  if (add_edges && node < limit - 1) {
    insert(adjMatrix[node], node + 1);
    insert(adjMatrix[node + 1], node);
    node_inputs[node + 1]++;
    edges += 2;
    artificial_edge[node] = GRP_updateConnections(artificial_edge[node], FORWARD_CONN);
    artificial_edge[node + 1] = GRP_updateConnections(artificial_edge[node + 1], BACKWARD_CONN);
  }
  return edges;
}

void GRP_addHMatrixEntry(vMatrix h_matrix, int from, int to, Conn_t *artificial_edge, int *node_inputs)
{
  if (from != to) {
    insert(h_matrix[from], to);
    node_inputs[to]++;
  }
  if (to == from + 1) {
    artificial_edge[from] = NO_ART_CONN;
  }
}

void GRP_insertHMatrix(vMatrix h_matrix, int from, int to, int *node_inputs)
{
  insert(h_matrix[from], to);
  node_inputs[to]++;
}

GRP_Status GRP_generateHGraphFile(char *name, grp_t **xadj, grp_t **adjncy, int edges, int npins, int rhe, grp_t **hevars)
{
  char store_results[256];
  FILE *store_file = NULL;
  sprintf(store_results, "%s.hgraph", name);
  store_file = fopen(store_results, "wb");
  if (fwrite(&edges, sizeof(grp_t), 1, store_file) != 1) {
    fclose(store_file);
    return GRP_WriteError;
  }
  if (fwrite(&(xadj[0][1]), sizeof(grp_t), edges, store_file) != edges) {
    fclose(store_file);
    return GRP_WriteError;
  }
  if (fwrite(adjncy[0], sizeof(grp_t), npins, store_file) != npins) {
    fclose(store_file);
    return GRP_WriteError;
  }
  if (rhe) {
    if (fwrite(hevars[0], sizeof(grp_t), edges, store_file) != edges) {
      fclose(store_file);
      return GRP_WriteError;
    }
  }
  fclose(store_file);
  return GRP_Success;
}

GRP_Status GRP_generateGraphFile(char *name, grp_t **xadj, grp_t **adjncy, int nvtxs, int edges)
{
  char store_results[256];
  FILE *store_file = NULL;
  sprintf(store_results, "%s.graph", name);
  store_file = fopen(store_results, "wb");
  if (fwrite(&(xadj[0][1]), sizeof(grp_t), nvtxs, store_file) != nvtxs) {
    fclose(store_file);
    return GRP_WriteError;
  }
  if (fwrite(adjncy[0], sizeof(grp_t), edges, store_file) != edges) {
    fclose(store_file);
    return GRP_WriteError;
  }
  fclose(store_file);
  return GRP_Success;
}

GRP_Status GRP_generateEdgeWeightFile(char *name, grp_t **ewgt, int edges)
{
  char store_results[256];
  FILE *store_file = NULL;
  sprintf(store_results, "%s.eweights", name);
  store_file = fopen(store_results, "wb");
  if (fwrite(ewgt[0], sizeof(grp_t), edges, store_file) != edges) {
    fclose(store_file);
    return GRP_WriteError;
  }
  fclose(store_file);
  return GRP_Success;
}

GRP_Status GRP_generateNodeWeightFile(char *name, grp_t **vwgt, int nvtxs)
{
  char store_results[256];
  FILE *store_file = NULL;
  sprintf(store_results, "%s.vweights", name);
  store_file = fopen(store_results, "wb");
  if (fwrite(vwgt[0], sizeof(grp_t), nvtxs, store_file) != nvtxs) {
    fclose(store_file);
    return GRP_WriteError;
  }
  fclose(store_file);
  return GRP_Success;
}

GRP_Status GRP_createGraph(char *name, QSS_data data, grp_t **xadj, grp_t **adjncy, grp_t *edges, grp_t **vwgt, grp_t **ewgt, int rhe,
                           grp_t **hevars)
{
  grp_t states = data->states, events = data->events, nvtxs = states + events, edges_count = 0, i, j;
  Conn_t *artificial_edge = (Conn_t *)checkedMalloc((nvtxs) * sizeof(Conn_t));
  int *node_inputs = (int *)checkedMalloc((nvtxs) * sizeof(int));
  *vwgt = (grp_t *)checkedMalloc(nvtxs * sizeof(grp_t));
  for (i = 0; i < nvtxs; i++) {
    artificial_edge[i] = NO_ART_CONN;
    node_inputs[i] = 0;
  }
  if (data->params->pm == SD_HMetis || data->params->pm == SD_Patoh) {
    vMatrix hMatrix = VMatrix(nvtxs, 32);
    for (i = 0; i < states; i++) {
      int nSD = data->nSD[i];
      int nSZ = 0;
      artificial_edge[i] = FORWARD_CONN;
      for (j = 0; j < nSD; j++) {
        GRP_addHMatrixEntry(hMatrix, i, data->SD[i][j], artificial_edge, node_inputs);
      }
      if (artificial_edge[i] == FORWARD_CONN && i < nvtxs - 1) {
        GRP_insertHMatrix(hMatrix, i, i + 1, node_inputs);
      }
      if (data->nSZ != NULL) {
        nSZ = data->nSZ[i];
        for (j = 0; j < nSZ; j++) {
          GRP_insertHMatrix(hMatrix, i, states + data->SZ[i][j], node_inputs);
        }
      }
      if (nSD > 0 || nSZ > 0) {
        edges_count++;
        GRP_insertHMatrix(hMatrix, i, i, node_inputs);
      }
    }
    for (i = 0; i < events; i++) {
      int nHD = data->nHD[i];
      for (j = 0; j < nHD; j++) {
        GRP_insertHMatrix(hMatrix, states + i, data->HD[i][j], node_inputs);
      }
      int nHH = data->nHH[i];
      artificial_edge[states + i] = FORWARD_CONN;
      for (j = 0; j < nHH; j++) {
        GRP_addHMatrixEntry(hMatrix, states + i, states + data->HH[i][j], artificial_edge, node_inputs);
      }
      int nHZ = data->nHZ[i];
      for (j = 0; j < nHZ; j++) {
        GRP_addHMatrixEntry(hMatrix, states + i, states + data->HZ[i][j], artificial_edge, node_inputs);
      }
      if (artificial_edge[states + i] == FORWARD_CONN && i < events - 1) {
        GRP_insertHMatrix(hMatrix, states + i, states + i + 1, node_inputs);
      }
      int nLHSSt = data->event[i].nLHSSt;
      for (j = 0; j < nLHSSt; j++) {
        GRP_insertHMatrix(hMatrix, states + i, data->event[i].LHSSt[j], node_inputs);
      }
      if (nHD > 0 || nHZ > 0 || nLHSSt > 0 || nHH > 0) {
        GRP_insertHMatrix(hMatrix, states + i, states + i, node_inputs);
        edges_count++;
      }
      int nRHSSt = data->event[i].nRHSSt;
      for (j = 0; j < nRHSSt; j++) {
        int k = data->event[i].RHSSt[j];
        GRP_insertHMatrix(hMatrix, k, states + i, node_inputs);
        if (data->nSD[k] == 0 && data->nSZ[k] == 0) {
          GRP_insertHMatrix(hMatrix, k, k, node_inputs);
          edges_count++;
        }
      }
    }
    int pins_size = 0;
    for (i = 0; i < nvtxs; i++) {
      for (j = vectorFirst(hMatrix[i]); !vectorEnd(hMatrix[i]); j = vectorNext(hMatrix[i])) {
        pins_size++;
      }
    }
    printf("PRE %d\n", edges_count);
    *xadj = (grp_t *)checkedMalloc((edges_count + 1) * sizeof(grp_t));
    *adjncy = (grp_t *)checkedMalloc(pins_size * sizeof(grp_t));
    if (rhe) {
      *hevars = (grp_t *)checkedMalloc(edges_count * sizeof(grp_t));
    }
    xadj[0][0] = 0;
    int end = 0;
    *ewgt = (grp_t *)checkedMalloc(edges_count * sizeof(grp_t));
    edges_count = 0;
    const bool STATE = TRUE;
    for (i = 0; i < states; i++) {
      int edge_cost = 0;
      for (j = vectorFirst(hMatrix[i]); !vectorEnd(hMatrix[i]); j = vectorNext(hMatrix[i])) {
        adjncy[0][end++] = j;
        edge_cost += GRP_getWeight(artificial_edge[i], j, i, STATE);
      }
      if (edge_cost > 0) {
        xadj[0][edges_count + 1] = end;
        ewgt[0][edges_count] = edge_cost;
        if (rhe) {
          hevars[0][edges_count] = i;
        }
        edges_count++;
      }
      vwgt[0][i] = GRP_nodeWeight(i, data, node_inputs);
    }
    const bool EVENT = FALSE;
    for (i = states; i < nvtxs; i++) {
      int edge_cost = 0;
      for (j = vectorFirst(hMatrix[i]); !vectorEnd(hMatrix[i]); j = vectorNext(hMatrix[i])) {
        adjncy[0][end++] = j;
        edge_cost += GRP_getWeight(artificial_edge[i], j, i, EVENT);
      }
      if (edge_cost > 0) {
        xadj[0][edges_count + 1] = end;
        ewgt[0][edges_count] = edge_cost;
        if (rhe) {
          hevars[0][edges_count] = i;
        }
        edges_count++;
      }
      vwgt[0][i] = GRP_nodeWeight(i, data, node_inputs);
    }
    printf("POS %d\n", edges_count);
    edges[0] = edges_count;
    freeVMatrix(hMatrix, nvtxs);
    GRP_Status saved_files = GRP_generateHGraphFile(name, xadj, adjncy, edges_count, pins_size, rhe, hevars);
    saved_files = GRP_generateEdgeWeightFile(name, ewgt, edges_count);
    saved_files = GRP_generateNodeWeightFile(name, vwgt, nvtxs);
    if (saved_files != GRP_Success) {
      return saved_files;
    }
  } else {
    vMatrix adjMatrix = VMatrix(nvtxs, 32);
    for (i = 0; i < states; i++) {
      int nSD = data->nSD[i];
      bool add_edges = TRUE;
      for (j = 0; j < nSD; j++) {
        edges_count += GRP_addEdges(adjMatrix, i, data->SD[i][j], node_inputs, &add_edges);
      }
      edges_count += GRP_connectNodesIfNeeded(adjMatrix, i, node_inputs, add_edges, artificial_edge, nvtxs);
      if (data->nSZ != NULL) {
        int nSZ = data->nSZ[i];
        for (j = 0; j < nSZ; j++) {
          edges_count += GRP_addEdges(adjMatrix, i, states + data->SZ[i][j], node_inputs, NULL);
        }
      }
    }
    for (i = 0; i < events; i++) {
      int nHD = data->nHD[i];
      for (j = 0; j < nHD; j++) {
        edges_count += GRP_addEdges(adjMatrix, states + i, data->HD[i][j], node_inputs, NULL);
      }
      int nHH = data->nHH[i];
      bool add_edges = TRUE;
      for (j = 0; j < nHH; j++) {
        edges_count += GRP_addEdges(adjMatrix, states + i, states + data->HH[i][j], node_inputs, &add_edges);
      }
      int nHZ = data->nHZ[i];
      for (j = 0; j < nHZ; j++) {
        edges_count += GRP_addEdges(adjMatrix, states + i, states + data->HZ[i][j], node_inputs, &add_edges);
      }
      edges_count += GRP_connectNodesIfNeeded(adjMatrix, states + i, node_inputs, add_edges, artificial_edge, nvtxs);
      int nLHSSt = data->event[i].nLHSSt;
      for (j = 0; j < nLHSSt; j++) {
        edges_count += GRP_addEdges(adjMatrix, states + i, data->event[i].LHSSt[j], node_inputs, NULL);
      }
      int nRHSSt = data->event[i].nRHSSt;
      for (j = 0; j < nRHSSt; j++) {
        edges_count += GRP_addEdges(adjMatrix, states + i, data->event[i].RHSSt[j], node_inputs, NULL);
      }
    }
    *xadj = (grp_t *)checkedMalloc((nvtxs + 1) * sizeof(grp_t));
    *adjncy = (grp_t *)checkedMalloc(edges_count * sizeof(grp_t));
    xadj[0][0] = 0;
    int end = 0;
    *ewgt = (grp_t *)checkedMalloc(edges_count * sizeof(grp_t));
    const bool STATE = TRUE;
    for (i = 0; i < states; i++) {
      for (j = vectorFirst(adjMatrix[i]); !vectorEnd(adjMatrix[i]); j = vectorNext(adjMatrix[i])) {
        adjncy[0][end] = j;
        ewgt[0][end++] = GRP_getWeight(artificial_edge[i], j, i, STATE);
      }
      xadj[0][i + 1] = end;
      vwgt[0][i] = GRP_nodeWeight(i, data, node_inputs);
    }
    const bool EVENT = FALSE;
    for (i = states; i < nvtxs; i++) {
      for (j = vectorFirst(adjMatrix[i]); !vectorEnd(adjMatrix[i]); j = vectorNext(adjMatrix[i])) {
        adjncy[0][end] = j;
        ewgt[0][end++] = GRP_getWeight(artificial_edge[i], j, i, EVENT);
      }
      xadj[0][i + 1] = end;
      vwgt[0][i] = GRP_nodeWeight(i, data, node_inputs);
    }
    edges_count = xadj[0][nvtxs];
    freeVMatrix(adjMatrix, nvtxs);
    GRP_Status saved_files = GRP_generateGraphFile(name, xadj, adjncy, nvtxs, edges_count);
    saved_files = GRP_generateEdgeWeightFile(name, ewgt, edges_count);
    saved_files = GRP_generateNodeWeightFile(name, vwgt, nvtxs);
    if (saved_files != GRP_Success) {
      return saved_files;
    }
    edges[0] = edges_count;
  }
  free(artificial_edge);
  free(node_inputs);
  return GRP_Success;
}

GRP_Status GRP_readFile(FILE *file, grp_t *buffer, grp_t size, bool close)
{
  GRP_Status result = GRP_Success;
  if (file) {
    if (fread(buffer, sizeof(grp_t), size, file) == size) {
      if (close) {
        fclose(file);
      }
    } else {
      result = GRP_ReadError;
    }
  } else {
    result = GRP_WrongFile;
  }
  return result;
}

GRP_Status GRP_readGraph(char *name, QSS_data data, grp_t **xadj, grp_t **adjncy, grp_t *edges, grp_t **vwgt, grp_t **ewgt, int rhe,
                         grp_t **hevars)
{
  char file_name[256];
  FILE *graph_file = NULL, *edge_weight_file = NULL, *node_weight_file = NULL;
  SD_PartitionMethod pm = data->params->pm;
  grp_t nvtxs = data->states + data->events;
  GRP_Status result = GRP_Success;

  sprintf(file_name, "%s.vweights", name);
  node_weight_file = fopen(file_name, "rb");
  *vwgt = (grp_t *)checkedMalloc(nvtxs * sizeof(grp_t));
  result |= GRP_readFile(node_weight_file, vwgt[0], nvtxs, TRUE);

  if (pm == SD_Scotch || pm == SD_Metis) {
    sprintf(file_name, "%s.graph", name);
    graph_file = fopen(file_name, "rb");
    if (graph_file) {
      *xadj = (grp_t *)checkedMalloc((nvtxs + 1) * sizeof(grp_t));
      xadj[0][0] = 0;
      result |= GRP_readFile(graph_file, &(xadj[0][1]), nvtxs, FALSE);
      edges[0] = xadj[0][nvtxs];
      *adjncy = (grp_t *)checkedMalloc(edges[0] * sizeof(grp_t));
      result |= GRP_readFile(graph_file, adjncy[0], edges[0], TRUE);
    } else {
      result = GRP_ReadError;
    }

    sprintf(file_name, "%s.eweights", name);
    edge_weight_file = fopen(file_name, "rb");
    if (edge_weight_file) {
      *ewgt = (grp_t *)checkedMalloc(edges[0] * sizeof(grp_t));
      result |= GRP_readFile(edge_weight_file, ewgt[0], edges[0], TRUE);
    } else {
      result = GRP_ReadError;
    }
  } else if (pm == SD_Patoh || pm == SD_HMetis) {
    sprintf(file_name, "%s.hgraph", name);
    graph_file = fopen(file_name, "rb");
    if (graph_file) {
      result |= GRP_readFile(graph_file, edges, 1, FALSE);
      *xadj = (grp_t *)checkedMalloc((edges[0] + 1) * sizeof(grp_t));
      xadj[0][0] = 0;
      result |= GRP_readFile(graph_file, &(xadj[0][1]), edges[0], FALSE);
      int npins = xadj[0][edges[0]];
      *adjncy = (grp_t *)checkedMalloc(npins * sizeof(grp_t));
      bool close_file = (rhe > 0) ? FALSE : TRUE;
      result |= GRP_readFile(graph_file, adjncy[0], npins, close_file);

      if (rhe) {
        *hevars = (grp_t *)checkedMalloc(edges[0] * sizeof(grp_t));
        result |= GRP_readFile(graph_file, hevars[0], edges[0], TRUE);
      }
    } else {
      result = GRP_ReadError;
    }

    sprintf(file_name, "%s.heweights", name);
    edge_weight_file = fopen(file_name, "rb");
    if (edge_weight_file) {
      *ewgt = (grp_t *)checkedMalloc(edges[0] * sizeof(grp_t));
      result |= GRP_readFile(edge_weight_file, ewgt[0], edges[0], TRUE);
    }
  }
  return result;
}
