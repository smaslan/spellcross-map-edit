///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_text.h"
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include <filesystem>
#include "wx_other.h"

///////////////////////////////////////////////////////////////////////////

FormText::FormText( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	m_spell_data = spell_data;

	// === AUTO GENERATED START ===
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormText' on 2026-06-19 22:08:22
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	m_menubar13 = new wxMenuBar( 0 );
	mFile = new wxMenu();
	wxMenuItem* mmNew;
	mmNew = new wxMenuItem( mFile, wxID_MM_NEW, wxString( wxT("New Resource") ) + wxT('\t') + wxT("Ctrl+N"), wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mmNew );
	
	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( mFile, wxID_MM_SAVE, wxString( wxT("Export Resource") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mmSave );
	
	wxMenuItem* mmSaveAll;
	mmSaveAll = new wxMenuItem( mFile, wxID_MM_SAVE_ALL, wxString( wxT("Export All") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mmSaveAll );
	
	mFile->AppendSeparator();
	
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mFile, wxID_MM_EXIT, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	mFile->Append( mmExit );
	
	m_menubar13->Append( mFile, wxT("File") );
	
	mEdit = new wxMenu();
	wxMenuItem* mmRestore;
	mmRestore = new wxMenuItem( mEdit, wxID_MM_RESTORE, wxString( wxT("Revert chagnes") ) + wxT('\t') + wxT("Ctrl+R"), wxEmptyString, wxITEM_NORMAL );
	mEdit->Append( mmRestore );
	
	wxMenuItem* mmApplyChanges;
	mmApplyChanges = new wxMenuItem( mEdit, wxID_MM_APPLY, wxString( wxT("Save changes") ) + wxT('\t') + wxT("Ctrl+Enter"), wxEmptyString, wxITEM_NORMAL );
	mEdit->Append( mmApplyChanges );
	
	m_menubar13->Append( mEdit, wxT("Edit") );
	
	this->SetMenuBar( m_menubar13 );
	
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	wxBoxSizer* bSizer105;
	bSizer105 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText106 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText106->Wrap( -1 );
	bSizer107->Add( m_staticText106, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chSourceChoices;
	chSource = new wxChoice( this, wxID_CH_SOURCE, wxDefaultPosition, wxDefaultSize, chSourceChoices, 0 );
	chSource->SetSelection( 0 );
	bSizer107->Add( chSource, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );
	
	m_staticText107 = new wxStaticText( this, wxID_ANY, wxT("Filter (wildcard: ?*):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText107->Wrap( -1 );
	bSizer107->Add( m_staticText107, 0, wxRIGHT|wxLEFT, 5 );
	
	txtFilter = new wxTextCtrl( this, wxID_TXT_WILD, wxT("*"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer107->Add( txtFilter, 0, wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND, 5 );
	
	m_staticText108 = new wxStaticText( this, wxID_ANY, wxT("Resources:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText108->Wrap( -1 );
	bSizer107->Add( m_staticText108, 0, wxRIGHT|wxLEFT, 5 );
	
	listResources = new wxListBox( this, wxID_LIST_RESOURCES, wxDefaultPosition, wxSize( 180,-1 ), 0, NULL, 0 );
	bSizer107->Add( listResources, 1, wxBOTTOM|wxLEFT|wxRIGHT, 5 );
	
	m_staticText131 = new wxStaticText( this, wxID_ANY, wxT("Resource name:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText131->Wrap( -1 );
	bSizer107->Add( m_staticText131, 0, wxRIGHT|wxLEFT, 5 );
	
	txtName = new wxTextCtrl( this, wxID_TXT_NAME, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer107->Add( txtName, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer106->Add( bSizer107, 0, wxEXPAND, 5 );
	
	m_staticline39 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer106->Add( m_staticline39, 0, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer108;
	bSizer108 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText109 = new wxStaticText( this, wxID_ANY, wxT("Source text:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText109->Wrap( -1 );
	bSizer108->Add( m_staticText109, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	textEdit = new wxTextCtrl( this, wxID_TXT_EDIT, wxEmptyString, wxDefaultPosition, wxSize( 400,-1 ), wxTE_MULTILINE );
	bSizer108->Add( textEdit, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText111 = new wxStaticText( this, wxID_ANY, wxT("Target window:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	bSizer108->Add( m_staticText111, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chTargetChoices;
	chTarget = new wxChoice( this, wxID_CH_TARGET, wxDefaultPosition, wxDefaultSize, chTargetChoices, 0 );
	chTarget->SetSelection( 0 );
	bSizer108->Add( chTarget, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticText114 = new wxStaticText( this, wxID_ANY, wxT("Text align mode:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText114->Wrap( -1 );
	bSizer108->Add( m_staticText114, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chAlignChoices;
	chAlign = new wxChoice( this, wxID_CH_ALIGN, wxDefaultPosition, wxDefaultSize, chAlignChoices, 0 );
	chAlign->SetSelection( 0 );
	bSizer108->Add( chAlign, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer106->Add( bSizer108, 0, wxEXPAND, 5 );
	
	m_staticline40 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer106->Add( m_staticline40, 0, wxEXPAND|wxTOP, 5 );
	
	wxBoxSizer* bSizer109;
	bSizer109 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText110 = new wxStaticText( this, wxID_ANY, wxT("Render:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText110->Wrap( -1 );
	bSizer109->Add( m_staticText110, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer110;
	bSizer110 = new wxBoxSizer( wxHORIZONTAL );
	
	canvasText = new wxPanel( this, wxID_CANVAS_TEXT, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer110->Add( canvasText, 1, wxEXPAND | wxALL, 5 );
	
	scrollText = new wxScrollBar( this, wxID_SCROLL_TEXT, wxDefaultPosition, wxDefaultSize, wxSB_VERTICAL );
	bSizer110->Add( scrollText, 0, wxEXPAND|wxALL, 5 );
	
	
	bSizer109->Add( bSizer110, 1, wxEXPAND, 5 );
	
	
	bSizer106->Add( bSizer109, 1, wxEXPAND, 5 );
	
	
	bSizer105->Add( bSizer106, 1, wxEXPAND, 5 );
	
	m_staticline38 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer105->Add( m_staticline38, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText105 = new wxStaticText( this, wxID_ANY, wxT("Font preview:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText105->Wrap( -1 );
	bSizer105->Add( m_staticText105, 0, wxLEFT, 5 );
	
	canvasFont = new wxPanel( this, wxID_CANVAS_FONT, wxDefaultPosition, wxSize( -1,80 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer105->Add( canvasFont, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	this->SetSizer( bSizer105 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormText' on 2026-06-19 22:08:22
	// === AUTO GENERATED END ===

	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	AssignSVGresourceToMenu(mFile,wxID_MM_EXIT,"IDR_EXIT");
	AssignSVGresourceToMenu(mFile,wxID_MM_NEW,"IDR_NEW");
	AssignSVGresourceToMenu(mFile,wxID_MM_SAVE,"IDR_SAVE");
	AssignSVGresourceToMenu(mFile,wxID_MM_SAVE_ALL,"IDR_SAVE");
	AssignSVGresourceToMenu(mEdit,wxID_MM_APPLY,"IDR_SAVE");
	AssignSVGresourceToMenu(mEdit,wxID_MM_RESTORE,"IDR_RELOAD");
	
	Bind(wxEVT_MENU,&FormText::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_MENU,&FormText::OnChangeResource,this,wxID_MM_RESTORE);
	Bind(wxEVT_MENU,&FormText::OnChangeApply,this,wxID_MM_APPLY);
	Bind(wxEVT_MENU,&FormText::OnNew,this,wxID_MM_NEW);
	Bind(wxEVT_MENU,&FormText::OnSave,this,wxID_MM_SAVE);
	Bind(wxEVT_MENU,&FormText::OnSaveAll,this,wxID_MM_SAVE_ALL);
	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormText::OnChangeFilter,this,wxID_TXT_WILD);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormText::OnChangeResource,this,wxID_LIST_RESOURCES);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormText::OnChangeSource,this,wxID_CH_SOURCE);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormText::OnChangeTargetPanel,this,wxID_CH_TARGET);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormText::OnChangeTargetPanel,this,wxID_CH_ALIGN);
	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormText::OnChangeText,this,wxID_TXT_EDIT);

	canvasFont->SetDoubleBuffered(true);
	canvasFont->Bind(wxEVT_PAINT,&FormText::OnPaintFont,this,wxID_CANVAS_FONT);
	canvasText->SetDoubleBuffered(true);
	canvasText->Bind(wxEVT_PAINT,&FormText::OnPaintText,this,wxID_CANVAS_TEXT);
	
	Bind(wxEVT_SCROLL_THUMBTRACK,&FormText::OnTextScroll,this,wxID_SCROLL_TEXT);
	Bind(wxEVT_SCROLL_LINEUP,&FormText::OnTextScroll,this,wxID_SCROLL_TEXT);
	Bind(wxEVT_SCROLL_LINEDOWN,&FormText::OnTextScroll,this,wxID_SCROLL_TEXT);
	Bind(wxEVT_SCROLL_PAGEUP,&FormText::OnTextScroll,this,wxID_SCROLL_TEXT);
	Bind(wxEVT_SCROLL_PAGEDOWN,&FormText::OnTextScroll,this,wxID_SCROLL_TEXT);

	Bind(wxEVT_SIZE,&FormText::OnTextCanvasSize,this,wxID_ANY);

	// source menu
	chSource->Clear();
	chSource->Append("TEXTS.FS");
	chSource->Append("RESEARCH.FS");
	chSource->Append("INFO.FS");
	chSource->Select(0);

	FillResources();
		
	// fill window layouts list
	m_windows = {
		{"Generic in-game message box", NULL, 10,8, 375,-1, 1, 255,0, SpellFont::FontShadow::DIAG3,SpellTextLines::WrapMode::LEFT,SpellTextRec::TextPanel::MESSAGE,"TEXTS.FS",{"U*","MSG*","E*","T*.S","T*.OK","T*.BAD"}},
		{"Unit info panel", m_spell_data->gres.inf_panel, 45,30, 370,350, 4, 248,0, SpellFont::FontShadow::DIAG3,SpellTextLines::WrapMode::STRETCH,SpellTextRec::TextPanel::UNIT_INFO,"INFO.FS",{"*"}},
		{"Big map research info panel", m_spell_data->gres.bm_info_panel, 20,5, 376,416, 4, 249,0, SpellFont::FontShadow::RIGHT_DOWN,SpellTextLines::WrapMode::STRETCH,SpellTextRec::TextPanel::BM_RESEARCH_INFO,"RESEARCH.FS",{"*.INF"}},
		{"Big map research panel", m_spell_data->gres.bm_research_panel, 18,65, 352,123, 0, 249,0, SpellFont::FontShadow::RIGHT_DOWN,SpellTextLines::WrapMode::CENTER,SpellTextRec::TextPanel::BM_RESEARCH_PANEL,"RESEARCH.FS",{"*.BRF"}},
		{"Big map mission info panel", m_spell_data->gres.bm_mission_info_panel, 28,29, 352,126, 0, 127,0, SpellFont::FontShadow::RIGHT_DOWN,SpellTextLines::WrapMode::CENTER,SpellTextRec::TextPanel::BM_MISSION,"TEXTS.FS",{"T*"}},
	};
	chTarget->Freeze();
	chTarget->Clear();
	for(auto &item: m_windows)
		chTarget->Append(item.name);
	chTarget->Thaw();
	chTarget->Select(0);

	// make alignment menu	
	chAlign->Freeze();
	chAlign->Clear();
	static std::vector<SpellTextLines::WrapMode> c_align_modes = {SpellTextLines::WrapMode::LEFT, SpellTextLines::WrapMode::CENTER, SpellTextLines::WrapMode::STRETCH};
	chAlign->Append("Left",&c_align_modes[0]);
	chAlign->Append("Center",&c_align_modes[1]);
	chAlign->Append("Stretch",&c_align_modes[2]);
	chAlign->Thaw();
}



FormText::~FormText()
{
	
}

void FormText::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(),ev);
	ev.Skip();
	Destroy();
}

// close form
void FormText::OnCloseClick(wxCommandEvent& event)
{
	Close();
}


// save resource
void FormText::OnSave(wxCommandEvent& event)
{
	// selected resource
	/*auto name = listResources->GetStringSelection();
	if(name.empty())
		return;*/
	
	if(m_text.isEmpty())
		return;

	// apply changes
	OnChangeApply(event);
			
	// show save dialog
	wxFileDialog saveFileDialog(this,_("Export text resource"),m_spell_data->export_path,m_text.name,"Spellcross text resource file (*.*)|*.*",
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());
	m_spell_data->export_path = saveFileDialog.GetDirectory().ToStdWstring();

	// save it
	savestr(path, m_text.raw_text);
}

// save all resources
void FormText::OnSaveAll(wxCommandEvent& event)
{
	// show save dialog
	wxDirDialog saveDirDialog(this,"Export multiple text resources",m_spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring dir = wstring(saveDirDialog.GetPath().ToStdWstring());
	m_spell_data->export_path = dir;

	// rather ask for permission
	wxMessageDialog msg(NULL,"Files in the selected folder might be overwritten! Continue?","Exporting text resources",wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if(msg.ShowModal() != wxID_YES)
		return;

	// list through all resources
	for(int k = 0; k < listResources->GetCount(); k++)
	{
		// select resource
		listResources->Select(k);		
		OnChangeResource(event);
		if(m_text.isEmpty())
			continue;

		// apply changes
		OnChangeApply(event);

		// save it
		auto path = std::filesystem::path(dir).append(m_text.name).wstring();
		savestr(path,m_text.raw_text);
	}
}



// change source filter
void FormText::OnChangeFilter(wxCommandEvent& event)
{
	FillResources();
}

// fill resource list
void FormText::FillResources()
{
	auto src_name = chSource->GetString(chSource->GetSelection());
	auto item_name = txtName->GetValue();
		
	listResources->Freeze();
	listResources->Clear();
	if(src_name == "TEXTS.FS")
	{
		for(auto& item: m_spell_data->texts->GetTexts())
		{
			if(!wildcmp(txtFilter->GetValue(),item->name.c_str()))
				continue;
			listResources->Append(item->name);
		}
	}
	else if(src_name == "RESEARCH.FS")
	{
		for(auto& item: m_spell_data->research_texts->GetTexts())
		{
			if(!wildcmp(txtFilter->GetValue(),item->name.c_str()))
				continue;
			listResources->Append(item->name);
		}
	}
	else if(src_name == "INFO.FS")
	{
		for(auto& item: m_spell_data->units->GetUnits())
		{			
			if(!item->info_text || !wildcmp(txtFilter->GetValue(),item->info_text->name.c_str()))
				continue;
			listResources->Append(item->info_text->name);
		}
	}
	listResources->Thaw();
	if(!item_name.empty())
		listResources->SetStringSelection(item_name);
}

// change resource
void FormText::OnChangeSource(wxCommandEvent& event)
{
	FillResources();
}

// change target panel
void FormText::OnChangeTargetPanel(wxCommandEvent& event)
{
	PrepareText();
}

// apply text changes
void FormText::OnChangeApply(wxCommandEvent& event)
{
	if(m_text.isEmpty())
		return;
	if(!m_text_org)
		return;

	// update local text
	m_text.text = textEdit->GetValue();	
	m_text.modified = true;
	
	// get window layout
	auto win = GetTextWindow(m_text_org);
	if(!win)
		return;
	
	auto win_sel = chTarget->GetSelection();
	auto win_type = win->panel;
	if(win_sel >= 0)
		win_type = m_windows[win_sel].panel;	
	
	// update text
	if(m_text_org->UpdateText(m_text.text, win->x_size, m_spell_data->font))
		return;
	m_text_org->text_panel = win_type;
	m_text.raw_text = m_text_org->raw_text;
}

// change text
void FormText::OnChangeText(wxCommandEvent& event)
{
	m_text.text = textEdit->GetValue().ToStdWstring();
	PrepareText();
}

// get window setup for this text resource
FormText::SpellWindow *FormText::GetTextWindow(SpellTextRec *text)
{
	if(!text)
		return(NULL);
	// try to auto select target panel based on text resource name
	for(auto &window: m_windows)
	{
		if(window.panel != text->text_panel)
			continue;
		return(&window);
	}
	return(NULL);
}


// create new resource
void FormText::OnNew(wxCommandEvent& event)
{
	auto name = txtName->GetValue().ToStdString();
	auto text = GetText(name);
	if(text)
	{
		wxMessageBox(string_format("Text resource name \"%s\" already present! Use different name.",name.c_str()),"Creating new text resource",wxICON_EXCLAMATION);
		return;
	}	
	if(name.empty() || std::filesystem::path(name).stem().string().length() > 8)
	{
		wxMessageBox(string_format("Text resource name \"%s\" not valid! Must be valid DOS name of maximum 8.3 size.",name.c_str()),"Creating new text resource",wxICON_EXCLAMATION);
		return;
	}
	text = CreateText(name);
	if(!text)
	{
		wxMessageBox(string_format("Creating text resource name \"%s\" failed!",name.c_str()),"Creating new text resource",wxICON_EXCLAMATION);
		return;
	}	
	OnChangeApply(event);
	FillResources();
}


// change resource
void FormText::OnChangeResource(wxCommandEvent& event)
{	
	// clear old one
	m_text = SpellTextRec();
	m_text_org = NULL;

	txtName->Clear();

	// get new one
	auto text = GetText();
	if(!text)
		return;
	m_text_org = text;
	// make local work copy
	m_text = *text;

	txtName->SetValue(text->name);
	textEdit->SetValue(m_text.text);

	auto src_name = chSource->GetStringSelection();
	if(src_name.empty())
		return;
	
	auto win = GetTextWindow(&m_text);
	if(!win)
		return;
	auto win_id = win - m_windows.data();
	chTarget->Select(win_id);

	// select align mode
	for(int k = 0; k < chAlign->GetCount(); k++)
	{
		auto data = (SpellTextLines::WrapMode*)chAlign->GetClientData(k);
		if(data && *data == win->align)
		{
			chAlign->Select(k);
			break;
		}
	}

	PrepareText();
}


SpellTextRec *FormText::GetText(std::string name)
{
	if(name.empty())
		name = listResources->GetStringSelection();
	if(name.empty())
		return(NULL);

	auto src_name = chSource->GetStringSelection();
	if(src_name == "TEXTS.FS")
	{
		return(m_spell_data->texts->GetText(name));
	}
	else if(src_name == "RESEARCH.FS")
	{
		return(m_spell_data->research_texts->GetText(name));
	}
	else if(src_name == "INFO.FS")
	{
		for(auto &item: m_spell_data->units->GetUnits())
		{
			if(item->info_text && item->info_text->name.compare(name) == 0)
				return(item->info_text);
		}
	}
	return(NULL);
}

SpellTextRec* FormText::CreateText(std::string name)
{
	auto src_name = chSource->GetStringSelection();
	if(src_name == "TEXTS.FS")
	{
		auto text = m_spell_data->texts->AddText(name);
		m_spell_data->texts->Sort();
		return(text);
	}
	else if(src_name == "RESEARCH.FS")
	{
		auto text = m_spell_data->research_texts->AddText(name);
		m_spell_data->research_texts->Sort();
		return(text);
	}
	else if(src_name == "INFO.FS")
	{
		// not implemented
		return(NULL);
	}
	return(NULL);
}

void FormText::PrepareText()
{
	m_font = NULL;
	m_text_lines.lines.clear();
		
	// select window layout
	int win_sel = chTarget->GetSelection();
	if(win_sel < 0)
		return;
	m_window = &m_windows[win_sel];

	// select text align mode
	SpellTextLines::WrapMode align = m_window->align;
	if(chAlign->GetSelection() >= 0)
		align = *(SpellTextLines::WrapMode*)chAlign->GetClientData(chAlign->GetSelection());
	
	// canvas size
	auto rect = canvasText->GetClientSize();
	int x_surf = rect.GetWidth();
	int y_surf = rect.GetHeight();

	//int y_size = m_window->y_size;
	auto corn = m_spell_data->gres.wm_frame_corner;
	if(!m_window->grp)
	{
		m_window->x_panel = m_window->x_size + 4*corn->x_size;
		m_window->y_size = y_surf - 4*corn->y_size;
		m_window->x_org = 2*corn->x_size;
		m_window->y_org = 2*corn->y_size;
	}

	m_font = m_spell_data->font;	
	m_text_lines = m_text.WordWrap(m_font,m_window->x_size,m_window->y_gap,align);
	if(m_text_lines.lines.empty())
		return;

	int y_line = m_text_lines.line_step;
	int lines = m_text_lines.lines.size();
	int max_lines = m_window->y_size/y_line;

	m_window->y_panel = min(max_lines,lines)*y_line + 4*corn->y_size;
	
	scrollText->SetRange(max(lines - max_lines,0) + 1);
	scrollText->SetThumbSize(1);

	canvasText->Refresh();
}

void FormText::OnTextScroll(wxScrollEvent& event)
{
	canvasText->Refresh();
}

void FormText::OnTextCanvasSize(wxSizeEvent& event)
{
	//PrepareText();
	event.Skip();
}

// render text
void FormText::OnPaintText(wxPaintEvent& event)
{
	wxBitmap bmp(canvasText->GetClientSize(),24);
	auto surf_x = bmp.GetWidth();
	auto surf_y = bmp.GetHeight();
		
	// recalculate word wrapping?
	static int surf_x_last = -1;
	static int surf_y_last = -1;
	if(surf_x_last != surf_x || surf_y_last != surf_y)
		PrepareText();
	surf_x_last = surf_x;
	surf_y_last = surf_y;
		
	if(m_text_lines.lines.empty())
		return;
	if(!m_font)
		return;
	if(!m_window)
		return;

	// target panel
	auto panel = m_window->grp;
	int x_size = m_window->x_panel;
	int y_size = m_window->y_panel;
	if(panel)
	{
		x_size = panel->x_size;
		y_size = panel->y_size;
	}
	
	// make render buffer
	std::vector<uint8_t> text_buf;
	text_buf.assign(x_size*y_size,0);
	uint8_t* buf = text_buf.data();
	uint8_t* buf_end = buf + text_buf.size();

	// render panel background
	uint8_t (*pal)[3];
	if(panel)
	{
		// known panel
		panel->Render(buf, buf_end, x_size, 0, 0);
		pal = panel->pal;
	}
	else
	{
		// generic message window
		auto mpal = m_spell_data->GetPalette("MAP");
		if(!mpal)
			return;
		pal = (uint8_t(*)[3])mpal->m_pal.data();

		// frame border graphics
		auto corn = m_spell_data->gres.wm_frame_corner;
		auto horz = m_spell_data->gres.wm_frame_horz;
		auto vert = m_spell_data->gres.wm_frame_vert;

		// render frame
		int pos_x = corn->x_size;
		do {
			horz->Render(buf,buf_end,x_size,pos_x,0);
			horz->Render(buf,buf_end,x_size,pos_x,y_size - horz->y_size);
			pos_x += horz->x_size;
		} while(pos_x < x_size);
		int pos_y = 10;
		do {
			vert->Render(buf,buf_end,x_size,0,pos_y);
			vert->Render(buf,buf_end,x_size,x_size - corn->x_size,pos_y);
			pos_y += vert->y_size;
		} while(pos_y < y_size);
		corn->Render(buf,buf_end,x_size,0,0);
		corn->Render(buf,buf_end,x_size,x_size - corn->x_size,0);
		corn->Render(buf,buf_end,x_size,0,y_size - corn->y_size);
		corn->Render(buf,buf_end,x_size,x_size - corn->x_size,y_size - corn->y_size);
	}
		
	// render text chunks
	int line = scrollText->GetThumbPosition();
	if(line >= m_text_lines.lines.size())
		return;
	int y_ref = m_text_lines.lines[line].pos_y;
	while(true)
	{
		if(line >= m_text_lines.lines.size())
			break;
		auto words = &m_text_lines.lines[line++];
		int y_pos = words->pos_y - y_ref + m_window->y_org;
		int y_end = y_pos + words->size_y;
		if(y_end > m_window->y_org + m_window->y_size)
			break;
		for(auto &chunk: words->chunks)
			m_font->Render(buf, buf_end, x_size,m_window->x_org + chunk.pos_x, y_pos, chunk.text,m_window->txt_color,m_window->bg_color,m_window->shadow);
	}

	// leave if surface not big enough
	if(surf_x < x_size || surf_y < y_size)
		return;

	// image should fit to surface
	int x_ofs = (surf_x - x_size)/2;
	int y_ofs = (surf_y - y_size)/2;

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData pdata(bmp);
	wxNativePixelData::Iterator p(pdata);
	for(int y = 0; y < surf_y; y++)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && buf < buf_end;
			if(is_visible)
			{
				*scan++ = pal[*buf][2];
				*scan++ = pal[*buf][1];
				*scan++ = pal[*buf][0];
				buf++;
			}
			else
			{
				uint8_t checkers = (!(x&32) == !(y&32))?0x88:0xAA;
				*scan++ = checkers;
				*scan++ = checkers;
				*scan++ = checkers;
				/**scan++ = 0;
				*scan++ = 0;
				*scan++ = 0;*/
				if(is_visible)
					buf++;
			}
		}
		p.OffsetY(pdata,1);
	}

	// blit to screen
	wxPaintDC pdc(canvasText);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}

// render font
void FormText::OnPaintFont(wxPaintEvent& event)
{
	wxBitmap bmp(canvasFont->GetClientSize(),24);
	auto surf_x = bmp.GetWidth();
	auto surf_y = bmp.GetHeight();

	int w_max = 0;
	for(int sym = 32; sym < 256; sym++)
	{
		int w = m_spell_data->font->GetSymbolWidth((uint8_t)(unsigned)sym);
		w_max = max(w_max, w);
	}
	int h = m_spell_data->font->GetHeight();

	int rows = 4;
	int columns = 256/rows;
	int x_size = w_max*columns;
	int y_size = h*rows;
	std::vector<uint8_t> font_buf;
	font_buf.assign(x_size*y_size,0);
	uint8_t *buf = font_buf.data();
	uint8_t *buf_end = buf + font_buf.size();
			
	int sym = -1;
	for(int row = 0; row < rows; row++)
	{
		for(int col = 0; col < columns; col++)
		{
			sym++;
			if(sym < 32)
				continue;
			
			int w = m_spell_data->font->GetSymbolWidth((uint8_t)(unsigned)sym);
			std::string str = " ";
			str[0] = sym;
			m_spell_data->font->Render(buf,buf_end,x_size, col*w_max, row*h, str, 255);

			
		}
	}

	// leave if surface not big enough
	if(surf_x < x_size || surf_y < y_size)
		return;

	// image should fit to surface
	int x_ofs = (surf_x - x_size)/2;
	int y_ofs = (surf_y - y_size)/2;
	
	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData pdata(bmp);
	wxNativePixelData::Iterator p(pdata);
	for(int y = 0; y < surf_y; y++)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && buf < buf_end;
			if(is_visible)
			{
				*scan++ = *buf;
				*scan++ = *buf;
				*scan++ = *buf;
				buf++;
			}
			else
			{
				/*uint8_t checkers = (!(x&32) == !(y&32))?0x88:0xAA;
				*scan++ = checkers;
				*scan++ = checkers;
				*scan++ = checkers;*/
				*scan++ = 0;
				*scan++ = 0;
				*scan++ = 0;
				if(is_visible)
					buf++;
			}
		}
		p.OffsetY(pdata,1);
	}

	// blit to screen
	wxPaintDC pdc(canvasFont);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}
