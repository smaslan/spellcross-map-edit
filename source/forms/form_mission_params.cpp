///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_mission_params.h"

///////////////////////////////////////////////////////////////////////////

FormMissionParams::FormMissionParams( wxWindow* parent,SpellData* spell_data,SpellMap *spell_map,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	m_spell_data = spell_data;
	m_spell_map = spell_map;

	// === START OF AUTO GENERATED STUFF ===

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


	bSizer69->Add( bSizer71, 1, wxEXPAND, 5 );

	m_staticline27 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer69->Add( m_staticline27, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	btnOK = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer69->Add( btnOK, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );


	this->SetSizer( bSizer69 );
	this->Layout();

	this->Centre( wxBOTH );

	// === END OF AUTO GENERATED STUFF ===

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMissionParams::OnClose,this,wxID_BTN_OK);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_TEXT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_START_TEXT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_GOOD_END_TEXT);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMissionParams::OnSelectTxt,this,wxID_CH_BAD_END_TEXT);

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
