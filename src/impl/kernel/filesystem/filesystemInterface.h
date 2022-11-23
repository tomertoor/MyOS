#pragma once
#ifdef FIELSYSTEM
extern "C" {
#endif
    typedef void* FSHandler;
    FSHandler createFSHandler(int num);
    void freeFSHandler(FSHandler);
    int getNum(FSHandler);
    int getInode(FSHandler);
#ifdef FIELSYSTEM
}
#endif