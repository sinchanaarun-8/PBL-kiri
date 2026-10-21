#ifndef RAG_H
#define RAG_H
#include <stdbool.h>
#include <stddef.h>
#define RAG_MAX_PROCESSES 10
#define RAG_MAX_RESOURCES 10
#define RAG_MAX_NODES (RAG_MAX_PROCESSES + RAG_MAX_RESOURCES)
#define RAG_MAX_EDGES (RAG_MAX_PROCESSES * RAG_MAX_RESOURCES * 2)

typedef enum { RAG_OK = 0, RAG_ERR_ARGUMENT, RAG_ERR_DIMENSIONS,
    RAG_ERR_NEGATIVE_VALUE, RAG_ERR_CAPACITY, RAG_ERR_NOT_FOUND } RagStatus;
typedef enum { RAG_NODE_PROCESS = 0, RAG_NODE_RESOURCE } RagNodeType;
typedef enum { RAG_EDGE_ALLOCATION = 0, RAG_EDGE_REQUEST } RagEdgeType;

typedef struct {
    RagNodeType type;
    size_t index;
    size_t instances;
} RagNode;

typedef struct {
    RagEdgeType type;
    size_t process_index;
    size_t resource_index;
    size_t instances;
} RagEdge;

typedef struct {
    size_t process_count, resource_count, node_count, edge_count;
    size_t resource_instances[RAG_MAX_RESOURCES];
    RagNode nodes[RAG_MAX_NODES];
    RagEdge edges[RAG_MAX_EDGES];
} RagGraph;

RagStatus rag_init(RagGraph *graph, size_t process_count, size_t resource_count,
                   const size_t resource_instances[RAG_MAX_RESOURCES]);
RagStatus rag_set_allocations(RagGraph *graph,
    int allocation[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES]);
RagStatus rag_set_requests(RagGraph *graph,
    int request[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES]);
RagStatus rag_update(RagGraph *graph,
    int allocation[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES],
    int request[RAG_MAX_PROCESSES][RAG_MAX_RESOURCES]);
RagStatus rag_release_process(RagGraph *graph, size_t process_index);
bool rag_has_cycle(const RagGraph *graph);
const RagGraph *rag_render_data(const RagGraph *graph);
const char *rag_status_message(RagStatus status);
#endif
