#pragma once

#include <stdint.h>

#define BLOCK_SIZE 4096
#define BLOCKS_PER_BUCKET 8

// typedef struct bitmap {
//     unsigned int width;
//     unsigned int height;
//     char* image_bytes;
//     char* buf;
//     unsigned int total_size;
//     uint32_t bpp;
// }bitmap;

extern uint32_t end;

void pmm_init(uint32_t mem_size);
uint32_t allocate_block();
void free_block(uint32_t blk_num);
uint32_t first_free_block();