#include "SaveState.h"


std::string SaveState::splitFile(const char* filename, int enum_index)
{
	std::ifstream file(filename);
	std::string line;
	std::string str = "";

	if (!file.is_open())//file not exist
		return "";

	int part = 0;
	std::array<std::string, (int)(SAVE_last)> split;

	while (std::getline(file, line))
	{
		if (line.find(split_by.substr(1, split_by.size() - 2)) != std::string::npos)
			part++;
		else
			split[part] = split[part].append(line);
	}
	file.close();
	
	return split[enum_index - 1];// .c_str();
}

void SaveState::writeFile(const char* filename, std::string content)
{
	std::ofstream file;
	file.open(filename);
	file << content;
	file.close();
}


bool SaveState::saving(Saving_enum enum_name, const char* filename, std::string content)
{

	std::string building_file;
	//std::array<std::string, 6> value_content;


	for (int i = 0; i < (int)(SAVE_last); i++)
	{
		if (i == ((int)(enum_name) - 1))
		{
			building_file.append(content);
			building_file.append(split_by);
		}
		else
		{
			building_file.append(splitFile(filename, (i + 1)));
			building_file.append(split_by);
		}
	}

	writeFile(filename, building_file);

	return true;
}


std::string SaveState::retrieve(Saving_enum enum_name, const char* filename)
{
	return splitFile(filename, (int)(enum_name));
}
