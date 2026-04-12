///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_text.h"

///////////////////////////////////////////////////////////////////////////

FormText::FormText( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	m_spell_data = spell_data;

	// === AUTO GENERATED START ===	

	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	m_menubar13 = new wxMenuBar(0);
	m_menu24 = new wxMenu();
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(m_menu24,wxID_MM_EXIT,wxString(wxT("Exit")),wxEmptyString,wxITEM_NORMAL);
	m_menu24->Append(mmExit);

	m_menubar13->Append(m_menu24,wxT("File"));

	this->SetMenuBar(m_menubar13);

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SBAR);
	wxBoxSizer* bSizer105;
	bSizer105 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer(wxVERTICAL);

	m_staticText106 = new wxStaticText(this,wxID_ANY,wxT("Source:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText106->Wrap(-1);
	bSizer107->Add(m_staticText106,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chSourceChoices;
	chSource = new wxChoice(this,wxID_CH_SOURCE,wxDefaultPosition,wxDefaultSize,chSourceChoices,0);
	chSource->SetSelection(0);
	bSizer107->Add(chSource,0,wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND,5);

	m_staticText107 = new wxStaticText(this,wxID_ANY,wxT("Filter (wildcard: ?*):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText107->Wrap(-1);
	bSizer107->Add(m_staticText107,0,wxRIGHT|wxLEFT,5);

	txtFilter = new wxTextCtrl(this,wxID_TXT_WILD,wxT("*"),wxDefaultPosition,wxDefaultSize,0);
	bSizer107->Add(txtFilter,0,wxBOTTOM|wxLEFT|wxRIGHT|wxEXPAND,5);

	m_staticText108 = new wxStaticText(this,wxID_ANY,wxT("Resources:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText108->Wrap(-1);
	bSizer107->Add(m_staticText108,0,wxRIGHT|wxLEFT,5);

	listResources = new wxListBox(this,wxID_LIST_RESOURCES,wxDefaultPosition,wxSize(180,-1),0,NULL,0);
	bSizer107->Add(listResources,1,wxBOTTOM|wxLEFT|wxRIGHT,5);


	bSizer106->Add(bSizer107,0,wxEXPAND,5);

	m_staticline39 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer106->Add(m_staticline39,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* bSizer108;
	bSizer108 = new wxBoxSizer(wxVERTICAL);

	m_staticText109 = new wxStaticText(this,wxID_ANY,wxT("Source text:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText109->Wrap(-1);
	bSizer108->Add(m_staticText109,0,wxTOP|wxRIGHT|wxLEFT,5);

	textEdit = new wxTextCtrl(this,wxID_TXT_EDIT,wxEmptyString,wxDefaultPosition,wxSize(400,-1),wxTE_MULTILINE);
	bSizer108->Add(textEdit,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText111 = new wxStaticText(this,wxID_ANY,wxT("Target window:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText111->Wrap(-1);
	bSizer108->Add(m_staticText111,0,wxRIGHT|wxLEFT,5);

	wxArrayString chTargetChoices;
	chTarget = new wxChoice(this,wxID_CH_TARGET,wxDefaultPosition,wxDefaultSize,chTargetChoices,0);
	chTarget->SetSelection(0);
	bSizer108->Add(chTarget,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer106->Add(bSizer108,0,wxEXPAND,5);

	m_staticline40 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer106->Add(m_staticline40,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* bSizer109;
	bSizer109 = new wxBoxSizer(wxVERTICAL);

	m_staticText110 = new wxStaticText(this,wxID_ANY,wxT("Render:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText110->Wrap(-1);
	bSizer109->Add(m_staticText110,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer110;
	bSizer110 = new wxBoxSizer(wxHORIZONTAL);

	canvasText = new wxPanel(this,wxID_CANVAS_TEXT,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer110->Add(canvasText,1,wxEXPAND | wxALL,5);

	scrollText = new wxScrollBar(this,wxID_SCROLL_TEXT,wxDefaultPosition,wxDefaultSize,wxSB_VERTICAL);
	bSizer110->Add(scrollText,0,wxEXPAND|wxALL,5);


	bSizer109->Add(bSizer110,1,wxEXPAND,5);


	bSizer106->Add(bSizer109,1,wxEXPAND,5);


	bSizer105->Add(bSizer106,1,wxEXPAND,5);

	m_staticline38 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer105->Add(m_staticline38,0,wxEXPAND | wxALL,5);

	m_staticText105 = new wxStaticText(this,wxID_ANY,wxT("Font preview:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText105->Wrap(-1);
	bSizer105->Add(m_staticText105,0,wxLEFT,5);

	canvasFont = new wxPanel(this,wxID_CANVAS_FONT,wxDefaultPosition,wxSize(-1,100),wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer105->Add(canvasFont,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	this->SetSizer(bSizer105);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	Bind(wxEVT_MENU,&FormText::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormText::OnChangeFilter,this,wxID_TXT_WILD);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormText::OnChangeResource,this,wxID_LIST_RESOURCES);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormText::OnChangeSource,this,wxID_CH_SOURCE);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormText::OnChangeTargetPanel,this,wxID_CH_TARGET);

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

	chSource->Clear();
	chSource->Append("TEXTS.FS");
	chSource->Append("RESEARCH.FS");
	chSource->Append("INFO.FS");
	chSource->Select(0);

	FillResources();
		
	m_windows = {
		{"Generic ingame message", NULL, 10,8, 375,-1, 255,0, SpellFont::FontShadow::DIAG3,"TEXTS.FS",{"U*","MSG*","E*","T*.S","T*.OK","T*.BAD"}},
		{"Unit info panel", m_spell_data->gres.inf_panel, 45,30, 372,350, 248,0, SpellFont::FontShadow::DIAG3,"INFO.FS",{"*"}},
		{"Big map research info panel", m_spell_data->gres.bm_info_panel, 20,5, 378,408, 249,0, SpellFont::FontShadow::RIGHT_DOWN,"RESEARCH.FS",{"*.INF"}},
		{"Big map research panel", m_spell_data->gres.bm_research_panel, 18,65, 352,123, 249,0, SpellFont::FontShadow::RIGHT_DOWN,"RESEARCH.FS",{"*.BRF"}},
		{"Big map mission info panel", m_spell_data->gres.bm_mission_info_panel, 28,31, 352,120, 127,0, SpellFont::FontShadow::RIGHT_DOWN,"TEXTS.FS",{"T*"}},
	};

	// fill window layouts list
	chTarget->Freeze();
	chTarget->Clear();
	for(auto &item: m_windows)
		chTarget->Append(item.name);
	chTarget->Thaw();
	chTarget->Select(0);
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

// change source filter
void FormText::OnChangeFilter(wxCommandEvent& event)
{
	FillResources();
}

// fill resource list
void FormText::FillResources()
{
	auto src_name = chSource->GetString(chSource->GetSelection());
		
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

// change resource
void FormText::OnChangeResource(wxCommandEvent& event)
{
	auto text = GetText();
	if(!text)
		return;

	textEdit->SetValue(text->text);

	int src_sel = chSource->GetSelection();
	if(src_sel < 0)
		return;
	auto src_name = chSource->GetString(src_sel);

	// try to auto select target panel based on text resource name
	[&] {
		for(int k = 0; k < m_windows.size(); k++)
		{
			auto window = &m_windows[k];
			if(window->source != src_name)
				continue;
			for(auto &wild: window->wild)
				if(wildcmp(wild.c_str(), text->name.c_str()))
				{
					chTarget->Select(k);
					return;
				}
		}
	}();

	PrepareText();
}

SpellTextRec *FormText::GetText()
{
	auto res_sel = listResources->GetSelection();
	if(res_sel < 0)
		return(NULL);
	auto name = listResources->GetString(res_sel);

	int src_sel = chSource->GetSelection();
	if(src_sel < 0)
		return(NULL);
	auto src_name = chSource->GetString(src_sel);
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

void FormText::PrepareText()
{
	m_font = NULL;
	m_text_lines.clear();
	
	auto text = GetText();
	if(!text)
		return;
	
	int win_sel = chTarget->GetSelection();
	if(win_sel < 0)
		return;
	m_window = &m_windows[win_sel];
	
	// canvas size
	auto rect = canvasText->GetClientSize();
	int x_surf = rect.GetWidth();
	int y_surf = rect.GetHeight();

	//int y_size = m_window->y_size;
	if(!m_window->grp)
	{
		m_window->x_panel = m_window->x_size + 2*m_window->x_org;
		m_window->y_panel = y_surf;
		m_window->y_size = y_surf - 2*m_window->y_org;
	}

	m_font = m_spell_data->font;	
	m_text_lines = text->WordWrap(m_font,m_window->x_size);
	if(m_text_lines.empty())
		return;

	int y_line = m_text_lines[0].size_y;

	int lines = m_text_lines.size();
	
	

	int max_lines = m_window->y_size/y_line;
	
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
		
	if(m_text_lines.empty())
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
		panel->Render(buf, buf_end, x_size, 0, 0);
		pal = panel->pal;
	}
	else
	{
		auto mpal = m_spell_data->GetPalette("MAP");
		if(!mpal)
			return;
		pal = (uint8_t(*)[3])mpal->m_pal.data();
	}
		
	int line = scrollText->GetThumbPosition();
	if(line >= m_text_lines.size())
		return;
	int y_ref = m_text_lines[line].pos_y;
	while(true)
	{
		if(line >= m_text_lines.size())
			break;
		auto chunk = &m_text_lines[line++];
		int y_pos = chunk->pos_y - y_ref + m_window->y_org;
		int y_end = y_pos + chunk->size_y;
		if(y_end > m_window->y_org + m_window->y_size)
			break;
		m_font->Render(buf, buf_end, x_size,m_window->x_org, y_pos, chunk->text,m_window->txt_color,m_window->bg_color,m_window->shadow);
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
