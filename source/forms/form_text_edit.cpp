///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_text_edit.h"
#include "other.h"
#include "wx_other.h"

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

#include <string>

///////////////////////////////////////////////////////////////////////////

FormTextEdit::FormTextEdit(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	// === AUTO GENERATOR START ===
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormTextEdit' on 2026-05-17 15:16:44
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_menubar14 = new wxMenuBar( 0 );
	mFile = new wxMenu();
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mFile, wxID_MM_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mmExit );
	
	m_menubar14->Append( mFile, wxT("File") );
	
	mLeft = new wxMenu();
	wxMenuItem* mmLeftOpen;
	mmLeftOpen = new wxMenuItem( mLeft, wxID_MM_LEFT_OPEN, wxString( wxT("Open folder") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mLeft->Append( mmLeftOpen );
	
	wxMenuItem* mmLeftSave;
	mmLeftSave = new wxMenuItem( mLeft, wxID_MM_LEFT_SAVE, wxString( wxT("Save") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mLeft->Append( mmLeftSave );
	
	m_menubar14->Append( mLeft, wxT("Left") );
	
	mRight = new wxMenu();
	wxMenuItem* mmRightOpen;
	mmRightOpen = new wxMenuItem( mRight, wxID_MM_RIGHT_OPEN, wxString( wxT("Open folder") ) + wxT('\t') + wxT("Alt+O"), wxEmptyString, wxITEM_NORMAL );
	mRight->Append( mmRightOpen );
	
	wxMenuItem* mmRightSave;
	mmRightSave = new wxMenuItem( mRight, wxID_MM_RIGHT_SAVE, wxString( wxT("Save") ) + wxT('\t') + wxT("Alt+S"), wxEmptyString, wxITEM_NORMAL );
	mRight->Append( mmRightSave );
	
	m_menubar14->Append( mRight, wxT("Right") );
	
	this->SetMenuBar( m_menubar14 );
	
	wxBoxSizer* bSizer119;
	bSizer119 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer120;
	bSizer120 = new wxBoxSizer( wxVERTICAL );
	
	bSizer120->SetMinSize( wxSize( 180,-1 ) );
	m_staticText122 = new wxStaticText( this, wxID_ANY, wxT("Wildcard filter (?*):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText122->Wrap( -1 );
	bSizer120->Add( m_staticText122, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	txtLeftFilter = new wxTextCtrl( this, wxID_TXT_LEFT_FILTER, wxT("*"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer120->Add( txtLeftFilter, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticText123 = new wxStaticText( this, wxID_ANY, wxT("List:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText123->Wrap( -1 );
	bSizer120->Add( m_staticText123, 0, wxRIGHT|wxLEFT, 5 );
	
	listLeft = new wxListBox( this, wxID_LIST_LEFT, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer120->Add( listLeft, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText125 = new wxStaticText( this, wxID_ANY, wxT("Encoding:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText125->Wrap( -1 );
	bSizer120->Add( m_staticText125, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chLeftEncodingChoices;
	chLeftEncoding = new wxChoice( this, wxID_CB_LEFT_ENCODING, wxDefaultPosition, wxDefaultSize, chLeftEncodingChoices, 0 );
	chLeftEncoding->SetSelection( 0 );
	bSizer120->Add( chLeftEncoding, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer119->Add( bSizer120, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText124 = new wxStaticText( this, wxID_ANY, wxT("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText124->Wrap( -1 );
	bSizer121->Add( m_staticText124, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	txtLeft = new wxTextCtrl( this, wxID_TXT_LEFT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer121->Add( txtLeft, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer119->Add( bSizer121, 1, wxEXPAND, 5 );
	
	m_staticline45 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer119->Add( m_staticline45, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer128;
	bSizer128 = new wxBoxSizer( wxVERTICAL );
	
	
	bSizer128->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnToLeft = new wxBitmapButton( this, wxID_BTN_TO_LEFT, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	btnToLeft->SetMinSize( wxSize( 40,40 ) );
	
	bSizer128->Add( btnToLeft, 0, wxALL|wxEXPAND, 5 );
	
	btnToRight = new wxBitmapButton( this, wxID_BTN_TO_RIGHT, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	btnToRight->SetMinSize( wxSize( 40,40 ) );
	
	bSizer128->Add( btnToRight, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer128->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	bSizer119->Add( bSizer128, 0, wxEXPAND, 5 );
	
	m_staticline451 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer119->Add( m_staticline451, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer1201;
	bSizer1201 = new wxBoxSizer( wxVERTICAL );
	
	bSizer1201->SetMinSize( wxSize( 180,-1 ) );
	m_staticText1221 = new wxStaticText( this, wxID_ANY, wxT("Wildcard filter (?*):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1221->Wrap( -1 );
	bSizer1201->Add( m_staticText1221, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	txtRightFilter = new wxTextCtrl( this, wxID_TXT_RIGHT_FILTER, wxT("*"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer1201->Add( txtRightFilter, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticText1231 = new wxStaticText( this, wxID_ANY, wxT("List:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1231->Wrap( -1 );
	bSizer1201->Add( m_staticText1231, 0, wxRIGHT|wxLEFT, 5 );
	
	listRight = new wxListBox( this, wxID_LIST_RIGHT, wxDefaultPosition, wxDefaultSize, 0, NULL, wxLB_ALWAYS_SB );
	bSizer1201->Add( listRight, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText1251 = new wxStaticText( this, wxID_ANY, wxT("Encoding:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1251->Wrap( -1 );
	bSizer1201->Add( m_staticText1251, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chRightEncodingChoices;
	chRightEncoding = new wxChoice( this, wxID_CB_RIGHT_ENCODING, wxDefaultPosition, wxDefaultSize, chRightEncodingChoices, 0 );
	chRightEncoding->SetSelection( 0 );
	bSizer1201->Add( chRightEncoding, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer119->Add( bSizer1201, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer1211;
	bSizer1211 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1241 = new wxStaticText( this, wxID_ANY, wxT("Text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1241->Wrap( -1 );
	bSizer1211->Add( m_staticText1241, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	txtRight = new wxTextCtrl( this, wxID_TXT_RIGHT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer1211->Add( txtRight, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer119->Add( bSizer1211, 1, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer119 );
	this->Layout();
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormTextEdit' on 2026-05-17 15:16:44
	// === AUTO GENERATOR END ===

	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	AssignSVGresourceToMenu(mFile,wxID_MM_EXIT,"IDR_EXIT");
	AssignSVGresourceToMenu(mLeft,wxID_MM_LEFT_OPEN,"IDR_OPEN3");
	AssignSVGresourceToMenu(mRight,wxID_MM_RIGHT_OPEN,"IDR_OPEN3");
	AssignSVGresourceToMenu(mLeft,wxID_MM_LEFT_SAVE,"IDR_SAVE");
	AssignSVGresourceToMenu(mRight,wxID_MM_RIGHT_SAVE,"IDR_SAVE");

	auto op_sz = FromDIP(wxSize(32,32));
	btnToLeft->SetBitmap(LoadSVGiconsBundle("IDR_MOVE_LEFT").GetBitmap(op_sz));
	btnToLeft->SetBitmapHover(LoadSVGiconsBundle("IDR_MOVE_LEFT_RED").GetBitmap(op_sz));
	btnToRight->SetBitmap(LoadSVGiconsBundle("IDR_MOVE_RIGHT").GetBitmap(op_sz));
	btnToRight->SetBitmapHover(LoadSVGiconsBundle("IDR_MOVE_RIGHT_RED").GetBitmap(op_sz));

	Bind(wxEVT_MENU,&FormTextEdit::OnCloseClick,this,wxID_MM_EXIT);

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTextEdit::OnOpenClick,this,wxID_MM_LEFT_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTextEdit::OnOpenClick,this,wxID_MM_RIGHT_OPEN);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTextEdit::OnSaveText,this,wxID_MM_LEFT_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTextEdit::OnSaveText,this,wxID_MM_RIGHT_SAVE);
	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormTextEdit::OnChangeFilter,this,wxID_TXT_LEFT_FILTER);
	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormTextEdit::OnChangeFilter,this,wxID_TXT_RIGHT_FILTER);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormTextEdit::OnChangeSource,this,wxID_LIST_LEFT);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormTextEdit::OnChangeSource,this,wxID_LIST_RIGHT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormTextEdit::OnMoveText,this,wxID_BTN_TO_LEFT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormTextEdit::OnMoveText,this,wxID_BTN_TO_RIGHT);

	chLeftEncoding->Clear();
	chLeftEncoding->Append("CP895");
	chLeftEncoding->Select(0);
	chRightEncoding->Clear();
	chRightEncoding->Append("CP895");
	chRightEncoding->Select(0);

	sbar->SetFieldsCount(2);
	const int widths[] = {-1,-1};
	SetStatusWidths(2,widths);
}

FormTextEdit::~FormTextEdit()
{
}

void FormTextEdit::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormTextEdit::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

// move text
void FormTextEdit::OnMoveText(wxCommandEvent& event)
{	
	if(event.GetId() == wxID_BTN_TO_LEFT)
		txtLeft->SetValue(txtRight->GetValue());
	else
		txtRight->SetValue(txtLeft->GetValue());	
}

// open folder
void FormTextEdit::OnOpenClick(wxCommandEvent& event)
{
	// show save dialog
	wxDirDialog saveDirDialog(this,"Open folder","",wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	auto dir = saveDirDialog.GetPath().ToStdWstring();
	
	if(event.GetId() == wxID_MM_LEFT_OPEN)
		m_dir_left = dir;
	else
		m_dir_right = dir;

	OnChangeFilter(event);
}

// change wildcard filter
void FormTextEdit::OnChangeFilter(wxCommandEvent& event)
{
	wxListBox *list;
	wxTextCtrl *filter;	
	if(event.GetId() == wxID_TXT_LEFT_FILTER || event.GetId() == wxID_MM_LEFT_OPEN || event.GetId() == wxID_TXT_LEFT)
	{
		list = listLeft;
		filter = txtLeftFilter;
		m_dir = m_dir_left;
		SetStatusText(m_dir_left.wstring(),0);
	}
	else
	{
		list = listRight;
		filter = txtRightFilter;
		m_dir = m_dir_right;
		SetStatusText(m_dir_right.wstring(),1);
	}

	auto wild = filter->GetValue().ToStdString();
	std::vector<std::string> names;
	fs_list_dir(m_dir, wild, true, false, &names);
	std::sort(names.begin(), names.end());
		
	list->Freeze();
	list->Clear();	
	for(auto &name: names)
		list->Append(name);
	list->Thaw();
	if(list->GetCount())
		list->Select(0);
}


// change file selection
void FormTextEdit::OnChangeSource(wxCommandEvent& event)
{
	wxListBox* list;
	wxTextCtrl* text;
	wxChoice* lang;
	std::filesystem::path dir;
	if(event.GetId() == wxID_TXT_LEFT_FILTER || event.GetId() == wxID_MM_LEFT_OPEN || event.GetId() == wxID_LIST_LEFT)
	{
		list = listLeft;
		text = txtLeft;
		lang = chLeftEncoding;
		dir = m_dir_left;
	}
	else
	{
		list = listRight;
		text = txtRight;
		lang = chRightEncoding;
		dir = m_dir_right;
	}
	
	auto path = dir / list->GetStringSelection().ToStdWstring();
	std::string str;
	loadstr(path, str);

	std::wstring wstr;
	auto langstr = lang->GetStringSelection();
	if(langstr == "CP895")
		wstr = char2wstringCP895(str.c_str());
	else
		wstr = char2wstring(str.c_str());
	text->SetValue(wstr);
}

// change file selection
void FormTextEdit::OnSaveText(wxCommandEvent& event)
{
	wxListBox* list;
	wxTextCtrl* text;
	wxChoice* lang;
	std::filesystem::path dir;
	if(event.GetId() == wxID_MM_LEFT_SAVE)
	{
		list = listLeft;
		text = txtLeft;
		lang = chLeftEncoding;
		dir = m_dir_left;
	}
	else
	{
		list = listRight;
		text = txtRight;
		lang = chRightEncoding;
		dir = m_dir_right;
	}

	std::wstring wstr = text->GetValue().ToStdWstring();
	std::string str;
	auto langstr = lang->GetStringSelection();
	if(langstr == "CP895")
		str = wstring2stringCP895(wstr);
	else
		str = wstring2string(wstr);
		
	auto path = dir / list->GetStringSelection().ToStdWstring();
	savestr(path,str);
}

