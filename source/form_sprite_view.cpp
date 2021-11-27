///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_sprite_view.h"
#include "spellcross.h"
#include "sprites.h"

#include <wx/filedlg.h>

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

FormSprite::FormSprite( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->spell_data = spell_data;
	
	
	// === AUTO GENERATED STUFF STARTS HERE ===
	
	this->SetSizeHints(wxSize(900,-1),wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	mMenu = new wxMenuBar(0);
	mnuFile = new wxMenu();
	wxMenuItem* m_menuItem1;
	m_menuItem1 = new wxMenuItem(mnuFile,wxID_ANY,wxString(wxT("Save sprite")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(m_menuItem1);

	mnuFile->AppendSeparator();

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem(mnuFile,wxID_BTN_CLOSE,wxString(wxT("Close")),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnClose);

	mMenu->Append(mnuFile,wxT("File"));

	mnuTerr = new wxMenu();
	mMenu->Append(mnuTerr,wxT("Terrain"));

	mnuEdit = new wxMenu();
	wxMenuItem* btnNext;
	btnNext = new wxMenuItem(mnuEdit,wxID_BTN_NEXT,wxString(wxT("Next sprite")) + wxT('\t') + wxT("]"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnNext);

	wxMenuItem* btnPrev;
	btnPrev = new wxMenuItem(mnuEdit,wxID_BTN_PREV,wxString(wxT("Previous sprite")) + wxT('\t') + wxT("["),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnPrev);

	mnuEdit->AppendSeparator();

	wxMenuItem* btnBuildContext;
	btnBuildContext = new wxMenuItem(mnuEdit,wxID_EDIT_TILE_CONTEXT_AUTO,wxString(wxT("Update tiles' context")),wxT("Auto generate tile context from edge class data. Not it may take some time!"),wxITEM_NORMAL);
	mnuEdit->Append(btnBuildContext);

	wxMenuItem* btnAutoShadeFlags;
	btnAutoShadeFlags = new wxMenuItem(mnuEdit,wxID_BTN_AUTO_SHADING,wxString(wxT("Auto set shading flags")),wxT("This will try to automatically decode sprite names such as PL???_?? and set the edge/corner shading flags."),wxITEM_NORMAL);
	mnuEdit->Append(btnAutoShadeFlags);

	wxMenuItem* btnSaveContext;
	btnSaveContext = new wxMenuItem(mnuEdit,wxID_BTN_SAVE_CONTEXT,wxString(wxT("Save context data")),wxT("Save this terrain's sprite context data to a file. "),wxITEM_NORMAL);
	mnuEdit->Append(btnSaveContext);

	mMenu->Append(mnuEdit,wxT("Edit"));

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
	sizerCanvas = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT(" Sprite(s) view:")),wxVERTICAL);

	canvas = new wxPanel(sizerCanvas->GetStaticBox(),wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	sizerCanvas->Add(canvas,1,wxEXPAND | wxALL,5);


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

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT(" Tile properties:")),wxVERTICAL);

	cbIsGrass = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_GRASS,wxT("Normal grass"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsGrass,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsDarkGrass = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_DGRASS,wxT("Dark grass"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsDarkGrass,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsBlood = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_BLOOD,wxT("Blood"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsBlood,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsMud = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_MUD,wxT("Mud"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsMud,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsSwamp = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_SWAMP,wxT("Swamp"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsSwamp,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsAsh = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_ASH,wxT("Ash"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsAsh,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsHigh = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_HIGH_LAND,wxT("High land"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsHigh,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsRoad = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_ASH_ROAD,wxT("Asphalt road"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsRoad,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsBrokeAshroad = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_BROKE_ASH_ROAD,wxT("Broken asphalt road"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsBrokeAshroad,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsDirtRoad = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_DIRT_ROAD,wxT("Dirt road"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsDirtRoad,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsMudPath = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_MUD_PATH,wxT("Mud path"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsMudPath,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsCliff = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_CLIFF,wxT("Cliff"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsCliff,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsWater = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_WATER,wxT("Water"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsWater,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsWBridge = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_WBRIDGE,wxT("Wooden bridge"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsWBridge,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsBridge = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_BRIDGE,wxT("Bridge"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsBridge,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsFord = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_FORD,wxT("Ford"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsFord,0,wxLEFT|wxRIGHT|wxTOP,5);

	cbIsSand = new wxCheckBox(sbSizer2->GetStaticBox(),wxID_CB_IS_SAND,wxT("Sand"),wxDefaultPosition,wxDefaultSize,0);
	sbSizer2->Add(cbIsSand,0,wxLEFT|wxRIGHT|wxTOP,5);


	bSizer1->Add(sbSizer2,0,wxEXPAND|wxLEFT|wxTOP,5);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT("Tile Params:")),wxVERTICAL);

	sbSizer3->SetMinSize(wxSize(150,-1));
	m_staticText5 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q1 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText5->Wrap(-1);
	sbSizer3->Add(m_staticText5,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ1classChoices;
	chbQ1class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q1_CLASS,wxDefaultPosition,wxDefaultSize,chbQ1classChoices,0);
	chbQ1class->SetSelection(0);
	sbSizer3->Add(chbQ1class,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText6 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q2 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText6->Wrap(-1);
	sbSizer3->Add(m_staticText6,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ2classChoices;
	chbQ2class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q2_CLASS,wxDefaultPosition,wxDefaultSize,chbQ2classChoices,0);
	chbQ2class->SetSelection(0);
	sbSizer3->Add(chbQ2class,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText7 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q3 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText7->Wrap(-1);
	sbSizer3->Add(m_staticText7,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ3classChoices;
	chbQ3class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q3_CLASS,wxDefaultPosition,wxDefaultSize,chbQ3classChoices,0);
	chbQ3class->SetSelection(0);
	sbSizer3->Add(chbQ3class,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText8 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q4 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText8->Wrap(-1);
	sbSizer3->Add(m_staticText8,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ4classChoices;
	chbQ4class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q4_CLASS,wxDefaultPosition,wxDefaultSize,chbQ4classChoices,0);
	chbQ4class->SetSelection(0);
	sbSizer3->Add(chbQ4class,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	txtShadeEdge = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Shaded edges:"),wxDefaultPosition,wxDefaultSize,0);
	txtShadeEdge->Wrap(-1);
	sbSizer3->Add(txtShadeEdge,0,wxLEFT|wxRIGHT|wxTOP,5);

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer(2,4,0,0);

	cbShadeQ1 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q1,wxT("Q1"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeQ1,0,wxALL,5);

	cbShadeQ2 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q2,wxT("Q2"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeQ2,0,wxALL,5);

	cbShadeQ3 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q3,wxT("Q3"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeQ3,0,wxALL,5);

	cbShadeQ4 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q4,wxT("Q4"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeQ4,0,wxALL,5);

	cbShadeC1 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C1,wxT("C1"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeC1,0,wxALL,5);

	cbShadeC2 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C2,wxT("C2"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeC2,0,wxALL,5);

	cbShadeC3 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C3,wxT("C3"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeC3,0,wxALL,5);

	cbShadeC4 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C4,wxT("C4"),wxDefaultPosition,wxDefaultSize,0);
	gSizer1->Add(cbShadeC4,0,wxALL,5);


	sbSizer3->Add(gSizer1,0,0,5);


	bSizer1->Add(sbSizer3,0,wxEXPAND|wxLEFT|wxTOP,5);


	this->SetSizer(bSizer1);
	this->Layout();
	statBar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_ANY);

	this->Centre(wxBOTH);

	// === AUTO GENERATED STUFF ENDS HERE ===



	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		mnuTerr->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
		Bind(wxEVT_MENU,&FormSprite::OnTerrainChange,this,TERR_ID0 + k);
	}
	
	// fill quadrant class selectors
	wxChoice *chc[4] = {chbQ1class, chbQ2class, chbQ3class, chbQ4class};
	for(int k = 0; k < 4; k++)
	{
		int id = 0;
		while(class_list[id].name)
		{
			chc[k]->Append(class_list[id].name);
			id++;
		}
		chc[k]->Select(0);
	}


	// bind events
	Bind(wxEVT_MENU,&FormSprite::OnSelectSpriteBtn,this,wxID_BTN_NEXT);
	Bind(wxEVT_MENU,&FormSprite::OnSelectSpriteBtn,this,wxID_BTN_PREV);
	Bind(wxEVT_MENU,&FormSprite::OnClose,this,wxID_BTN_CLOSE);
	Bind(wxEVT_MENU,&FormSprite::OnUpdateContext,this,wxID_EDIT_TILE_CONTEXT_AUTO);	
	Bind(wxEVT_MENU,&FormSprite::OnAutoShadeFlags,this,wxID_BTN_AUTO_SHADING);
	Bind(wxEVT_MENU,&FormSprite::OnSaveTileContext,this,wxID_BTN_SAVE_CONTEXT);


	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSprite::OnSelectSprite,this,wxID_LBOX_SPRITES);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSprite::OnSelectNeighbor,this,wxID_LBOX_NEIGHBOR);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormSprite::OnChangeGamma,this,wxID_SLIDE_GAMMA);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnChangeZoom,this,wxID_CB_ZOOM);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED, &FormSprite::OnQchange,this,wxID_CH_SIDE);
	canvas->Bind(wxEVT_PAINT,&FormSprite::OnCanvasRepaint,this);

	// bind flags events
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_GRASS);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_DGRASS);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_BLOOD);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_MUD);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_SWAMP);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_ASH);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_HIGH_LAND);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_ASH_ROAD);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_BROKE_ASH_ROAD);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_DIRT_ROAD);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_MUD_PATH);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_CLIFF);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_WATER);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_WBRIDGE);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_BRIDGE);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_FORD);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_IS_SAND);

	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSprite::OnEdgeClassChange,this,wxID_CHB_Q1_CLASS);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSprite::OnEdgeClassChange,this,wxID_CHB_Q2_CLASS);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSprite::OnEdgeClassChange,this,wxID_CHB_Q3_CLASS);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSprite::OnEdgeClassChange,this,wxID_CHB_Q4_CLASS);

	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_Q1);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_Q2);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_Q3);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_Q4);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_C1);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_C2);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_C3);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnEdgeShadeChange,this,wxID_CB_SHADE_C4);
	

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
// close button
void FormSprite::OnClose(wxCommandEvent& event)
{
	Close();
}


// update tiles' context from edge class data
void FormSprite::OnUpdateContext(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terr = FindTerrain();
	// build context from edge class data
	terr->UpdateSpriteContext();
	Refresh();
}
// auto set edge shading flags
void FormSprite::OnAutoShadeFlags(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terr = FindTerrain();
	// build context from edge class data
	terr->InitSpriteContextShading();
	Refresh();
}
// save tiles' context to file
void FormSprite::OnSaveTileContext(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terrain = FindTerrain();
	
	// split path to folder and file
	std::filesystem::path last_path = terrain->GetSpriteContextPath();
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Save Spellcross Tile Context File"),dir,name,"Tile context file (*.con)|*.con",
		wxFD_SAVE);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());

	// save file
	terrain->SaveSpriteContext(path);
}


// on change of edge/corner shading
void FormSprite::OnEdgeShadeChange(wxCommandEvent& event)
{
	const long list[] = {
		wxID_CB_SHADE_Q1, wxID_CB_SHADE_Q2, wxID_CB_SHADE_Q3, wxID_CB_SHADE_Q4,
		wxID_CB_SHADE_C1, wxID_CB_SHADE_C2, wxID_CB_SHADE_C3, wxID_CB_SHADE_C4};
	const uint32_t flags[] = {
		SpriteContext::SHADING_SIDE_Q1, SpriteContext::SHADING_SIDE_Q2, SpriteContext::SHADING_SIDE_Q3, SpriteContext::SHADING_SIDE_Q4,
		SpriteContext::SHADING_CORNER_Q1, SpriteContext::SHADING_CORNER_Q2, SpriteContext::SHADING_CORNER_Q3, SpriteContext::SHADING_CORNER_Q4};

	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();

		// update flags
		for(int k = 0; k < 8;k++)
		{
			wxCheckBox* cb = (wxCheckBox*)FindItem(list[k]);
			if(cb->IsChecked())
				terr->context[lboxSprites->GetSelection()].OrShadingFlags(flags[k]);
			else
				terr->context[lboxSprites->GetSelection()].ClrShadingFlags(flags[k]);
		}
	}	
}
void FormSprite::SetShadingFlags()
{
	const long list[] ={
		wxID_CB_SHADE_Q1, wxID_CB_SHADE_Q2, wxID_CB_SHADE_Q3, wxID_CB_SHADE_Q4,
		wxID_CB_SHADE_C1, wxID_CB_SHADE_C2, wxID_CB_SHADE_C3, wxID_CB_SHADE_C4};
	const uint32_t flags[] ={
		SpriteContext::SHADING_SIDE_Q1, SpriteContext::SHADING_SIDE_Q2, SpriteContext::SHADING_SIDE_Q3, SpriteContext::SHADING_SIDE_Q4,
		SpriteContext::SHADING_CORNER_Q1, SpriteContext::SHADING_CORNER_Q2, SpriteContext::SHADING_CORNER_Q3, SpriteContext::SHADING_CORNER_Q4};

	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();
		// update checkboxes
		for(int k = 0; k < 8;k++)
		{
			wxCheckBox* cb = (wxCheckBox*)FindItem(list[k]);
			cb->SetValue(!!(terr->context[lboxSprites->GetSelection()].GetShadingFlags() & flags[k]));				
		}
	}
}


// change edge class
void FormSprite::OnEdgeClassChange(wxCommandEvent& event)
{
	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// get this terrain
		Terrain *terr = FindTerrain();
	
		// update class(es)
		wxChoice* chc[4] ={chbQ1class, chbQ2class, chbQ3class, chbQ4class};
		for(int k = 0; k < 4; k++)
			terr->context[lboxSprites->GetSelection()].SetEdgeClass(k, chc[k]->GetSelection());
	}
}
void FormSprite::SetEdgeClasses()
{
	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();

		// set classes
		wxChoice* chc[4] ={chbQ1class, chbQ2class, chbQ3class, chbQ4class};
		for(int k = 0; k < 4; k++)
		{
			int edge_class = terr->context[lboxSprites->GetSelection()].GetEdgeClass(k);
			chc[k]->Select(edge_class);
		}
	}
}

// on change of tile flags list
void FormSprite::OnFlagsChange(wxCommandEvent& event)
{
	int cbid = event.GetId();
	bool check = event.IsChecked();

	const uint32_t list[][2] = {
		{wxID_CB_IS_GRASS, SpriteContext::IS_GRASS},
		{wxID_CB_IS_DGRASS, SpriteContext::IS_DGRASS},
		{wxID_CB_IS_BLOOD, SpriteContext::IS_BLOOD},
		{wxID_CB_IS_MUD, SpriteContext::IS_MUD},
		{wxID_CB_IS_SWAMP, SpriteContext::IS_SWAPM},
		{wxID_CB_IS_ASH, SpriteContext::IS_ASH},
		{wxID_CB_IS_HIGH_LAND, SpriteContext::IS_HIGHLAND},
		{wxID_CB_IS_ASH_ROAD, SpriteContext::IS_ROAD},
		{wxID_CB_IS_BROKE_ASH_ROAD, SpriteContext::IS_BROKE_ROAD},
		{wxID_CB_IS_DIRT_ROAD, SpriteContext::IS_DIRT_ROAD},
		{wxID_CB_IS_MUD_PATH, SpriteContext::IS_MUD_PATH},
		{wxID_CB_IS_CLIFF, SpriteContext::IS_CLIFF},
		{wxID_CB_IS_WATER, SpriteContext::IS_WATER},
		{wxID_CB_IS_WBRIDGE, SpriteContext::IS_WOOD_BRIDGE},
		{wxID_CB_IS_BRIDGE, SpriteContext::IS_BRIDGE},
		{wxID_CB_IS_FORD, SpriteContext::IS_FORD},
		{wxID_CB_IS_SAND, SpriteContext::IS_SAND},
		{0,0}
	};

	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();
	
		// build flags from checkbox states
		uint32_t flags = terr->context[lboxSprites->GetSelection()].GetFlags();
		int flag_id = 0;
		while(list[flag_id][0])
		{
			if(cbid == list[flag_id][0] && check)
				flags |= list[flag_id][1];
			else if(cbid == list[flag_id][0] && !check)
				flags &= ~list[flag_id][1];
			flag_id++;
		}

		// update flags
		terr->context[lboxSprites->GetSelection()].SetFlags(flags);
	}	
}
// show tile flags
void FormSprite::SetFlags()
{
	const struct {
		wxCheckBox *cb;
		uint32_t flag;
	} list[] = {
		{cbIsGrass, SpriteContext::IS_GRASS},
		{cbIsDarkGrass, SpriteContext::IS_DGRASS},
		{cbIsBlood, SpriteContext::IS_BLOOD},
		{cbIsMud, SpriteContext::IS_MUD},
		{cbIsSwamp, SpriteContext::IS_SWAPM},
		{cbIsAsh, SpriteContext::IS_ASH},
		{cbIsHigh, SpriteContext::IS_HIGHLAND},
		{cbIsRoad, SpriteContext::IS_ROAD},
		{cbIsBrokeAshroad, SpriteContext::IS_BROKE_ROAD},
		{cbIsDirtRoad, SpriteContext::IS_DIRT_ROAD},
		{cbIsMudPath, SpriteContext::IS_MUD_PATH},
		{cbIsCliff, SpriteContext::IS_CLIFF},
		{cbIsWater, SpriteContext::IS_WATER},
		{cbIsWBridge, SpriteContext::IS_WOOD_BRIDGE},
		{cbIsBridge, SpriteContext::IS_BRIDGE},
		{cbIsFord, SpriteContext::IS_FORD},
		{cbIsSand, SpriteContext::IS_SAND},
		{NULL,0}
	};

	int flags = 0;
	if(lboxSprites->GetCount() && lboxSprites->GetSelection() >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();

		// get this tile flags
		flags = terr->context[lboxSprites->GetSelection()].GetFlags();

		// set particular checkboxes
		int flag_id = 0;
		while(list[flag_id].cb)
		{
			list[flag_id].cb->SetValue(flags & list[flag_id].flag);
			flag_id++;
		}
	}
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
	SetFlags();
	SetEdgeClasses();
	SetShadingFlags();
	canvas->Refresh();
}
// select sprite by shortcuts
void FormSprite::OnSelectSpriteBtn(wxCommandEvent& event)
{
	if(event.GetId() == wxID_BTN_NEXT)
	{
		if(lboxSprites->GetSelection() < lboxSprites->GetCount()-1)
			lboxSprites->Select(lboxSprites->GetSelection()+1);
	}
	else if(event.GetId() == wxID_BTN_PREV)
	{
		if(lboxSprites->GetSelection() > 0)
			lboxSprites->Select(lboxSprites->GetSelection()-1);
	}
	OnSelectSprite(event);
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