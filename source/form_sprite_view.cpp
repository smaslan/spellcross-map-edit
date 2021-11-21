///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_sprite_view.h"
#include "spellcross.h"
#include "sprites.h"

///////////////////////////////////////////////////////////////////////////

FormSprite::FormSprite( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->spell_data = spell_data;
	
	
	
	
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	mMenu = new wxMenuBar(0);
	mnuFile = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem(mnuFile,wxID_ANY,wxString(wxT("Save sprite")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(m_menuItem1);

	mnuFile->AppendSeparator();

	wxMenuItem* m_menuItem2;
	m_menuItem2 = new wxMenuItem(mnuFile,wxID_ANY,wxString(wxT("Close")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(m_menuItem2);

	mMenu->Append(mnuFile,wxT("File"));

	mnuTerr = new wxMenu();
	mMenu->Append(mnuTerr,wxT("Terrain"));
	
	this->SetMenuBar(mMenu);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	txtSpriteList = new wxStaticText(this,wxID_ANY,wxT("Sprite list:"),wxDefaultPosition,wxDefaultSize,0);
	txtSpriteList->Wrap(-1);
	bSizer2->Add(txtSpriteList,0,wxLEFT|wxTOP,5);

	lboxSprites = new wxListBox(this,wxID_LBOX_SPRITES,wxDefaultPosition,wxSize(120,-1),0,NULL,0|wxVSCROLL);
	bSizer2->Add(lboxSprites,1,wxBOTTOM|wxLEFT,5);


	bSizer1->Add(bSizer2,0,wxEXPAND|wxRIGHT,5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer(wxVERTICAL);


	bSizer5->Add(0,0,0,wxALL|wxEXPAND,4);

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT("Sprite view")),wxVERTICAL);

	canvas = new wxPanel(sizerCanvas->GetStaticBox(),wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	sizerCanvas->Add(canvas,1,wxEXPAND | wxALL,5);
	canvas->Bind(wxEVT_PAINT,&FormSprite::OnCanvasRepaint,this);


	bSizer5->Add(sizerCanvas,1,wxALL|wxEXPAND,5);

	txtGamma = new wxStaticText(this,wxID_ANY,wxT("Set gamma correction:"),wxDefaultPosition,wxDefaultSize,0);
	txtGamma->Wrap(-1);
	bSizer5->Add(txtGamma,0,wxLEFT|wxTOP,5);

	slideGamma = new wxSlider(this,wxID_SLIDE_GAMMA,1300,500,2000,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL);
	bSizer5->Add(slideGamma,0,wxEXPAND|wxLEFT|wxRIGHT|wxTOP,1);

	cbZoom = new wxCheckBox(this,wxID_CB_ZOOM,wxT("Zoom 2x"),wxDefaultPosition,wxDefaultSize,0);
	bSizer5->Add(cbZoom,0,wxALL,5);


	bSizer4->Add(bSizer5,1,wxEXPAND,5);


	bSizer1->Add(bSizer4,1,wxEXPAND,5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_staticText2 = new wxStaticText(this,wxID_ANY,wxT("Neighboring sprites:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText2->Wrap(-1);
	bSizer3->Add(m_staticText2,0,wxLEFT|wxTOP,5);

	lboxNeighbor = new wxListBox(this,wxID_LBOX_NEIGHBOR,wxDefaultPosition,wxSize(120,-1),0,NULL,0|wxVSCROLL);
	bSizer3->Add(lboxNeighbor,1,wxBOTTOM|wxLEFT|wxRIGHT,5);

	m_staticline1 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer3->Add(m_staticline1,0,wxEXPAND | wxALL,5);

	m_staticText3 = new wxStaticText(this,wxID_ANY,wxT("Side:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText3->Wrap(-1);
	bSizer3->Add(m_staticText3,0,wxLEFT,5);

	wxString chbSideChoices[] ={wxT("Q1"), wxT("Q2"), wxT("Q3"), wxT("Q4")};
	int chbSideNChoices = sizeof(chbSideChoices) / sizeof(wxString);
	chbSide = new wxChoice(this,wxID_CH_SIDE,wxDefaultPosition,wxDefaultSize,chbSideNChoices,chbSideChoices,0);
	chbSide->SetSelection(0);
	bSizer3->Add(chbSide,0,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	bSizer1->Add(bSizer3,0,wxEXPAND,5);


	this->SetSizer(bSizer1);
	this->Layout();

	this->Centre(wxBOTH);


	// === AUTO GENERATED STUFF ENDS HERE ===

	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mnuTerr->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormSprite::OnTerrainChange,this,TERR_ID0 + k);
	}

	// bind events
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSprite::OnSelectSprite,this,wxID_LBOX_SPRITES);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSprite::OnSelectNeighbor,this,wxID_LBOX_NEIGHBOR);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormSprite::OnChangeGamma,this,wxID_SLIDE_GAMMA);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnChangeZoom,this,wxID_CB_ZOOM);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED, &FormSprite::OnQchange,this,wxID_CH_SIDE);

	// default 2x zoom
	cbZoom->SetValue(true);
	// default gamma 1.50
	slideGamma->SetValue(1500);

	// stops flickering
	this->SetDoubleBuffered(1);

	// select terrain
	SelectTerrain();
	wxCommandEvent ev(wxEVT_COMMAND_SLIDER_UPDATED,wxID_SLIDE_GAMMA);
	OnChangeGamma(ev);

}
FormSprite::~FormSprite()
{
}

// change zoom
void FormSprite::OnChangeZoom(wxCommandEvent& event)
{
	canvas->Refresh();
}
// change gamma
void FormSprite::OnChangeGamma(wxCommandEvent& event)
{
	double gamma = 0.001*(double)slideGamma->GetValue();
	txtGamma->SetLabel(wxString::Format(wxT("Gamma correction (%#0.2f):"),gamma));	
	canvas->Refresh();
}

// repain canvas event
void FormSprite::OnCanvasRepaint(wxPaintEvent& event)
{
	// allocate buffer
	int x_size = canvas->GetClientSize().x;
	int y_size = canvas->GetClientSize().y;
	wxBitmap m_buffer(x_size,y_size,24);	
	wxPaintDC pdc(canvas);

	// get terrain
	Terrain *terr = FindTerrain();
	if(!terr)
		return;

	// render flags
	int flags = cbZoom->IsChecked()?(Terrain::RENDER_ZOOMX2):0;

	// get gamma
	double gamma = 0.001*(double)slideGamma->GetValue();

	int tiles[5];
	int *tile = tiles;
	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// something selected
		int ref_id = lboxSprites->GetSelection();
		*tile++ = ref_id;
		
		for(int quid = 0; quid < 4; quid++)
		{
			Sprite* spr;
			if(quid == chbSide->GetSelection())
			{
				// is selected side
				spr = terr->context[ref_id].GetContext(quid, lboxNeighbor->GetSelection());				
			}
			else
			{
				// not selected side
				spr = terr->context[ref_id].GetContext(quid,0);
			}			
			int sid = terr->GetSpriteID(spr);						
			*tile++ = sid;			
		}

		// render tile to buffer
		terr->RenderPreview(m_buffer, tile-tiles, tiles, flags, gamma);
	}
	
	// blit to screend
	pdc.DrawBitmap(m_buffer,wxPoint(0,0));
	event.Skip();
}


// select sprite
void FormSprite::OnSelectSprite(wxCommandEvent& event)
{
	txtSpriteList->SetLabel(wxString::Format("Sprite list (#%d):",lboxSprites->GetSelection()));
	SelectQuad();
	canvas->Refresh();
}

// select neighbor sprite
void FormSprite::OnSelectNeighbor(wxCommandEvent& event)
{
	canvas->Refresh();
}


// select terrain
Terrain *FormSprite::FindTerrain()
{
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		if(GetMenuBar()->FindItem(TERR_ID0 + k)->IsChecked())
		{
			// found selection
			Terrain* terr = spell_data->GetTerrain(k);
			return(terr);
		}
	}
	return(NULL);
}
void FormSprite::SelectTerrain()
{
	// loose old sprites list
	lboxSprites->Clear();

	// found selection
	Terrain* terr = FindTerrain();
	if(!terr)
		return;

	// update title
	SetTitle(wxString::Format("Sprite viewer (%s)",terr->name));

	// for each sprite:
	vector<wxString> list;
	for(int sid = 0;sid < terr->GetSpriteCount();sid++)
	{
		Sprite* spr = terr->GetSprite(sid);
		list.push_back(wxString(spr->name));
	}
	// fill list of sprites
	lboxSprites->Append(list);

	// select default
	lboxSprites->Select(0);	
}
// change terrain click
void FormSprite::OnTerrainChange(wxCommandEvent& event)
{
	SelectTerrain();
	SelectQuad();
	canvas->Refresh();
}
// change context quadrant
void FormSprite::SelectQuad()
{
	// get terrain
	Terrain* terr = FindTerrain();
	// get quadrant
	int quid = chbSide->GetSelection();
	// get center tile
	int refid = lboxSprites->GetSelection();
	// get center tile context
	SpriteContext* cont = &terr->context[refid];

	// fill list of context sprites
	lboxNeighbor->Clear();
	for(int sid = 0;sid < cont->GetContextCount(quid); sid++)
	{
		Sprite* spr = cont->GetContext(quid,sid);
		lboxNeighbor->Append(wxString(spr->name));
	}
	if(cont->GetContextCount(quid))
		lboxNeighbor->Select(0);
}
void FormSprite::OnQchange(wxCommandEvent& event)
{
	SelectQuad();
	canvas->Refresh();
}