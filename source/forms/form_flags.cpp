///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_flags.h"

///////////////////////////////////////////////////////////////////////////

FormFlags::FormFlags( wxWindow* parent, Terrain *terr, int flags, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints(wxSize(400,350),wxDefaultSize);

	wxBoxSizer* bSizer87;
	bSizer87 = new wxBoxSizer(wxVERTICAL);

	m_staticText90 = new wxStaticText(this,wxID_ANY,wxT("Tile flag value:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText90->Wrap(-1);
	bSizer87->Add(m_staticText90,0,wxLEFT|wxRIGHT|wxTOP,5);

	wxArrayString chbFlagChoices;
	chbFlag = new wxChoice(this,wxID_CHB_FLAG,wxDefaultPosition,wxDefaultSize,chbFlagChoices,0);
	chbFlag->SetSelection(0);
	bSizer87->Add(chbFlag,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT(" Sprite preview: ")),wxVERTICAL);

	canvas = new wxPanel(sizerCanvas->GetStaticBox(),wxID_CANVAS,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	sizerCanvas->Add(canvas,1,wxALL|wxEXPAND,5);


	bSizer87->Add(sizerCanvas,1,wxALL|wxEXPAND,5);

	wxBoxSizer* bSizer88;
	bSizer88 = new wxBoxSizer(wxHORIZONTAL);

	btnOk = new wxButton(this,wxID_BTN_OK,wxT("OK"),wxDefaultPosition,wxDefaultSize,0);
	bSizer88->Add(btnOk,1,wxALL,5);

	btnCancel = new wxButton(this,wxID_BTN_CANCEL,wxT("Cancel"),wxDefaultPosition,wxDefaultSize,0);
	bSizer88->Add(btnCancel,1,wxALL,5);


	bSizer87->Add(bSizer88,0,wxEXPAND,5);


	this->SetSizer(bSizer87);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	m_terrain = terr;


	m_list = {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x88,0x90, 0xA0,0xB0,0xC0,0xD0,0xE0,0xF0};
	m_sprites = {
		NULL, // 0x00
		NULL, // 0x10
		NULL, // 0x20
		NULL, // 0x30
		NULL, // 0x40
		NULL, // 0x50
		terr->GetSprite("MTA1B_01"), // 0x60
		NULL, // 0x70
		NULL, // 0x80
		NULL, // 0x88
		terr->GetSprite("STA_0003"), // 0x90
		terr->GetSprite("MRA05_00"), // 0xA0
		terr->GetSprite("MRA0A_00"), // 0xB0
		terr->GetSprite("MRA06_00"), // 0xC0
		terr->GetSprite("MRA0C_00"), // 0xD0
		terr->GetSprite("MRA09_00"), // 0xE0
		terr->GetSprite("MRA03_00")  // 0xF0
	};
	
	chbFlag->Clear();
	chbFlag->Append("0x00 - no flags");
	chbFlag->Append("0x10 - building height 1");
	chbFlag->Append("0x20 - building height 2");
	chbFlag->Append("0x30 - building height 3");
	chbFlag->Append("0x40 - general not walkable");
	chbFlag->Append("0x50 - destructible special objects");
	chbFlag->Append("0x60 - watter/ridge");
	chbFlag->Append("0x70 - bridges");
	chbFlag->Append("0x80 - objects");
	chbFlag->Append("0x88 - objects (tents)");
	chbFlag->Append("0x90 - trees");
	chbFlag->Append("0xA0 - wall section \\");
	chbFlag->Append("0xB0 - wall section /");
	chbFlag->Append("0xC0 - wall section v");
	chbFlag->Append("0xD0 - wall section >");
	chbFlag->Append("0xE0 - wall section ^");
	chbFlag->Append("0xF0 - wall section <");
	
	SelectFlag(flags);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormFlags::OnClose,this,wxID_BTN_OK);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormFlags::OnClose,this,wxID_BTN_CANCEL);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormFlags::OnSelect,this,wxID_CHB_FLAG);
	canvas->Bind(wxEVT_PAINT,&FormFlags::OnCanvasRepaint,this);

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_CANCEL);
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

	// paint preview
	canvas->Refresh();
}

// select flag
void FormFlags::SelectFlag(int flag)
{
	auto fid = std::find(m_list.begin(), m_list.end(), flag);
	if(fid == m_list.end())
		return;
	chbFlag->Select(fid - m_list.begin());
}

// get selected flag
int FormFlags::GetSelectedFlags()
{
	auto fid = chbFlag->GetSelection();
	if(fid < 0 || fid >= m_list.size())
		return(0x00);
	return(m_list[fid]);
}

FormFlags::~FormFlags()
{
}

// on close form
void FormFlags::OnClose(wxCommandEvent& event)
{	
	EndModal((event.GetId() == wxID_BTN_OK)?wxID_OK:wxID_CANCEL);
}

// select flag
void FormFlags::OnSelect(wxCommandEvent& event)
{
	// paint preview
	canvas->Refresh();
}


// repaint canvas event
void FormFlags::OnCanvasRepaint(wxPaintEvent& event)
{
	// allocate buffer
	int x_size = canvas->GetClientSize().x;
	int y_size = canvas->GetClientSize().y;
	wxBitmap m_buffer(x_size,y_size,24);
	wxPaintDC pdc(canvas);

	// render flags
	//int flags = cbZoom->IsChecked()?(Terrain::RENDER_ZOOMX2):0;
	int flags = 0;

	// get gamma
	//double gamma = 0.001*(double)slideGamma->GetValue();
	double gamma = 1.30;
	
	Sprite *spr = NULL;
	auto fid = chbFlag->GetSelection();
	if(fid >= 0 || fid < m_list.size())
		spr = m_sprites[fid];

	// render tile to buffer
	m_terrain->RenderSpritePreview(m_buffer,spr,flags,gamma);
	
	// blit to screend
	pdc.DrawBitmap(m_buffer,wxPoint(0,0));
	event.Skip();
}
