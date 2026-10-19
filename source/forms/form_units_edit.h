///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormUnitsEdit' on 2026-10-19 18:36:51
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormUnitsEdit' on 2026-10-19 18:36:51

#include <wx/timer.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormUnitsEdit
///////////////////////////////////////////////////////////////////////////////

class SpellData;
class SpellUnits;
class wxListCtrlVirtual;
class SpellUnitRec;

class FormUnitsEdit : public wxFrame
{
	private:

		void OnClose(wxCloseEvent& ev);
		void OnCloseClick(wxCommandEvent& event);
		void OnOpenClick(wxCommandEvent& event);
		void OnSaveClick(wxCommandEvent& event);
		void OnSelectDEF(wxCommandEvent& event);
		void OnSelectTargetDEF(wxCommandEvent& event);
		void OnSelectUnit(wxCommandEvent& event);
		void OnPGpropChange(wxPropertyGridEvent& event);
		void OnDefsPupupOpen(wxMouseEvent& event);
		void OnDefsPupup(wxCommandEvent& event);
		void OnUnitsPupupOpen(wxMouseEvent& event);
		void OnUnitsPupup(wxCommandEvent& event);

		wxString OnGetUnitName(long item);
		void OnUnitBeginDrag(wxListEvent& event);
		void OnUnitDragMotion(wxMouseEvent& event);
		void OnUnitDragLost(wxMouseCaptureLostEvent& event);
		void OnUnitEndDrag(wxMouseEvent& event);

		void OnPaintIcon(wxPaintEvent& event);
		void OnPaintInfo(wxPaintEvent& event);
		void OnPaintProjectile(wxPaintEvent& event);
		void OnPaintFSU(wxPaintEvent& event);
		void OnPaintPNM(wxPaintEvent& event);
		void RefreshGrp(bool all=false);
		void FillSounds();
		void OnSndDoubleClick(wxPropertyGridEvent& event);

		void OnUnitPropsPupupOpen(wxPropertyGridEvent& event);
		void OnUnitPropsPupup(wxCommandEvent& event);

		void OnTimer(wxTimerEvent& event);
		int m_rel_time;

		wxTimer m_timer;
		SpellData *m_spell_data;
		std::vector<std::shared_ptr<SpellUnits>> m_defs;
		std::shared_ptr<SpellUnits> m_units;
		SpellUnitRec *m_unit;
		std::unique_ptr<wxDragImage> m_drag_unit_img;
		int m_last_target_def_id = -1;
		int m_drag_unit_id = -1;

		enum class PopupActions {
			SEL_ALL_DEFS,
			CLR_ALL_DEFS,
			SEL_ALL_UNITS,
			CLR_ALL_UNITS,
			ADD_UNIT,
			COPY_TO_TARGET,
			RESET_ANIM_FRAMES
		};

	protected:
		
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormUnitsEdit' on 2026-10-19 18:36:51
		enum
		{
			wxID_MM_OPEN_DEF = 5999,
			wxID_MM_SAVE_DEF,
			wxID_MM_SAVE_DEF_AS,
			wxID_MM_EXIT,
			wxID_LBOX_DEFS,
			wxID_LBOX_TARGET_DEFS,
			wxID_LBOX_UNITS,
			wxID_PAGE_PROPS,
			wxID_PAGE_BASIC,
			wxID_PG_PROP_BASIC,
			wxID_PG_PROP_BASIC2,
			wxID_PAGE_GRP,
			wxID_PG_PROP_GRP,
			wxID_CANVAS_ICON,
			wxID_CANVAS_INFO,
			wxID_CANVAS_GRP,
			wxID_CANVAS_GRP_AUX,
			wxID_CANVAS_PROJECTILE,
			wxID_CANVAS_LIGHT_FIRE_ANIM,
			wxID_CANVAS_LIGHT_FIRE_PNM,
			wxID_CANVAS_LIGHT_HIT_PNM,
			wxID_CANVAS_ARMORED_FIRE_ANIM,
			wxID_CANVAS_ARMORED_FIRE_PNM,
			wxID_CANVAS_ARMORED_HIT_PNM,
			wxID_CANVAS_AIR_FIRE_ANIM,
			wxID_CANVAS_AIR_FIRE_PNM,
			wxID_CANVAS_AIR_HIT_PNM,
			wxID_PAGE_SOUND,
			wxID_PG_PROP_SOUND,
			wxID_PG_SND_RES,
			wxID_PAGE_ACTION,
			wxID_PG_PROP_ACTION,
			wxID_CANVAS_GRP_SPEC,
			wxID_CANVAS_GRP_DIE,
		};
		
		wxStatusBar* m_statusBar12;
		wxMenuBar* m_menubar15;
		wxMenu* mmFile;
		wxStaticText* m_staticText138;
		wxListBox* lboxDEF;
		wxStaticText* m_staticText140;
		wxListBox* lboxTargetDef;
		wxStaticText* m_staticText137;
		wxListCtrlVirtual* lboxUnits;
		wxNotebook* pageProps;
		wxPanel* pagePropBasic;
		wxPropertyGrid* pgPropBasic;
		wxPropertyGrid* pgPropBasic2;
		wxPanel* pageGrp;
		wxPropertyGrid* pgPropGrp;
		wxStaticText* m_staticText141;
		wxPanel* canvasIcon;
		wxStaticText* m_staticText1411;
		wxPanel* canvasInfo;
		wxStaticText* m_staticText143;
		wxPanel* canvasGrp;
		wxStaticText* m_staticText1431;
		wxPanel* canvasGrpAux;
		wxStaticText* m_staticText142;
		wxPanel* canvasProjectile;
		wxStaticText* m_staticText147;
		wxPanel* canvasLightFireAnim;
		wxStaticText* m_staticText1471;
		wxPanel* canvasLightFirePNM;
		wxStaticText* m_staticText14711;
		wxPanel* canvasLightHitPNM;
		wxStaticText* m_staticText1472;
		wxPanel* canvasArmoredFireAnim;
		wxStaticText* m_staticText14712;
		wxPanel* canvasArmoredFirePNM;
		wxStaticText* m_staticText147111;
		wxPanel* canvasArmoredHitPNM;
		wxStaticText* m_staticText14721;
		wxPanel* canvasAirFireAnim;
		wxStaticText* m_staticText147121;
		wxPanel* canvasAirFirePNM;
		wxStaticText* m_staticText1471111;
		wxPanel* canvasAirHitPNM;
		wxPanel* pageSound;
		wxPropertyGrid* pgPropSound;
		wxStaticText* m_staticText170;
		wxPropertyGrid* pgSndRes;
		wxPanel* pageAction;
		wxPropertyGrid* pgPropAction;
		wxStaticText* m_staticText1412;
		wxPanel* canvasGrpSpec;
		wxStaticText* m_staticText14121;
		wxPanel* canvasGrpDie;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormUnitsEdit' on 2026-10-19 18:36:51

	public:

		FormUnitsEdit( wxWindow* parent, wxWindowID id,SpellData *spell_data,const wxString& title = wxT("Units edit tool"), const wxPoint& pos = wxDefaultPosition, 
			const wxSize& size = wxSize( 1400,900 ), long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL );
		~FormUnitsEdit();

};

