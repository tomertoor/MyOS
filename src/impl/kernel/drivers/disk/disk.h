#pragma once

#include "../drivers/low_level.h"
#include <stdint.h>

#define SECTOR_SIZE 256

#define ATA_PORT 0x1F0


//registers
#define SECTOR_COUNT_REGISTER 2
#define SECTOR_NUMBER_REGISTER 3
#define SECTOR_NUMBER_LOW_REGISTER 4 
#define SECTOR_NUMBER_HIGH_REGISTER 5

#define STATUS_REGISTER 7

//statuses
#define BSY_RDY 0x80
#define DRQ_RDY 0x8


void read(uint32_t target_address, uint32_t address, uint8_t sector_count);
void write(uint32_t address, unsigned char sector_count, uint32_t* data);
void waitBSY();
void waitDRQ();
