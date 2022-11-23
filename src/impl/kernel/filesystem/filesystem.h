#pragma once

class Filesystem
{
    private:
        int _num;
        int _inode;
    public:
        Filesystem(int num);
        ~Filesystem();
        int getNum();
        int getInode();
};
