///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_mission_params.h"
#include "other.h"
#include "wx_other.h"

#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////

FormMissionParams::FormMissionParams( wxWindow* parent,SpellData* spell_data,SpellMap *spell_map,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	m_spell_data = spell_data;
	m_spell_map = spell_map;

	// === START OF AUTO GENERATED STUFF ===
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormMissionParams' on 2026-07-06 19:31:50
	this->SetSizeHints( wxSize( 600,400 ), wxDefaultSize );
	
	wxBoxSizer* bSizer69;
	bSizer69 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer70;
	bSizer70 = new wxBoxSizer( wxVERTICAL );
	
	bSizer70->SetMinSize( wxSize( 250,-1 ) );
	m_staticText75 = new wxStaticText( this, wxID_ANY, wxT("Mission text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	bSizer70->Add( m_staticText75, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	wxArrayString chTextChoices;
	chText = new wxChoice( this, wxID_CH_TEXT, wxDefaultPosition, wxDefaultSize, chTextChoices, 0 );
	chText->SetSelection( 0 );
	bSizer70->Add( chText, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticText76 = new wxStaticText( this, wxID_ANY, wxT("Mission start text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	bSizer70->Add( m_staticText76, 0, wxLEFT|wxRIGHT, 5 );
	
	wxArrayString chStartTextChoices;
	chStartText = new wxChoice( this, wxID_CH_START_TEXT, wxDefaultPosition, wxDefaultSize, chStartTextChoices, 0 );
	chStartText->SetSelection( 0 );
	bSizer70->Add( chStartText, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticText77 = new wxStaticText( this, wxID_ANY, wxT("Mission good end text resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText77->Wrap( -1 );
	bSizer70->Add( m_staticText77, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	wxArrayString chGoodEndTextChoices;
	chGoodEndText = new wxChoice( this, wxID_CH_GOOD_END_TEXT, wxDefaultPosition, wxDefaultSize, chGoodEndTextChoices, 0 );
	chGoodEndText->SetSelection( 0 );
	bSizer70->Add( chGoodEndText, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticText78 = new wxStaticText( this, wxID_ANY, wxT("Mission bad end txt resource:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText78->Wrap( -1 );
	bSizer70->Add( m_staticText78, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	wxArrayString chBadEndTextChoices;
	chBadEndText = new wxChoice( this, wxID_CH_BAD_END_TEXT, wxDefaultPosition, wxDefaultSize, chBadEndTextChoices, 0 );
	chBadEndText->SetSelection( 0 );
	bSizer70->Add( chBadEndText, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	cbNight = new wxCheckBox( this, wxID_CB_NIGHT, wxT("Is Night Mission?"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer70->Add( cbNight, 0, wxALL, 5 );
	
	
	bSizer70->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer71->Add( bSizer70, 0, wxEXPAND, 5 );
	
	m_staticline28 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer71->Add( m_staticline28, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer72;
	bSizer72 = new wxBoxSizer( wxVERTICAL );
	
	txtPrevLbl = new wxStaticText( this, wxID_ANY, wxT("Text resource preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	txtPrevLbl->Wrap( -1 );
	bSizer72->Add( txtPrevLbl, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	txtPreview = new wxTextCtrl( this, wxID_TEXT_PREVIEW, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_WORDWRAP );
	bSizer72->Add( txtPreview, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer71->Add( bSizer72, 1, wxEXPAND, 5 );
	
	m_staticline47 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer71->Add( m_staticline47, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer127;
	bSizer127 = new wxBoxSizer( wxVERTICAL );
	
	bSizer127->SetMinSize( wxSize( 250,-1 ) );
	m_staticText134 = new wxStaticText( this, wxID_ANY, wxT("Unit randomizer rules:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText134->Wrap( -1 );
	bSizer127->Add( m_staticText134, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	lboxRndRules = new wxListBox( this, wxID_LBOX_RAND_RULES, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bSizer127->Add( lboxRndRules, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticText135 = new wxStaticText( this, wxID_ANY, wxT("Change units randomization:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText135->Wrap( -1 );
	bSizer127->Add( m_staticText135, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	btnRandToAuto = new wxButton( this, wxID_BTN_RAND_TO_AUTO, wxT("All non-explicit to auto"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer127->Add( btnRandToAuto, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	btnRandToOff = new wxButton( this, wxID_BTN_RAND_TO_OFF, wxT("All non-explicit to off"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer127->Add( btnRandToOff, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	btnRandEnemyAuto = new wxButton( this, wxID_BTN_RAND_ENEMY_AUTO, wxT("For non-explicit:\nEnemy to auto\nPlayers to off"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer127->Add( btnRandEnemyAuto, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer71->Add( bSizer127, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer126;
	bSizer126 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText133 = new wxStaticText( this, wxID_ANY, wxT("Randomizer rule:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText133->Wrap( -1 );
	bSizer126->Add( m_staticText133, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	pgRandRule = new wxPropertyGrid(this, wxID_PG_RAND_RULE, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer126->Add( pgRandRule, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer71->Add( bSizer126, 1, wxEXPAND, 5 );
	
	
	bSizer69->Add( bSizer71, 1, wxEXPAND, 5 );
	
	m_staticline27 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer69->Add( m_staticline27, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	btnOK = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer69->Add( btnOK, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizer69 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormMissionParams' on 2026-07-06 19:31:50
	// === END OF AUTO GENERATED STUFF ===

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMissionParams::OnClose,this,wxID_BTN_OK);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_TEXT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_START_TEXT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_GOOD_END_TEXT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_BAD_END_TEXT);	
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormMissionParams::OnSelectRandRule,this,wxID_LBOX_RAND_RULES);
	Bind(wxEVT_PG_CHANGED,&FormMissionParams::OnUnitPropChange,this,wxID_PG_RAND_RULE);	
	lboxRndRules->Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(FormMissionParams::OnRulesClick),NULL,this);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMissionParams::OnUnitsRandChange,this,wxID_BTN_RAND_TO_AUTO);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMissionParams::OnUnitsRandChange,this,wxID_BTN_RAND_TO_OFF);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMissionParams::OnUnitsRandChange,this,wxID_BTN_RAND_ENEMY_AUTO);
	
	
		

	// fill and assign text resources
	std::vector<std::string*> list_values = {&m_spell_map->params.mission_text, &m_spell_map->params.start_text, &m_spell_map->params.end_ok_text, &m_spell_map->params.end_bad_text};
	std::vector<wxChoice*> list = {chText, chStartText, chGoodEndText, chBadEndText};
	for(int k = 0; k < list.size(); k++)
	{
		auto &ch = list[k];
		auto val = list_values[k];

		ch->Freeze();
		ch->Clear();
		ch->Append("<no text resource>");
		for(auto &txt: m_spell_data->texts->GetTexts())
			ch->Append(txt->name);
		ch->Select(0);
		auto tid = ch->FindString(*val);
		if(tid >= 0)
			ch->Select(tid);
		ch->Thaw();		
	}

	// night flag
	cbNight->SetValue(m_spell_map->params.is_night);

	// list of available units
	int unit_type = 0;
	for(auto& unit : m_spell_data->units->GetUnits())
	{
		auto str = string_format("#%02d: %ls",unit_type,unit->name.c_str());
		m_units_list.insert({unit_type++,str});
	}

	LoadRandRules();

}

FormMissionParams::~FormMissionParams()
{
}

// on close form
void FormMissionParams::OnClose(wxCommandEvent& event)
{	
	if(m_spell_map)
	{
		// is night mission?
		m_spell_map->params.is_night = cbNight->GetValue();

		// get text resources
		std::vector<std::string*> list_values ={&m_spell_map->params.mission_text, &m_spell_map->params.start_text, &m_spell_map->params.end_ok_text, &m_spell_map->params.end_bad_text};
		std::vector<wxChoice*> list ={chText, chStartText, chGoodEndText, chBadEndText};
		for(int k = 0; k < list.size(); k++)
		{
			auto& ch = list[k];
			auto val = list_values[k];
			if(ch->GetSelection() < 1)
				*val = "";
			else
				*val = ch->GetString(ch->GetSelection());
		}
	}
	
	EndModal(wxID_OK);
}

// on select some text resource
void FormMissionParams::OnSelectTxt(wxCommandEvent& event)
{
	/*std::vector<int> id_list = {wxID_CH_TEXT, wxID_CH_START_TEXT, wxID_CH_GOOD_END_TEXT, wxID_CH_BAD_END_TEXT};
	std::vector<std::string*> dest_list ={&m_spell_map->params.mission_text, &m_spell_map->params.start_text, &m_spell_map->params.end_ok_text, &m_spell_map->params.end_bad_text};
	auto res_id = std::find(id_list.begin(),id_list.end(),event.GetId());
	if(res_id == id_list.end())
		return;*/

	auto ch = (wxChoice*)event.GetEventObject();
	auto name = ch->GetString(ch->GetSelection()).ToStdString();
	auto text = m_spell_data->texts->GetText(name.c_str());
	if(!text)
	{
		// not found
		txtPreview->Clear();
		return;
	}
	txtPreview->SetValue(text->text);

}

// fill unit randomizer list
void FormMissionParams::LoadRandRules()
{
	pgRandRule->Clear();

	auto sel = lboxRndRules->GetStringSelection();
	lboxRndRules->Freeze();
	lboxRndRules->Clear();
	for(auto &rule: m_spell_map->unit_randomizer.rules)
	{
		auto unit = m_spell_data->units->GetUnit(rule.ref_unit);
		if(!unit)
			break;
		lboxRndRules->Append(string_format("#%d: %ls",unit->type_id,unit->name.c_str()));
	}
	lboxRndRules->Thaw();	
	if(lboxRndRules->GetCount())
		lboxRndRules->Select(0);
	if(!sel.empty())
		lboxRndRules->SetStringSelection(sel);

	if(lboxRndRules->GetSelection() >= 0)
	{
		wxCommandEvent evt;
		OnSelectRandRule(evt);
	}
}



const int ID_POP_NEW_RULE = 100;
const int ID_POP_REM_RULE = 101;

// auto randomizer rules
void FormMissionParams::OnRulesClick(wxMouseEvent& event)
{	
	wxMenu menu;
	menu.Append(ID_POP_NEW_RULE,"Add new rule");

	auto rid = lboxRndRules->GetSelection();
	if(rid >= 0 || rid < m_spell_map->unit_randomizer.rules.size())
		menu.Append(ID_POP_REM_RULE,"Remove rule");
	
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormMissionParams::OnRulesPopup),NULL,this);
	PopupMenu(&menu);
}
void FormMissionParams::OnRulesPopup(wxCommandEvent& event)
{
	auto menu_id = event.GetId();
	if(menu_id == ID_POP_NEW_RULE)
	{
		// add new rule
		int new_pos;
		if(m_spell_map->unit_randomizer.AddRule(m_spell_data->units,new_pos))
			wxMessageBox(m_spell_map->unit_randomizer.last_error,"Error",wxICON_ERROR);
	}
	else if(menu_id == ID_POP_REM_RULE)
	{
		// remove rule
		auto rid = lboxRndRules->GetSelection();
		if(rid >= 0 || rid < m_spell_map->unit_randomizer.rules.size())
			m_spell_map->unit_randomizer.RemoveRule(rid);
	}
	
	LoadRandRules();
}

// randomzier rule selected
void FormMissionParams::OnSelectRandRule(wxCommandEvent& event)
{
	pgRandRule->Clear();
	if(!m_spell_map)
		return;

	auto rid = lboxRndRules->GetSelection();
	if(rid < 0 || rid >= m_spell_map->unit_randomizer.rules.size())
		return;
	auto &rule = m_spell_map->unit_randomizer.rules[rid];

	pgRandRule->Freeze();
	pgRandRule->Append(new wxEnumPropertyExt(wxT("Reference unit"),wxT(""),MapToPGenumChoices(m_units_list),&rule.ref_unit));
	pgRandRule->Append(new wxMultiChoicePropertyExt(wxT("Randomizer list"),wxT(""),MapToPGenumChoices(m_units_list),&rule.rand_units));
	pgRandRule->Thaw();
	pgRandRule->FitColumns();
}

// edit randomizer rule properties
void FormMissionParams::OnUnitPropChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		// get old rule
		auto rid = lboxRndRules->GetSelection();
		if(rid < 0 || rid >= m_spell_map->unit_randomizer.rules.size())
			return;
		auto& rule = m_spell_map->unit_randomizer.rules[rid];
		auto old_rule = rule;
		
		// update
		obj->Update(prop);		
		if(m_spell_map->unit_randomizer.CheckRules())
		{
			// failed - restore original
			wxMessageBox(m_spell_map->unit_randomizer.last_error,"Error",wxICON_ERROR);
			rule = old_rule;
		}
		m_spell_map->unit_randomizer.SortRules();

		LoadRandRules();
	}
}

// batch change of units randomization setup
void FormMissionParams::OnUnitsRandChange(wxCommandEvent& event)
{
	auto id = event.GetId();
	if(id == wxID_BTN_RAND_TO_AUTO)
	{
		for(auto &unit: m_spell_map->units)
			if(unit->randomize_mode != MapUnit::RandomizeMode::EXPLICIT)
				unit->randomize_mode = MapUnit::RandomizeMode::AUTO;
		for(auto &evt: m_spell_map->events->GetEvents())
			for(auto &unit: evt->units)
				if(unit.unit->randomize_mode != MapUnit::RandomizeMode::EXPLICIT)
					unit.unit->randomize_mode = MapUnit::RandomizeMode::AUTO;
	}
	else if(id == wxID_BTN_RAND_TO_OFF)
	{
		for(auto& unit: m_spell_map->units)
			if(unit->randomize_mode != MapUnit::RandomizeMode::EXPLICIT)
				unit->randomize_mode = MapUnit::RandomizeMode::OFF;
		for(auto& evt: m_spell_map->events->GetEvents())
			for(auto& unit: evt->units)
				if(unit.unit->randomize_mode != MapUnit::RandomizeMode::EXPLICIT)
					unit.unit->randomize_mode = MapUnit::RandomizeMode::OFF;
	}
	else if(id == wxID_BTN_RAND_ENEMY_AUTO)
	{
		for(auto& unit: m_spell_map->units)
			if(unit->randomize_mode != MapUnit::RandomizeMode::EXPLICIT)
				unit->randomize_mode = MapUnit::RandomizeMode::AUTO;		
		for(auto& evt: m_spell_map->events->GetEvents())
			for(auto& unit: evt->units)
			{
				if(unit.unit->randomize_mode != MapUnit::RandomizeMode::EXPLICIT && unit.unit->is_enemy)
					unit.unit->randomize_mode = MapUnit::RandomizeMode::AUTO;
				if(!unit.unit->is_enemy)
					unit.unit->randomize_mode = MapUnit::RandomizeMode::OFF;
			}
	}
}
