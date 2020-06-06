#pragma once

#include <vector>
#include <array>

#include <opencv2/opencv.hpp>

#include <stdio.h> 
#include <stdlib.h> 
#include<time.h> 

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
	int blocks_count = 140;
	int block_size = 5;

	int procent_accurate = 90; // 0 - 100%   // for now prefer to use 2
	bool logo_bg_white = true; // white (dark logo) or black(light logo) 

	void Sort_small_to_large(std::vector<int>& a);
	bool logo_is_dark(cv::Mat* overlay);


public:	

	int padding = 10;// 10 pixels width border
	// std::array<unsigned char, 3> color = { 255, 255, 255 };// RGB border
	int margin = 30;	// x > y ? x_img / 30% margin from bottom and right side of main image

	double clarity_color(cv::Mat* src, cv::Mat* overlay, const cv::Point& location);

};

