///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormGResEncoder' on 2026-05-09 14:29:43
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
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/treectrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/spinctrl.h>
#include <wx/timer.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/notebook.h>
#include <wx/radiobut.h>
#include <wx/scrolbar.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormGResEncoder' on 2026-05-09 14:29:43

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////


class ProcTh : public wxThread
{
public:

	enum Event : int
	{
		STATUS,
		DONE
	};

	class Status
	{
	public:
		std::vector<std::string> status;
		wxBitmap src;
	};

	class Params
	{
	public:
		std::mutex *mutex;
		std::vector<std::filesystem::path> *list;
		std::vector<std::string>* failed_list;
		std::filesystem::path target_dir;
		int x_offset;
		int y_offset;
		int dither_randomize;
	};

	ProcTh(wxFrame* parent,Params& config);
	virtual ExitCode Entry();

private:
	wxFrame* m_parent;
	Params m_config;

	void SetStatusCallback(std::vector<std::string> &info, wxBitmap *src=NULL);
};


// graphic resource metadata
class SpellGresInfo {
public:
	std::wstring path;
	std::string info_name;
	std::string name;
	std::string img_name;
	std::vector<std::string> img_names;
	std::string pal_name;
	std::string colors_str;
	std::string format;
	int x_size;
	int y_size;
	int y_offset;
	int shadow_color[3];
	bool is_transparent;

	SpellGresInfo();
	void Clear();
	int LoadInfo(std::wstring path);
	bool isLoaded();
	bool isPNM() {return(format == "PNM");};
	bool isUnitsFSU() { return(format == "UNITS.FSU"); };
};


///////////////////////////////////////////////////////////////////////////////
/// Class FormGResEncoder
///////////////////////////////////////////////////////////////////////////////
class FormGResEncoder : public wxFrame
{
private:
	
	SpellData *spell_data;

	wxBitmap m_source;
	std::mutex m_source_mutex;
	SpellPalette m_pal;
	SpellGraphicItem m_gres;
	SpellGresInfo m_info;
	
	std::vector<ProcTh*> m_threads;
	std::mutex m_mutex;
	std::vector<std::filesystem::path> m_task_list;
	std::vector<std::string> m_task_failed_list;
	int m_thread_active;

	int LoadResource(std::wstring path, int frame_id=-1);
	
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

	void OnThreadEvent(wxThreadEvent& event);

protected:
	
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormGResEncoder' on 2026-05-09 14:29:43
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
		wxID_SPIN_EX_OFS,
		wxID_SPIN_EY_OFS,
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
	wxStaticText* m_staticText115;
	wxSpinCtrl* spinExtraXoffset;
	wxStaticText* m_staticText116;
	wxSpinCtrl* spinExtraYoffset;
	wxStaticLine* m_staticline42;
	wxButton* btnRegen;
	wxButton* btnRegenPalette;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormGResEncoder' on 2026-05-09 14:29:43

public:
	
	
	static const int wxID_PROC_THREAD = 7000;

	FormGResEncoder(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("Graphics Resource Encoder"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(1600,800),long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL);
	~FormGResEncoder();

};
