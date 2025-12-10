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
	this->SetSizeHints(wxSize(400,190),wxDefaultSize);

	wxBoxSizer* szrNewObj;
	szrNewObj = new wxBoxSizer(wxVERTICAL);

	m_staticText11 = new wxStaticText(this,wxID_ANY,wxT("Object description:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText11->Wrap(-1);
	szrNewObj->Add(m_staticText11,0,wxLEFT|wxTOP,5);

	txtDescription = new wxTextCtrl(this,wxID_TXT_DESC,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szrNewObj->Add(txtDescription,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText92 = new wxStaticText(this,wxID_ANY,wxT("Objects class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText92->Wrap(-1);
	szrNewObj->Add(m_staticText92,0,wxLEFT|wxRIGHT|wxTOP,5);

	wxArrayString chbClassChoices;
	chbClass = new wxChoice(this,wxID_CHB_CLASS,wxDefaultPosition,wxDefaultSize,chbClassChoices,0);
	chbClass->SetSelection(0);
	szrNewObj->Add(chbClass,0,wxEXPAND|wxLEFT|wxRIGHT,5);


	szrNewObj->Add(0,0,1,wxEXPAND,5);

	m_staticline4 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szrNewObj->Add(m_staticline4,0,wxEXPAND | wxALL,5);

	btnOk = new wxButton(this,wxID_BTN_OK,wxT("OK"),wxDefaultPosition,wxDefaultSize,0);
	szrNewObj->Add(btnOk,0,wxALL|wxEXPAND,5);


	this->SetSizer(szrNewObj);
	this->Layout();

	this->Centre(wxBOTH);

	// === END OF AUTO GENERATED STUFF ===

	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	// default object name
	txtDescription->SetValue("New object");

	// fill classes list
	chbClass->Freeze();
	chbClass->Clear();
	chbClass->Append("None");
	for(int k = 0; k < terrain->GetToolsCount(); k++)
		chbClass->Append(terrain->GetToolSetName(k));
	chbClass->Thaw();
	chbClass->Select(0);

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
int FormNewObject::GetClass()
{
	return(chbClass->GetSelection());
}

