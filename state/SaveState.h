#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>

class SaveState
{
public:
	std::string split_by = "\n##%$#TUYTEL#$%##\n";

	enum Saving_enum
	{
		SAVE_null,
		SAVE_folderDir = 1,
		SAVE_fileDir,
		SAVE_logoPos,
		SAVE_logoSize,
		SAVE_borderRadius,
		SAVE_borderColor,
		SAVE_last
	};


	bool saving(Saving_enum saving, const char* filename, std::string content);


	 std::string retrieve(Saving_enum enum_name, const char* filename);


private:

	void writeFile(const char* filename, std::string content);
	//std::string splitFile(const char* filename, std::string p_content, Saving_enum saving );
	std::string splitFile(const char* filename, int index_split);


		
};

