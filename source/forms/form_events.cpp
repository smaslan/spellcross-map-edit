///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_events.h"
#include "sprites.h"
#include "other.h"

//#include <filesystem>
#include <wx/stdpaths.h>
#include <wx/filedlg.h>

///////////////////////////////////////////////////////////////////////////

FormEvent::FormEvent(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->spell_data = spell_data;
	spell_map = NULL;

	
	// === AUTO GENERATED START ===	
	
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	mMenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmOk;
	mmOk = new wxMenuItem(mmFile,wxID_MM_OK,wxString(wxT("Place/Update")) + wxT('\t') + wxT("Ctrl+Enter"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmOk);

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(wxT("Exit")) + wxT('\t') + wxT("ESC"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	mMenu->Append(mmFile,wxT("File"));

	this->SetMenuBar(mMenu);

	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer531;
	bSizer531 = new wxBoxSizer(wxVERTICAL);

	bSizer531->SetMinSize(wxSize(200,-1));
	m_staticText57 = new wxStaticText(this,wxID_ANY,wxT("Events list:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText57->Wrap(-1);
	bSizer531->Add(m_staticText57,0,wxTOP|wxRIGHT|wxLEFT,5);

	lbEvents = new wxListBox(this,wxID_LB_EVENTS,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB);
	bSizer531->Add(lbEvents,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer54;
	bSizer54 = new wxBoxSizer(wxHORIZONTAL);

	btnAddEvent = new wxButton(this,wxID_BTN_ADD_EVENT,wxT("New"),wxDefaultPosition,wxDefaultSize,0);
	bSizer54->Add(btnAddEvent,1,wxALL,5);

	btnRemoveEvent = new wxButton(this,wxID_BTN_REM_EVENT,wxT("Remove"),wxDefaultPosition,wxDefaultSize,0);
	bSizer54->Add(btnRemoveEvent,1,wxALL,5);


	bSizer531->Add(bSizer54,0,wxEXPAND,5);


	bSizer47->Add(bSizer531,0,wxEXPAND,5);

	m_staticline21 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer47->Add(m_staticline21,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer50;
	bSizer50 = new wxBoxSizer(wxVERTICAL);

	bSizer50->SetMinSize(wxSize(300,-1));
	m_staticText50 = new wxStaticText(this,wxID_ANY,wxT("Event type:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText50->Wrap(-1);
	bSizer50->Add(m_staticText50,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chbTypeChoices;
	chbType = new wxChoice(this,wxID_CHB_TYPE,wxDefaultPosition,wxDefaultSize,chbTypeChoices,0);
	chbType->SetSelection(0);
	bSizer50->Add(chbType,0,wxEXPAND|wxRIGHT|wxLEFT,5);

	m_staticText62 = new wxStaticText(this,wxID_ANY,wxT("MissionObjective() description:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText62->Wrap(-1);
	bSizer50->Add(m_staticText62,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtObjectiveDesc = new wxTextCtrl(this,wxID_TXT_OBJ_DESC,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	bSizer50->Add(txtObjectiveDesc,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	cbIsObjective = new wxCheckBox(this,wxID_CB_IS_OBJECTIVE,wxT("is MissionObjective?"),wxDefaultPosition,wxDefaultSize,0);
	bSizer50->Add(cbIsObjective,0,wxALL,5);

	m_staticText51 = new wxStaticText(this,wxID_ANY,wxT("Probability:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText51->Wrap(-1);
	bSizer50->Add(m_staticText51,0,wxTOP|wxRIGHT|wxLEFT,5);

	spinProb = new wxSpinCtrl(this,wxID_SPIN_PROB,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,1,100,100);
	bSizer50->Add(spinProb,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText52 = new wxStaticText(this,wxID_ANY,wxT("Map x-position:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText52->Wrap(-1);
	bSizer48->Add(m_staticText52,0,wxALIGN_CENTER|wxALL,5);

	spinXpos = new wxSpinCtrl(this,wxID_SPIN_XPOS,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	spinXpos->SetMinSize(wxSize(70,-1));

	bSizer48->Add(spinXpos,0,wxTOP|wxBOTTOM|wxRIGHT,5);

	m_staticText54 = new wxStaticText(this,wxID_ANY,wxT("Map y-position:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText54->Wrap(-1);
	bSizer48->Add(m_staticText54,0,wxALIGN_CENTER|wxALL,5);

	spinYpos = new wxSpinCtrl(this,wxID_SPIN_YPOS,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	spinYpos->SetMinSize(wxSize(70,-1));

	bSizer48->Add(spinYpos,0,wxTOP|wxBOTTOM|wxRIGHT,5);


	bSizer50->Add(bSizer48,0,wxEXPAND,5);

	wxBoxSizer* bSizer56;
	bSizer56 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText59 = new wxStaticText(this,wxID_ANY,wxT("Trigger unit index:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText59->Wrap(-1);
	bSizer56->Add(m_staticText59,0,wxALIGN_CENTER_VERTICAL|wxALL,5);

	spinTrigUnit = new wxSpinCtrl(this,wxID_SPIN_TRIG_UNIT,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,99,0);
	spinTrigUnit->SetMinSize(wxSize(70,-1));

	bSizer56->Add(spinTrigUnit,0,wxALL,5);


	bSizer50->Add(bSizer56,0,wxEXPAND,5);

	m_staticline20 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer50->Add(m_staticline20,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer55;
	bSizer55 = new wxBoxSizer(wxHORIZONTAL);

	strUnits = new wxStaticText(this,wxID_ANY,wxT("Event units count:"),wxDefaultPosition,wxDefaultSize,0);
	strUnits->Wrap(-1);
	bSizer55->Add(strUnits,0,wxALIGN_CENTER|wxRIGHT|wxLEFT,5);

	txtUnits = new wxTextCtrl(this,wxID_TXT_UNITS,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	bSizer55->Add(txtUnits,1,wxRIGHT|wxLEFT,5);


	bSizer50->Add(bSizer55,0,wxEXPAND,5);

	m_staticline22 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer50->Add(m_staticline22,0,wxEXPAND | wxALL,5);

	m_staticText53 = new wxStaticText(this,wxID_ANY,wxT("Event message(s) list:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText53->Wrap(-1);
	bSizer50->Add(m_staticText53,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chbMsgItemChoices;
	chbMsgItem = new wxChoice(this,wxID_CHB_MSG_ITEM,wxDefaultPosition,wxDefaultSize,chbMsgItemChoices,0);
	chbMsgItem->SetSelection(0);
	bSizer50->Add(chbMsgItem,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer49;
	bSizer49 = new wxBoxSizer(wxHORIZONTAL);

	btnNewMsg = new wxButton(this,wxID_BTN_NEW_MSG,wxT("New"),wxDefaultPosition,wxDefaultSize,0);
	bSizer49->Add(btnNewMsg,1,wxALL,5);

	btnDelMsg = new wxButton(this,wxID_BNT_DEL_MSG,wxT("Delete"),wxDefaultPosition,wxDefaultSize,0);
	bSizer49->Add(btnDelMsg,1,wxALL,5);

	btnMsgUp = new wxButton(this,wxID_BTN_MSG_UP,wxT("Move up"),wxDefaultPosition,wxDefaultSize,0);
	bSizer49->Add(btnMsgUp,1,wxALL,5);

	btnMsgDown = new wxButton(this,wxID_BTN_MSG_DOWN,wxT("Move down"),wxDefaultPosition,wxDefaultSize,0);
	bSizer49->Add(btnMsgDown,1,wxALL,5);


	bSizer50->Add(bSizer49,0,wxEXPAND,5);

	m_staticText55 = new wxStaticText(this,wxID_ANY,wxT("Message text resource:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText55->Wrap(-1);
	bSizer50->Add(m_staticText55,0,wxTOP|wxRIGHT|wxLEFT,5);

	lbMsg = new wxListBox(this,wxID_LB_MSG,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer50->Add(lbMsg,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer47->Add(bSizer50,0,wxEXPAND,5);

	m_staticline19 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer47->Add(m_staticline19,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* bSizer52;
	bSizer52 = new wxBoxSizer(wxVERTICAL);

	m_staticText56 = new wxStaticText(this,wxID_ANY,wxT("Message text:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText56->Wrap(-1);
	bSizer52->Add(m_staticText56,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtMessage = new wxTextCtrl(this,wxID_TXT_MSG,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_BESTWRAP|wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP);
	bSizer52->Add(txtMessage,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer(wxHORIZONTAL);

	btnMsgPlay = new wxButton(this,wxID_BTN_PLAY_MSG,wxT("Play narration"),wxDefaultPosition,wxDefaultSize,0);
	bSizer53->Add(btnMsgPlay,1,wxALL,5);

	btnMsgStop = new wxButton(this,wxID_BTN_STOP_MSG,wxT("Stop narration"),wxDefaultPosition,wxDefaultSize,0);
	bSizer53->Add(btnMsgStop,1,wxALL,5);


	bSizer52->Add(bSizer53,0,wxEXPAND,5);


	bSizer47->Add(bSizer52,1,wxEXPAND,5);


	this->SetSizer(bSizer47);
	this->Layout();
	sBar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_ANY);

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===


	// fill event types list
	SpellMapEventRec map_ev((SpellMap*)NULL);
	chbType->Freeze();
	chbType->Clear();
	for(auto& type : map_ev.GetEventTypes())
		chbType->Append(type);
	chbType->Thaw();

	// fill available text message resources
	lbMsg->Freeze();
	lbMsg->Clear();
	for(auto& txt : spell_data->texts->GetTexts())
	{
		wstring name = char2wstring(txt->name.c_str());
		if(txt->audio)
			name += L"  \x266B";
		lbMsg->Append(name);
	}
	lbMsg->Thaw();


	// not sound yet
	spell_sound = NULL;

	// original map event (if exist)
	spell_new_event = NULL;
	spell_event = NULL;

	Bind(wxEVT_CLOSE_WINDOW,&FormEvent::OnClose,this,this->m_windowId);
	Bind(wxEVT_MENU,&FormEvent::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_MENU,&FormEvent::OnCloseClick,this,wxID_MM_OK);

	Bind(wxEVT_LISTBOX,&FormEvent::OnSelectEvent,this,wxID_LB_EVENTS);
	Bind(wxEVT_CHOICE,&FormEvent::OnSelectMsgItem,this,wxID_CHB_MSG_ITEM);
	Bind(wxEVT_LISTBOX,&FormEvent::OnSelectMsgResource,this,wxID_LB_MSG);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEvent::OnNewEventClick,this,wxID_BTN_ADD_EVENT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEvent::OnRemoveEventClick,this,wxID_BTN_REM_EVENT);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEvent::OnNewMsgClick,this,wxID_BTN_NEW_MSG);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEvent::OnRemoveMsgClick,this,wxID_BNT_DEL_MSG);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEvent::OnPlayNarration,this,wxID_BTN_PLAY_MSG);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEvent::OnPlayNarration,this,wxID_BTN_STOP_MSG);

	Bind(wxEVT_TEXT_ENTER,&FormEvent::OnSelectMsgResource,this,wxID_TXT_OBJ_DESC);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormEvent::OnEditParams,this,wxID_CB_IS_OBJECTIVE);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormEvent::OnEditParams,this,wxID_CHB_TYPE);
	Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,&FormEvent::OnEditParams,this,wxID_SPIN_PROB);
	Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,&FormEvent::OnEditParams,this,wxID_SPIN_XPOS);
	Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,&FormEvent::OnEditParams,this,wxID_SPIN_YPOS);
	Bind(wxEVT_COMMAND_SPINCTRL_UPDATED,&FormEvent::OnEditParams,this,wxID_SPIN_TRIG_UNIT);

}

FormEvent::~FormEvent()
{
}



// reset event changes to initial state
void FormEvent::ResetChanges()
{
	auto unit = spell_map->GetSelectedUnit();
	int unit_id = -1;
	if(unit)
		unit_id = unit->id;
	spell_map->SelectUnit(NULL);
		
	// restore events
	spell_map->events->ClearEvents();
	for(auto & evt : spell_orig_events)
	{
		spell_map->events->AddEvent(evt);		
		if(unit_id >= 0)
		{
			// ###todo: fix unit selection (not nice solution - needs to be optimized? also possibly not thread safe)
			for(auto & unit: evt->units)
			{
				if(unit.unit->id == unit_id)
					spell_map->SelectUnit(unit.unit);
			}
		}
	}

	spell_new_event = NULL;
	spell_event = NULL;
}

void FormEvent::OnClose(wxCloseEvent& ev)
{		
	// stop sound
	CleanupSound();
	
	// reset changes if [X] pressed
	if(spell_event)
		ResetChanges();
	
	// resort events map
	spell_map->events->ResetEvents();

	// try select new event
	if(spell_new_event)
		spell_map->SelectEvent(spell_new_event);
	
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormEvent::OnCloseClick(wxCommandEvent& event)
{	
	if(event.GetId() == wxID_MM_OK)
	{
		// OK pressed: confirm changes
		
		// loose backup events list
		for(auto & evt : spell_orig_events)
			delete evt;
		spell_orig_events.clear();
		spell_map->SelectEvent(spell_event);
		spell_event = NULL;
	}
	if(event.GetId() == wxID_MM_EXIT)
	{
		// EXIT pressed: reset changes
		ResetChanges();
	}	
	Close();
}


// set current map poitner
void FormEvent::SetMap(SpellMap* map)
{
	spell_map = map;
	if(!map)
		return;
	
	// make backup of events so it can be restored eventually
	for(auto & evt : spell_map->events->GetEvents())
		spell_orig_events.push_back(new SpellMapEventRec(evt));

	//chbType->Clear();
	chbMsgItem->Clear();
	lbEvents->Clear();
	
	spell_new_event = NULL;

	// is some event selected?
	auto event = spell_map->GetSelectEvent();
	if(!event)
	{
		// nope: select some existing?
		auto list = spell_map->events->GetEvents();
		if(!list.empty())
			event = list[0];
		/*else
		{
			// nope: make new
			event = new SpellMapEventRec(spell_map);
			auto pos = spell_map->GetSelection();
			event->position = pos; // set some position		
			spell_map->events->AddEvent(event);
			spell_new_event = event;
		}*/
	}

	// update events list
	FillEventsList();

	// try select some event	
	SelectEvent(event);
}

// fill events list with current map events
void FormEvent::FillEventsList()
{
	// fill events list
	lbEvents->Freeze();
	lbEvents->Clear();
	auto events = spell_map->events->GetEvents();
	for(int k = 0; k < events.size(); k++)
	{
		string label = string_format("#%d - %s",k+1,events[k]->type_name.c_str());
		if(events[k]->probability != 100)
			label += string_format(", (p=%d%%)",events[k]->probability);
		lbEvents->Append(label,(void*)events[k]);
	}
	lbEvents->Thaw();
}

// on add new event to list
void FormEvent::OnNewEventClick(wxCommandEvent& event)
{
	// create event to main list
	auto evt = new SpellMapEventRec(spell_map);
	auto pos = spell_map->GetSelection();
	evt->position = pos; // set some position		
	auto unit = spell_map->GetSelectedUnit();
	if(unit)
		evt->trig_unit_id = unit->id;
	spell_map->events->AddEvent(evt);
	spell_new_event = evt;

	// update events list
	FillEventsList();

	// try select this new event	
	SelectEvent(evt);
}

// on remove event from list
void FormEvent::OnRemoveEventClick(wxCommandEvent& event)
{
	int sel_id = lbEvents->GetSelection();
	if(lbEvents->IsEmpty() || sel_id < 0)
		return;
	
	// try remove event from map events list
	auto evt = (SpellMapEventRec*)lbEvents->GetClientData(sel_id);
	auto rem_evt = spell_map->events->ExtractEvent(evt);
	if(rem_evt)
		delete rem_evt;

	spell_new_event = NULL;

	// update events list
	FillEventsList();

	// select some event
	evt = NULL;
	if(!lbEvents->IsEmpty())
		evt = (SpellMapEventRec*)lbEvents->GetClientData(lbEvents->GetCount() - 1);
	SelectEvent(evt);

}

// on select event
void FormEvent::OnSelectEvent(wxCommandEvent& event)
{	
	if(lbEvents->IsEmpty())
		return;
	auto spell_evt = (SpellMapEventRec*)lbEvents->GetClientData(lbEvents->GetSelection());
	SelectEvent(spell_evt);
}

// select event
void FormEvent::SelectEvent(SpellMapEventRec *evt)
{	
	// get selected event (if exists)
	spell_event = evt;

	if(!spell_event)
		return;

	// select events list item
	for(int k = 0; k < lbEvents->GetCount(); k++)
		if(lbEvents->GetClientData(k) == evt)
		{
			lbEvents->Select(k);
			break;
		}

	// fill event types
	chbType->Select(spell_event->evt_type);

	// fill probability
	spinProb->SetValue(spell_event->probability);

	// fill position (if applicable)
	spinXpos->SetRange(0,spell_map->x_size - 1);
	spinXpos->SetValue(spell_event->position.x);
	spinYpos->SetRange(0,spell_map->y_size - 1);
	spinYpos->SetValue(spell_event->position.y);

	// fill SeeUnit() target
	spinTrigUnit->SetValue(spell_event->trig_unit_id);

	// fill associated units count
	txtUnits->SetValue(string_format("%d", spell_event->units.size()));

	// is objective?
	cbIsObjective->SetValue(spell_event->is_objective);

	// objective label
	txtObjectiveDesc->SetValue(spell_event->label);

	// update edits
	wxCommandEvent ev;
	OnEditParams(ev);
	
	// fill text messages list
	FillMsgItems();
	SelectMsgItem();
	SelectMsgResource();
}


void FormEvent::FillMsgItems()
{	
	int old_sel = chbMsgItem->GetSelection();
	chbMsgItem->Clear();
	if(!spell_event)
		return;		
	chbMsgItem->Freeze();
	for(int k = 0; k < spell_event->texts.size(); k++)
		chbMsgItem->Append(string_format("#%d - %s",k+1,spell_event->texts[k].text->name.c_str()));	
	if(chbMsgItem->GetCount())
		chbMsgItem->Select(0);
	chbMsgItem->Thaw();
	if(old_sel >= 0 && old_sel < chbMsgItem->GetCount())
		chbMsgItem->Select(old_sel);
}

void FormEvent::SelectMsgItem()
{
	wxCommandEvent evt;
	OnSelectMsgItem(evt);
}
void FormEvent::OnSelectMsgItem(wxCommandEvent& event)
{		
	// get selection
	auto sel = chbMsgItem->GetSelection();
	if(chbMsgItem->IsEmpty() || sel < 0)
		return;	
	auto msg = spell_event->texts[sel];

	// select matching item in the list of resources
	int res_id = spell_data->texts->GetTextId(msg.text);
	lbMsg->Select(res_id);

	// show message
	txtMessage->SetValue(msg.text->text);
}

void FormEvent::OnNewMsgClick(wxCommandEvent& event)
{
	if(!spell_event)
		return;

	// get resource selection
	int sel_res = lbMsg->GetSelection();
	if(lbMsg->IsEmpty() || sel_res < 0)
		return;
	auto text = spell_data->texts->GetText(sel_res);
	
	// append text message
	spell_event->texts.emplace_back(text);

	// update list
	FillMsgItems();
	chbMsgItem->Select(chbMsgItem->GetCount() - 1);
	SelectMsgItem();	
	SelectMsgResource();
}

void FormEvent::OnRemoveMsgClick(wxCommandEvent& event)
{
	if(!spell_event)
		return;

	// get resource selection
	int sel = chbMsgItem->GetSelection();
	if(chbMsgItem->IsEmpty() || sel < 0 || sel >= spell_event->texts.size())
		return;
	// remove it
	spell_event->texts.erase(spell_event->texts.begin() + sel);
	
	// update list
	FillMsgItems();
	if(!chbMsgItem->IsEmpty())
		chbMsgItem->Select(chbMsgItem->GetCount() - 1);
	SelectMsgItem();
	//SelectMsgResource();
}


void FormEvent::SelectMsgResource()
{
	wxCommandEvent evt;
	OnSelectMsgResource(evt);
}
void FormEvent::OnSelectMsgResource(wxCommandEvent& event)
{
	btnMsgPlay->Enable(false);
	btnMsgStop->Enable(false);
	CleanupSound();
	txtMessage->Clear();

	if(!spell_event || spell_event->texts.empty())
	{
		// show message with no assignement to event
		int sel_res = lbMsg->GetSelection();
		if(lbMsg->IsEmpty() || sel_res < 0)
			return;
		auto text = spell_data->texts->GetText(sel_res);
		txtMessage->SetValue(text->text);
		if(text->audio)
		{
			btnMsgPlay->Enable(true);
			btnMsgStop->Enable(true);
		}
		return;	
	}

	// get message item selection
	auto sel = chbMsgItem->GetSelection();
	if(chbMsgItem->IsEmpty() || sel < 0)
	{
		txtMessage->SetValue("");
		btnMsgPlay->Enable(false);
		btnMsgStop->Enable(false);
		return;
	}
	auto &msg = spell_event->texts[sel];

	// get resource selection
	int sel_res = lbMsg->GetSelection();
	if(lbMsg->IsEmpty() || sel_res < 0)
		return;
	auto text = spell_data->texts->GetText(sel_res);
	
	// update text in event record
	msg.text = text;

	FillMsgItems();
		
	// show text
	txtMessage->SetValue(text->text);

	// show narration play buttons?
	if(text->audio)
	{
		btnMsgPlay->Enable(true);
		btnMsgStop->Enable(true);
	}	
}

// edit event parameters
void FormEvent::OnEditParams(wxCommandEvent& event)
{
	if(!spell_event)
		return;

	// update event type
	int type_id = chbType->GetSelection();
	if(!chbType->IsEmpty() && type_id >= 0)
		spell_event->SetType(type_id);

	// mission objective flag
	if(cbIsObjective->GetValue() && spell_event->evt_type == SpellMapEventRec::EvtTypes::EVT_MISSION_START)
		cbIsObjective->SetValue(false);
	spell_event->is_objective = cbIsObjective->GetValue();

	// update probability	
	if(spell_event->is_objective)
	{
		spell_event->probability = 100;
		spinProb->SetValue(spell_event->probability);
		spinProb->Enable(false);
	}
	else
	{
		spell_event->probability = spinProb->GetValue();
		spinProb->Enable(true);
	}

	// objective text label
	if(spell_event->is_objective)
	{
		spell_event->label = txtObjectiveDesc->GetValue();
		txtObjectiveDesc->Enable(true);
	}
	else
	{
		spell_event->label = L"";
		txtObjectiveDesc->SetValue("");
		txtObjectiveDesc->Enable(false);
	}

	// update coordinates
	spell_event->position = MapXY(spinXpos->GetValue(),spinYpos->GetValue());
	spinXpos->Enable(spell_event->hasPosition());
	spinYpos->Enable(spell_event->hasPosition());

	// update target unit id
	spell_event->trig_unit_id = spinTrigUnit->GetValue();
	spinTrigUnit->Enable(spell_event->hasTargetUnit());

	if(spell_event->is_objective)
	{
		// clear event stuff (messages and units)
		spell_event->ClearUnits();
		spell_event->ClearTexts();
	}
	lbMsg->Enable(!spell_event->is_objective);
	btnDelMsg->Enable(!spell_event->is_objective);
	btnMsgDown->Enable(!spell_event->is_objective);
	btnMsgUp->Enable(!spell_event->is_objective);
	btnNewMsg->Enable(!spell_event->is_objective);
	chbMsgItem->Enable(!spell_event->is_objective);
	txtMessage->Enable(!spell_event->is_objective);

	// resort events (note: possibly slow?)
	spell_map->events->ResetEvents();

	/*spell_event->SetType(chbType->GetSelection());
	spell_event->probability = spinProb->GetValue();	
	spell_event->position = MapXY(spinXpos->GetValue(),spinYpos->GetValue());
	spell_event->is_objective = cbIsObjective->GetValue();
	spell_event->label = txtObjectiveDesc->GetValue();
	
	// ###not sure if I need also pointer to unit?
	spell_event->trig_unit_id = spinTrigUnit->GetValue();*/



}





// play/stop narration sound
void FormEvent::CleanupSound()
{
	if(spell_sound)
	{
		// stop old playback
		spell_sound->Stop();
		while(!spell_sound->isDone())
			this_thread::sleep_for(10ms);
		delete spell_sound;
		spell_sound = NULL;
	}
}
void FormEvent::OnPlayNarration(wxCommandEvent& event)
{
	// stop old sound
	CleanupSound();
	
	if(event.GetId() == wxID_BTN_PLAY_MSG)
	{
		// start new playback

		// get message item selection
		auto sel = chbMsgItem->GetSelection();
		if(chbMsgItem->IsEmpty() || sel < 0)
			return;
		auto msg = spell_event->texts[sel];

		// play
		if(!msg.text->audio)
			return;
		spell_sound = new SpellSound(spell_data->sounds->channels, msg.text->audio);
		spell_sound->Play();
	}
}



