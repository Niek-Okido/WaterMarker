#include "wxImagePanel.h"



BEGIN_EVENT_TABLE(wxImagePanel, wxPanel)
EVT_ERASE_BACKGROUND(wxImagePanel::OnEraseBackground)
EVT_PAINT(wxImagePanel::paintEvent)
EVT_SIZE(wxImagePanel::OnSize)
END_EVENT_TABLE()


wxImagePanel::wxImagePanel( wxPanel* panel, wxImage image, 
	bool border, wxSize image_size, double radius, wxColour colorTint) 
	
	: wxPanel{ panel }, image{ image }, border{ border }, radius{ radius }, image_size{ image_size }, colorTint{ colorTint }
{

	w = -1;
	h = -1;

	m_mapMode = wxMM_TEXT;
	m_xUserScale = 1.0;
	m_yUserScale = 1.0;
	m_xLogicalOrigin = 0;
	m_yLogicalOrigin = 0;
	m_xAxisReversed =
		m_yAxisReversed = false;
	#if wxUSE_DC_TRANSFORM_MATRIX
		m_transform_dx = 0.0;
		m_transform_dy = 0.0;
		m_transform_scx = 1.0;
		m_transform_scy = 1.0;
		m_transform_rot = 0.0;
	#endif // wxUSE_DC_TRANSFORM_MATRIX

	#if wxUSE_GRAPHICS_CONTEXT
		m_renderer = NULL;
		m_useAntiAliasing = true;
	#endif
	m_useBuffer = false;

	// add transparency
	#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	// Make sure we do use a graphics context when selecting one of the screens
	// DC doesn't support drawing with alpha
	// so GC is necessary both for alpha and graphics screen.

		if (!HasRenderer())
			UseGraphicRenderer(wxGraphicsRenderer::GetDefaultRenderer());
	#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT


}

void wxImagePanel::paintEvent(wxPaintEvent& evt)
{
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC dc(this);
	render(dc);
}

void wxImagePanel::paintNow()
{
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(dc);
}

void wxImagePanel::OnSize(wxSizeEvent& event) {
	this->Refresh();
	//this->Update();
	//skip the event.
	event.Skip();
}

void wxImagePanel::render(wxDC& dc)
{

	if (border)
	{
		if (m_useBuffer)
		{
			wxBufferedPaintDC bpdc(this);
			Draw(bpdc);
		}
		else
		{
			wxPaintDC pdc(this);
			Draw(pdc);
		}
	}
	else
	{
		int neww, newh;
		dc.GetSize(&neww, &newh);

		if (neww != w || newh != h)
		{
			resized = wxBitmap(image.Scale(neww, newh /*, wxIMAGE_QUALITY_HIGH*/));
			w = neww;
			h = newh;
			dc.DrawBitmap(resized, 0, 0, true);
		}
		else {
			dc.DrawBitmap(resized, 0, 0, true);
		}
	}




	// draw a rectangle
	//dc.SetBrush(*wxBLUE_BRUSH); // blue filling
	//dc.SetPen(wxPen(wxColor(255, 175, 175), 10)); // 10-pixels-thick pink outline
	//dc.DrawRectangle(wxRect(100, 100, 100, 100));

}

void wxImagePanel::Draw(wxDC& pdc)
{

#if wxUSE_GRAPHICS_CONTEXT
	wxGCDC gdc;

	if (m_renderer)
	{
		wxGraphicsContext* context;
		if (wxPaintDC* paintdc = wxDynamicCast(&pdc, wxPaintDC))
		{
			context = m_renderer->CreateContext(*paintdc);
		}
		else if (wxMemoryDC* memdc = wxDynamicCast(&pdc, wxMemoryDC))
		{
			context = m_renderer->CreateContext(*memdc);
		}
#if wxUSE_METAFILE && defined(wxMETAFILE_IS_ENH)
		else if (wxMetafileDC* metadc = wxDynamicCast(&pdc, wxMetafileDC))
		{
			context = m_renderer->CreateContext(*metadc);
		}
#endif
		else
		{
			wxFAIL_MSG("Unknown wxDC kind");
			return;
		}

		context->SetAntialiasMode(m_useAntiAliasing ? wxANTIALIAS_DEFAULT : wxANTIALIAS_NONE);

		gdc.SetBackground(GetBackgroundColour());
		gdc.SetGraphicsContext(context);
	}

	wxDC& dc = m_renderer ? static_cast<wxDC&>(gdc) : pdc;
#else
	wxDC& dc = pdc;
#endif


	PrepareDC(dc);

	//set background transparently
	dc.SetBackground(*wxTRANSPARENT_BRUSH);

	//dc.Clear();
	DrawAlpha(dc);
}

#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
void wxImagePanel::DrawAlpha(wxDC& dc)
{

	// dc.SetPen(wxPen(wxColour(128, 0, 0), 12));
	dc.SetBrush(*wxTRANSPARENT_BRUSH);

	//update color tint
	colorTint = wxColour(255, 255, 255, 127);
	dc.SetBrush(wxBrush(colorTint));

	dc.DrawRoundedRectangle(0, 0, image_size.x, image_size.y, radius);

}
#endif // wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT

void wxImagePanel::PrepareDC(wxDC& dc)
{
#if wxUSE_DC_TRANSFORM_MATRIX
	if (dc.CanUseTransformMatrix())
	{
		wxAffineMatrix2D mtx;
		mtx.Translate(m_transform_dx, m_transform_dy);
		mtx.Rotate(wxDegToRad(m_transform_rot));
		mtx.Scale(m_transform_scx, m_transform_scy);
		dc.SetTransformMatrix(mtx);
	}
#endif // wxUSE_DC_TRANSFORM_MATRIX
	dc.SetLogicalOrigin(m_xLogicalOrigin, m_yLogicalOrigin);
	dc.SetAxisOrientation(!m_xAxisReversed, m_yAxisReversed);
	dc.SetUserScale(m_xUserScale, m_yUserScale);
	dc.SetMapMode(m_mapMode);
}

#if wxUSE_GRAPHICS_CONTEXT

bool wxImagePanel::IsDefaultRenderer() const
{
	if (!m_renderer) return false;
	return m_renderer == wxGraphicsRenderer::GetDefaultRenderer();
}

void wxImagePanel::UseGraphicRenderer(wxGraphicsRenderer* renderer)
{
	m_renderer = renderer;
	if (renderer)
	{
		int major, minor, micro;
		renderer->GetVersion(&major, &minor, &micro);

		/*wxString str = wxString::Format(
			"Graphics renderer: %s %i.%i.%i (wxImagePanel.cpp::UseGraphicsRenderer)",
			renderer->GetName(), major, minor, micro
		);

		std::cout << str << std::endl;*/
	}

	this->Refresh();
	//this->Update();
}
#endif // wxUSE_GRAPHICS_CONTEXT

void wxImagePanel::OnEraseBackground(wxEraseEvent& event)
{

}
