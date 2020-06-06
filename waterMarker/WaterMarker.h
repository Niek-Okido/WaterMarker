#pragma once

#include <string>
#include <vector>
#include <array>
#include <filesystem>

#include <iostream>
#include <fstream>


#include <opencv2/opencv.hpp>

#include "Border.h"
#include "Logo.h"

namespace fs = std::filesystem;

class WaterMarker
{
private:
	BorderRadius border_radius;
	Border border;
	Logo logo;


	std::string default_folder_name = "water_marked"; // folder name


	void create_folder(std::string folderDir, std::string new_folder);

	double px_to_procent(const double px_full, const double px_section);
	double procent_to_px(const int pixels, const double procent);
	double centerPX_flow(double px_full, double px_section, double px_size);

public:


	//TODO next update:
	//get all folders after this directory add to <vector> array
	//getall_folders(folderPath);
	void start( 
		const std::string logo_path, 
		cv::Mat logo_img, 
		const cv::Point loc_logo, 
		double logo_scale,
		const int radius_logo,
		const std::array<unsigned char, 3> color_border,
		const std::string folderPath, 
		std::vector<std::string> folderFiles
	);

	/*const int radius = 10;
	std::array<unsigned char, 3> bg_color_logo = {255, 255, 255};*/



	int max_Thread_size = 5;

	void run_on_threads(
		std::string folderDir, 
		std::string logo_path, 
		const cv::Point loc_logo,
		double logo_scale,
		const int radius_logo,
		const std::array<unsigned char, 3> color_border
	);


	// https://www.bfilipek.com/2019/04/dir-iterate.html
	std::vector<std::string> get_all_file_names(const fs::path, int count = 0);
};

