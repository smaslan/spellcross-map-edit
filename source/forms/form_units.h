///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include "spellcross.h"
#include "map.h"

#include <wx/filedlg.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/textctrl.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/notebook.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormUnits
///////////////////////////////////////////////////////////////////////////////
class FormUnits : public wxFrame
{
	private:

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnSaveAuxClick(wxCommandEvent& event);
		void OnSelectUnit(wxCommandEvent& event);
		void OnSelectArt(wxCommandEvent& event);
		void OnPaintIcon(wxPaintEvent& event);
		void OnPaintArt(wxPaintEvent& event);
		void OnCopyGrpOriginsClick(wxCommandEvent& event);
		
		void OnPaintGrp(wxPaintEvent& event);
		void OnGrpCanvasLMouseDown(wxMouseEvent& event);
		void OnGrpCanvasMouseMove(wxMouseEvent& event);
		void OnGrpCanvasMouseInOut(wxMouseEvent& event);
		void OnGrpResChange(wxCommandEvent& event);
		void OnChangeGrpFrame(wxCommandEvent& event);
		void ChangeGrpResource();


		void SelectUnit(MapUnit *unit=NULL);
		void WriteInfo();
		void EditUnit();

		SpellData *m_spell_data;
		MapUnit *m_unit;
		bool m_update;		

		vector<uint8_t> m_grpbuf;
		int m_grp_x;
		int m_grp_y;

		class TGrpRes:public wxClientData
		{
		public:
			FSU_resource *res;
			int type;
			int flag;
		};

		TGrpRes m_grp_copy_buffer;

		enum
		{
			GRP_BASE = 0,
			GRP_AUX = 1,
			GRP_LIGHT = 2,
			GRP_ARMOR = 3,
			GRP_AIR = 4,
			GRP_ACTION = 5,
			GRP_DIE = 6
		};
		enum
		{
			GRP_STATIC = 0x100,
			GRP_ANIMATION = 0x200
		};

	protected:
		enum
		{
			wxID_SB = 1000,
			wxID_MM_SAVE_AUX,
			wxID_MM_EXIT,
			wxID_MM_COPY_GRP_ORG,
			wxID_MM_PASTE_GRP_ORG,
			wxID_MM_SET,
			wxID_LB_UNITS,
			wxID_CANVAS_ICON,
			wxID_PROPS,
			wxID_SPIN_ID,
			wxID_NAME,
			wxID_SPIN_HEALTH,
			wxID_UNIT_BEHAVE,
			wxID_UNIT_TYPE,
			wxID_PAGE_CTRL,
			wxID_PAGE_ART,
			wxID_CANVAS_ART,
			wxID_LB_ART,
			wxID_TXT_INFO,
			wxID_PAGE_GRP,
			wxID_CANVAS_GRP,
			wxID_CHB_GRP_TYPE,
			wxID_TXT_GRP_AZIM,
			wxID_SLIDE_AZIM,
			wxID_TXT_GRP_SLOPE,
			wxID_SLIDE_SLOPE,
			wxID_SLIDE_FRAME,
			wxID_SLIDE_GRP_ZOOM,
			wxID_SLIDE_GRP_GAMMA,
			wxID_CB_GRP_ORG,
			wxID_CB_GRP_FIRE_ORG,
			wxID_CB_GRP_FIRE_ORG_MEAN,
			wxID_CB_GRP_FIRE_CENTER,
		};

		wxStatusBar* sbar;
		wxMenuBar* mmenu;
		wxMenu* mmFile;
		wxMenu* mmEdit;
		wxStaticText* m_staticText32;
		wxListBox* lboxUnits;
		wxStaticLine* m_staticline9;
		wxStaticText* m_staticText35;
		wxPanel* icon_canvas;
		wxStaticText* m_staticText34;
		wxPropertyGrid* props;
		wxStaticLine* m_staticline13;
		wxStaticText* m_staticText60;
		wxSpinCtrl* spinID;
		wxStaticText* m_staticText39;
		wxTextCtrl* txtName;
		wxStaticText* m_staticText40;
		wxSpinCtrl* spinHealth;
		wxStaticText* m_staticText401;
		wxChoice* chUnitBehave;
		wxStaticText* m_staticText74;
		wxChoice* chUnitType;
		wxStaticLine* m_staticline10;
		wxNotebook* pages;
		wxPanel* pageArt;
		wxStaticText* m_staticText36;
		wxPanel* art_canvas;
		wxStaticLine* m_staticline11;
		wxStaticText* m_staticText37;
		wxListBox* lboxArt;
		wxStaticLine* m_staticline12;
		wxStaticText* m_staticText38;
		wxTextCtrl* txtInfo;
		wxPanel* pageGrp;
		wxStaticText* m_staticText42;
		wxPanel* grp_canvas;
		wxStaticLine* m_staticline14;
		wxStaticText* m_staticText43;
		wxChoice* chbGrpType;
		wxStaticText* txtGrpZim;
		wxSlider* slideAzim;
		wxStaticLine* m_staticline15;
		wxStaticText* txtGrpSlope;
		wxSlider* slideSlope;
		wxStaticLine* m_staticline16;
		wxStaticText* m_staticText46;
		wxSlider* slideFrame;
		wxStaticLine* m_staticline17;
		wxStaticText* txtGrpZoom;
		wxSlider* slideGrpZoom;
		wxStaticText* txtGrpGamma;
		wxSlider* slideGrpGamma;
		wxStaticLine* m_staticline18;
		wxCheckBox* cbGrpOrigin;
		wxCheckBox* cbGrpFireOrg;
		wxCheckBox* cbGrpFireMean;
		wxCheckBox* cbGrpFireCenter;
		wxListBox* m_listBox11;

	public:

		FormUnits(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Units viewer"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1100,720 ),
			long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxSTAY_ON_TOP);
		~FormUnits();

		void SetSpellData(SpellData *spelldata);
		void SetMapUnit(MapUnit* unit=NULL);
		bool DoUpdateUnit();
		

};

