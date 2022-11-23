#include "filesystemInterface.h"
#include "filesystem.h"
extern "C"
{
    FSHandler createFSHandler(int num){
        return (Filesystem*) new Filesystem(num);
    }
    void freeFSHandler(FSHandler handler){
        delete (Filesystem*) handler;
    }
    int printNum(FSHandler handler){
        return ((Filesystem*)handler)->getNum();
    }
    int printInode(FSHandler handler){
        return ((Filesystem*)handler)->getInode();
    }
}