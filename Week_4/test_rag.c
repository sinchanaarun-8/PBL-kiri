#include "../rag.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define P RAG_MAX_PROCESSES
#define R RAG_MAX_RESOURCES

static void test_nodes(void) {
    RagGraph g; size_t instances[R] = {2, 1};
    assert(rag_init(&g, 2, 2, instances) == RAG_OK);
    assert(g.node_count == 4);
    assert(g.nodes[0].type == RAG_NODE_PROCESS);
    assert(g.nodes[2].type == RAG_NODE_RESOURCE && g.nodes[2].instances == 2);
    puts("PASS: process/resource nodes and initialization");
}
static void test_edges(void) {
    RagGraph g; size_t instances[R] = {1, 1};
    int a[P][R] = {{0}}, q[P][R] = {{0}};
    assert(rag_init(&g, 2, 2, instances) == RAG_OK);
    a[0][0] = 1; q[1][0] = 1;
    assert(rag_update(&g, a, q) == RAG_OK && g.edge_count == 2);
    bool alloc = false, req = false;
    for (size_t i = 0; i < g.edge_count; ++i) {
        if (g.edges[i].type == RAG_EDGE_ALLOCATION) {
            assert(g.edges[i].resource_index == 0 && g.edges[i].process_index == 0);
            alloc = true;
        } else {
            assert(g.edges[i].process_index == 1 && g.edges[i].resource_index == 0);
            req = true;
        }
    }
    assert(alloc && req);
    puts("PASS: allocation and request edge directions");
}
static void test_cycle(void) {
    RagGraph g; size_t instances[R] = {1, 1};
    int a[P][R] = {{0}}, q[P][R] = {{0}};
    assert(rag_init(&g, 2, 2, instances) == RAG_OK);
    a[0][0] = 1; a[1][1] = 1; q[0][1] = 1; q[1][0] = 1;
    assert(rag_update(&g, a, q) == RAG_OK);
    assert(rag_has_cycle(&g));
    puts("PASS: directed cycle detected");
}
static void test_no_cycle(void) {
    RagGraph g; size_t instances[R] = {2};
    int a[P][R] = {{0}}, q[P][R] = {{0}};
    assert(rag_init(&g, 2, 1, instances) == RAG_OK);
    a[0][0] = 1; q[1][0] = 1;
    assert(rag_update(&g, a, q) == RAG_OK);
    assert(!rag_has_cycle(&g));
    puts("PASS: acyclic graph identified");
}
static void test_release(void) {
    RagGraph g; size_t instances[R] = {1};
    int a[P][R] = {{0}}, q[P][R] = {{0}};
    assert(rag_init(&g, 2, 1, instances) == RAG_OK);
    a[0][0] = 1; q[0][0] = 1; q[1][0] = 1;
    assert(rag_update(&g, a, q) == RAG_OK);
    assert(rag_release_process(&g, 0) == RAG_OK);
    assert(g.edge_count == 1 && g.edges[0].process_index == 1);
    puts("PASS: process release removes its graph edges");
}
static void test_invalid_values(void) {
    RagGraph g; size_t instances[R] = {1};
    int a[P][R] = {{0}}, q[P][R] = {{0}};
    assert(rag_init(&g, 1, 1, instances) == RAG_OK);
    a[0][0] = 1;
    assert(rag_set_allocations(&g, a) == RAG_OK);
    RagGraph before = g;
    q[0][0] = -1;
    assert(rag_set_requests(&g, q) == RAG_ERR_NEGATIVE_VALUE);
    assert(g.edge_count == before.edge_count);
    assert(memcmp(g.edges, before.edges, g.edge_count * sizeof(g.edges[0])) == 0);
    puts("PASS: invalid values rejected without modifying graph");
}
int main(void) {
    test_nodes(); test_edges(); test_cycle(); test_no_cycle(); test_release(); test_invalid_values();
    puts("\\nAll RAG tests passed.");
    return 0;
}
