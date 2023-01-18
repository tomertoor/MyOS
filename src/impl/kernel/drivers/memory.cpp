#include "memory.h"
#include "screen.h"
#include "utils.h"

#define SETBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] | (1 << (i % BLOCKS_PER_BUCKET))
#define CLEARBIT(i) bitmap[i / BLOCKS_PER_BUCKET] = bitmap[i / BLOCKS_PER_BUCKET] & (~(1 << (i % BLOCKS_PER_BUCKET)))
#define ISSET(i) ((bitmap[i / BLOCKS_PER_BUCKET] >> (i % BLOCKS_PER_BUCKET)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t*) &bitmap[i / 32]))
#define BLOCK_ALIGN(addr) (((addr) & 0xFFFFF000) + 0x1000)

uint8_t * bitmap = (uint8_t*)(&end);
uint8_t * mem_start;
uint32_t total_blocks;
uint32_t bitmap_size;

void pmm_init(uint32_t mem_size) {
    total_blocks = mem_size / BLOCK_SIZE;
    bitmap_size = total_blocks / BLOCKS_PER_BUCKET;
    if(bitmap_size * BLOCKS_PER_BUCKET < total_blocks)
        bitmap_size++;

    memset(bitmap, 0, bitmap_size);

    mem_start = (uint8_t*)BLOCK_ALIGN(((uint32_t)(bitmap + bitmap_size)));
}

uint32_t allocate_block() {
    uint32_t free_block = first_free_block();
    SETBIT(free_block);
    return free_block;
}

void free_block(uint32_t blk_num) {
    CLEARBIT(blk_num);
}


uint32_t first_free_block() {
    uint32_t i;
    for(i = 0; i < total_blocks; i++) {
        if(!ISSET(i))
            return i;
    }
    my_print("pmm: Running out of free blocks!\n", 0x0);
    return (uint32_t) -1;
}
