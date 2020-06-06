#include "Logo.h"

void Logo::set(cv::Mat* src, cv::Mat* overlay, const cv::Point& location, double clarity, const int radius_logo, std::array<unsigned char, 3> c_bgLogo)
{

	std::array<std::vector<cv::Point>, 4> loc_radius = border_radius.get_location(overlay, location, radius_logo);

	for (int y = cv::max((location.y), 0); y < src->rows; ++y)
	{
		int fY = y - (location.y);
		if (fY >= overlay->rows) break;

		for (int x = cv::max((location.x), 0); x < src->cols; ++x)
		{
			int fX = x - (location.x);
			if (fX >= overlay->cols) break;

			double opacity = (((double)overlay->data[fY * overlay->step + fX * overlay->channels() + 3]) / 255) * this->brightness;
			double limpid = clarity;


			if (opacity < 1)//background
			{
				// get point in img 
				if (fX > (overlay->cols / 2))
				{
					if (fY > (overlay->rows / 2))
					{
						// bottom right
						for (cv::Point p : loc_radius[0])
							if (p.x < x && p.y < y)
								limpid = 0.00;
					}
					else
					{
						//top right
						for (cv::Point p : loc_radius[3])
							if (p.x < x && p.y > y)
								limpid = 0.00;
					}
				}
				else
				{
					if (fY > (overlay->rows / 2))
					{
						// bottom left
						for (cv::Point p : loc_radius[1])
							if (p.x > x && p.y < y)
								limpid = 0.00;
					}
					else
					{
						// top left
						for (cv::Point p : loc_radius[2])
							if (p.x > x && p.y > y)
								limpid = 0.00;
					}
				}


				for (int c = 0; c < src->channels(); ++c)
				{
					unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];

					//if logo is light // set opposite 
					if (!border.logo_bg_white) c_bgLogo = { 0, 0, 0 };

					int px_color = srcPx * (1. - limpid) + c_bgLogo[c] * limpid;// color background logo

					if (px_color > 255)	
						px_color = 255;
					else if (px_color < 0)		
						px_color = 0;

					src->data[y * src->step + src->channels() * x + c] = px_color;

				}
			}

			if (opacity > 0) {

				// get point in img 
				if (fX > (overlay->cols / 2))
				{
					if (fY > (overlay->rows / 2))
					{
						// bottom right
						for (cv::Point p : loc_radius[0])
							if (p.x < x && p.y < y)
								opacity = 0.00;
					}
					else
					{
						//top right
						for (cv::Point p : loc_radius[3])
							if (p.x < x && p.y > y)
								opacity = 0.00;
					}
				}
				else
				{
					if (fY > (overlay->rows / 2))
					{
						// bottom left
						for (cv::Point p : loc_radius[1])
							if (p.x > x && p.y < y)
								opacity = 0.00;
					}
					else
					{
						// top left
						for (cv::Point p : loc_radius[2])
							if (p.x > x && p.y > y)
								opacity = 0.00;
					}
				}


				for (int c = 0; c < src->channels(); ++c)
				{
					unsigned char overlayPx = overlay->data[fY * overlay->step + fX * overlay->channels() + c];
					unsigned char srcPx = src->data[y * src->step + x * src->channels() + c];
					src->data[y * src->step + src->channels() * x + c] = srcPx * (1. - opacity) + overlayPx * opacity;
				}
			}

		}
	}
	//Testing

	//test code here
	
	//Testing
}

cv::Mat Logo::create(std::string logo_path)
{
	cv::Mat logo_img = cv::imread(logo_path, cv::IMREAD_UNCHANGED);

	if (!logo_img.data)
		std::cerr << "Could not open watermark filename :" << logo_path << std::endl;

	// border padding
	cv::copyMakeBorder(logo_img, logo_img,
		border.padding, border.padding, border.padding, border.padding,//top bottom left right
		cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0, 0)
	);


	//choose color logo (white or black) size
	border.logo_bg_white = border.logo_is_dark(&logo_img);

	return logo_img;
}

