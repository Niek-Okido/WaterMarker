#pragma once

#include "wx/wx.h"
#include "wx/dc.h"

#include "wxImagePanel.h"

#include "state/SaveState.h"
#include "waterMarker/WaterMarker.h"
#include "waterMarker/Border.h"
#include "waterMarker/Logo.h"

#include "wx/colordlg.h"
#include "wx/dcbuffer.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/settings.h"

#include <opencv2/opencv.hpp>

#include <string>
#include <iostream>
#include <streambuf>
#include <thread>

// Standard DC supports drawing with alpha on OSX and GTK3.
#if defined(__WXOSX__) || defined(__WXGTK3__)
#define wxDRAWING_DC_SUPPORTS_ALPHA 1
#else
#define wxDRAWING_DC_SUPPORTS_ALPHA 0
#endif // __WXOSX__ || __WXGTK3__


class MainApp : public wxApp 
{
public:
	virtual bool OnInit();
};


enum
{
	BUTTON_create = wxID_HIGHEST,
	BUTTON_folder,
	BUTTON_image,
	BUTTON_settings,
	PANEL_2,
	SLIDER_x, SLIDER_y,
	SLIDER_logoSize,
	SLIDER_logoRadius,
	MOUSE_handle
};

class MyFrame : public wxFrame
{
private:

	wxSize window_size;
	wxSize size_window_default = wxSize(600, 182);
	wxSize size_window_expand = wxSize(600, 600);


	WaterMarker watermarker;
	SaveState savestate;
	BorderRadius border_radius;
	Border border;
	Logo logo;

public:
	
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style);


private:
	const char* filename_SaveState = ".saved_data_waterMarker.txt";

	std::string defaultFolder	= "";
	std::string defaultImage	= "";
	std::string defaultLocation = "";
	std::string defaultScale	= "";
	std::string defaultRadius	= "";
	//std::string defaultColor	= "";


	std::string src_path		= "./res/example.jpg";
	std::string logo_path		= "./res/my_logo.png";
	double defaultx				= 0.00;
	double defaulty				= 0.00;
	double logo_scale			= 0.40;
	int    radius				= 5;
	std::array<unsigned char, 3> bg_color_logo = { 255, 255, 255 };


	int new_width_img, 
		new_height_img;

	int new_width_logo, 
		new_height_logo;

	float src_scale;
	double l_scale = 1.00;
	int src_padding = 20;

	int max_logo_width;
	int max_logo_height;
	int val_size = -1;





	//--UI--//

	wxPanel* panel;
	wxPanel* DynamicPanel;

	wxStaticText* txt_folder;
	wxStaticText* txt_size;
	wxStaticText* txt_radius;
	wxStaticText* txt_image;

	wxBoxSizer* x_box_1;
	wxBoxSizer* x_box_2;
	wxBoxSizer* x_box_3;
	wxBoxSizer* DynamicBoxSizer;
	wxBoxSizer* x_box_4;
	wxBoxSizer* x_box_5;

	wxBoxSizer* y_box_1;

	wxBitmap image_src;
	wxBitmap image_logo;

	wxBitmapButton* bitButton_settings;

	wxButton* Button_folder;
	wxButton* Button_image;
	wxButton* Button_create;

	wxImagePanel* draw_src;
	wxImagePanel* draw_logo;

	wxImage img_src;
	wxImage img_logo;

	wxSlider* sliderx;
	wxSlider* slidery;
	wxSlider* sliderSize;
	wxSlider* sliderRadius;

	wxPoint p_slider_x;
	wxPoint p_slider_y;
	wxPoint p_slider_size;
	wxPoint p_slider_radius;

	wxSize s_slider_x;
	wxSize s_slider_y;
	wxSize s_slider_size;
	wxSize s_slider_radius;


	int mV_sliderSize;
	bool settings_clicked = false;

	void Button_Folder(wxCommandEvent& event);
	void Button_Image(wxCommandEvent& event);
	void Button_Create(wxCommandEvent& event);

	void Button_Settings(wxCommandEvent& event);
	
	void Save_Settings(wxCommandEvent& event);
	void Save_LogoSize(wxCommandEvent& event);
	void Save_LogoRadius(wxCommandEvent& event);

	std::vector<std::string> split(std::string s, std::string delimiter);

	double px_to_procent(const double px_full, const double px_section);
	double procent_to_px(const int pixels, const double procent);
	double centerPX_flow(double px_full, double px_section, double px_size);

	void save_toFile(enum SaveState::Saving_enum enum_value);

	DECLARE_EVENT_TABLE()

public:

	void callThread(
		std::string folderPath, 
		std::string logo_path, 
		wxPoint pos, 
		double logo_scale, 
		const int radius, 
		std::array<unsigned char, 3> color
	);

};

