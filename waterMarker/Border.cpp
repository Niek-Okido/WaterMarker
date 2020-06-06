#include "Border.h"

using std::cos;
using std::sin;


std::array<std::vector<cv::Point>, 4> BorderRadius::get_location(cv::Mat* overlay, cv::Point location, const int radius)
{
	int devided = std::min((overlay->cols / 4), (overlay->rows / 4));
	int r = devided + ( (radius * 4) - 30);

	cv::Point point;
	std::array<std::vector<cv::Point>, 4> loc_arr;

	for (int p = 360; p > 0; p--)
	{
		// right bottom
		if (p <= 90)
			point = cv::Point((location.x + overlay->cols) - r, (location.y + overlay->rows) - r);

		// left bottom
		else if (p <= 180)
			point = cv::Point(location.x + r, (location.y + overlay->rows) - r);

		// left top
		else if (p <= 270)
			point = cv::Point(location.x + r, location.y + r);

		// right top
		else if (p <= 360)
			point = cv::Point((location.x + overlay->cols) - r, location.y + r);

		point.x = (int)(r * cos(get_degrees(p)) + point.x);
		point.y = (int)(r * sin(get_degrees(p)) + point.y);

		loc_arr[p / (90 + 1)].push_back({ point.x, point.y });
	}

	return loc_arr;
}

bool Border::logo_is_dark(cv::Mat* overlay)
{
	int count = 0;
	std::array<unsigned long long, 3> arr_color = { 0 };

	for (int y = 0; y < overlay->rows; ++y)
	{
		//if (y >= overlay->rows) break;

		for (int x = 0; x < overlay->cols; ++x)
		{
			//if (x >= overlay->cols) break;

			double opacity = (((double)overlay->data[y * overlay->step + x * overlay->channels() + 3]) / 255) * brightness;

			if (opacity > 0)
			{
				count++;
				arr_color[0] += overlay->data[y * overlay->step + x * overlay->channels() + 0];
				arr_color[1] += overlay->data[y * overlay->step + x * overlay->channels() + 1];
				arr_color[2] += overlay->data[y * overlay->step + x * overlay->channels() + 2];
			}
		}
	}
	
	// get average color choose dark or light conclusion on logo
	int new_color = (arr_color[0] / count) + (arr_color[1] / count) + (arr_color[2] / count);

	if (new_color > (127 * 3))
		return false;

	return true;
}



// can cause rarely a error (get color) << location logo is not correct !!!!
double Border::clarity_color(cv::Mat* src, cv::Mat* overlay, const cv::Point& location)
{

	int sC = src->cols;
	int sR = src->rows;
	int oC = overlay->cols;
	int oR = overlay->rows;


	int sMax = std::max(sC, sR);

	double pSpace = std::floor((double)(sMax / std::max(block_size, 1)) / std::max(blocks_count, 1));
	int px_space = (double)(block_size * pSpace);

	int point_x = cv::max(location.x, 0);
	int point_y = cv::max(location.y, 0);

	int cLogo = std::min(oC, oR) / 2;

	srand(time(0));
	std::vector<int> small_to_large;

	for (int i = 0; i < blocks_count; i++)
	{
		std::array<unsigned long, 3> Ablock_color = { 0 };
		int count = 0;

		int rVal = ((rand() % cLogo) - block_size);
		if(i % 2) rVal += cLogo; // made not sticking to 1 place

		int new_point_x = sC > sR ? (point_x + (px_space * i)) : (point_x + rVal);
		int new_point_y = sC < sR ? (point_y + (px_space * i)) : (point_y + rVal);

		for (int y = new_point_y; (y - point_y) < oR; ++y)
		{
			if ((y - new_point_y) > block_size) break;

			for (int x = new_point_x; (x - point_x) < oC; ++x)
			{
				if ((x - new_point_x) > block_size) break;

				count++;

				//get color
				Ablock_color[0] += src->data[(y) * src->step + (x) * src->channels() + 0];
				Ablock_color[1] += src->data[(y) * src->step + (x) * src->channels() + 1];
				Ablock_color[2] += src->data[(y) * src->step + (x) * src->channels() + 2];

				// // set color
				// src->data[(y)*src->step + src->channels() * (x)+0] = 20;
				// src->data[(y)*src->step + src->channels() * (x)+1] = 255;
				// src->data[(y)*src->step + src->channels() * (x)+2] = 40;
   
			}
		}
		if (count > 0) {
			small_to_large.push_back(
				(unsigned char)(Ablock_color[0] / count) +
				(unsigned char)(Ablock_color[1] / count) +
				(unsigned char)(Ablock_color[2] / count)
			);
		}

	}

	Sort_small_to_large(small_to_large);

	int number_accurate = std::ceil((double)(
		(small_to_large.size() / 100.00) * std::max((double)(procent_accurate), 1.00)
	));

	int t_count = 0;

	if (logo_bg_white)
	{
		for (int i = 0; i < number_accurate; i++)// the light side add (logo dark)
			t_count += small_to_large[i];
	}
	else
	{
		for (int i = 0; i < number_accurate; i++)// the dark side add (logo light)
			t_count += small_to_large[(int)((small_to_large.size() - 1) - i)];
	}

	double clarity;
	// not using return (in future versions this will be improved)
	if (t_count <= ((70 * 3) * number_accurate))
	{
		clarity = 0.50;
	}
	else if (t_count <= ((127 * 3) * number_accurate))
	{
		clarity = 0.35;
	}
	else if (t_count <= ((177 * 3) * number_accurate))
	{
		clarity = 0.20;
	}
	else if (t_count <= ((255 * 3) * number_accurate))
	{
		clarity = 0.00;
	}
	
	return clarity;

}



void Border::Sort_small_to_large(std::vector<int>& a) {
	int i, j, min, temp;

	for (i = 0; i < (a.size() - 1); i++)
	{
		min = i;
		for (j = i + 1; j < a.size(); j++)
			if (a[j] < a[min])
				min = j;
		temp = a[i];
		a[i] = a[min];
		a[min] = temp;

	}

}


