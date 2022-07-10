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
#include <wx/listbox.h>
#include <wx/radiobut.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/slider.h>
#include <wx/frame.h>

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormMIDI
///////////////////////////////////////////////////////////////////////////////
class FormMIDI : public wxFrame
{
	private:
		SpellData *m_spell_data;
		std::wstring m_src_folder;

		void OnClose(wxCloseEvent &ev);
		void OnExitClick(wxCommandEvent& ev);
		void OnSetVolume(wxCommandEvent& ev);
		void OnPlayClick(wxCommandEvent& ev);
		void OnStopClick(wxCommandEvent& ev);
		void OnSelectFolderClick(wxCommandEvent& ev);
		void OnChangeSource(wxCommandEvent& ev);
		void OnSaveFileClick(wxCommandEvent& ev);
		void OnSaveAllClick(wxCommandEvent& ev);
		void OnSelectMidClick(wxCommandEvent& ev);

		void ChangeSource();
		void GetFileInfo();

	protected:
		enum
		{
			wxID_WIN_MIDI = 1000,
			wxID_MM_CUST_FOLDER,
			wxID_MM_SAVE_MIDI,
			wxID_MM_SAVE_ALL,
			wxID_MM_EXIT,
			wxID_LB_LIST,
			wxID_RB_SRC_MUSIC_FS,
			wxID_RB_SRC_FOLDER,
			wxID_BTN_SRC_FOLDER,
			wxID_TXT_INFO,
			wxID_SLIDE_VOLUME,
			wxID_BTN_STOP,
			wxID_BTN_PLAY,
			wxID_BTN_OK
		};

		wxMenuBar* mmenu;
		wxMenu* mmFile;
		wxStaticText* labelList;
		wxListBox* lbList;
		wxStaticText* m_staticText69;
		wxRadioButton* rbSrcMusicFS;
		wxRadioButton* rbSrcFolder;
		wxButton* btnSrcFolder;
		wxStaticLine* m_staticline24;
		wxStaticText* labelInfo;
		wxTextCtrl* txtInfo;
		wxStaticText* m_staticText68;
		wxSlider* slideVolume;
		wxStaticLine* m_staticline26;
		wxButton* btnStop;
		wxButton* btnPlay;
		wxStaticLine* m_staticline23;
		wxButton* btnOK;

	public:

		FormMIDI( wxWindow* parent, SpellData *spell_data, wxWindowID id = wxID_WIN_MIDI, const wxString& title = wxT("Spellcross MIDI player"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,500 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FormMIDI();

};

