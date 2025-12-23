///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_edit_toolset.h"
#include "spellcross.h"

///////////////////////////////////////////////////////////////////////////

FormEditToolset::FormEditToolset( wxWindow* parent,Terrain* terr,int toolset_id, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	m_terr = terr;
	m_toolset_id = toolset_id;
	
	// === AUTO GENERATED STUFF STARTS HERE ===
	this->SetSizeHints(wxSize(400,-1),wxDefaultSize);

	wxBoxSizer* szrNewObj;
	szrNewObj = new wxBoxSizer(wxVERTICAL);

	m_staticText11 = new wxStaticText(this,wxID_ANY,wxT("Toolset name (keep short):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText11->Wrap(-1);
	szrNewObj->Add(m_staticText11,0,wxLEFT|wxTOP,5);

	txtName = new wxTextCtrl(this,wxID_TXT_NAME,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szrNewObj->Add(txtName,0,wxEXPAND|wxRIGHT|wxLEFT,5);

	m_staticText111 = new wxStaticText(this,wxID_ANY,wxT("Toolset description:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText111->Wrap(-1);
	szrNewObj->Add(m_staticText111,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtDescription = new wxTextCtrl(this,wxID_TXT_DESC,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szrNewObj->Add(txtDescription,0,wxEXPAND|wxRIGHT|wxLEFT,5);

	wxBoxSizer* szrScaling;
	szrScaling = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szrScale;
	szrScale = new wxBoxSizer(wxVERTICAL);

	m_staticText26 = new wxStaticText(this,wxID_ANY,wxT("Toolset glyph scaling mode:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText26->Wrap(-1);
	szrScale->Add(m_staticText26,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chbScalingChoices;
	chbScaling = new wxChoice(this,wxID_CHB_SCALE,wxDefaultPosition,wxDefaultSize,chbScalingChoices,0);
	chbScaling->SetSelection(0);
	szrScale->Add(chbScaling,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	szrScaling->Add(szrScale,1,wxEXPAND,5);

	wxBoxSizer* szrWidth;
	szrWidth = new wxBoxSizer(wxVERTICAL);

	m_staticText27 = new wxStaticText(this,wxID_ANY,wxT("Width:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText27->Wrap(-1);
	szrWidth->Add(m_staticText27,0,wxTOP|wxRIGHT|wxLEFT,5);

	scWidth = new wxSpinCtrl(this,wxID_SC_WIDTH,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,32,200,80);
	scWidth->SetMinSize(wxSize(80,-1));
	scWidth->SetMaxSize(wxSize(80,-1));

	szrWidth->Add(scWidth,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	szrScaling->Add(szrWidth,0,wxEXPAND,5);

	wxBoxSizer* szrHeight;
	szrHeight = new wxBoxSizer(wxVERTICAL);

	m_staticText28 = new wxStaticText(this,wxID_ANY,wxT("Height:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText28->Wrap(-1);
	szrHeight->Add(m_staticText28,0,wxTOP|wxRIGHT|wxLEFT,5);

	scHeight = new wxSpinCtrl(this,wxID_SC_HEIGHT,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,32,200,80);
	scHeight->SetMinSize(wxSize(80,-1));
	scHeight->SetMaxSize(wxSize(80,-1));

	szrHeight->Add(scHeight,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	szrScaling->Add(szrHeight,0,wxEXPAND,5);


	szrNewObj->Add(szrScaling,1,wxEXPAND,5);


	//szrNewObj->Add(0,0,1,wxEXPAND,5);

	m_staticline4 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szrNewObj->Add(m_staticline4,0,wxEXPAND | wxALL,5);

	btnOk = new wxButton(this,wxID_BTN_OK,wxT("OK"),wxDefaultPosition,wxDefaultSize,0);
	szrNewObj->Add(btnOk,0,wxALL|wxEXPAND,5);


	this->SetSizer(szrNewObj);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED STUFF ENDS HERE ===
	this->Fit();

	if(!m_terr || m_toolset_id < 0)
	{
		Close();
		return;
	}

	// set initial params
	txtName->SetValue(m_terr->GetToolSetName(m_toolset_id));
	txtDescription->SetValue(m_terr->GetToolSetTitle(m_toolset_id));
	auto [ww,hh] = m_terr->GetToolSetGlyphScaling(m_toolset_id);
	scWidth->SetValue(ww);
	scHeight->SetValue(hh);

	// fill glyph scaling selector
	chbScaling->Append("Mean aspect, fixed w/h");
	chbScaling->Append("Max size");
	chbScaling->Append("Max size, no zoom");
	chbScaling->Select(m_terr->GetToolSetGlyphScalingMode(m_toolset_id));	

	Bind(wxEVT_CLOSE_WINDOW,&FormEditToolset::OnClose,this,this->m_windowId);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEditToolset::OnCloseClick,this,wxID_BTN_OK);

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

}

FormEditToolset::~FormEditToolset()
{
}

// on close
void FormEditToolset::OnClose(wxCloseEvent& ev)
{
	if(!m_terr || m_toolset_id < 0)
		return;

	// update toolset
	m_terr->SetToolSetName(m_toolset_id,txtName->GetValue().ToStdString());
	m_terr->SetToolSetTitle(m_toolset_id,txtDescription->GetValue().ToStdString());
	m_terr->SetToolSetGlyphScalingMode(m_toolset_id,chbScaling->GetSelection());
	m_terr->SetToolSetGlyphScaling(m_toolset_id,scWidth->GetValue(),scHeight->GetValue());

	EndModal(wxID_OK);
}

// close form
void FormEditToolset::OnCloseClick(wxCommandEvent& event)
{
	Close();
}