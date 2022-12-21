#pragma once

#include "disk/disk.h"
#include "drivers/screen.h"
#include <stdint.h>
#include <vector>
#include <string>
#include <math.h>
#include <sstream>
#include "drivers/utils.h"

class Filesystem
{
    public:
        Filesystem();
        ~Filesystem();
        /*
        * dir_list_entry struct
        * This struct is used by list_dir method to return directory entry
        * information.
        */   
        typedef struct dir_list_entry
        {
            int inode;
            std::string name;
            int file_size;
            bool is_dir;
        }dir_list_entry;
        typedef std::vector<dir_list_entry> dir_list;

        typedef struct dir_list_data
        {
            int inode;
            std::string contents;
        }dir_list_data;
        typedef std::vector<dir_list_data> data_list;
        
        //functions
        void format();
	    void create_file(std::string path_str, bool directory);
        std::string get_content(std::string path_str);
        void set_content(std::string path_str, std::string content);
        dir_list list_dir(std::string path_str);

    private:
        //helper functions
        void parse(std::string str_parse);
        void parseEntries(std::string toParse);
        void parseContents(std::string toParse);
        void updateContent(int inode, std::string content);
        void updateFileSize(int inode, int newSize);
        void updateBlockDev(bool type, std::string toInsert);

        typedef struct myfs_header {
		char magic[4];
		uint8_t version;
        }myfs_header;

        static const uint8_t CURR_VERSION = 0x03;
        static const char *FS_MAGIC;
        
        dir_list entries;
        data_list contents;
        std::string blockDevData;
        int lastInode;
        int headerSize;
};  
