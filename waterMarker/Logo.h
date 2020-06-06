#pragma once

#include <vector>
#include <array>
#include <opencv2/opencv.hpp>

#include "Border.h" 


class Logo
{
private:
	BorderRadius border_radius;
	Border border;

public:
	double brightness = 0.90;		// 0.30 = 30% sticking out transparency
	double resize_scale = 0.30;			// dimensions 30% of image
	// double reduce_step = 0.99;			// reduce 1% per round

	void set(cv::Mat* src, cv::Mat* overlay, const cv::Point& location, double border_color, const int radius_logo, std::array<unsigned char, 3> c_bgLogo);

	cv::Mat create(std::string logo_path);
};

