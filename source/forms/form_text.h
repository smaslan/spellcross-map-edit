///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
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
#include <wx/choice.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/scrolbar.h>
#include <wx/frame.h>

#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormText
///////////////////////////////////////////////////////////////////////////////
class FormText : public wxFrame
{
	private:

		SpellData *m_spell_data;

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnChangeFilter(wxCommandEvent& event);
		void OnChangeSource(wxCommandEvent& event);
		void OnChangeResource(wxCommandEvent& event);
		void OnChangeTargetPanel(wxCommandEvent& event);
		void OnPaintFont(wxPaintEvent& event);
		void OnPaintText(wxPaintEvent& event);
		void OnTextScroll(wxScrollEvent& event);
		void OnTextCanvasSize(wxSizeEvent& event);

		SpellTextRec *GetText();
		void FillResources();
		void PrepareText();

		class SpellWindow {
		public:
			std::string name;
			SpellGraphicItem* grp;
			int x_org;
			int y_org;
			int x_size;
			int y_size;
			int txt_color;
			int bg_color;
			SpellFont::FontShadow shadow;
			std::string source;
			std::vector<std::string> wild;
			int x_panel;
			int y_panel;
		};
		std::vector<SpellWindow> m_windows;
		SpellWindow *m_window;

		SpellFont* m_font;
		SpellTextChunks m_text_lines;
		//int m_wrap_width;
		//int m_wrap_height;

	protected:
		enum
		{
			wxID_MM_EXIT = 5999,
			wxID_SBAR,
			wxID_CH_SOURCE,
			wxID_TXT_WILD,
			wxID_LIST_RESOURCES,
			wxID_TXT_EDIT,
			wxID_CH_TARGET,
			wxID_CANVAS_TEXT,
			wxID_SCROLL_TEXT,
			wxID_CANVAS_FONT,
		};

		wxMenuBar* m_menubar13;
		wxMenu* m_menu24;
		wxStatusBar* sbar;
		wxStaticText* m_staticText106;
		wxChoice* chSource;
		wxStaticText* m_staticText107;
		wxTextCtrl* txtFilter;
		wxStaticText* m_staticText108;
		wxListBox* listResources;
		wxStaticLine* m_staticline39;
		wxStaticText* m_staticText109;
		wxTextCtrl* textEdit;
		wxStaticText* m_staticText111;
		wxChoice* chTarget;
		wxStaticLine* m_staticline40;
		wxStaticText* m_staticText110;
		wxPanel* canvasText;
		wxScrollBar* scrollText;
		wxStaticLine* m_staticline38;
		wxStaticText* m_staticText105;
		wxPanel* canvasFont;

	public:

		FormText( wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY, const wxString& title = wxT("Text resource viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1350,750 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FormText();

};

