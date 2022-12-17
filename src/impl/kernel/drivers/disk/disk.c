#include "disk.h"

void read(uint32_t target_address, uint32_t address, uint8_t sector_count)
{
    waitBSY();
    port_byte_out(ATA_PORT+SECTOR_COUNT_REGISTER, sector_count);
    port_byte_out(ATA_PORT+SECTOR_NUMBER_REGISTER, address);
    port_byte_out(ATA_PORT+SECTOR_NUMBER_LOW_REGISTER, address >> 8);
    port_byte_out(ATA_PORT+SECTOR_NUMBER_HIGH_REGISTER, address >> 16);
    port_byte_out(STATUS_REGISTER, 0x20); //READ COMMAND

    uint16_t *target = (uint16_t*) target_address;
    for(int i = 0; i < sector_count; i++)
    {
        waitBSY();
        waitDRQ();
        for(int j = 0; j < SECTOR_SIZE; j++)
        {
            target[i] = port_byte_in(ATA_PORT);
        }
        target+=256;
    }
}

void write(uint32_t address, uint8_t sector_count, uint32_t* data)
{
    waitBSY();
    port_byte_out(ATA_PORT+SECTOR_COUNT_REGISTER, sector_count);
    port_byte_out(ATA_PORT+SECTOR_NUMBER_REGISTER, address);
    port_byte_out(ATA_PORT+SECTOR_NUMBER_LOW_REGISTER, address >> 8);
    port_byte_out(ATA_PORT+SECTOR_NUMBER_HIGH_REGISTER, address >> 16);
    port_byte_out(STATUS_REGISTER, 0x30); // WRITE COMMAND
    for(int i = 0; i < sector_count; i++)
    {
        waitBSY();
        waitDRQ();
        for(int j = 0; j < SECTOR_SIZE; j++)
        {
            port_byte_out(ATA_PORT, data[j]);
        }
    }
}


void waitBSY()
{
    while(port_byte_in(ATA_PORT+STATUS_REGISTER) & BSY_RDY);
}

void waitDRQ()
{
    while(!port_byte_in(ATA_PORT+STATUS_REGISTER) & DRQ_RDY);
}
