///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_new_object.h"

///////////////////////////////////////////////////////////////////////////

FormNewObject::FormNewObject(wxWindow* parent,Terrain* terrain,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxDialog(parent,id,title,pos,size,style)
{
	this->SetSizeHints(wxSize(400,165),wxDefaultSize);

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer(wxVERTICAL);

	m_staticText11 = new wxStaticText(this,wxID_ANY,wxT("Object description:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText11->Wrap(-1);
	bSizer7->Add(m_staticText11,0,wxLEFT|wxTOP,5);

	txtDescription = new wxTextCtrl(this,wxID_TXT_DESC,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	bSizer7->Add(txtDescription,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText12 = new wxStaticText(this,wxID_ANY,wxT("Object glyph name (must be unique):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText12->Wrap(-1);
	bSizer7->Add(m_staticText12,0,wxLEFT|wxRIGHT|wxTOP,5);

	txtGlyphName = new wxTextCtrl(this,wxID_TXT_GLYPH,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	bSizer7->Add(txtGlyphName,0,wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND,5);


	bSizer7->Add(0,0,1,wxEXPAND,5);

	m_staticline4 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer7->Add(m_staticline4,0,wxEXPAND | wxALL,5);

	btnOk = new wxButton(this,wxID_BTN_OK,wxT("OK"),wxDefaultPosition,wxDefaultSize,0);
	bSizer7->Add(btnOk,0,wxALL|wxEXPAND,5);


	this->SetSizer(bSizer7);
	this->Layout();

	this->Centre(wxBOTH);

	// === END OF AUTO GENERATED STUFF ===

	// default object name
	txtDescription->SetValue("New object");
	// suggest unique glyph name
	txtGlyphName->SetValue(terrain->SuggestObjectGlyphName());

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormNewObject::OnClose,this,wxID_BTN_OK);
}

FormNewObject::~FormNewObject()
{
}

// on close form
void FormNewObject::OnClose(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

// return stuff
std::string FormNewObject::GetDescription()
{
	return(txtDescription->GetValue().ToStdString());
}
std::string FormNewObject::GetGlyphName()
{
	return(txtGlyphName->GetValue().ToStdString());
}

