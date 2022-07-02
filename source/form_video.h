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
#include <wx/stattext.h>
#include <wx/panel.h>
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

#include "spellcross.h"
#include "spell_video.h"



///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormVideo
///////////////////////////////////////////////////////////////////////////////
class FormVideo : public wxFrame
{
private:

	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnOpenClick(wxCommandEvent& event);
	void OnChangeOffst(wxCommandEvent& event);
	void OnSaveFramesClick(wxCommandEvent& event);
	void OnPlayClick(wxCommandEvent& event);
	void OnStopClick(wxCommandEvent& event);
	void OnPaintCanvas(wxPaintEvent& event);
	void OnNewAudioFrame(wxThreadEvent& event);

	void StopPlayback();	
	void cbNewAudioFrame();

	SpellData *m_spell_data;
	SpellVideo *m_data;

	SpellSound *m_sound;

	int m_zoom;
	

protected:
	enum
	{
		wxID_MM_OPEN = 1000,
		wxID_MM_SAVE_FRAMES,
		wxID_MM_EXIT,
		wxID_CANVAS,
		wxID_DATA_POS,
		wxID_SLIDE_OFS,
		wxID_BTN_PLAY,
		wxID_BTN_STOP
	};

	wxMenuBar* mmenu;
	wxMenu* mmFile;
	wxStaticText* m_staticText64;
	wxPanel* canvas;
	wxStaticText* m_staticText65;
	wxSlider* slideOffset;
	wxButton* btnPlay;
	wxButton* btnStop;
	wxStatusBar* m_statusBar8;

public:

	FormVideo(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("Spellcross Video Viewer"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(696,385),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxFRAME_FLOAT_ON_PARENT);

	~FormVideo();

};