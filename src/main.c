#include "mem_internals.h"
#include "mem.h"
#include <assert.h>

void heap_init_and_check(size_t initial_size) {
    void* test_heap = heap_init(initial_size);
    assert(test_heap != NULL);
}

void test_successful_allocation() {
    heap_init_and_check(0);
    debug_heap(stdout, HEAP_START);
    void* test_block = _malloc(1024);
    assert(test_block != NULL);
    debug_heap(stdout, HEAP_START);
    _free(test_block);
    heap_term();
}
void test_free_one_block() {
    heap_init_and_check(0);
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
    heap_init_and_check(0);
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
    heap_init_and_check(0);
    void* test_block1 = _malloc(4096);
    assert(test_block1 != NULL);
    debug_heap(stdout, HEAP_START);
    void* test_block2 = _malloc(4096*3);
    assert(test_block2 != NULL);
    debug_heap(stdout, HEAP_START);
    _free(test_block2);
    _free(test_block1);
    heap_term();
}

int main() {

    test_successful_allocation();
    test_free_one_block();
    test_free_two_blocks();
    test_new_region_extends_old();

    return 0;
}