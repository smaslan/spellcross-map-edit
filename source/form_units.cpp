///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_units.h"
#include "other.h"

#include <wx/rawbmp.h>

///////////////////////////////////////////////////////////////////////////

FormUnits::FormUnits( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	// === AUTO GENERATED START ===	
	
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SB);
	mmenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(wxT("Exit")) + wxT('\t') + wxT("Esc"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	mmenu->Append(mmFile,wxT("File"));

	mmEdit = new wxMenu();
	wxMenuItem* mmSet;
	mmSet = new wxMenuItem(mmEdit,wxID_MM_SET,wxString(wxT("Update unit")) + wxT('\t') + wxT("Enter"),wxEmptyString,wxITEM_NORMAL);
	mmEdit->Append(mmSet);

	mmenu->Append(mmEdit,wxT("Edit"));

	this->SetMenuBar(mmenu);

	wxBoxSizer* szMain;
	szMain = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szList;
	szList = new wxBoxSizer(wxVERTICAL);

	szList->SetMinSize(wxSize(180,-1));
	m_staticText32 = new wxStaticText(this,wxID_ANY,wxT("Units list:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText32->Wrap(-1);
	szList->Add(m_staticText32,0,wxTOP|wxRIGHT|wxLEFT,5);

	lboxUnits = new wxListBox(this,wxID_LB_UNITS,wxDefaultPosition,wxSize(-1,-1),0,NULL,wxLB_ALWAYS_SB|wxALWAYS_SHOW_SB|wxVSCROLL);
	lboxUnits->SetMaxSize(wxSize(180,-1));

	szList->Add(lboxUnits,1,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	szMain->Add(szList,0,wxEXPAND,5);

	m_staticline9 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szMain->Add(m_staticline9,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* szProps;
	szProps = new wxBoxSizer(wxVERTICAL);

	m_staticText35 = new wxStaticText(this,wxID_ANY,wxT("Unit icon:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText35->Wrap(-1);
	szProps->Add(m_staticText35,0,wxTOP|wxRIGHT|wxLEFT,5);

	icon_canvas = new wxPanel(this,wxID_CANVAS_ICON,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	icon_canvas->SetMaxSize(wxSize(-1,100));

	szProps->Add(icon_canvas,1,wxEXPAND|wxALL,5);

	m_staticText34 = new wxStaticText(this,wxID_ANY,wxT("Properties:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText34->Wrap(-1);
	szProps->Add(m_staticText34,0,wxTOP|wxRIGHT|wxLEFT,5);

	props = new wxPropertyGrid(this,wxID_PROPS,wxDefaultPosition,wxDefaultSize,wxPG_DEFAULT_STYLE);
	props->SetMinSize(wxSize(200,-1));

	szProps->Add(props,1,wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticline13 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szProps->Add(m_staticline13,0,wxEXPAND | wxALL,5);

	m_staticText39 = new wxStaticText(this,wxID_ANY,wxT("Custom name:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText39->Wrap(-1);
	szProps->Add(m_staticText39,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtName = new wxTextCtrl(this,wxID_NAME,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	szProps->Add(txtName,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText40 = new wxStaticText(this,wxID_ANY,wxT("Health:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText40->Wrap(-1);
	szProps->Add(m_staticText40,0,wxRIGHT|wxLEFT,5);

	spinHealth = new wxSpinCtrl(this,wxID_SPIN_HEALTH,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	szProps->Add(spinHealth,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	szMain->Add(szProps,0,wxEXPAND,5);

	m_staticline10 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szMain->Add(m_staticline10,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* szArt;
	szArt = new wxBoxSizer(wxVERTICAL);

	m_staticText36 = new wxStaticText(this,wxID_ANY,wxT("Unit art:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText36->Wrap(-1);
	szArt->Add(m_staticText36,0,wxTOP|wxRIGHT|wxLEFT,5);

	art_canvas = new wxPanel(this,wxID_CANVAS_ART,wxDefaultPosition,wxSize(-1,-1),wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	art_canvas->SetMinSize(wxSize(-1,400));

	szArt->Add(art_canvas,1,wxEXPAND | wxALL,5);

	m_staticline11 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	szArt->Add(m_staticline11,0,wxEXPAND | wxALL,5);

	wxBoxSizer* szArtSel;
	szArtSel = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* szArtSelector;
	szArtSelector = new wxBoxSizer(wxVERTICAL);

	szArtSelector->SetMinSize(wxSize(150,-1));
	m_staticText37 = new wxStaticText(this,wxID_ANY,wxT("Art selection:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText37->Wrap(-1);
	szArtSelector->Add(m_staticText37,0,wxTOP|wxRIGHT|wxLEFT,5);

	lboxArt = new wxListBox(this,wxID_LB_ART,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB);
	szArtSelector->Add(lboxArt,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	szArtSel->Add(szArtSelector,0,wxEXPAND,5);

	m_staticline12 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	szArtSel->Add(m_staticline12,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* szInfo;
	szInfo = new wxBoxSizer(wxVERTICAL);

	m_staticText38 = new wxStaticText(this,wxID_ANY,wxT("Unit description:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText38->Wrap(-1);
	szInfo->Add(m_staticText38,0,wxTOP|wxRIGHT|wxLEFT,5);

	txtInfo = new wxTextCtrl(this,wxID_TXT_INFO,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_LEFT|wxTE_MULTILINE|wxTE_WORDWRAP);
	szInfo->Add(txtInfo,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	szArtSel->Add(szInfo,1,wxEXPAND,5);


	szArt->Add(szArtSel,1,wxEXPAND,5);


	szMain->Add(szArt,1,wxEXPAND,5);


	this->SetSizer(szMain);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	m_spell_data = NULL;
	m_unit = NULL;
	m_update = false;
	
	// close
	Bind(wxEVT_CLOSE_WINDOW,&FormUnits::OnClose,this,this->m_windowId);
	Bind(wxEVT_MENU,&FormUnits::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_MENU,&FormUnits::OnCloseClick,this,wxID_MM_SET);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormUnits::OnSelectUnit,this,wxID_LB_UNITS);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormUnits::OnSelectArt,this,wxID_LB_ART);

	// icon canvas stuff
	icon_canvas->SetDoubleBuffered(true);
	icon_canvas->Bind(wxEVT_PAINT,&FormUnits::OnPaintIcon,this,wxID_CANVAS_ICON);

	// art canvas stuff
	art_canvas->SetDoubleBuffered(true);
	art_canvas->Bind(wxEVT_PAINT,&FormUnits::OnPaintArt,this,wxID_CANVAS_ART);
	


}

FormUnits::~FormUnits()
{
}

void FormUnits::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(),ev);
	ev.Skip();
	Destroy();
}
// close form
void FormUnits::OnCloseClick(wxCommandEvent& event)
{
	m_update = (event.GetId() == wxID_MM_SET) && m_unit;
	if(m_update && lboxUnits->GetSelection() >= 0)
	{
		// update unit record (if attached)
		m_unit->type_id = lboxUnits->GetSelection();
		m_unit->unit = m_spell_data->units->GetUnit(m_unit->type_id);
		m_unit->man = m_unit->unit->cnt;
		m_unit->was_moved = true;
		if(strcmp(m_unit->name,txtName->GetValue().c_str()) != 0)
			strcpy_s(m_unit->name,sizeof(m_unit->name),txtName->GetValue().c_str());
	}
	Close();
}

// check if unit shall be updated or not
bool FormUnits::DoUpdateUnit()
{
	return(m_update);
}

// set spellcross data ref
void FormUnits::SetSpellData(SpellData* spelldata)
{
	m_spell_data = spelldata;

	// fill units list
	lboxUnits->Freeze();
	lboxUnits->Clear();
	for(auto & unit : m_spell_data->units->GetUnits())
		lboxUnits->Append(string_format("#%02d: %s ",lboxUnits->GetCount(),unit->name));
	lboxUnits->Thaw();
	if(lboxUnits->GetCount())
		lboxUnits->Select(0);

	SelectUnit();
}

// set spellcross map unit ref (or null to deselect unit)
void FormUnits::SetMapUnit(MapUnit *unit)
{
	m_unit = unit;
	SelectUnit(unit);
}

// on unit selection change
void FormUnits::OnSelectUnit(wxCommandEvent& event)
{
	SelectUnit();
}

// fill form stuff when unit selected
void FormUnits::SelectUnit(MapUnit *unit)
{
	if(unit)
	{
		// search map unit in list and select it:
		for(int uid = 0; uid < m_spell_data->units->Count(); uid++)
		{
			auto *item = m_spell_data->units->GetUnit(uid);
			if(item == unit->unit)
			{
				lboxUnits->Select(uid);
				break;
			}
		}
	}
	// get unit selection:
	if(!lboxUnits->GetCount() || lboxUnits->GetSelection() < 0)
		return;
	auto sel = lboxUnits->GetSelection();
	auto *unit_rec = m_spell_data->units->GetUnit(sel);

	// show some properties
	props->Freeze();
	props->Clear();
	props->Append(new wxStringProperty(wxT("Sight"),wxT(""),string_format("%d",unit_rec->sdir)));
	props->Append(new wxStringProperty(wxT("Move range"),wxT(""),string_format("%d",unit_rec->apw)));
	props->Append(new wxStringProperty(wxT("Defence"),wxT(""),string_format("%d",unit_rec->def)));		
	props->Append(new wxStringProperty(wxT("Light attack"),wxT(""),string_format("%d",unit_rec->alig)));
	props->Append(new wxStringProperty(wxT("Armor attack"),wxT(""),string_format("%d",unit_rec->aarm)));
	props->Append(new wxStringProperty(wxT("Air attack"),wxT(""),string_format("%d",unit_rec->aair)));
	props->Append(new wxStringProperty(wxT("Objects attack"),wxT(""),string_format("%d",unit_rec->aobj)));
	props->Append(new wxStringProperty(wxT("Man count"),wxT(""),string_format("%d",unit_rec->cnt)));
	props->Thaw();

	// set health control
	int health = unit_rec->GetHP();
	if(m_unit)
		health = m_unit->man;
	spinHealth->SetRange(1, unit_rec->cnt);
	spinHealth->SetValue(health);

	// show map unit name
	string name = "";
	if(m_unit)
		name = m_unit->name;
	txtName->SetValue(name);

	// fill art list
	lboxArt->Freeze();
	lboxArt->Clear();
	for(auto & art : unit_rec->GetArtList(m_spell_data->info))
		lboxArt->Append(art);
	lboxArt->Thaw();
	if(lboxArt->GetCount())
		lboxArt->Select(0);

	// paint icon
	icon_canvas->Refresh();
	
	// paint art
	art_canvas->Refresh();

	// show info text
	WriteInfo();
}

// show unit info
void FormUnits::WriteInfo()
{
	// get art selection:
	if(!lboxArt->GetCount() || lboxArt->GetSelection() < 0)
		return;
	auto sel = lboxArt->GetSelection();
	string art_name = lboxArt->GetString(sel).ToStdString();

	vector<string> langs = {"CZ","ENG"};
	for(auto & lang : langs)
	{
		// info text file
		string art_info_name = art_name + "." + lang;
		uint8_t* txt_buf;
		int txt_size;
		if(m_spell_data->info->GetFile(art_info_name.c_str(),&txt_buf,&txt_size))
			continue;
		char *text = new char[txt_size+1];
		memset(text,'\0',txt_size+1);
		memcpy(text, txt_buf, txt_size);		

		// convert codepage
		wstring info;
		if(lang == "CZ")
			info = char2wstringCP895(text);
		else
			info = char2wstring(text);
		delete[] text;

		txtInfo->SetValue(info);		
		break;
	}
}

// on paint icon
void FormUnits::OnPaintIcon(wxPaintEvent& event)
{
	// get unit selection:
	if(!lboxUnits->GetCount() || lboxUnits->GetSelection() < 0)
		return;
	auto sel = lboxUnits->GetSelection();
	auto* unit_rec = m_spell_data->units->GetUnit(sel);
	
	// render icon
	wxBitmap* bmp = unit_rec->icon_glyph->Render(icon_canvas->GetClientSize().GetWidth(),icon_canvas->GetClientSize().GetHeight());
	wxPaintDC pdc(icon_canvas);
	pdc.DrawBitmap(*bmp,wxPoint(0,0));
	delete bmp;
}

// on art selection change
void FormUnits::OnSelectArt(wxCommandEvent& event)
{
	art_canvas->Refresh();
}

// on paint unit art
void FormUnits::OnPaintArt(wxPaintEvent& event)
{
	// get art selection:
	if(!lboxArt->GetCount() || lboxArt->GetSelection() < 0)
		return;
	auto sel = lboxArt->GetSelection();
	string art_name = lboxArt->GetString(sel).ToStdString();
	
	// graphics content
	string art_lz_name = art_name + ".LZ";
	string art_pal_name = art_name + ".PAL";

	// load LZ graphics file
	uint8_t *lz_buf;
	int lz_size;
	if(m_spell_data->info->GetFile(art_lz_name.c_str(), &lz_buf, &lz_size))
		return;

	// decode LZ to RAW image
	uint8_t *data;
	int len;
	LZWexpand *lzw = new LZWexpand(1048576);
	if(lzw->Decode(lz_buf,&lz_buf[lz_size],&data,&len))
	{
		delete lzw;
		return;
	}
	delete lzw;
	uint8_t *end = &data[len];

	// load palette
	uint8_t *art_pal;
	int art_pal_size;
	if(m_spell_data->info->GetFile(art_pal_name.c_str(),&art_pal,&art_pal_size))
		return;
	auto terr = m_spell_data->GetTerrain(0);
	uint8_t pal[256][3];
	memcpy((void*)pal,(void*)terr->pal,256*3);
	memcpy((void*)&pal[0][0],(void*)art_pal,art_pal_size);

	
	// input data dimensions
	int x_size = 437;
	int y_size = len/x_size;

	// make redner buffer
	wxBitmap bmp(art_canvas->GetClientSize(),24);
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	if(x_size <= surf_x && y_size <= surf_y)
	{
		// image should fit to surface
		int x_ofs = (surf_x - x_size)/2;
		int y_ofs = (surf_y - y_size)/2;

		// render 24bit RGB data to raw bmp buffer
		uint8_t* buf = data;
		wxNativePixelData pdata(bmp);
		wxNativePixelData::Iterator p(pdata);
		for(int y = 0; y < surf_y; y++)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < surf_x; x++)
			{
				int is_visible = y >= y_ofs && y < y_ofs+y_size && x >= x_ofs && x < x_ofs+x_size && data < end;
				if(is_visible && *buf)
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
					if(is_visible)
						buf++;
				}
			}
			p.OffsetY(pdata,1);
		}		
	}
	// loose raw data
	delete[] data;

	// blit to screen
	wxPaintDC pdc(art_canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
	
}


