///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_sprite_view.h"
#include "form_edit_toolset.h"
#include "spellcross.h"
#include "sprites.h"
#include "other.h"

#include <wx/filedlg.h>
#include <wx/msgdlg.h>


#include <filesystem>
#include <regex>

///////////////////////////////////////////////////////////////////////////

FormSprite::FormSprite( wxWindow* parent,SpellData* spell_data,wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->spell_data = spell_data;
	
	
	// === AUTO GENERATED STUFF STARTS HERE ===
	
	this->SetSizeHints(wxSize(1050,700),wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	mMenu = new wxMenuBar(0);
	mnuFile = new wxMenu();
	wxMenuItem* btnSave;
	btnSave = new wxMenuItem(mnuFile,wxID_BTN_SAVE,wxString(wxT("Save sprite")) + wxT('\t') + wxT("Ctrl+S"),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnSave);

	mnuFile->AppendSeparator();

	wxMenuItem* btnSelect;
	btnSelect = new wxMenuItem(mnuFile,wxID_BTN_SELECT,wxString(wxT("Select and Close")) + wxT('\t') + wxT("Enter"),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnSelect);

	wxMenuItem* btnClose;
	btnClose = new wxMenuItem(mnuFile,wxID_BTN_CLOSE,wxString(wxT("Close")) + wxT('\t') + wxT("Esc"),wxEmptyString,wxITEM_NORMAL);
	mnuFile->Append(btnClose);

	mMenu->Append(mnuFile,wxT("File"));

	mnuTerr = new wxMenu();
	mMenu->Append(mnuTerr,wxT("Terrain"));

	mnuEdit = new wxMenu();
	wxMenuItem* btnSelQ1;
	btnSelQ1 = new wxMenuItem(mnuEdit,wxID_BTN_SEL_Q1,wxString(wxT("Select edge Q1")) + wxT('\t') + wxT("F1"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnSelQ1);

	wxMenuItem* btnSelQ2;
	btnSelQ2 = new wxMenuItem(mnuEdit,wxID_BTN_SEL_Q2,wxString(wxT("Select edge Q2")) + wxT('\t') + wxT("F2"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnSelQ2);

	wxMenuItem* btnSelQ3;
	btnSelQ3 = new wxMenuItem(mnuEdit,wxID_BTN_SEL_Q3,wxString(wxT("Select edge Q3")) + wxT('\t') + wxT("F3"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnSelQ3);

	wxMenuItem* btnSelQ4;
	btnSelQ4 = new wxMenuItem(mnuEdit,wxID_BTN_SEL_Q4,wxString(wxT("Select edge Q4")) + wxT('\t') + wxT("F4"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnSelQ4);

	mnuEdit->AppendSeparator();

	wxMenuItem* btnNext;
	btnNext = new wxMenuItem(mnuEdit,wxID_BTN_NEXT,wxString(wxT("Next sprite")) + wxT('\t') + wxT("]"),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnNext);

	wxMenuItem* btnPrev;
	btnPrev = new wxMenuItem(mnuEdit,wxID_BTN_PREV,wxString(wxT("Previous sprite")) + wxT('\t') + wxT("["),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnPrev);

	mnuEdit->AppendSeparator();

	wxMenuItem* btnSetKnownParams;
	btnSetKnownParams = new wxMenuItem(mnuEdit,wxID_BTN_SET_KNOWS,wxString(wxT("Set parameters of known sprites")),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnSetKnownParams);

	wxMenuItem* btnGenWallCont;
	btnGenWallCont = new wxMenuItem(mnuEdit,wxID_BTN_GEN_WALL_CONT,wxString(wxT("Generate wall neighbors")),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnGenWallCont);

	wxMenuItem* btnClearContext;
	btnClearContext = new wxMenuItem(mnuEdit,wxID_BTN_CLR_CONTEXT,wxString(wxT("Clear tile context")),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnClearContext);

	wxMenuItem* btnClearAllContext;
	btnClearAllContext = new wxMenuItem(mnuEdit,wxID_BTN_CLR_ALL_CONTEXT,wxString(wxT("Clear all tiles context")),wxEmptyString,wxITEM_NORMAL);
	mnuEdit->Append(btnClearAllContext);

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

	mnuTools = new wxMenu();
	wxMenuItem* mmNewToolset;
	mmNewToolset = new wxMenuItem(mnuTools,wxID_MM_NEW_TOOLSET,wxString(wxT("New Toolset")) + wxT('\t') + wxT("Ctrl+N"),wxEmptyString,wxITEM_NORMAL);
	mnuTools->Append(mmNewToolset);

	wxMenuItem* mmNewTool;
	mmNewTool = new wxMenuItem(mnuTools,wxID_MM_NEW_TOOL,wxString(wxT("New Tool")) + wxT('\t') + wxT("Ctrl+T"),wxEmptyString,wxITEM_NORMAL);
	mnuTools->Append(mmNewTool);

	wxMenuItem* mmRemoveTool;
	mmRemoveTool = new wxMenuItem(mnuTools,wxID_MM_REMOVE_TOOL,wxString(wxT("Remove Tool(set)")) + wxT('\t') + wxT("Delete"),wxEmptyString,wxITEM_NORMAL);
	mnuTools->Append(mmRemoveTool);

	wxMenuItem* mmEditToolset;
	mmEditToolset = new wxMenuItem(mnuTools,wxID_MM_EDIT_TOOLSET,wxString(wxT("Edit Toolset")) + wxT('\t') + wxT("Ctrl+E"),wxEmptyString,wxITEM_NORMAL);
	mnuTools->Append(mmEditToolset);

	mMenu->Append(mnuTools,wxT("Tools"));

	this->SetMenuBar(mMenu);

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizerSpriteList;
	sizerSpriteList = new wxBoxSizer(wxVERTICAL);

	txtSpriteList = new wxStaticText(this,wxID_ANY,wxT("Sprite list:"),wxDefaultPosition,wxDefaultSize,0);
	txtSpriteList->Wrap(-1);
	sizerSpriteList->Add(txtSpriteList,0,wxLEFT|wxTOP,5);

	lboxSprites = new wxListCtrlVirtual(this,wxID_LBOX_SPRITES,wxDefaultPosition,wxSize(120,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL);
	sizerSpriteList->Add(lboxSprites,1,wxEXPAND|wxBOTTOM|wxLEFT,5);


	bSizer1->Add(sizerSpriteList,0,wxEXPAND|wxRIGHT,5);

	wxBoxSizer* bSizer241;
	bSizer241 = new wxBoxSizer(wxVERTICAL);

	txtAltList = new wxStaticText(this,wxID_TXT_ALT,wxT("Alternatives:"),wxDefaultPosition,wxDefaultSize,0);
	txtAltList->Wrap(-1);
	bSizer241->Add(txtAltList,0,wxTOP|wxLEFT,5);

	lboxAlt = new wxListBox(this,wxID_LBOX_ALT,wxDefaultPosition,wxSize(120,-1),0,NULL,0|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer241->Add(lboxAlt,1,wxBOTTOM|wxLEFT|wxRIGHT,5);


	bSizer1->Add(bSizer241,0,wxEXPAND,5);

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer(wxVERTICAL);


	bSizer4->Add(0,0,0,wxALL|wxEXPAND,4);

	wxStaticBoxSizer* sizerCanvas;
	sizerCanvas = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT(" Sprite(s) view:")),wxVERTICAL);

	canvas = new wxPanel(sizerCanvas->GetStaticBox(),wxID_ANY,wxDefaultPosition,wxDefaultSize,wxTAB_TRAVERSAL);
	sizerCanvas->Add(canvas,1,wxEXPAND | wxALL,5);


	bSizer4->Add(sizerCanvas,1,wxALL|wxEXPAND,5);

	txtGamma = new wxStaticText(this,wxID_ANY,wxT("Set gamma correction:"),wxDefaultPosition,wxDefaultSize,0);
	txtGamma->Wrap(-1);
	bSizer4->Add(txtGamma,0,wxLEFT|wxTOP,5);

	slideGamma = new wxSlider(this,wxID_SLIDE_GAMMA,1300,500,2000,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL);
	bSizer4->Add(slideGamma,0,wxEXPAND|wxLEFT|wxRIGHT|wxTOP,1);

	cbZoom = new wxCheckBox(this,wxID_CB_ZOOM,wxT("Zoom 2x"),wxDefaultPosition,wxDefaultSize,0);
	bSizer4->Add(cbZoom,0,wxALL,5);


	bSizer1->Add(bSizer4,1,wxEXPAND,5);

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer(wxVERTICAL);

	m_staticText2 = new wxStaticText(this,wxID_ANY,wxT("Neighboring sprites:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText2->Wrap(-1);
	bSizer3->Add(m_staticText2,0,wxLEFT|wxTOP,5);

	lboxNeighbor = new wxListBox(this,wxID_LBOX_NEIGHBOR,wxDefaultPosition,wxSize(120,-1),0,NULL,0|wxVSCROLL);
	bSizer3->Add(lboxNeighbor,1,wxBOTTOM,5);

	m_staticText3 = new wxStaticText(this,wxID_ANY,wxT("Side:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText3->Wrap(-1);
	bSizer3->Add(m_staticText3,0,0,5);

	wxString chbSideChoices[] ={wxT("Q1"), wxT("Q2"), wxT("Q3"), wxT("Q4")};
	int chbSideNChoices = sizeof(chbSideChoices) / sizeof(wxString);
	chbSide = new wxChoice(this,wxID_CH_SIDE,wxDefaultPosition,wxDefaultSize,chbSideNChoices,chbSideChoices,0);
	chbSide->SetSelection(0);
	bSizer3->Add(chbSide,0,wxBOTTOM|wxEXPAND,5);


	bSizer1->Add(bSizer3,0,wxEXPAND,5);

	sizerFlags = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT(" Tile properties:")),wxVERTICAL);

	cbUseAsGlyph = new wxCheckBox(sizerFlags->GetStaticBox(),wxID_CB_USE_AS_GLYPH,wxT("Use as class glyph"),wxDefaultPosition,wxDefaultSize,0);
	sizerFlags->Add(cbUseAsGlyph,0,wxTOP|wxRIGHT|wxLEFT,5);

	cbFaultCont = new wxCheckBox(sizerFlags->GetStaticBox(),wxID_CB_FAULTY,wxT("Faulty context"),wxDefaultPosition,wxDefaultSize,0);
	sizerFlags->Add(cbFaultCont,0,wxLEFT|wxRIGHT|wxTOP,5);

	m_staticline2 = new wxStaticLine(sizerFlags->GetStaticBox(),wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	sizerFlags->Add(m_staticline2,0,wxEXPAND | wxALL,5);

	sizerTerrFlags = new wxBoxSizer(wxVERTICAL);

	m_checkBox49 = new wxCheckBox(sizerFlags->GetStaticBox(),wxID_ANY,wxT("Check Me!"),wxDefaultPosition,wxDefaultSize,0);
	sizerTerrFlags->Add(m_checkBox49,0,wxALL,5);


	sizerFlags->Add(sizerTerrFlags,1,wxEXPAND,0);


	bSizer1->Add(sizerFlags,0,wxEXPAND|wxLEFT|wxTOP,5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT("Tile Params:")),wxVERTICAL);

	sbSizer3->SetMinSize(wxSize(300,-1));
	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText5 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q1 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText5->Wrap(-1);
	bSizer21->Add(m_staticText5,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ1classChoices;
	chbQ1class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q1_CLASS,wxDefaultPosition,wxDefaultSize,chbQ1classChoices,0);
	chbQ1class->SetSelection(0);
	bSizer21->Add(chbQ1class,1,wxEXPAND|wxLEFT|wxRIGHT,5);

	cbQ1nofilt = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_Q1_NOFILT,wxT("Don't filter"),wxDefaultPosition,wxDefaultSize,0);
	bSizer21->Add(cbQ1nofilt,0,wxALL,5);


	sbSizer3->Add(bSizer21,1,wxEXPAND,5);

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText6 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q2 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText6->Wrap(-1);
	bSizer22->Add(m_staticText6,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ2classChoices;
	chbQ2class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q2_CLASS,wxDefaultPosition,wxDefaultSize,chbQ2classChoices,0);
	chbQ2class->SetSelection(0);
	bSizer22->Add(chbQ2class,1,wxEXPAND|wxLEFT|wxRIGHT,5);

	cbQ2nofilt = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_Q2_NOFILT,wxT("Don't filter"),wxDefaultPosition,wxDefaultSize,0);
	bSizer22->Add(cbQ2nofilt,0,wxALL,5);


	sbSizer3->Add(bSizer22,1,wxEXPAND,5);

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText7 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q3 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText7->Wrap(-1);
	bSizer23->Add(m_staticText7,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ3classChoices;
	chbQ3class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q3_CLASS,wxDefaultPosition,wxDefaultSize,chbQ3classChoices,0);
	chbQ3class->SetSelection(0);
	bSizer23->Add(chbQ3class,1,wxEXPAND|wxLEFT|wxRIGHT,5);

	cbQ3nofilt = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_Q3_NOFILT,wxT("Don't filter"),wxDefaultPosition,wxDefaultSize,0);
	bSizer23->Add(cbQ3nofilt,0,wxALL,5);


	sbSizer3->Add(bSizer23,1,wxEXPAND,5);

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer(wxHORIZONTAL);

	m_staticText8 = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Q4 class:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText8->Wrap(-1);
	bSizer24->Add(m_staticText8,0,wxLEFT|wxTOP,5);

	wxArrayString chbQ4classChoices;
	chbQ4class = new wxChoice(sbSizer3->GetStaticBox(),wxID_CHB_Q4_CLASS,wxDefaultPosition,wxDefaultSize,chbQ4classChoices,0);
	chbQ4class->SetSelection(0);
	bSizer24->Add(chbQ4class,1,wxEXPAND|wxLEFT|wxRIGHT,5);

	cbQ4nofilt = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_Q4_NOFILT,wxT("Don't filter"),wxDefaultPosition,wxDefaultSize,0);
	bSizer24->Add(cbQ4nofilt,0,wxALL,5);


	sbSizer3->Add(bSizer24,1,wxEXPAND,5);

	txtShadeEdge = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Shaded edges:"),wxDefaultPosition,wxDefaultSize,0);
	txtShadeEdge->Wrap(-1);
	sbSizer3->Add(txtShadeEdge,0,wxLEFT|wxRIGHT|wxTOP,5);

	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer(2,4,0,0);

	cbShadeQ1 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q1,wxT("Q1"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeQ1,0,wxALL,5);

	cbShadeQ2 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q2,wxT("Q2"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeQ2,0,wxALL,5);

	cbShadeQ3 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q3,wxT("Q3"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeQ3,0,wxALL,5);

	cbShadeQ4 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_Q4,wxT("Q4"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeQ4,0,wxALL,5);

	cbShadeC1 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C1,wxT("C1"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeC1,0,wxALL,5);

	cbShadeC2 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C2,wxT("C2"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeC2,0,wxALL,5);

	cbShadeC3 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C3,wxT("C3"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeC3,0,wxALL,5);

	cbShadeC4 = new wxCheckBox(sbSizer3->GetStaticBox(),wxID_CB_SHADE_C4,wxT("C4"),wxDefaultPosition,wxDefaultSize,wxCHK_3STATE|wxCHK_ALLOW_3RD_STATE_FOR_USER);
	gSizer1->Add(cbShadeC4,0,wxALL,5);


	sbSizer3->Add(gSizer1,0,0,5);

	txtFlags = new wxStaticText(sbSizer3->GetStaticBox(),wxID_ANY,wxT("Map tile flags:"),wxDefaultPosition,wxDefaultSize,0);
	txtFlags->Wrap(-1);
	sbSizer3->Add(txtFlags,0,wxLEFT|wxRIGHT|wxTOP,5);

	editMapFlags = new wxTextCtrl(sbSizer3->GetStaticBox(),wxID_EDIT_MAP_TILE_FLAGS,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_PROCESS_ENTER);
	sbSizer3->Add(editMapFlags,0,wxBOTTOM|wxLEFT|wxRIGHT,5);


	bSizer10->Add(sbSizer3,0,wxLEFT|wxTOP|wxEXPAND,5);

	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer(new wxStaticBox(this,wxID_ANY,wxT("Object/class/tools sorting:")),wxVERTICAL);

	cbToolGlyph = new wxCheckBox(sbSizer6->GetStaticBox(),wxID_CB_TOOL_GLYPH,wxT("Use as tool glyph"),wxDefaultPosition,wxDefaultSize,0);
	cbToolGlyph->SetValue(true);
	sbSizer6->Add(cbToolGlyph,0,wxALL,5);

	treeCtrlObjects = new wxTreeCtrl(sbSizer6->GetStaticBox(),wxID_TREE_OBJECTS,wxDefaultPosition,wxDefaultSize,wxTR_DEFAULT_STYLE|wxTR_EDIT_LABELS|wxTR_HIDE_ROOT|wxTR_SINGLE|wxALWAYS_SHOW_SB|wxVSCROLL);
	sbSizer6->Add(treeCtrlObjects,1,wxALL|wxEXPAND,5);


	bSizer10->Add(sbSizer6,1,wxEXPAND|wxLEFT|wxTOP,5);


	bSizer1->Add(bSizer10,0,wxEXPAND,5);


	this->SetSizer(bSizer1);
	this->Layout();
	statBar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_ANY);

	this->Centre(wxBOTH);

	// === AUTO GENERATED STUFF ENDS HERE ===

	/*lboxSprites = new wxListCtrlVirtual(this,wxID_LBOX_SPRITES,wxDefaultPosition,wxSize(120,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL);
	sizerSpriteList->Add(lboxSprites,1,wxBOTTOM|wxEXPAND|wxLEFT,5);*/
	lboxSprites->SetGetItemTextCb(bind(&FormSprite::OnGetItemText,this,placeholders::_1));
	lboxSprites->SetGetItemImageCb(bind(&FormSprite::OnGetItemImage,this,placeholders::_1));
		

	//lboxSprites = new wxListCtrlSprite(this,wxID_LBOX_SPRITES,wxDefaultPosition,wxSize(120,-1),wxALWAYS_SHOW_SB|wxVSCROLL|wxLC_REPORT|wxLC_NO_HEADER|wxLC_SINGLE_SEL|wxLC_VIRTUAL);
	//lboxSprites = new wxListCtrl(this,wxID_LBOX_SPRITES,wxDefaultPosition,wxSize(120,-1),wxALWAYS_SHOW_SB|wxVSCROLL|wxLC_SINGLE_SEL|wxLC_SMALL_ICON);
	//bSizer2->Add(lboxSprites,1,wxBOTTOM|wxEXPAND|wxLEFT,5);


	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON_SPR",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);


	// no sprite selected yet
	m_was_set = false;
	sprite_id = -1;

	// generate terrain menu content
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		Terrain* terr = spell_data->GetTerrain(k);
		auto titem = mnuTerr->Append(TERR_ID0 + k,terr->name,wxEmptyString,wxITEM_RADIO);
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

	// fill in special class selector
	/*Sprite tspr;
	for(int k = 0; k < tspr.GetSpecClassCount(); k++)
		chbSpecClass->Append(tspr.GetSpecClassName(k));*/

	// bind events
	Bind(wxEVT_CLOSE_WINDOW, &FormSprite::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU,&FormSprite::OnCloseClick, this, wxID_BTN_CLOSE);
	Bind(wxEVT_MENU,&FormSprite::OnSelectClick,this,wxID_BTN_SELECT);
	Bind(wxEVT_MENU,&FormSprite::OnSelectSpriteBtn,this,wxID_BTN_NEXT);
	Bind(wxEVT_MENU,&FormSprite::OnSelectSpriteBtn,this,wxID_BTN_PREV);
	Bind(wxEVT_MENU,&FormSprite::OnGenerateWallContext,this,wxID_BTN_GEN_WALL_CONT);
	Bind(wxEVT_MENU,&FormSprite::OnAssignKnowns,this,wxID_BTN_SET_KNOWS);
	Bind(wxEVT_MENU,&FormSprite::OnClearContext,this,wxID_BTN_CLR_CONTEXT);
	Bind(wxEVT_MENU,&FormSprite::OnClearAllContext,this,wxID_BTN_CLR_ALL_CONTEXT);
	Bind(wxEVT_MENU,&FormSprite::OnUpdateContext,this,wxID_EDIT_TILE_CONTEXT_AUTO);	
	Bind(wxEVT_MENU,&FormSprite::OnAutoShadeFlags,this,wxID_BTN_AUTO_SHADING);
	Bind(wxEVT_MENU,&FormSprite::OnSaveTileContext,this,wxID_BTN_SAVE_CONTEXT);
	Bind(wxEVT_MENU,&FormSprite::OnSelectEdgeBtn,this,wxID_BTN_SEL_Q1);
	Bind(wxEVT_MENU,&FormSprite::OnSelectEdgeBtn,this,wxID_BTN_SEL_Q2);
	Bind(wxEVT_MENU,&FormSprite::OnSelectEdgeBtn,this,wxID_BTN_SEL_Q3);
	Bind(wxEVT_MENU,&FormSprite::OnSelectEdgeBtn,this,wxID_BTN_SEL_Q4);	


	Bind(wxEVT_COMMAND_LIST_ITEM_SELECTED,&FormSprite::OnSelectSpriteAlt,this,wxID_LBOX_SPRITES);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSprite::OnSelectSpriteAlt,this,wxID_LBOX_ALT);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSprite::OnSelectNeighbor,this,wxID_LBOX_NEIGHBOR);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormSprite::OnChangeGamma,this,wxID_SLIDE_GAMMA);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnChangeZoom,this,wxID_CB_ZOOM);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED, &FormSprite::OnQchange,this,wxID_CH_SIDE);
	canvas->Bind(wxEVT_PAINT,&FormSprite::OnCanvasRepaint,this);


	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_TOOL_GLYPH);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_USE_AS_GLYPH);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_FAULTY);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_Q1_NOFILT);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_Q2_NOFILT);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_Q3_NOFILT);
	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_Q4_NOFILT);
	Bind(wxEVT_TEXT_ENTER,&FormSprite::OnFlagsChange,this,wxID_EDIT_MAP_TILE_FLAGS);

	//Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSprite::OnSpecClassChange,this,wxID_CHB_SPEC_CLASS);

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


	Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,wxID_CB_TOOL_GLYPH);
	

	statBar->SetFieldsCount(2);
	statBar->SetAutoLayout(true);


	// default 2x zoom
	cbZoom->SetValue(true);
	// default gamma 1.50
	slideGamma->SetValue(1500);

	// stops flickering
	canvas->SetDoubleBuffered(true);

	// select terrain
	SelectTerrain();
	wxCommandEvent ev(wxEVT_COMMAND_SLIDER_UPDATED,wxID_SLIDE_GAMMA);
	OnChangeGamma(ev);

	// load icons for tree
	imlist = new wxImageList(16,16);
	wxBitmap img;
	img.LoadFile("IDI_MULTI",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_SINGLE",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_FOLDER",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_FOLDER_OPEN",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_SPR_A",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	img.LoadFile("IDI_SPR_OBJ",wxBITMAP_TYPE_BMP_RESOURCE);
	if(img.IsOk())
		imlist->Add(img);
	treeCtrlObjects->SetImageList(imlist);
	lboxSprites->SetImageList(imlist,wxIMAGE_LIST_SMALL);
		


	Bind(wxEVT_LIST_BEGIN_DRAG,&FormSprite::OnDragSprite,this,wxID_LBOX_SPRITES);
	//Bind(wxEVT_TREE_END_DRAG,&FormSprite::OnDragSpriteEnd,this,wxID_TREE_OBJECTS);

	Bind(wxEVT_TREE_SEL_CHANGED,&FormSprite::OnTreeSelectionChanged,this,wxID_TREE_OBJECTS);
	Bind(wxEVT_TREE_BEGIN_LABEL_EDIT,&FormSprite::OnTreeClassBeginLabelEdit,this,wxID_TREE_OBJECTS);
	Bind(wxEVT_TREE_END_LABEL_EDIT,&FormSprite::OnTreeClassEndLabelEdit,this,wxID_TREE_OBJECTS);
	Bind(wxEVT_TREE_BEGIN_DRAG,&FormSprite::OnTreeClassBeginDrag,this,wxID_TREE_OBJECTS);
	Bind(wxEVT_TREE_END_DRAG,&FormSprite::OnTreeClassEndDrag,this,wxID_TREE_OBJECTS);
	Bind(wxEVT_TREE_ITEM_MENU,&FormSprite::OnTreeClassMenu,this,wxID_TREE_OBJECTS);
	//Bind(wxEVT_TREE_con,&FormSprite::OnTreeClassMenu,this,wxID_TREE_OBJECTS);
	Bind(wxEVT_MENU,&FormSprite::OnRemoveToolset,this,wxID_MM_REMOVE_TOOL);
	Bind(wxEVT_MENU,&FormSprite::OnNewToolset,this,wxID_MM_NEW_TOOLSET);
	Bind(wxEVT_MENU,&FormSprite::OnNewTool,this,wxID_MM_NEW_TOOL);
	Bind(wxEVT_MENU,&FormSprite::OnEditToolset,this,wxID_MM_EDIT_TOOLSET);
		
	SpriteDropTarget* drop_target = new SpriteDropTarget(this);
	treeCtrlObjects->SetDropTarget((wxDropTarget*)drop_target);

	FillToolsTree();

}
FormSprite::~FormSprite()
{
	delete imlist;
}


// begin sprite drag from sprite list
void FormSprite::OnDragSprite(wxListEvent& event)
{
	auto text = lboxSprites->GetItemText(event.GetIndex(),0);
	//statBar->SetStatusText("drag: " + text,0);
	
	wxTextDataObject tdo(text);
	wxDropSource tds(tdo, this);
	tds.DoDragDrop(wxDrag_CopyOnly);
}
// end sprite drag from sprite list
bool FormSprite::SpriteDropTarget::OnDropText(wxCoord x,wxCoord y,const wxString& data)
{
	//m_owner->statBar->SetStatusText("end drag: " + data,0);

	auto item_id = m_owner->treeCtrlObjects->HitTest(wxPoint(x,y));
	if(!item_id.IsOk())
		return false;
	auto item_data = (TreeNode*)m_owner->treeCtrlObjects->GetItemData(item_id);
	if(!item_data)
		return false;
	auto class_id = item_data->m_class_id;
	auto tool_id = item_data->m_tool_id;
	if(item_data->m_tool_id < 0)
	{
		auto parent_id = m_owner->treeCtrlObjects->GetItemParent(item_id);
		if(!parent_id.IsOk())
			return false;
		auto parent_data = (TreeNode*)m_owner->treeCtrlObjects->GetItemData(parent_id);
		if(!parent_data || parent_data->m_tool_id < 0)
			return false;
		class_id = parent_data->m_class_id;
		tool_id = parent_data->m_tool_id;
		item_id = parent_id;
	}
	
	auto terr = m_owner->FindTerrain();
	SpellTool tool;
	tool.Set(class_id,tool_id);
	auto list = terr->GetToolSprites(tool);
	for(auto &spr: list)
		if(spr->name.compare(data) == 0)
			return false;
	
	auto *sprite = terr->GetSprite(data);
	if(!sprite)
		return false;
	sprite->SetToolClass(class_id + 1);
	sprite->SetToolClassGroup(tool_id + 1);

	auto new_id = m_owner->treeCtrlObjects->AppendItem(item_id,data,Icons::SPR_OBJ,-1,(wxTreeItemData*)new TreeNode(sprite));
	m_owner->treeCtrlObjects->SelectItem(new_id);

	m_owner->FillToolsTree();

	return true;
}

void FormSprite::OnDragSpriteEnd(wxTreeEvent& evt)
{
	statBar->SetStatusText("end drag: ",0);
}


// build toolset title from name and description
std::string FormSprite::MakeToolsetTitle(std::string name,std::string desc)
{
	if(name.compare(desc) == 0 || desc.empty())
		return(name);
	return(name + ": " + desc);
}
// build toolset title from toolset id
std::string FormSprite::GetToolsetTitle(int toolset_class_id)
{
	auto terr = FindTerrain();
	return(MakeToolsetTitle(terr->GetToolSetName(toolset_class_id),terr->GetToolSetTitle(toolset_class_id)));
}
// rename toolset from single string "name: title"
void FormSprite::RenameToolset(int toolset_class_id,std::string title)
{
	regex secexp("([^:]*)[:\\s]*(.*)");
	smatch match;
	std::regex_search(title,match,secexp);
	if(match.size() != 3)
	{
		// failed
		return;
	}
	std::string name = match[1];
	title = match[2];
	if(title.empty())
		title = name;
	auto terr = FindTerrain();
	terr->SetToolSetName(toolset_class_id,name);
	terr->SetToolSetTitle(toolset_class_id,title);
}

// rename object class stuff
void FormSprite::OnTreeClassBeginLabelEdit(wxTreeEvent& evt)
{
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(evt.GetItem());
	if(!obj)
	{
		// no edit root
		evt.Veto();
		return;
	}
	if(obj->m_spr || (obj->m_class_id < 0 && obj->m_tool_id < 0))
	{
		// no edit unassigned class name
		evt.Veto();
		return;
	}
}
void FormSprite::OnTreeClassEndLabelEdit(wxTreeEvent& evt)
{
	std::string text = evt.GetLabel().ToStdString();
	if(text.empty())
	{
		evt.Veto();
		return;
	}
	auto item_id = evt.GetItem();
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(item_id);
	if(!obj)
	{
		// no edit root
		evt.Veto();
		return;
	}
	if(!obj->m_spr && obj->m_tool_id >= 0)
	{
		// rename tool name
		auto terr = FindTerrain();
		terr->RenameToolSetItem(obj->m_class_id,text,obj->m_tool_id);
		treeCtrlObjects->SetItemText(item_id,text);
		treeCtrlObjects->SelectItem(item_id);
		evt.Veto();
		return;
	}
	if(!obj->m_spr && obj->m_class_id >= 0)
	{
		// edit class name
		RenameToolset(obj->m_class_id,text);
		treeCtrlObjects->SetItemText(item_id,GetToolsetTitle(obj->m_class_id));
		treeCtrlObjects->SelectItem(item_id);
		evt.Veto();
		return;
	}
	// edit tool item name
	//obj->m_obj->SetDescription(text);
}

// drag object tree stuff
void FormSprite::OnTreeClassBeginDrag(wxTreeEvent& evt)
{
	m_drag_item = evt.GetItem();
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(m_drag_item);
	if(!obj)
		m_drag_item.Unset();
	treeCtrlObjects->SelectItem(m_drag_item);
	evt.Allow();
}
// drop object tree stuff
void FormSprite::OnTreeClassEndDrag(wxTreeEvent& evt)
{
	if(!m_drag_item)
		return;
	auto obj = (TreeNode*)treeCtrlObjects->GetItemData(m_drag_item);
	auto target_item = evt.GetItem();
	if(!target_item.IsOk())
		return;
	if(treeCtrlObjects->GetRootItem() == target_item)
		return; // cannot move to root
	auto target_obj = (TreeNode*)treeCtrlObjects->GetItemData(target_item);
	auto parent_node = treeCtrlObjects->GetItemParent(target_item);
	if(!obj->m_spr && obj->m_tool_id < 0)
	{
		// moving toolset class
		if(treeCtrlObjects->GetRootItem() == parent_node && target_obj->m_class_id >= 0)
		{
			// target is class node		
			auto terr = FindTerrain();
			terr->MoveToolSet(obj->m_class_id,target_obj->m_class_id,true);
			treeCtrlObjects->SelectItem(m_drag_item);
			FillToolsTree();
			treeCtrlObjects->Refresh();
			evt.Veto();
		}
	}
	else if(obj->m_tool_id >= 0)
	{
		// moving tool class
		if(target_obj->m_class_id == obj->m_class_id)
		{
			// within same toolset
			auto terr = FindTerrain();
			terr->MoveToolSetItem(obj->m_class_id,obj->m_tool_id,target_obj->m_tool_id,true);
			treeCtrlObjects->SelectItem(m_drag_item);			
			FillToolsTree();
			treeCtrlObjects->Refresh();
			evt.Veto();
		}
	}
	else
	{
		// moving tool item
		
	}
}

// remove object by menu
void FormSprite::OnRemoveToolset(wxCommandEvent& evt)
{
	auto item_id = treeCtrlObjects->GetSelection();
	if(!item_id.IsOk())
		return;
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(item_id);
	if(!obj)
		return;
	if(!obj->m_spr && obj->m_class_id < 0)
		return;
	auto terr = FindTerrain();
	if(obj->m_spr)
	{
		// removing sprite
		obj->m_spr->SetToolClass(0);
		obj->m_spr->SetToolClassGroup(0);
		treeCtrlObjects->Delete(item_id);
	}
	else if(obj->m_tool_id >= 0)
	{
		// remove tool
		terr->RemoveToolSetItem(obj->m_class_id,obj->m_tool_id);
		treeCtrlObjects->Delete(item_id);
		//FillToolsTree();
	}
	else
	{
		// removing toolset class
		terr->RemoveToolSet(obj->m_class_id);
		treeCtrlObjects->Delete(item_id);
		//FillToolsTree();
	}
}

// new toolset class
void FormSprite::OnNewToolset(wxCommandEvent& evt)
{
	auto terr = FindTerrain();
	std::string name = "New toolset";
	terr->AddToolSet(name,name);
	auto toolset_id = terr->GetToolSetID(name);

	auto root_id = treeCtrlObjects->GetRootItem();
	auto new_id = treeCtrlObjects->AppendItem(root_id,GetToolsetTitle(toolset_id),Icons::FOLDER,Icons::FOLDER_OPEN,(wxTreeItemData*)new TreeNode(toolset_id));
	treeCtrlObjects->SelectItem(new_id);
	if(!treeCtrlObjects->IsVisible(new_id))
		treeCtrlObjects->EnsureVisible(new_id);	
}

// new tool within class
void FormSprite::OnNewTool(wxCommandEvent& evt)
{
	auto item_id = treeCtrlObjects->GetSelection();
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(item_id);
	if(!obj)
		return; // nope in root
	auto parent_id = treeCtrlObjects->GetItemParent(item_id);
	int class_id = 0;
	if(obj->m_spr)
		return; // nope on sprite
	if(obj->m_class_id < 0)
		return;
	if(obj->m_tool_id < 0)
		parent_id = item_id;

	auto terr = FindTerrain();
	std::string name = "New tool";
	terr->AddToolSetItem(obj->m_class_id,name);
	auto tool_class_id = terr->GetToolSetItem(obj->m_class_id,name);	
	auto tool_id = treeCtrlObjects->AppendItem(parent_id,name,Icons::MULTI,-1,(wxTreeItemData*)new TreeNode(obj->m_class_id,tool_class_id));
	treeCtrlObjects->SelectItem(tool_id);
	FillToolsTree();
}

// edit toolset class properties
void FormSprite::OnEditToolset(wxCommandEvent& evt)
{
	auto item_id = treeCtrlObjects->GetSelection();
	if(!item_id.IsOk())
		return;
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(item_id);
	auto terr = FindTerrain();
	auto form = new FormEditToolset(this,terr,obj->m_class_id);
	if(form->ShowModal() == wxID_OK)
	{
		// --- confirmed
	}
	delete form;
	FillToolsTree();
}

// tool tree menu popup
void FormSprite::OnTreeClassMenu(wxTreeEvent& evt)
{
	auto item_id = evt.GetItem();
	if(!item_id.IsOk())
		return;
	treeCtrlObjects->SelectItem(item_id);
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(item_id);
	
	wxMenu menu;// = new wxMenu();
	menu.SetClientData(item_id);
	if(obj)
		menu.Append(MNU_REMOVE,"Remove\tDelete");
	if(!obj->m_spr && obj->m_class_id >= 0)
		menu.Append(MNU_NEW_TOOL,"New tool\tCtrl+T");
	menu.Append(MNU_NEW_TOOLSET,"New toolset\tCtrl+N");
	if(!obj->m_spr && obj->m_class_id >= 0 && obj->m_tool_id < 0)
		menu.Append(MNU_EDIT_TOOLSET,"Edit toolset parameters\tCtrl+E");
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormSprite::OnTreeClassMenuClick),NULL,this);
	treeCtrlObjects->PopupMenu(&menu,evt.GetPoint());
}
// on tool tree menu click
void FormSprite::OnTreeClassMenuClick(wxCommandEvent& evt)
{
	auto menu_id = evt.GetId();
	auto menu = (wxMenu*)evt.GetEventObject();
	auto item_id = (wxTreeItemId)menu->GetClientData();
	if(menu_id == MNU_REMOVE)
	{
		wxCommandEvent event;
		OnRemoveToolset(event);
	}
	else if(menu_id == MNU_NEW_TOOL)
	{
		wxCommandEvent event;
		OnNewTool(event);
	}
	else if(menu_id == MNU_NEW_TOOLSET)
	{
		wxCommandEvent event;
		OnNewToolset(event);
	}
	else if(menu_id == MNU_EDIT_TOOLSET)
	{
		wxCommandEvent event;
		OnEditToolset(event);
	}
}

// selected sprite in objects tree
void FormSprite::OnTreeSelectionChanged(wxTreeEvent& evt)
{
	wxTreeItemId selectedNode = evt.GetItem();
	auto* obj = (TreeNode*)treeCtrlObjects->GetItemData(selectedNode);
	if(!obj)
	{
		// root
		evt.Skip();
		return;
	}
	if(!obj->m_spr)
	{
		// no sprite
		evt.Skip();
		return;
	}

	// is node
	auto terr = FindTerrain();
	sprite_id = terr->GetSpriteID(obj->m_spr);

	if(sprite_id < lboxSprites->GetItemCount())
	{
		lboxSprites->SetItemState(sprite_id,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);	
		if(!lboxSprites->IsVisible(sprite_id))
			lboxSprites->EnsureVisible(sprite_id);
	}
	
	SelectQuad();
	SetFlags();
	SetEdgeClasses();
	SetShadingFlags();
	FillAltList();
	canvas->Refresh();
}




// fills tool class menu
void FormSprite::FillToolsTree()
{
	// get this terrain
	Terrain* terr = FindTerrain();

	// remember last expand states
	wxTreeLister lister(treeCtrlObjects);
	
	treeCtrlObjects->DeleteAllItems();
	auto root_id = treeCtrlObjects->AddRoot("Toolsets",Icons::FOLDER,Icons::FOLDER_OPEN);
	for(int k = 0; k < terr->GetToolsCount(); k++)
	{
		wxTreeItemId cid;
		cid = treeCtrlObjects->AppendItem(root_id,GetToolsetTitle(k),Icons::FOLDER,Icons::FOLDER_OPEN,(wxTreeItemData*)new TreeNode(k));

		for(int tid = 0; tid < terr->GetToolSetItemsCount(k); tid++)
		{
			auto name = terr->GetToolSetItem(k,tid);
			auto group_id = treeCtrlObjects->AppendItem(cid,name,Icons::MULTI,-1,(wxTreeItemData*)new TreeNode(k,tid));

			SpellTool tool;
			tool.Set(k,tid);
			auto sprite_list = terr->GetToolSprites(tool);
			for(auto &spr: sprite_list)			
				treeCtrlObjects->AppendItem(group_id,spr->name,(spr->land_type > 0)?(Icons::SPR_A):(Icons::SPR_OBJ),-1,(wxTreeItemData*)new TreeNode(spr));
		}
	}
	// try restore last expand states
	lister.treeCtrlSetStates(treeCtrlObjects);
}


// set initial sprite
void FormSprite::SetSprite(Terrain *terr, Sprite *spr)
{
	// select terrain
	for(int k = 0;k<spell_data->GetTerrainCount();k++)
	{
		if(spell_data->GetTerrain(k) == terr)
		{
			// found selection
			GetMenuBar()->FindItem(TERR_ID0 + k)->Check(true);			
			break;
		}
	}
	SelectTerrain();

	if(!spr)
		return;

	// select sprite in list
	auto spr_id = lboxSprites->FindItem(-1,spr->name);
	if(spr_id >= 0)
	{
		lboxSprites->SetItemState(spr_id,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
		lboxSprites->EnsureVisible(spr_id);
	}

	wxCommandEvent evt;
	OnSelectSpriteAlt(evt);
	
	// sprite was set externally
	m_was_set = true;
}

// on close form
void FormSprite::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}
// close button
void FormSprite::OnCloseClick(wxCommandEvent& event)
{
	// update list of tile to be used as class type glyphs
	Terrain* terr = FindTerrain();
	terr->UpdateTileGlyphs();

	// no exit sprite selection
	m_terrain = NULL;
	m_sprite = NULL;
	
	Close();
}
// select + close button
void FormSprite::OnSelectClick(wxCommandEvent& event)
{
	// update list of tile to be used as class type glyphs
	Terrain* terr = FindTerrain();
	terr->UpdateTileGlyphs();

	// exit sprite selection
	m_terrain = terr;
	if(sprite_id >= 0)
		m_sprite = terr->GetSprite(sprite_id);
	else
		m_sprite = NULL;

	Close();
}

// result querry
Terrain* FormSprite::GetSelectedTerrain()
{
	return(m_terrain);
}
Sprite* FormSprite::GetSelectedSprite()
{
	return(m_sprite);
}
bool FormSprite::wasSet()
{
	return(m_was_set);
}










// assign known parameter of sprites
void FormSprite::OnAssignKnowns(wxCommandEvent& event)
{
	wxMessageDialog dial(NULL,"Assign known tiles' map flags? These are tile flags used in map terrain and objects layer, e.g. 0x90 for trees, etc.","Set known tiles flags...",wxYES_NO);
	if(dial.ShowModal() != wxID_YES)
		return;

	// get this terrain
	Terrain* terr = FindTerrain();
	// init map tile flags
	terr->InitSpriteMapTileFlags();
	terr->FixSpriteLandTypes();
	
	OnSelectSpriteAlt(event);
}

// regenerate neighbors for walls
void FormSprite::OnGenerateWallContext(wxCommandEvent& event)
{
	wxMessageDialog dial(NULL,"Generate wall tiles valid neighbors? These are used in auto tile mapping.","Generate wall neighbors...",wxYES_NO);
	if(dial.ShowModal() != wxID_YES)
		return;

	// get this terrain
	Terrain* terr = FindTerrain();
	// init map tile flags
	terr->GenWallNeighbors();

	OnSelectSpriteAlt(event);
}

// clear tiles context
void FormSprite::OnClearAllContext(wxCommandEvent& event)
{
	wxMessageDialog dial(NULL,"Clear all tiles' context data (valid neighboring tiles)? Context data are used for auto tile mapping.","Clearing tiles' context data...",wxYES_NO);
	if(dial.ShowModal() != wxID_YES)
		return;

	// get this terrain
	Terrain* terr = FindTerrain();
	// clear all tiles context
	terr->ClearSpriteContext();
	
	OnSelectSpriteAlt(event);
}
// clear selected context
void FormSprite::OnClearContext(wxCommandEvent& event)
{
	wxMessageDialog dial(NULL,"Clear this tile context data (valid neighboring tiles)? Context data are used for auto tile mapping.","Clearing tile's context data...",wxYES_NO);
	if(dial.ShowModal() != wxID_YES)
		return;

	if(sprite_id < 0)
		return;
	// get this terrain
	Terrain* terr = FindTerrain();
	if(!terr)
		return;
	auto sprite = terr->GetSprite(sprite_id);
	if(!sprite)
		return;
	for(int k = 0; k < 4; k++)
		sprite->ClearContext(k);
	
	OnSelectSpriteAlt(event);
}
// update tiles' context from edge class data
void FormSprite::OnUpdateContext(wxCommandEvent& event)
{
	wxMessageDialog dial(NULL,"Update all tiles' context data (valid neighboring tiles) from tile edge classes? Context data are used for auto tile mapping.","Update tiles' context data...",wxYES_NO);
	if(dial.ShowModal() != wxID_YES)
		return;

	// get this terrain
	Terrain* terr = FindTerrain();
	// build context from edge class data
	terr->UpdateSpriteContext(bind(&FormSprite::OnUpdateContextCb,this, placeholders::_1));
	
	OnSelectSpriteAlt(event);
}
void FormSprite::OnUpdateContextCb(string status)
{
	SetStatusText(status);
}
// auto set edge shading flags
void FormSprite::OnAutoShadeFlags(wxCommandEvent& event)
{
	wxMessageDialog dial(NULL,"Set all tiles' edge shading flags for known sprites? Edge shading flags are part of auto tile mapping.","Update tiles' context data...",wxYES_NO);
	if(dial.ShowModal() != wxID_YES)
		return;

	// get this terrain
	Terrain* terr = FindTerrain();
	// build context from edge class data
	terr->InitSpriteContextShading();
	
	OnSelectSpriteAlt(event);
}
// save tiles' context to file
void FormSprite::OnSaveTileContext(wxCommandEvent& event)
{
	// get this terrain
	Terrain* terrain = FindTerrain();
	
	// split path to folder and file
	std::filesystem::path last_path = terrain->GetSpriteContextPath();
	wstring dir = last_path.parent_path(); //dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Save Spellcross terrain context file"),dir,name,"Context file (*.con)|*.con",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
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
		Sprite::SHADING_SIDE_Q1, Sprite::SHADING_SIDE_Q2, Sprite::SHADING_SIDE_Q3, Sprite::SHADING_SIDE_Q4,
		Sprite::SHADING_CORNER_Q1, Sprite::SHADING_CORNER_Q2, Sprite::SHADING_CORNER_Q3, Sprite::SHADING_CORNER_Q4};

	if(sprite_id >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();

		// update flags
		uint32_t mask = terr->sprites[sprite_id]->GetShadingMask();
		uint32_t flag = terr->sprites[sprite_id]->GetShadingFlags();
		for(int k = 0; k < 8;k++)
		{
			wxCheckBox* cb = (wxCheckBox*)FindItem(list[k]);
			if(cb->Get3StateValue() == wxCHK_CHECKED)
			{
				flag |= flags[k];
				mask |= flags[k];
			}
			else if(cb->Get3StateValue() == wxCHK_UNCHECKED)
			{
				flag &= ~flags[k];
				mask |= flags[k];
			}
			else
			{
				mask &= ~flags[k];
			}
		}
		terr->sprites[sprite_id]->SetShadingFlags(flag & 0xFF);
		terr->sprites[sprite_id]->SetShadingMask(mask & 0xFF);
	}	
}
void FormSprite::SetShadingFlags()
{
	const long list[] ={
		wxID_CB_SHADE_Q1, wxID_CB_SHADE_Q2, wxID_CB_SHADE_Q3, wxID_CB_SHADE_Q4,
		wxID_CB_SHADE_C1, wxID_CB_SHADE_C2, wxID_CB_SHADE_C3, wxID_CB_SHADE_C4};
	const uint32_t flags[] ={
		Sprite::SHADING_SIDE_Q1, Sprite::SHADING_SIDE_Q2, Sprite::SHADING_SIDE_Q3, Sprite::SHADING_SIDE_Q4,
		Sprite::SHADING_CORNER_Q1, Sprite::SHADING_CORNER_Q2, Sprite::SHADING_CORNER_Q3, Sprite::SHADING_CORNER_Q4};

	if(sprite_id >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();
		// update checkboxes
		uint32_t mask = terr->sprites[sprite_id]->GetShadingMask();
		uint32_t flag = terr->sprites[sprite_id]->GetShadingFlags();
		for(int k = 0; k < 8;k++)
		{
			wxCheckBox* cb = (wxCheckBox*)FindItem(list[k]);			
			if(!(mask & flags[k]))
				cb->Set3StateValue(wxCHK_UNDETERMINED);
			else if(flag & flags[k])
				cb->Set3StateValue(wxCHK_CHECKED);
			else
				cb->Set3StateValue(wxCHK_UNCHECKED);
		}
	}
}


// change edge class
void FormSprite::OnEdgeClassChange(wxCommandEvent& event)
{
	if(sprite_id >= 0)
	{
		// get this terrain
		Terrain *terr = FindTerrain();
	
		// update class(es)
		wxChoice* chc[4] ={chbQ1class, chbQ2class, chbQ3class, chbQ4class};
		for(int k = 0; k < 4; k++)
			terr->sprites[sprite_id]->SetEdgeClass(k, chc[k]->GetSelection());
	}
}
void FormSprite::SetEdgeClasses()
{
	if(sprite_id >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();

		// set classes
		wxChoice* chc[4] ={chbQ1class, chbQ2class, chbQ3class, chbQ4class};
		for(int k = 0; k < 4; k++)
		{
			int edge_class = terr->sprites[sprite_id]->GetEdgeClass(k);
			chc[k]->Select(edge_class);
		}
	}
}

// on change of tile flags list
void FormSprite::OnFlagsChange(wxCommandEvent& event)
{
	int cbid = event.GetId();
	bool check = event.IsChecked();
	
	const uint32_t list2[][2] ={
		{wxID_CB_USE_AS_GLYPH, Sprite::LandFlags::IS_GLYPH},
		{wxID_CB_TOOL_GLYPH, Sprite::LandFlags::IS_TOOL_ITEM_GLYPH},
		{wxID_CB_FAULTY, Sprite::LandFlags::IS_FAULTY},		
		{wxID_CB_Q1_NOFILT, Sprite::LandFlags::Q1_NOFILT},
		{wxID_CB_Q2_NOFILT, Sprite::LandFlags::Q2_NOFILT},
		{wxID_CB_Q3_NOFILT, Sprite::LandFlags::Q3_NOFILT},
		{wxID_CB_Q4_NOFILT, Sprite::LandFlags::Q4_NOFILT},
		{0,0}
	};

	if(sprite_id >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();

		// set sprite flags
		uint32_t flags = terr->sprites[sprite_id]->GetFlags();
		auto terr_flag_list = terr->GetSpriteFlagList();
		auto& flag_list = sizerTerrFlags->GetChildren();
		for(int k = 0; k < min(flag_list.size(),terr_flag_list.size()); k++)
		{
			auto cb = (wxCheckBox*)flag_list[k]->GetWindow();
			auto flag = terr_flag_list[k];
			if(cb->GetValue())
				flags |= flag.flag;
			else
				flags &= ~flag.flag;
		}
		terr->sprites[sprite_id]->SetFlags(flags);


		// build special flags from checkbox states
		flags = terr->sprites[sprite_id]->GetGlyphFlags();
		int flag_id = 0;
		while(list2[flag_id][0])
		{
			if(cbid == list2[flag_id][0] && check)
				flags |= list2[flag_id][1];
			else if(cbid == list2[flag_id][0] && !check)
				flags &= ~list2[flag_id][1];
			flag_id++;
		}
		terr->sprites[sprite_id]->SetGlyphFlags(flags);

		// get map tile flag
		int map_flags;
		if(std::sscanf(editMapFlags->GetValue().c_str(),"%x",&map_flags) == 1)
			terr->sprites[sprite_id]->SetMapFlags(map_flags);
		editMapFlags->SetValue(string_format("0x%02X",map_flags));
	}	
}
// show tile flags
void FormSprite::SetFlags()
{

	const struct {
		wxCheckBox* cb;
		uint32_t flag;
	} list2[] ={
		{cbUseAsGlyph, Sprite::LandFlags::IS_GLYPH},
		{cbToolGlyph, Sprite::LandFlags::IS_TOOL_ITEM_GLYPH},
		{cbFaultCont, Sprite::LandFlags::IS_FAULTY},
		{cbQ1nofilt, Sprite::LandFlags::Q1_NOFILT},
		{cbQ2nofilt, Sprite::LandFlags::Q2_NOFILT},
		{cbQ3nofilt, Sprite::LandFlags::Q3_NOFILT},
		{cbQ4nofilt, Sprite::LandFlags::Q4_NOFILT},
		{NULL,0}
	};

	int flags = 0;
	if(sprite_id >= 0)
	{
		// get this terrain
		Terrain* terr = FindTerrain();
		auto *sprite = terr->sprites[sprite_id];

		// set sprite flags
		flags = sprite->GetFlags();
		auto terr_flag_list = terr->GetSpriteFlagList();
		auto &flag_list = sizerTerrFlags->GetChildren();
		for(int k = 0; k < min(flag_list.size(),terr_flag_list.size()); k++)
		{
			auto cb = (wxCheckBox*)flag_list[k]->GetWindow();
			auto flag = terr_flag_list[k];
			cb->SetValue(flags & flag.flag);
		}

		// set special flags
		flags = sprite->GetGlyphFlags();
		int flag_id = 0;
		while(list2[flag_id].cb)
		{
			list2[flag_id].cb->SetValue(flags & list2[flag_id].flag);
			flag_id++;
		}
		
		// special map layer 2 flags (extracted from maps)
		auto map_flags = sprite->GetMapFlags();
		editMapFlags->SetValue(string_format("0x%02X",map_flags));

		// status bar
		statBar->SetStatusText(sprite->name,0);
		std::string typestr;
		if(sprite->land_type)
			typestr = string_format("Land Type: %c (%d)",sprite->land_type + 'A' - 1,sprite->land_type);
		else
			typestr = "Land Type: Object";
		statBar->SetStatusText(typestr,1);
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
	if(sprite_id >= 0)
	{
		// something selected
		*tile++ = sprite_id;
		
		for(int quid = 0; quid < 4; quid++)
		{
			Sprite* spr;
			if(quid == chbSide->GetSelection())
			{
				// is selected side				
				spr = terr->sprites[sprite_id]->GetContext(quid, lboxNeighbor->GetSelection());
			}
			else
			{
				// not selected side
				spr = terr->sprites[sprite_id]->GetContext(quid,0);
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
void FormSprite::OnSelectSpriteAlt(wxCommandEvent& event)
{
	sprite_id = -1;
	int is_alt = event.GetId() == wxID_LBOX_ALT;
	if(is_alt)
	{
		// alternative selected
		if(lboxAlt->GetCount() && lboxAlt->GetSelection() >= 0)
		{
			Sprite *spr = (Sprite*)lboxAlt->GetClientData(lboxAlt->GetSelection());
			sprite_id = spr->GetIndex();
		}
	}
	else
	{
		// main list selected		
		auto id = lboxSprites->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
		if(id >= 0)
		{			
			sprite_id = id;
		}
	}	
	if(sprite_id >= 0)
		txtSpriteList->SetLabel(wxString::Format("Sprite list (#%d):",sprite_id));
	else
		txtSpriteList->SetLabel(wxString::Format("Sprite list:"));

	SelectQuad();
	SetFlags();
	SetEdgeClasses();
	SetShadingFlags();
	if(!is_alt)
		FillAltList();
	canvas->Refresh();
}
// select sprite by shortcuts
void FormSprite::OnSelectSpriteBtn(wxCommandEvent& event)
{
	if(!lboxSprites->GetItemCount())
		return;
	int id = lboxSprites->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(id < 0)
		return;
	if(event.GetId() == wxID_BTN_NEXT)
		id = min(id + 1,lboxSprites->GetItemCount()-1);
	else if(event.GetId() == wxID_BTN_PREV)
		id = max(id - 1,0);
	lboxSprites->SetItemState(id,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	lboxSprites->EnsureVisible(id);
	OnSelectSpriteAlt(event);
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
	// found selection
	Terrain* terr = FindTerrain();
	if(!terr)
		return;

	sizerTerrFlags->Clear(true);	
	for(auto& flag: terr->GetSpriteFlagList())
	{
		auto cb = new wxCheckBox(sizerFlags->GetStaticBox(),::wxNewId(),flag.name,wxDefaultPosition,wxDefaultSize,0);
		sizerTerrFlags->Add(cb,0,wxRIGHT|wxLEFT|wxBOTTOM,5);
		cb->Bind(wxEVT_COMMAND_CHECKBOX_CLICKED,&FormSprite::OnFlagsChange,this,cb->GetId());
	}
	sizerTerrFlags->Layout();
	sizerFlags->Layout();
	
	// loose old sprites list
	lboxSprites->ClearAll();
	
	
	lboxSprites->SetClientData(terr);

	// update title
	SetTitle(wxString::Format("Sprite viewer (%s)",terr->name));

	// for each sprite:
	//lboxSprites->report
	lboxSprites->Freeze();
	lboxSprites->AppendColumn("list",wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	lboxSprites->SetItemCount(terr->GetSpriteCount());
	lboxSprites->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);
	
	// select default
	if(lboxSprites->GetItemCount())
		lboxSprites->SetItemState(0,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	lboxSprites->Thaw();
	sprite_id = 0;

	FillToolsTree();
}

wxString FormSprite::OnGetItemText(long item)
{	
	auto terr = (Terrain*)lboxSprites->GetClientData();
	if(!terr)
		return("");
	auto spr = terr->GetSprite(item);
	if(!spr)
		return("");
	return(spr->name);
}

int FormSprite::OnGetItemImage(long item)
{
	auto terr = (Terrain*)lboxSprites->GetClientData();
	if(!terr)
		return(-1);
	auto spr = terr->GetSprite(item);
	if(!spr)
		return(-1);
	if(spr->land_type == 0)
		return(FormSprite::Icons::SPR_OBJ);
	else
		return(FormSprite::Icons::SPR_A);
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
	//int refid = lboxSprites->GetSelection();
	// get center tile context	
	Sprite* cont = terr->sprites[sprite_id];

	// fill list of context sprites
	lboxNeighbor->Clear();
	lboxNeighbor->Freeze();
	for(int sid = 0;sid < cont->GetContextCount(quid); sid++)
	{
		Sprite* spr = cont->GetContext(quid,sid);
		lboxNeighbor->Append(spr->name);
	}		
	if(cont->GetContextCount(quid))
		lboxNeighbor->Select(0);
	lboxNeighbor->Thaw();
}
void FormSprite::OnQchange(wxCommandEvent& event)
{
	SelectQuad();
	canvas->Refresh();
}
void FormSprite::OnSelectEdgeBtn(wxCommandEvent& event)
{
	vector<int> list = {wxID_BTN_SEL_Q1, wxID_BTN_SEL_Q2, wxID_BTN_SEL_Q3, wxID_BTN_SEL_Q4};
	int edge_id = 0;
	for(int k = 0; k < list.size(); k++)
	{
		if(event.GetId() == list[k])
			chbSide->Select(k);
	}
	SelectQuad();	
	canvas->Refresh();
}


// fill list of alternative sprites
void FormSprite::FillAltList()
{
	// get terrain
	Terrain* terr = FindTerrain();
	if(!terr)
		return;

	if(sprite_id >= 0)
	{
		// something selected
		Sprite *spr = terr->sprites[sprite_id];

		// fill list of alternatives
		lboxAlt->Freeze();
		lboxAlt->Clear();
		for(const auto & alt : terr->sprites)
		{
			if(alt == spr)
				continue;
			if(!spr->CompareSpriteContextAlt(alt))
				continue;
			lboxAlt->Append(alt->name,alt);
		}
		if(lboxAlt->GetCount())
			lboxAlt->Select(0);
		lboxAlt->Thaw();
	}
}