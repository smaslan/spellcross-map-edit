///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_gr_view.h"
#include "sprites.h"
#include "other.h"
#include "LZ_spell.h"

#include <wx/rawbmp.h>

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

FormGResView::FormGResView(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->spell_data = spell_data;

	// === AUTO GENERATED START ===	

	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	m_menubar5 = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmClose;
	mmClose = new wxMenuItem(mmFile,wxID_MM_CLOSE,wxString(wxT("Close")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmClose);

	m_menubar5->Append(mmFile,wxT("File"));

	this->SetMenuBar(m_menubar5);

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SB);
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer(wxVERTICAL);

	m_staticText31 = new wxStaticText(this,wxID_ANY,wxT("Files:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText31->Wrap(-1);
	bSizer27->Add(m_staticText31,0,wxTOP|wxRIGHT|wxLEFT,5);

	lboxFiles = new wxListBox(this,wxID_LB_FILES,wxDefaultPosition,wxSize(150,-1),0,NULL,0|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer27->Add(lboxFiles,1,wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText32 = new wxStaticText(this,wxID_ANY,wxT("Width:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText32->Wrap(-1);
	bSizer27->Add(m_staticText32,0,wxTOP|wxRIGHT|wxLEFT,5);

	spinWidth = new wxSpinCtrl(this,wxID_SPIN_W,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	bSizer27->Add(spinWidth,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer26->Add(bSizer27,0,wxEXPAND,5);

	m_staticline8 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer26->Add(m_staticline8,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	canvas = new wxPanel(this,wxID_CANVAS,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer26->Add(canvas,1,wxALL|wxEXPAND,5);


	this->SetSizer(bSizer26);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===
	
	// close
	Bind(wxEVT_CLOSE_WINDOW, &FormGResView::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU,&FormGResView::OnCloseClick,this,wxID_MM_CLOSE);
	
	// canvas stuff:
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormGResView::OnPaintCanvas,this);

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormGResView::OnSelectFile,this,wxID_LB_FILES);
	Bind(wxEVT_SPINCTRL,&FormGResView::OnWidthChange,this,wxID_SPIN_W);

	// try to load common.fs
	wstring common_path = spell_data->spell_data_root + L"\\common.fs";
	common = new FSarchive(common_path);

	// fill list
	//LoadFileList();
	LoadGrpList();

	spinWidth->SetMin(1);
	spinWidth->SetMax(1024);
	spinWidth->SetValue(16);
}

FormGResView::~FormGResView()
{
}

void FormGResView::OnClose(wxCloseEvent& ev)
{
	delete common;
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormGResView::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

// fill common file list
void FormGResView::LoadFileList()
{	
	lboxFiles->Freeze();
	lboxFiles->Clear();
	for(int k = 0; k < common->Count(); k++)
	{
		std::filesystem::path name = string(common->GetFileName(k));

		if( name.extension().compare(".LZ") == 0 ||
			name.extension().compare(".LZ0") == 0 ||
			name.extension().compare(".ICO") == 0 ||
			name.extension().compare(".BTN") == 0)
		{
			lboxFiles->Append(name.string());
		}
	}
	lboxFiles->Thaw();
}

// fill list of loaded graphics
void FormGResView::LoadGrpList()
{
	lboxFiles->Freeze();
	lboxFiles->Clear();
	for(int k = 0; k < spell_data->gres.Count(); k++)
	{
		auto *res = spell_data->gres.GetResource(k);
		lboxFiles->Append(res->name);
	}
	lboxFiles->Thaw();
}

// file selected
void FormGResView::OnSelectFile(wxCommandEvent& event)
{
	canvas->Refresh();
}

// change width
void FormGResView::OnWidthChange(wxSpinEvent& event)
{
	canvas->Refresh();
}


// render preview
void FormGResView::OnPaintCanvas(wxPaintEvent& event)
{	
	// make render buffer
	wxBitmap bmp(canvas->GetClientSize(),24);
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();	

	// select file
	if(lboxFiles->GetSelection() >= 0 && lboxFiles->GetCount())
	{
		std::filesystem::path name = lboxFiles->GetString(lboxFiles->GetSelection()).ToStdString();

		// load file data
		uint8_t *data;
		int len;
		int is_lzw = false;
		SpellGraphicItem *grpi;
		if((grpi = spell_data->gres.GetResource(name.string().c_str())) != NULL)
		{
			// loaded resource:
			data = grpi->GetPixels();
			len = grpi->x_size*grpi->y_size;
			spinWidth->SetValue(grpi->x_size);
		}
		else if(name.extension().compare(".LZ") == 0 || name.extension().compare(".LZ0") == 0)
		{						
			// compressed: try decompress
			is_lzw = true;

			// get lz file data
			uint8_t *lzdata;
			int lzlen;
			common->GetFile(name.string().c_str(), &lzdata, &lzlen);

			LZWexpand lzw = LZWexpand(1048576);
			lzw.Decode(lzdata, &lzdata[lzlen], &data, &len);
		}
		else
		{
			// raw data: load as is
			common->GetFile(name.string().c_str(),&data,&len);
		}
		uint8_t *end = &data[len];

		// load palette
		auto terr = spell_data->GetTerrain(0);
		uint8_t pal[256][3];
		memcpy((void*)pal, (void*)terr->pal, 256*3);

		// try to fetch specific palette
		uint8_t *pdata;
		int plen;
		if(!common->GetFile(name.stem().concat(".PAL").string().c_str(), &pdata, &plen) && plen == 768)
		{
			memcpy((void*)pal,(void*)pdata,256*3);
		}

		// input data dimensions
		int x_size = spinWidth->GetValue();
		int y_size = len/x_size + ((len%x_size)?1:0);

		if(x_size <= surf_x && y_size <= surf_y)
		{
			// image should fit to surface
			int x_ofs = (surf_x - x_size)/2;
			int y_ofs = (surf_y - y_size)/2;

			// render 24bit RGB data to raw bmp buffer
			uint8_t *buf = data;
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

		// loose LZW decompressed data
		if(is_lzw)
			delete[] data;
	}
	
	// blit to screen
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));

	
}


