///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/frame.h>

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormGResEncoder
///////////////////////////////////////////////////////////////////////////////
class FormGResEncoder : public wxFrame
{
private:
	
	// graphic resource metadata
	class SpellGresInfo {
	public:
		std::wstring path;
		std::string info_name;
		std::string name;
		std::string img_name;
		std::string pal_name;
		std::string colors_str;
		int x_size;
		int y_size;
		bool is_transparent;

		SpellGresInfo();
		void Clear();
		int LoadInfo(std::wstring path);
		bool isLoaded();
	};
	
	
	SpellData *spell_data;

	wxBitmap m_source;
	SpellPalette m_pal;
	SpellGraphicItem m_gres;
	SpellGresInfo m_info;

	int LoadResource(std::wstring path);
	
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnSaveClick(wxCommandEvent& event);
	void OnSaveAllClick(wxCommandEvent& event);
	void OnSavePalClick(wxCommandEvent& event);
	void OnOpenClick(wxCommandEvent& event);
	void OnRegenClick(wxCommandEvent& event);
	void OnSelectClick(wxCommandEvent& event);
	void OnRegenPaletteClick(wxCommandEvent& event);

	
	void OnPaintPalette(wxPaintEvent& event);
	void OnPaintSource(wxPaintEvent& event);
	void OnPaintResult(wxPaintEvent& event);

	


protected:

	enum
	{
		wxID_MM_OPEN = 5999,
		wxID_MM_SAVE,
		wxID_MM_SAVE_ALL,
		wxID_MM_SAVE_PAL,
		wxID_MM_EXIT,
		wxID_SBAR,
		wxID_LB_LIST,
		wxID_CANVAS_SRC,
		wxID_CANVAS_RES,
		wxID_PALETTE,
		wxID_SLIDE_MIN_DITHER,
		wxID_BTN_REGEN,
		wxID_BTN_REGEN_PAL,
	};

	wxMenuBar* m_menubar12;
	wxMenu* mmFile;
	wxStatusBar* sbar;
	wxStaticText* m_staticText103;
	wxListBox* lboxList;
	wxStaticLine* m_staticline37;
	wxStaticText* m_staticText99;
	wxPanel* canvasSrc;
	wxStaticLine* m_staticline33;
	wxStaticText* m_staticText100;
	wxPanel* canvasRes;
	wxStaticLine* m_staticline34;
	wxStaticText* m_staticText102;
	wxPanel* palette;
	wxStaticLine* m_staticline36;
	wxStaticText* m_staticText101;
	wxSlider* slideMinDither;
	wxStaticLine* m_staticline35;
	wxButton* btnRegen;
	wxButton* btnRegenPalette;

public:

	FormGResEncoder(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("Graphics Resource Encoder"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(1350,700),long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL);
	~FormGResEncoder();

};
