#include "rag.h"
#include <string.h>

static bool valid_dimensions(size_t p, size_t r) {
    return p > 0 && p <= RAG_MAX_PROCESSES &&
           r > 0 && r <= RAG_MAX_RESOURCES;
}
static bool valid_graph(const RagGraph *g) {
    return g && valid_dimensions(g->process_count, g->resource_count) &&
           g->node_count == g->process_count + g->resource_count &&
           g->edge_count <= RAG_MAX_EDGES;
}
static size_t node_id(const RagGraph *g, RagNodeType type, size_t index) {
    return type == RAG_NODE_PROCESS ? index : g->process_count + index;
}

RagStatus rag_init(RagGraph *graph, size_t pc, size_t rc,
                   const size_t instances[RAG_MAX_RESOURCES]) {
    RagGraph temp = {0};
    if (!graph || !instances) return RAG_ERR_ARGUMENT;
    if (!valid_dimensions(pc, rc)) return RAG_ERR_DIMENSIONS;
    temp.process_count = pc;
    temp.resource_count = rc;
    for (size_t r = 0; r < rc; ++r) temp.resource_instances[r] = instances[r];
    for (size_t p = 0; p < pc; ++p)
        temp.nodes[temp.node_count++] = (RagNode){RAG_NODE_PROCESS, p, 0};
    for (size_t r = 0; r < rc; ++r)
        temp.nodes[temp.node_count++] =
            (RagNode){RAG_NODE_RESOURCE, r, instances[r]};
    *graph = temp;
    return RAG_OK;
}

static RagStatus rebuild_edges(RagGraph *g,
    int values[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES], RagEdgeType type) {
    RagEdge temp[RAG_MAX_EDGES];
    size_t count = 0;
    if (!valid_graph(g) || !values) return RAG_ERR_ARGUMENT;
    /* Validate all inputs first, so invalid input cannot partially update graph. */
    for (size_t p = 0; p < g->process_count; ++p)
        for (size_t r = 0; r < g->resource_count; ++r)
            if (values[p][r] < 0) return RAG_ERR_NEGATIVE_VALUE;
    for (size_t i = 0; i < g->edge_count; ++i)
        if (g->edges[i].type != type) temp[count++] = g->edges[i];
    for (size_t p = 0; p < g->process_count; ++p) {
        for (size_t r = 0; r < g->resource_count; ++r) {
            if (values[p][r] == 0) continue;
            if (count >= RAG_MAX_EDGES) return RAG_ERR_CAPACITY;
            temp[count++] = (RagEdge){type, p, r, (size_t)values[p][r]};
        }
    }
    memcpy(g->edges, temp, count * sizeof(temp[0]));
    g->edge_count = count;
    return RAG_OK;
}
RagStatus rag_set_allocations(RagGraph *g,
    int a[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES]) {
    return rebuild_edges(g, a, RAG_EDGE_ALLOCATION);
}
RagStatus rag_set_requests(RagGraph *g,
    int q[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES]) {
    return rebuild_edges(g, q, RAG_EDGE_REQUEST);
}
RagStatus rag_update(RagGraph *g,
    int a[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES],
    int q[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES]) {
    RagGraph temp;
    RagStatus s;
    if (!g || !a || !q) return RAG_ERR_ARGUMENT;
    temp = *g;
    s = rag_set_allocations(&temp, a);
    if (s != RAG_OK) return s;
    s = rag_set_requests(&temp, q);
    if (s != RAG_OK) return s;
    *g = temp;
    return RAG_OK;
}
RagStatus rag_release_process(RagGraph *g, size_t p) {
    size_t out = 0;
    if (!valid_graph(g)) return RAG_ERR_ARGUMENT;
    if (p >= g->process_count) return RAG_ERR_NOT_FOUND;
    for (size_t i = 0; i < g->edge_count; ++i)
        if (g->edges[i].process_index != p) g->edges[out++] = g->edges[i];
    g->edge_count = out;
    return RAG_OK;
}
static bool dfs(const RagGraph *g, size_t node, unsigned char color[RAG_MAX_NODES]) {
    color[node] = 1;
    for (size_t i = 0; i < g->edge_count; ++i) {
        const RagEdge *e = &g->edges[i];
        size_t from = e->type == RAG_EDGE_ALLOCATION
            ? node_id(g, RAG_NODE_RESOURCE, e->resource_index)
            : node_id(g, RAG_NODE_PROCESS, e->process_index);
        size_t to = e->type == RAG_EDGE_ALLOCATION
            ? node_id(g, RAG_NODE_PROCESS, e->process_index)
            : node_id(g, RAG_NODE_RESOURCE, e->resource_index);
        if (from != node) continue;
        if (color[to] == 1) return true;
        if (color[to] == 0 && dfs(g, to, color)) return true;
    }
    color[node] = 2;
    return false;
}
bool rag_has_cycle(const RagGraph *g) {
    unsigned char color[RAG_MAX_NODES] = {0};
    if (!valid_graph(g)) return false;
    for (size_t n = 0; n < g->node_count; ++n)
        if (color[n] == 0 && dfs(g, n, color)) return true;
    return false;
}
const RagGraph *rag_render_data(const RagGraph *g) {
    return valid_graph(g) ? g : NULL;
}
const char *rag_status_message(RagStatus s) {
    switch (s) {
    case RAG_OK: return "Operation successful";
    case RAG_ERR_ARGUMENT: return "Invalid argument or graph";
    case RAG_ERR_DIMENSIONS: return "Invalid process or resource count";
    case RAG_ERR_NEGATIVE_VALUE: return "Allocation/request values must be non-negative";
    case RAG_ERR_CAPACITY: return "Graph edge capacity exceeded";
    case RAG_ERR_NOT_FOUND: return "Process index not found";
    default: return "Unknown RAG status";
    }
}
