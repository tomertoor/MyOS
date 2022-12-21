#include "filesystem.h"

const char *Filesystem::FS_MAGIC = "FS";

Filesystem::Filesystem(){
	headerSize = 0;
	lastInode = 0;
	entries = dir_list();
	contents = data_list();
	
	myfs_header header;
	read(0, sizeof(header), (uint8_t)&header); // Get Block Device.

	if (strncmp(header.magic, FS_MAGIC, sizeof(header.magic)) != 0 ||
	    (header.version != CURR_VERSION)) {
		format();
        my_print("There is no data exist in the filesystem\n", 0x0);
	} // Else: Exists!
	else
	{
		headerSize = sizeof(header);
		int bufferSize = (1024*1024) - headerSize;
		char* readOutput = new char[bufferSize];
		read(0, bufferSize, (uint8_t)readOutput);
		blockDevData = std::string(readOutput);
		parse(blockDevData.substr(headerSize));
        my_print("Successfully read the data out of the filesystem\n", 0x0);
        delete readOutput;
	}
}

Filesystem::~Filesystem()
{

}

void Filesystem::format() 
{
	myfs_header header;
	strncpy(header.magic, FS_MAGIC, sizeof(header.magic));
	header.version = CURR_VERSION;
	headerSize = sizeof(header);
	write(0, sizeof(header), (uint32_t*)&header);
	write(sizeof(header) + 1, 8, (uint32_t*)"{<>#<>}");
	blockDevData = std::string((const char*)&header) + "{<>#<>}";
}

void Filesystem::create_file(std::string path_str, bool directory) 
{
	if(directory)
		throw std::runtime_error("Not implemented directories\n");

	for (size_t i = 0; i < entries.size(); i++)
	{
		if(path_str == entries[i].name)
			throw std::runtime_error("File exists\n");
	}
	
	if(path_str.find("#") != std::string::npos)
		throw std::runtime_error("You cant use # as file name, part of the format\n");
	
	dir_list_entry entry = {lastInode + 1, path_str, 0, directory};
	entries.push_back(entry);

	std::string update = "(" + std::to_string(lastInode + 1) + "," + (path_str + ",") + "0,";
	update += (directory ? "true)" : "false)");

	updateBlockDev(true, update);
	lastInode++;
}

std::string Filesystem::get_content(std::string path_str) 
{
	for (size_t i = 0; i < entries.size(); i++)
	{
		if(entries[i].name == path_str)
		{
			for (size_t j = 0; j < contents.size(); j++)
			{
				if(entries[i].inode == contents[j].inode)
					return contents[j].contents;
			}
		}
	}
	throw std::runtime_error(std::string("File not exist\n"));
}

void Filesystem::set_content(std::string path_str, std::string content) 
{
	for (size_t i = 0; i < entries.size(); i++)
	{
		if(entries[i].name == path_str)
		{
			for (size_t j = 0; j < contents.size(); j++)
			{
				if(entries[i].inode == contents[j].inode)
				{
					updateContent(entries[i].inode, content);
					updateFileSize(entries[i].inode, content.size());
					entries[i].file_size = content.size();
					return;
				}
			}
			std::string update = "(" + std::to_string(entries[i].inode) + "," + content + ")";

			updateBlockDev(false, update);
			dir_list_data ToInsert = {entries[i].inode, content};
			contents.push_back(ToInsert);
			updateFileSize(entries[i].inode, content.size());
			entries[i].file_size = content.size();
			return;
		}
	}
	throw std::runtime_error(std::string("File not exist\n"));
}

Filesystem::dir_list Filesystem::list_dir(std::string path_str) 
{
	if(entries.size() == 0)
		throw std::runtime_error(std::string("Empty Directory\n"));
	else
		return entries;
}

void Filesystem::parse(std::string str_parse)
{	
	parseEntries(str_parse);
	parseContents(str_parse);
}

void Filesystem::updateBlockDev(bool type, std::string toInsert)
{
	std::string temp = blockDevData;

	int begin = (type ? (temp.find("<") + 1) : (temp.find("#") + 2));

	temp.insert(begin, toInsert);

	blockDevData = temp;
	write(0, blockDevData.length(), (uint32_t*)blockDevData.c_str());
}

void Filesystem::updateContent(int inode, std::string content)
{
	std::string newData;
	std::string oldData;
	data_list newDataVec;
	for (size_t i = 0; i < entries.size(); i++)
	{
		for (size_t j = 0; j < contents.size(); j++)
		{
			if (inode == entries[i].inode && inode == contents[j].inode)
			{
				oldData = "(" + std::to_string(entries[i].inode) + "," + contents[i].contents + ")";

				newData = "(" + std::to_string(entries[i].inode) + "," + content + ")";

				newDataVec.push_back({inode, content});
			}
			else
				newDataVec.push_back(contents[j]);
		}
	}

	contents = newDataVec;
	
	blockDevData.replace(blockDevData.find(oldData),oldData.length(),newData);
	write(0, blockDevData.size(), (uint32_t*)blockDevData.c_str());
}

void Filesystem::updateFileSize(int inode, int newSize)
{
	std::string newData;
	std::string oldData;
	for (size_t i = 0; i < entries.size(); i++)
	{
		if (inode == entries[i].inode)
		{
			oldData = "(" + std::to_string(entries[i].inode) + "," + entries[i].name + "," + std::to_string(entries[i].file_size) + ",";

			newData = "(" + std::to_string(entries[i].inode) + "," + entries[i].name + "," + std::to_string(newSize) + ",";

			if(entries[i].is_dir)
			{
				newData += "true)";
				oldData += "true)";
				entries[i] = {inode, entries[i].name, newSize, true};
			}
			else
			{
				newData += "false)";
				oldData += "false)";
				entries[i] = {inode, entries[i].name, newSize, false};
			}
		}
	}
	blockDevData.replace(blockDevData.find(oldData),oldData.length(),newData);
	write(0, blockDevData.size(), (uint32_t*)blockDevData.c_str());
}

void Filesystem::parseEntries(std::string toParse)
{
	unsigned int seperatorPos = toParse.find("#");
	unsigned int begin = 2;
	unsigned int prev = 0;
	unsigned int cur = 0;

	while (cur < seperatorPos)
	{
		cur = toParse.find("(", begin);
		
		// If we found an opening ( there has to be a closing ).
		prev = cur;
		cur = toParse.find(")", prev);

		if (cur == std::string::npos)
			return;
		else if (cur > seperatorPos)
			return;
		
		std::string sArr[4] = {};
		
		unsigned int len = 0;
		unsigned int prevComma = 0;
		unsigned int curComma = 0;

		curComma = toParse.find(",", prev + 1);
		len = curComma - prev;
		sArr[0] = toParse.substr(prev + 1, len - 1);

		for (size_t i = 1; i < 3; i++)
		{
			prevComma = curComma;
			curComma = toParse.find(",", prevComma + 1);
			len = curComma - prevComma;

			sArr[i] = toParse.substr(prevComma + 1, len - 1);
		}

		prevComma = curComma;
		unsigned int endIndex = toParse.find(")", prevComma + 1);
		len = endIndex - prevComma;

		sArr[3] = toParse.substr(prevComma + 1, len - 1);

		int inode = atoi(sArr[0].c_str());

		if (inode > lastInode)
			lastInode = inode;
		
		std::string name = sArr[1];
		int size = atoi(sArr[2].c_str());
		
		bool isDir;
		if(strcmp(sArr[3].c_str(), "false") == 0)
			isDir = false;
		else if(strcmp(sArr[3].c_str(), "true") == 0)
			isDir = true;

		dir_list_entry toPush = dir_list_entry{inode, name, size, isDir};

		entries.push_back(toPush);

		begin = cur + 1;
	}
}

void Filesystem::parseContents(std::string toParse)
{
	unsigned int begin = toParse.find("#") + 1;
	unsigned int cur = 0;
	unsigned int prev;
	unsigned int endPos = toParse.find("}");
	while (cur < endPos)
	{
		cur = toParse.find("(", begin);

		if (cur == std::string::npos) //Not Found.
			return;
		else if (cur >= endPos)
			return;
			
				
		// If we found an opening ( there has to be a closing ).
		prev = cur;
		cur = toParse.find(")", prev);
		
		std::string sArr[2] = {};
		unsigned int len = 0;
		unsigned int prevComma;
		unsigned int curComma;

		curComma = toParse.find(",", prev + 1);

		len = curComma - prev;
				
		sArr[0] = toParse.substr(prev + 1, len - 1);

		prevComma = curComma;
		unsigned int curEnd = toParse.find(")", prevComma + 1);
		len = curEnd - prevComma;

		sArr[1] = toParse.substr(prevComma + 1, len - 1);

		int inode = atoi(sArr[0].c_str());
		std::string content = sArr[1];

		dir_list_data toPush = dir_list_data{inode, content};
		contents.push_back(toPush);
		begin = cur + 1;
	}
}