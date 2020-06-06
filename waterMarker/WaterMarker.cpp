#include "WaterMarker.h"

namespace fs = std::filesystem;
using std::cos;
using std::sin;

std::vector<std::string> all_Folders;

void WaterMarker::start(
	const std::string logo_path, 
	cv::Mat logo_img, 
	const cv::Point loc_logo, 
	double logo_scale,
	const int radius_logo, 
	const std::array<unsigned char, 3> c_bgLogo,
	const std::string folderPath, 
	 std::vector<std::string> folderFiles = {})
{


	cv::Mat img;

	if (folderFiles.empty())
	{
		folderFiles = get_all_file_names(folderPath);
		logo_img = logo.create(logo_path);
	}


	for (std::string filename : folderFiles)
	{
		// image
		std::string image_path = folderPath + '\\' + filename;
		img = cv::imread(image_path, cv::IMREAD_UNCHANGED);

		if (!img.data) {
			std::cerr << "Could not open image filename : " << image_path << std::endl;
			continue;
		}
		// else
		//  	std::cout << "open filename : " << image_path << std::endl;

		int new_width = logo_img.cols * logo_scale;
		int new_height = logo_img.rows * logo_scale;
		int x_img = img.cols;
		int y_img = img.rows;

		// resizing to new image
		cv::Mat resized_logo = logo_img;
		if ( resized_logo.cols != new_width || resized_logo.rows != new_height )
			cv::resize(logo_img, resized_logo, cv::Point(new_width, new_height));

		double px_locX = procent_to_px(x_img, loc_logo.x);
		double px_locY = procent_to_px(y_img, loc_logo.y);

		// todo maybe cause some small issues 
		double x_value = (double)(px_locX);
		double y_value = (double)(px_locY);

		double clarity = border.clarity_color( &img, &resized_logo, cv::Point(x_value, y_value) );

		// create watermarked image
		logo.set(&img, &resized_logo,  cv::Point(x_value, y_value), clarity, radius_logo, c_bgLogo);

		//write
		std::string image_Path = folderPath + '\\' + default_folder_name + '\\' + filename;
		cv::imwrite(image_Path, img);

	}
}

double WaterMarker::centerPX_flow(double px_full, double px_section, double px_size)
{
	return (double)(px_section) / (double)(px_full) * px_size;
}

double WaterMarker::procent_to_px(const int pixels, const double procent)
{
	return (((double)(pixels) / (double)(100.00)) * procent);
}

double WaterMarker::px_to_procent(const double px_full, const double px_section)
{
	return (double)(px_section) / (double)(px_full / 100);
}

std::vector<std::string> WaterMarker::get_all_file_names(const fs::path filename, int count)
{

	if (filename.empty())
		return {};
	
	std::vector<std::string> filenames;
	for (const auto& entry : fs::directory_iterator(filename)) {
		const auto filenameStr = entry.path().filename().string();
		if (entry.is_regular_file()) 
			filenames.push_back(filenameStr);
	}

	if (count > 0)
		return std::vector<std::string>(filenames.begin(), filenames.begin() + count);

	return filenames;
}

void WaterMarker::create_folder(std::string folderDir, std::string folder_name )
{
	std::string newFolderPath = folderDir + '\\' + folder_name;
	fs::create_directories(newFolderPath);
}


void WaterMarker::run_on_threads(
	std::string folderDir, 
	std::string logo_path, 
	cv::Point logo_pos, 
	double logo_scale, 
	const int radius, 
	std::array<unsigned char,3> bg_color_logo
){
	
	create_folder(folderDir, default_folder_name);

	cv::Mat logo_img = logo.create(logo_path);
	
	std::vector<std::string> file_names = get_all_file_names(folderDir);

	int devided = (int)(file_names.size() / max_Thread_size);


	// thread 1
	if (devided == 0)
	{
		std::thread thread1([this, folderDir, file_names, logo_path, logo_img, logo_pos, logo_scale, radius, bg_color_logo]
			{ this->start(logo_path, logo_img, logo_pos, logo_scale, radius, bg_color_logo , folderDir, file_names); }
		);
		thread1.detach();
	}
	else if ((file_names.size() % max_Thread_size) > 0)
	{
		std::vector<std::string> separate_files = std::vector<std::string>
			(
				file_names.begin() + (file_names.size() - (file_names.size() % max_Thread_size)),
				file_names.begin() + (file_names.size())
				);

		std::thread thread1([this, folderDir, separate_files, logo_path, logo_img, logo_pos, logo_scale, radius, bg_color_logo]
			{ this->start(logo_path, logo_img, logo_pos, logo_scale, radius, bg_color_logo, folderDir, separate_files); }
		);
		thread1.detach();
	}

	for (int i = 1,d = 0; devided >= i && i <= max_Thread_size; d++, i++)
	{
		//std::cout << "start thread: " << i << std::endl;
		std::vector<std::string> separate_files = std::vector<std::string>
			(
				file_names.begin() + (devided * (d)),
				file_names.begin() + (devided * (d + 1))
				);

		std::thread thread([this, folderDir, separate_files, logo_path, logo_img, logo_pos, logo_scale, radius, bg_color_logo]
			{ this->start(logo_path, logo_img, logo_pos, logo_scale, radius, bg_color_logo, folderDir, separate_files); }
		);
		thread.detach();
	}

}
