#pragma once

#include <vector>
#include <array>
#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 

#include <opencv2/opencv.hpp>





class BorderRadius
{
private:
	double get_degrees(double input)
	{
		// pi/180
		const double halfC = 3.14159265358979323846 / 180; 
		return input * halfC;
	}

public:
	std::array<std::vector<cv::Point>, 4> get_location(cv::Mat* overlay, cv::Point location, const int radius);
};

class Border
{
private:
	double brightness = 0.90;		// 0.30 = 30% sticking out transparency

	int blocks_count = 140;
	int block_size = 5;

	int procent_accurate = 90; // 0 - 100%   // for now prefer to use 2

	void Sort_small_to_large(std::vector<int>& a);


public:	

	int padding = 10;
	int margin = 30;

	double clarity_color(cv::Mat* src, cv::Mat* overlay, const cv::Point& location);

	bool logo_bg_white; // white (dark logo) or black(light logo) 
	bool logo_is_dark(cv::Mat* overlay);

};

