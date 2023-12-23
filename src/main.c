#include "mem_internals.h"
#include "mem.h"
#include <assert.h>

void test_successful_allocation() {
    void* test_heap = heap_init(0);
    assert(test_heap != NULL);
    debug_heap(stdout, HEAP_START);
    void* test_block = _malloc(1024);
    assert(test_block != NULL);
    debug_heap(stdout, HEAP_START);
    _free(test_block);
    heap_term();
}
void test_free_one_block() {
    void* test_heap = heap_init(0);
    assert(test_heap != NULL);
    void* test_block1 = _malloc(1024);
    assert(test_block1 != NULL);
    void* test_block2 = _malloc(1024);
    assert(test_block2 != NULL);
    debug_heap(stdout, HEAP_START);
    _free(test_block2);
    debug_heap(stdout, HEAP_START);
    struct block_header* test_block_header1 = block_get_header(test_block1);
    struct block_header* test_block_header2 = block_get_header(test_block2);
    assert(test_block_header1->next == test_block_header2);
    assert(test_block_header2->is_free);
    _free(test_block1);
    heap_term();
}
void test_free_two_blocks() {
    void* test_heap = heap_init(0);
    assert(test_heap != NULL);
    void* test_block1 = _malloc(1024);
    assert(test_block1 != NULL);
    void* test_block2 = _malloc(1024);
    assert(test_block2 != NULL);
    void* test_block3 = _malloc(1024);
    assert(test_block3 != NULL);
    debug_heap(stdout, HEAP_START);
    _free(test_block3);
    debug_heap(stdout, HEAP_START);
    struct block_header* test_block_header1 = block_get_header(test_block1);
    struct block_header* test_block_header2 = block_get_header(test_block2);
    struct block_header* test_block_header3 = block_get_header(test_block3);
    assert(test_block_header2->next == test_block_header3);
    assert(test_block_header3->is_free);
    _free(test_block2);
    debug_heap(stdout, HEAP_START);
    assert(test_block_header1->next == test_block_header2);
    assert(test_block_header2->is_free);
    _free(test_block1);
    heap_term();
}
void test_new_region_extends_old() {
    void* test_heap = heap_init(1024);
    assert(test_heap != NULL);
    debug_heap(stdout, HEAP_START);
    size_t old_size = size_from_capacity(block_get_header(test_heap)->capacity).bytes;
    void* test_block = _malloc(2048);
    assert(test_block != NULL);
    debug_heap(stdout, HEAP_START);
    size_t new_size = size_from_capacity(block_get_header(test_block)->capacity).bytes;
    assert(test_block == HEAP_START);
    assert(new_size > old_size);
    _free(test_block);
    heap_term();
}

int main() {

    test_successful_allocation();
    test_free_one_block();
    test_free_two_blocks();
    test_new_region_extends_old();

    return 0;
}