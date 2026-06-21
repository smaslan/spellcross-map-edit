///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormText' on 2026-06-19 22:08:22
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
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/statbox.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/statline.h>
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
#include <wx/bmpbuttn.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormText' on 2026-06-19 22:08:22

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
		void OnChangeText(wxCommandEvent& event);
		void OnChangeTargetPanel(wxCommandEvent& event);
		void OnPaintFont(wxPaintEvent& event);
		void OnPaintText(wxPaintEvent& event);
		void OnTextScroll(wxScrollEvent& event);
		void OnTextCanvasSize(wxSizeEvent& event);
		void OnChangeApply(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnSaveAll(wxCommandEvent& event);
		void OnNew(wxCommandEvent& event);

		

		class SpellWindow {
		public:
			std::string name;
			SpellGraphicItem* grp;
			int x_org;
			int y_org;
			int x_size;
			int y_size;
			int y_gap;
			int txt_color;
			int bg_color;			
			SpellFont::FontShadow shadow;
			SpellTextLines::WrapMode align;
			SpellTextRec::TextPanel panel;
			std::string source;
			std::vector<std::string> wild;
			int x_panel;
			int y_panel;
		};
		std::vector<SpellWindow> m_windows;
		SpellWindow *m_window;

		SpellTextRec m_text;
		SpellTextRec *m_text_org;
		SpellFont* m_font;
		SpellTextLines m_text_lines;
		//int m_wrap_width;
		//int m_wrap_height;

		SpellWindow* GetTextWindow(SpellTextRec* text);
		SpellTextRec* GetText(std::string name="");
		SpellTextRec* CreateText(std::string name);
		void FillResources();
		void PrepareText();

	protected:
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormText' on 2026-06-19 22:08:22
		enum
		{
			wxID_MM_NEW = 5999,
			wxID_MM_SAVE,
			wxID_MM_SAVE_ALL,
			wxID_MM_EXIT,
			wxID_MM_RESTORE,
			wxID_MM_APPLY,
			wxID_SBAR,
			wxID_CH_SOURCE,
			wxID_TXT_WILD,
			wxID_LIST_RESOURCES,
			wxID_TXT_NAME,
			wxID_TXT_EDIT,
			wxID_CH_TARGET,
			wxID_CH_ALIGN,
			wxID_CANVAS_TEXT,
			wxID_SCROLL_TEXT,
			wxID_CANVAS_FONT,
		};
		
		wxMenuBar* m_menubar13;
		wxMenu* mFile;
		wxMenu* mEdit;
		wxStatusBar* sbar;
		wxStaticText* m_staticText106;
		wxChoice* chSource;
		wxStaticText* m_staticText107;
		wxTextCtrl* txtFilter;
		wxStaticText* m_staticText108;
		wxListBox* listResources;
		wxStaticText* m_staticText131;
		wxTextCtrl* txtName;
		wxStaticLine* m_staticline39;
		wxStaticText* m_staticText109;
		wxTextCtrl* textEdit;
		wxStaticText* m_staticText111;
		wxChoice* chTarget;
		wxStaticText* m_staticText114;
		wxChoice* chAlign;
		wxStaticLine* m_staticline40;
		wxStaticText* m_staticText110;
		wxPanel* canvasText;
		wxScrollBar* scrollText;
		wxStaticLine* m_staticline38;
		wxStaticText* m_staticText105;
		wxPanel* canvasFont;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormText' on 2026-06-19 22:08:22

	public:

		FormText( wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY, const wxString& title = wxT("Text resource viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1350,750 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~FormText();

};

