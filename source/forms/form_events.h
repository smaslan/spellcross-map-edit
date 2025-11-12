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
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

#include "spellcross.h"
#include "sprites.h"
#include "map.h"
#include "spell_filter.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEvent
///////////////////////////////////////////////////////////////////////////////
class FormEvent : public wxFrame
{
private:
	SpellData* spell_data;
	SpellMap* spell_map;	
	SpellSound* spell_sound;

	SpellMapEventRec* spell_event;
	SpellMapEventRec* spell_new_event;
	SpellMapEventsList spell_orig_events;

	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnNewEventClick(wxCommandEvent& event);
	void OnRemoveEventClick(wxCommandEvent& event);
	void OnNewMsgClick(wxCommandEvent& event);
	void OnRemoveMsgClick(wxCommandEvent& event);
	void OnSelectEvent(wxCommandEvent& event);
	void OnSelectMsgItem(wxCommandEvent& event);
	void OnSelectMsgResource(wxCommandEvent& event);
	void OnPlayNarration(wxCommandEvent& event);
	void OnEditParams(wxCommandEvent& event);
	
	void ResetChanges();

	void CleanupSound();
	
	void FillEventsList();
	void SelectEvent(SpellMapEventRec* evt);
	void FillMsgItems();
	void SelectMsgItem();
	void SelectMsgResource();

protected:
	enum
	{
		wxID_MM_OK = 1000,
		wxID_MM_EXIT,
		wxID_LB_EVENTS,
		wxID_BTN_ADD_EVENT,
		wxID_BTN_REM_EVENT,
		wxID_CHB_TYPE,
		wxID_TXT_OBJ_DESC,
		wxID_CB_IS_OBJECTIVE,
		wxID_SPIN_PROB,
		wxID_SPIN_XPOS,
		wxID_SPIN_YPOS,
		wxID_SPIN_TRIG_UNIT,
		wxID_TXT_UNITS,
		wxID_CHB_MSG_ITEM,
		wxID_BTN_NEW_MSG,
		wxID_BNT_DEL_MSG,
		wxID_BTN_MSG_UP,
		wxID_BTN_MSG_DOWN,
		wxID_LB_MSG,
		wxID_TXT_MSG,
		wxID_BTN_PLAY_MSG,
		wxID_BTN_STOP_MSG
	};

	wxMenuBar* mMenu;
	wxMenu* mmFile;
	wxStaticText* m_staticText57;
	wxListBox* lbEvents;
	wxButton* btnAddEvent;
	wxButton* btnRemoveEvent;
	wxStaticLine* m_staticline21;
	wxStaticText* m_staticText50;
	wxChoice* chbType;
	wxStaticText* m_staticText62;
	wxTextCtrl* txtObjectiveDesc;
	wxCheckBox* cbIsObjective;
	wxStaticText* m_staticText51;
	wxSpinCtrl* spinProb;
	wxStaticText* m_staticText52;
	wxSpinCtrl* spinXpos;
	wxStaticText* m_staticText54;
	wxSpinCtrl* spinYpos;
	wxStaticText* m_staticText59;
	wxSpinCtrl* spinTrigUnit;
	wxStaticLine* m_staticline20;
	wxStaticText* strUnits;
	wxTextCtrl* txtUnits;
	wxStaticLine* m_staticline22;
	wxStaticText* m_staticText53;
	wxChoice* chbMsgItem;
	wxButton* btnNewMsg;
	wxButton* btnDelMsg;
	wxButton* btnMsgUp;
	wxButton* btnMsgDown;
	wxStaticText* m_staticText55;
	wxListBox* lbMsg;
	wxStaticLine* m_staticline19;
	wxStaticText* m_staticText56;
	wxTextCtrl* txtMessage;
	wxButton* btnMsgPlay;
	wxButton* btnMsgStop;
	wxStatusBar* sBar;

public:

	FormEvent(wxWindow* parent,SpellData* spell_data,wxWindowID id = wxID_ANY,const wxString& title = wxT("Event editor"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(750,490),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL|wxFRAME_FLOAT_ON_PARENT);
	~FormEvent();

	void SetMap(SpellMap* map);

};
