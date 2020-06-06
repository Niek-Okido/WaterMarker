#pragma once
// example : D:\Programming\learn\C.C++_learn\wxWidgets\wxWidgets\samples\drawing


#include "wx/colordlg.h"
#include "wx/dcbuffer.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/settings.h"

#include <iostream>



class wxImagePanel : public wxPanel
{
public:
	double radius;
#if wxUSE_GRAPHICS_CONTEXT
	wxGraphicsRenderer* m_renderer;
	bool m_useAntiAliasing;
#endif

	wxImagePanel(
		wxPanel* frame, 
		wxImage image, 
		bool border = false, 
		wxSize image_size = { 0, 0 },
		double radius = 0.00,
		wxColour colorTint = { 255, 255, 255, 127 }
	);

private:
	wxImage image;
	bool border;
	wxSize image_size;
	wxColour colorTint;
	bool m_useBuffer;

	int w, h;
#if wxUSE_DC_TRANSFORM_MATRIX
	wxDouble    m_transform_dx, m_transform_dy;
	wxDouble    m_transform_scx, m_transform_scy;
	wxDouble    m_transform_rot;
#endif // wxUSE_DC_TRANSFORM_MATRIX
	int         m_xLogicalOrigin, m_yLogicalOrigin;
	bool        m_xAxisReversed, m_yAxisReversed;
	double      m_xUserScale, m_yUserScale;
	wxMappingMode m_mapMode;
	wxBitmap resized;

	void paintEvent(wxPaintEvent& evt);
	void paintNow();

	void OnSize(wxSizeEvent& event);
	void render(wxDC& dc);
	void Draw(wxDC& pdc);

#if wxDRAWING_DC_SUPPORTS_ALPHA || wxUSE_GRAPHICS_CONTEXT
	void DrawAlpha(wxDC& dc);
#endif

	void PrepareDC(wxDC& dc);

#if wxUSE_GRAPHICS_CONTEXT
	bool HasRenderer() const { return m_renderer != NULL; }
	bool IsDefaultRenderer() const;

	void UseGraphicRenderer(wxGraphicsRenderer* renderer);
	wxGraphicsRenderer* GetRenderer() const { return m_renderer; }
#endif // wxUSE_GRAPHICS_CONTEXT

	void OnEraseBackground(wxEraseEvent& event);

	DECLARE_EVENT_TABLE()

};
