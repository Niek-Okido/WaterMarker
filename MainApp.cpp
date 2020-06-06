#include "MainApp.h"

// IMPLEMENT_APP(MainApp) // a macro that tells wxWidgets to create an instance of our application
IMPLEMENT_APP_CONSOLE(MainApp)
bool MainApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;

	// Create the main application window
	MyFrame* frame = new MyFrame(
		_("WaterMarker"), wxPoint(1, 1), wxSize(600, 182), 
		wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)//  | wxNO_FULL_REPAINT_ON_RESIZE & wxCLIP_CHILDREN
	);

	// Show it
	frame->Show(true); // shows the window

	return true;
}


BEGIN_EVENT_TABLE(MyFrame, wxFrame)

	EVT_BUTTON(BUTTON_folder, MyFrame::Button_Folder)
	EVT_BUTTON(BUTTON_image, MyFrame::Button_Image)
	EVT_BUTTON(BUTTON_create, MyFrame::Button_Create)
	EVT_BUTTON(BUTTON_settings, MyFrame::Button_Settings)
	EVT_SLIDER(SLIDER_x, MyFrame::Save_Settings)
	EVT_SLIDER(SLIDER_y, MyFrame::Save_Settings)
	EVT_SLIDER(SLIDER_logoSize, MyFrame::Save_LogoSize)
	EVT_SLIDER(SLIDER_logoRadius, MyFrame::Save_LogoRadius)

END_EVENT_TABLE()



MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
	:wxFrame((wxFrame*)NULL, -1, title, pos, size),window_size(size)
{
	//TODO save Border color or change to needs
	//defaultColor = 
	//save_toFile(SaveState::SAVE_borderColor);

	//default settings
	defaultFolder = savestate.retrieve(SaveState::SAVE_folderDir, filename_SaveState).c_str();
	defaultImage = savestate.retrieve(SaveState::SAVE_fileDir, filename_SaveState).c_str();
	defaultLocation = savestate.retrieve(SaveState::SAVE_logoPos, filename_SaveState);
	defaultScale = savestate.retrieve(SaveState::SAVE_logoSize, filename_SaveState);
	defaultRadius = savestate.retrieve(SaveState::SAVE_borderRadius, filename_SaveState);
	
	//TODO contain some errors on "D:/images"
	if (defaultFolder != "")
		src_path = defaultFolder + '\\' + watermarker.get_all_file_names(defaultFolder, 1)[0];

	if (defaultImage != "")
		logo_path = defaultImage;

	if (defaultLocation != "")
	{
		std::vector<std::string> file = split(defaultLocation, ", ");

		defaultx = std::stod(file[0]);
		defaulty = std::stod(file[1]);
	}
	else
	{
		defaultLocation = std::to_string(defaultx) + " , " + std::to_string(defaulty);
	}

	if (defaultScale != "")
	{
		logo_scale = std::stod(defaultScale);
	}
	else
	{
		defaultScale = std::to_string(logo_scale);
	}

	if (defaultRadius != "") 
	{
		radius = std::stoi(defaultRadius);
	}
	else
	{
		defaultRadius = std::to_string(radius);
	}


		
	std::cout << "FolderPath: \t\t" << src_path << std::endl;
	std::cout << "ImagePath: \t\t" << logo_path << std::endl;
	std::cout << "defaultX: \t\t" << defaultx << std::endl;
	std::cout << "defaultY: \t\t" << defaulty << std::endl;
	std::cout << "logo_scale: \t\t" << logo_scale << std::endl;
	std::cout << "defaultRadius: \t\t" << radius << std::endl;


	panel = new wxPanel(this, -1);
	y_box_1 = new wxBoxSizer(wxVERTICAL);


	// folder
	x_box_1 = new wxBoxSizer(wxHORIZONTAL);

	txt_folder = new wxStaticText(panel, wxID_ANY, wxT("   Folder Directory : "));
	x_box_1->Add(txt_folder, 0, wxRIGHT, 8);
	y_box_1->Add(x_box_1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 15);

	Button_folder = new wxButton(panel, BUTTON_folder, defaultFolder);
	x_box_1->Add(Button_folder, 1);
	y_box_1->Add(-1, 10);


	// image button
	x_box_2 = new wxBoxSizer(wxHORIZONTAL);

	txt_image = new wxStaticText(panel, wxID_ANY, wxT("WaterMark Image : "));
	x_box_2->Add(txt_image, 0, wxRIGHT, 8);
	y_box_1->Add(x_box_2, 0, wxEXPAND | wxLEFT | wxRIGHT , 15);

	Button_image = new wxButton(panel, BUTTON_image, defaultImage);
	x_box_2->Add(Button_image, 1);


	// create button
	x_box_3 = new wxBoxSizer(wxHORIZONTAL);

	y_box_1->Add(x_box_3, 0, wxALIGN_RIGHT, 10);
	Button_create = new wxButton(panel, BUTTON_create, _("WaterMark it!") , wxDefaultPosition, wxSize(100, 15));
	x_box_3->Add(Button_create, 0, wxEXPAND | wxALL, 15);


	// settings image button

	wxPNGHandler* handler = new wxPNGHandler;
	wxImage::AddHandler(handler);

	wxBitmap bitmap = wxBitmap("./res/icon_settings.png", wxBITMAP_TYPE_PNG);
	wxImage img = bitmap.ConvertToImage();

	img.Rescale(32, 32);
	bitmap = img;

	wxBitmapButton* image = new wxBitmapButton(
		panel, BUTTON_settings, bitmap, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE
	);


	x_box_3->Add(image, 0, wxEXPAND | wxALL, 20);


	// DynamicPanel 
	DynamicPanel = new wxPanel(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL"));
	DynamicBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	DynamicPanel->SetSizer(DynamicBoxSizer);
	y_box_1->Add(DynamicPanel, 1, wxEXPAND, 5);


	// set mouse hover
	Button_folder->SetCursor(wxCursor(wxCURSOR_HAND));
	Button_image->SetCursor(wxCursor(wxCURSOR_HAND));
	Button_create->SetCursor(wxCursor(wxCURSOR_HAND));
	image->SetCursor(wxCursor(wxCURSOR_HAND));

	// connect to frame
	panel->SetSizer(y_box_1);
	this->Centre();

}

void MyFrame::Button_Folder(wxCommandEvent& event)
{
	wxDirDialog* OpenDialog = new wxDirDialog(
		this, _("the folder with all the photos"), defaultFolder
	);

	//wxFileDialog* OpenDialog = new wxFileDialog(
	//	this, _("Choose a folder to open"),
	//	wxEmptyString, wxEmptyString,
	//	_("all files (*.*)|*.*"),
	//	wxFD_OPEN, wxDefaultPosition
	//);
	
	if (OpenDialog->ShowModal() == wxID_OK)
	{
		defaultFolder = OpenDialog->GetPath().ToAscii();
		Button_folder->SetLabelText(defaultFolder);
		
		save_toFile(SaveState::SAVE_folderDir);

		//update settings info
		src_path = defaultFolder + '\\' + watermarker.get_all_file_names(defaultFolder, 1)[0];

		if (settings_clicked)//re-open it
		{
			Button_Settings(event);
			Button_Settings(event);
		}
	}

	//Clean up after ourselves
	OpenDialog->Destroy();
}

void MyFrame::Button_Image(wxCommandEvent& event)
{

	wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a file to open"), 
		wxEmptyString, wxEmptyString,
		_("PNG files (*.png)|*.png|JPG files (*.jpg)|*jpg|JPEG files (*.jpeg)|*.jpeg"),
		wxFD_OPEN, wxDefaultPosition
	);
	
	if (OpenDialog->ShowModal() == wxID_OK)
	{
		defaultImage = OpenDialog->GetPath().ToAscii();
		Button_image->SetLabel(defaultImage);
		
		save_toFile(SaveState::SAVE_fileDir);

		//update settings
		logo_path = defaultImage;
		
		if (settings_clicked)//re-open it
		{
			Button_Settings(event);
			Button_Settings(event);
		}

	}
	
	//Clean up after ourselves
	OpenDialog->Destroy();
}

void MyFrame::Button_Create(wxCommandEvent& event)
{
		
	std::cout << "\n\n\n" << std::endl;
	std::cout << "FolderPath: \t\t" << defaultFolder << std::endl;
	std::cout << "ImagePath: \t\t" << defaultImage << std::endl;
	std::cout << "location: \t\t" << defaultLocation << std::endl;
	std::cout << "scale: \t\t" << defaultScale << std::endl;
	std::cout << "radius: \t\t" << defaultRadius << std::endl;


	if (defaultFolder == "")
		Button_create->SetLabel( "Folder is empty");
	else if (defaultImage == "")
		Button_create->SetLabel("Image is empty");

	// saving logo location
	save_toFile(SaveState::SAVE_logoPos);

	// Save_LogoSize
	save_toFile(SaveState::SAVE_logoSize);
	
	// saving logo radius
	save_toFile(SaveState::SAVE_borderRadius);

	//// saving logo color
	//save_toFile(SaveState::SAVE_borderColor);


	if(defaultFolder != "" && defaultImage != "")
	{
		// save the current information to a file 
		std::thread thread1([this] { 
			this->callThread(
				defaultFolder, 
				defaultImage, 
				wxPoint(defaultx, defaulty), 
				logo_scale, 
				radius, 
				bg_color_logo); 
			}
		);
		thread1.detach();
	}

}

void MyFrame::Button_Settings(wxCommandEvent& event)
{
	settings_clicked = !settings_clicked;// expand window check

	// window expand
	if (settings_clicked)
	{
		this->SetSize(size_window_expand);

		wxInitAllImageHandlers();

		// file extensions
		if (src_path.substr(src_path.find_last_of(".") + 1) == "png")
		{
			img_src.LoadFile(src_path, wxBITMAP_TYPE_PNG);
		}
		else if (
			src_path.substr(src_path.find_last_of(".") + 1) == "jpg" || 
			src_path.substr(src_path.find_last_of(".") + 1) == "jpeg"
		){
			img_src.LoadFile(src_path, wxBITMAP_TYPE_JPEG);
		}

		if (logo_path.substr(logo_path.find_last_of(".") + 1) == "png")
		{
			img_logo.LoadFile(logo_path, wxBITMAP_TYPE_PNG);
		}
		else if (
			logo_path.substr(logo_path.find_last_of(".") + 1) == "jpg" || 
			logo_path.substr(logo_path.find_last_of(".") + 1) == "jpeg"
		){
			img_logo.LoadFile(logo_path, wxBITMAP_TYPE_JPEG);
		}


		//resize

		new_width_img = img_src.GetWidth();
		new_height_img = img_src.GetHeight();

		new_width_logo = img_logo.GetWidth();
		new_height_logo = img_logo.GetHeight();

		max_logo_width = img_logo.GetWidth();
		max_logo_height = img_logo.GetHeight();

		float src_scale_H = (float)((size_window_expand.GetHeight() - size_window_default.GetHeight()) - (src_padding * 4)) / (float)(new_height_img);
		float src_scale_W =  (float)(size_window_expand.GetWidth() - (src_padding * 4)) / (float)(new_width_img);
		
		//take biggest
		src_scale = std::min(src_scale_H, src_scale_W);

		new_width_img = new_width_img * src_scale;
		new_height_img = new_height_img * src_scale;

		new_width_logo = (new_width_logo * src_scale) * logo_scale;
		new_height_logo = (new_height_logo * src_scale) * logo_scale;

		//Testing

		max_logo_width = max_logo_width * src_scale;
		max_logo_height = max_logo_height * src_scale;

		new_width_logo = std::min(new_width_logo, max_logo_width);
		new_height_logo = std::min(new_height_logo, max_logo_height);

		//OOOO//

		//-----//
		

		double px_locX = procent_to_px(new_width_img, defaultx);
		double px_locY = procent_to_px(new_height_img, defaulty);
		double center_logoX = centerPX_flow(new_width_img, new_width_logo, px_locX);
		double center_logoY = centerPX_flow(new_height_img, new_height_logo, px_locY);

		wxSize size_info = wxSize(
			new_width_logo,	// width
			new_height_logo	// height
		);

		// draw image src
		draw_src = new wxImagePanel(DynamicPanel, img_src);
		draw_src->SetPosition(wxPoint(
			(src_padding / 2), 
			(src_padding / 2)
		));

		draw_src->SetSize(new_width_img, new_height_img);
		DynamicBoxSizer->Add(draw_src, 0);


		// draw logo src
		draw_logo = new wxImagePanel(DynamicPanel, img_logo, true, size_info, radius);


		//TODO made this flowing from left to right so use a piece of calculation 
		int x, y;
		x = (px_locX + (src_padding / 2));
		y = (px_locY + (src_padding / 2));

		if (px_locX > (new_width_img / 2))
			x += (src_padding / 2);
		if (px_locY > (new_height_img / 2))
			y += (src_padding / 2);


		draw_logo->SetPosition(wxPoint(x, y));

		//Testing
		draw_logo->SetSize(size_info.x, size_info.y);
		DynamicBoxSizer->Add(draw_logo, 0);


		// draw sliders

		int left_src = (((src_padding / 2)) + (new_width_logo / 2));
		int top_src = ((src_padding / 2) + (new_height_logo / 2));

		// - 13 to get the point of the slider accurate
		p_slider_x = wxPoint(left_src - 13, (new_height_img + 10));
		p_slider_y = wxPoint((new_width_img + 10), top_src - 13);
		
		// + 13 to get the point of the slider accurate
		s_slider_x = wxSize(((new_width_img + 13) - (left_src + (new_width_logo/2) - src_padding)), 20);
		s_slider_y = wxSize(20, (new_height_img + 13) - (top_src + (new_height_logo / 2) - src_padding));

		// + 13 to get the point of the slider accurate
		p_slider_size = wxPoint(40, (new_height_img + 40));
		s_slider_size = wxSize(((new_width_img + 13) / 2), 18);

		// + 13 to get the point of the slider accurate
		int xP_size = (p_slider_size.x + s_slider_size.GetWidth());
		p_slider_radius = wxPoint((new_width_img + 10) - 100, (new_height_img + 40));
		s_slider_radius = wxSize(100, 18);


		// set sliders

		sliderx = new wxSlider(
			DynamicPanel, SLIDER_x, x ,
			0,
			(new_width_img - new_width_logo),
			p_slider_x, s_slider_x, wxSL_TOP
		);
		DynamicBoxSizer->Add(sliderx, 0);

		slidery = new wxSlider(
			DynamicPanel, SLIDER_y, y,
			0,
			(new_height_img - new_height_logo),
			p_slider_y, s_slider_y, wxSL_LEFT
		);
		DynamicBoxSizer->Add(slidery, 0);

		//text
		txt_size = new wxStaticText(DynamicPanel, wxID_ANY, wxT(" size : "), wxPoint(10, (new_height_img + 43)));
		DynamicBoxSizer->Add(txt_size);

			
		if (new_width_logo > new_height_logo)
			mV_sliderSize = max_logo_width;
		else 
			mV_sliderSize = max_logo_height;
		
		if (val_size == -1)
			val_size = std::max(new_width_logo, new_height_logo);
		

		sliderSize = new wxSlider(
			DynamicPanel, SLIDER_logoSize, 
			val_size,
			0, 
			mV_sliderSize,
			p_slider_size, s_slider_size
		);
		DynamicBoxSizer->Add(sliderSize, 0);

		//text
		txt_radius = new wxStaticText(DynamicPanel, wxID_ANY, wxT(" radius : "), wxPoint(((new_width_img + 10) - 150), (new_height_img + 43)));
		DynamicBoxSizer->Add(txt_radius);

		sliderRadius = new wxSlider(
			DynamicPanel, SLIDER_logoRadius,
			radius,
			0,
			27,
			p_slider_radius, s_slider_radius
		);
		DynamicBoxSizer->Add(sliderRadius, 0);


	}
	else 
	{
		DynamicPanel->GetSizer()->Clear(true);
		this->SetSize(size_window_default);
	}


}


void MyFrame::Save_Settings(wxCommandEvent& event)
{
	int old_x, old_y;
	draw_logo->GetPosition(&old_x, &old_y);

	int new_x = old_x;
	int new_y = old_y;

	int new_val = event.GetInt() + (src_padding / 2);
	if(event.GetId() == SLIDER_x)
		new_x = new_val ;
	else
		new_y = new_val;

	draw_logo->SetPosition(wxPoint(new_x, new_y));

	draw_src->Refresh(false);
	draw_logo->Refresh(false);
 

	//TODO made this flowing from left to right 
	int x, y;
	x = new_x - (src_padding / 2);
	y = new_y - (src_padding / 2);

	if (new_x > (new_width_img / 2))  x -= (src_padding / 2);
	if (new_y > (new_height_img / 2)) y -= (src_padding / 2);

	defaultx = px_to_procent(new_width_img, x);
	defaulty = px_to_procent(new_height_img, y);

	// saving to file
	defaultLocation = std::to_string(defaultx) + " , " + std::to_string(defaulty);

	if ((new_width_logo * l_scale) >= max_logo_width ||
		(new_height_logo * l_scale) >= max_logo_height
	){
		return;
	}

	mV_sliderSize = std::max(
		((double)(new_height_img)-(double)((new_height_img - new_height_logo) * ((double)(defaulty) / 100.00)))
		,
		((double)(new_width_img)-(double)((new_width_img - new_width_logo) * ((double)(defaultx) / 100.00)))
	);

	//update size slider
	if (event.GetId() == SLIDER_x)
		std::cout << "mV_sliderSize x-bar : " << mV_sliderSize << std::endl;
	else
		std::cout << "mV_sliderSize y-bar : " << mV_sliderSize << std::endl;

	sliderSize->SetMax(mV_sliderSize);
	

}

void MyFrame::Save_LogoSize(wxCommandEvent& event)
{

	// update val size
	val_size = event.GetInt();
	l_scale = val_size / std::max((double)(new_width_logo), (double)(new_height_logo));

	//position bottom increase top i.p.o bottom//
	wxPoint point = draw_logo->GetPosition();

	int new_w = (new_width_logo * l_scale);
	int new_h = (new_height_logo * l_scale);

	//protect against overflowing the border
	if ((new_w >= max_logo_width) ||
		(new_h >= max_logo_height)||
		(point.x + new_w - (src_padding / 2)) >= new_width_img ||
		(point.y + new_h - (src_padding / 2)) >= new_height_img
	){
		return;
	}

	//TODO need to change to avoid flickering (Save_LogoRadius as example)
	draw_src = new wxImagePanel(DynamicPanel, img_src);
	draw_src->SetPosition(wxPoint((src_padding / 2), (src_padding / 2)));
	draw_src->SetSize(new_width_img, new_height_img);
	DynamicBoxSizer->Add(draw_src, 0);

	draw_logo = new wxImagePanel(DynamicPanel, img_logo, true, wxSize(new_w, new_h), radius);
	draw_logo->SetPosition(point);
	draw_logo->SetSize(new_w, new_h);
	DynamicBoxSizer->Add(draw_logo, 0);

	draw_logo->Update();
	draw_src->Update();

	//update max value slider x & y
	sliderx->SetMax((new_width_img - new_w));
	slidery->SetMax((new_height_img - new_h));

	// save data
	double max_value = std::max(max_logo_width, max_logo_height);
	logo_scale = val_size / max_value;
	defaultScale = std::to_string((double)(logo_scale));
	
}

void MyFrame::Save_LogoRadius(wxCommandEvent& event)
{
	radius = event.GetInt();
	std::cout << "radius value: " << radius << std::endl;
	// small flickering 

	draw_logo->radius = radius;

	draw_src->Refresh(false);
	draw_logo->Refresh(false);

	// saving to file
	defaultRadius = std::to_string(radius);

}


void MyFrame::callThread( 
	std::string folderDir, 
	std::string logo_path, 
	wxPoint pos, 
	double logo_scale, 
	const int logo_radius, 
	std::array<unsigned char,3> bg_color
){

	//set button text to "Working"
	Button_create->SetLabel("Working...");

	watermarker.run_on_threads(
		folderDir, 
		logo_path, 
		cv::Point(pos.x, pos.y), 
		logo_scale, 
		logo_radius, 
		bg_color
	);

	// finished
	Button_create->SetLabel("Start WaterMaker");
}

double MyFrame::centerPX_flow(double px_full, double px_section, double px_size)
{
	double a = (double)(px_section) / (double)(px_full);
	return a * px_size;
}

double MyFrame::procent_to_px(const int pixels, const double procent)
{
	return (((double)(pixels) / (double)(100.00)) * procent);
}

double MyFrame::px_to_procent(const double px_full, const double px_section)
{
	return (double)(px_section) / (double)(px_full / 100);
}

void MyFrame::save_toFile(enum SaveState::Saving_enum enum_value)
{
	bool saved = false;

	switch (enum_value)
	{
	case SaveState::SAVE_folderDir:
		saved = savestate.saving(enum_value, filename_SaveState, defaultFolder);
		break;
	case SaveState::SAVE_fileDir:
		saved = savestate.saving(enum_value, filename_SaveState, defaultImage);
		break;
	case SaveState::SAVE_logoPos:
		saved = savestate.saving(enum_value, filename_SaveState, defaultLocation);
		break;
	case SaveState::SAVE_logoSize:
		saved = savestate.saving(enum_value, filename_SaveState, defaultScale);
		break;
	case SaveState::SAVE_borderRadius:
		saved = savestate.saving(enum_value, filename_SaveState, defaultRadius);
		break;
	// 	case SaveState::SAVE_borderColor:
	// 		saved = savestate.saving(enum_value, filename_SaveState, defaultColor);
	// 		break;

	}

	// update saving file 
	if (!saved)
		std::cerr << "Error saving Folder directory to file MainApp.cpp:82 in MainFrame::Button_Folder()" << std::endl;
	

}


void file_exists(const fs::path& p, fs::file_status s = fs::file_status{})
{
	std::cout << p;
	if (fs::status_known(s) ? fs::exists(s) : fs::exists(p))
		std::cout << " exists\n";
	else
		std::cout << " does not exist\n";
};

// for string delimiter
std::vector<std::string> MyFrame::split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
}
