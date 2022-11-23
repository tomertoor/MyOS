#include "filesystem.h"

Filesystem::Filesystem(int num)
{
    _num = num;
    _inode = 1844;
}

Filesystem::~Filesystem()
{
}

int Filesystem::getNum()
{
    return _num;
}

int Filesystem::getInode()
{
    return _inode;
}