///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_gr_encode.h"
#include "sprites.h"
#include "other.h"
#include "LZ_spell.h"

#include <wx/rawbmp.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

#include <filesystem>
#include <string>

///////////////////////////////////////////////////////////////////////////

FormGResEncoder::FormGResEncoder(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->spell_data = spell_data;

	// === AUTO GENERATED START ===	

	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	m_menubar12 = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem(mmFile,wxID_MM_OPEN,wxString(wxT("Open resource")) + wxT('\t') + wxT("Ctrl+O"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmOpen);

	wxMenuItem* mmSave;
	mmSave = new wxMenuItem(mmFile,wxID_MM_SAVE,wxString(wxT("Save resource")) + wxT('\t') + wxT("Ctrl+S"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSave);

	wxMenuItem* mmExportAll;
	mmExportAll = new wxMenuItem(mmFile,wxID_MM_SAVE_ALL,wxString(wxT("Save All Resource")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExportAll);

	wxMenuItem* mmSavePal;
	mmSavePal = new wxMenuItem(mmFile,wxID_MM_SAVE_PAL,wxString(wxT("Save palette")) + wxT('\t') + wxT("Ctrl+P"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSavePal);

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(wxT("Exit")) + wxT('\t') + wxT("ESC"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	m_menubar12->Append(mmFile,wxT("File"));

	this->SetMenuBar(m_menubar12);

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SBAR);
	wxBoxSizer* bSizer98;
	bSizer98 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer(wxVERTICAL);

	m_staticText103 = new wxStaticText(this,wxID_ANY,wxT("Others sharing palette:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText103->Wrap(-1);
	bSizer104->Add(m_staticText103,0,wxLEFT|wxTOP,5);

	lboxList = new wxListBox(this,wxID_LB_LIST,wxDefaultPosition,wxDefaultSize,0,NULL,0|wxVSCROLL);
	lboxList->SetMinSize(wxSize(150,-1));

	bSizer104->Add(lboxList,1,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	bSizer101->Add(bSizer104,0,wxEXPAND,5);

	m_staticline37 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer101->Add(m_staticline37,0,wxEXPAND,5);

	wxBoxSizer* bSizer99;
	bSizer99 = new wxBoxSizer(wxVERTICAL);

	m_staticText99 = new wxStaticText(this,wxID_ANY,wxT("Source:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText99->Wrap(-1);
	bSizer99->Add(m_staticText99,0,wxLEFT|wxTOP,5);

	canvasSrc = new wxPanel(this,wxID_CANVAS_SRC,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer99->Add(canvasSrc,1,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	bSizer101->Add(bSizer99,1,wxEXPAND,5);

	m_staticline33 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer101->Add(m_staticline33,0,wxEXPAND,5);

	wxBoxSizer* bSizer100;
	bSizer100 = new wxBoxSizer(wxVERTICAL);

	m_staticText100 = new wxStaticText(this,wxID_ANY,wxT("Output:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText100->Wrap(-1);
	bSizer100->Add(m_staticText100,0,wxLEFT|wxTOP,5);

	canvasRes = new wxPanel(this,wxID_CANVAS_RES,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer100->Add(canvasRes,1,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	bSizer101->Add(bSizer100,1,wxEXPAND,5);


	bSizer98->Add(bSizer101,1,wxEXPAND,5);

	m_staticline34 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer98->Add(m_staticline34,0,wxBOTTOM|wxEXPAND,5);

	m_staticText102 = new wxStaticText(this,wxID_ANY,wxT("Palette:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText102->Wrap(-1);
	bSizer98->Add(m_staticText102,0,wxLEFT,5);

	palette = new wxPanel(this,wxID_PALETTE,wxDefaultPosition,wxSize(-1,-1),wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	palette->SetMaxSize(wxSize(-1,50));

	bSizer98->Add(palette,1,wxEXPAND | wxALL,5);

	m_staticline36 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer98->Add(m_staticline36,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer103;
	bSizer103 = new wxBoxSizer(wxVERTICAL);

	m_staticText101 = new wxStaticText(this,wxID_ANY,wxT("Min color distance for dithering:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText101->Wrap(-1);
	bSizer103->Add(m_staticText101,0,wxLEFT,5);

	slideMinDither = new wxSlider(this,wxID_SLIDE_MIN_DITHER,50,0,100,wxDefaultPosition,wxDefaultSize,wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS);
	slideMinDither->SetMinSize(wxSize(200,-1));

	bSizer103->Add(slideMinDither,1,wxALL|wxEXPAND,5);


	bSizer102->Add(bSizer103,0,wxEXPAND,5);

	m_staticline35 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer102->Add(m_staticline35,0,wxEXPAND|wxLEFT|wxRIGHT,5);


	bSizer102->Add(0,0,1,wxEXPAND,5);

	btnRegen = new wxButton(this,wxID_BTN_REGEN,wxT("Regenerate"),wxDefaultPosition,wxDefaultSize,0);
	bSizer102->Add(btnRegen,0,wxALL|wxEXPAND,5);

	btnRegenPalette = new wxButton(this,wxID_BTN_REGEN_PAL,wxT("Regenerate\nPalette"),wxDefaultPosition,wxDefaultSize,0);
	bSizer102->Add(btnRegenPalette,0,wxALL|wxEXPAND,5);


	bSizer98->Add(bSizer102,0,wxEXPAND,5);


	this->SetSizer(bSizer98);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);
		
	Bind(wxEVT_CLOSE_WINDOW, &FormGResEncoder::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU,&FormGResEncoder::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_MENU,&FormGResEncoder::OnOpenClick,this,wxID_MM_OPEN);
	Bind(wxEVT_MENU,&FormGResEncoder::OnSaveClick,this,wxID_MM_SAVE);
	Bind(wxEVT_MENU,&FormGResEncoder::OnSaveAllClick,this,wxID_MM_SAVE_ALL);
	Bind(wxEVT_MENU,&FormGResEncoder::OnSavePalClick,this,wxID_MM_SAVE_PAL);
	
	
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormGResEncoder::OnRegenClick,this,wxID_BTN_REGEN);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormGResEncoder::OnRegenPaletteClick,this,wxID_BTN_REGEN_PAL);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormGResEncoder::OnRegenClick,this,wxID_SLIDE_MIN_DITHER);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormGResEncoder::OnSelectClick,this,wxID_LB_LIST);

	
	// canvas stuff:	
	canvasSrc->SetDoubleBuffered(true);
	canvasSrc->Bind(wxEVT_PAINT,&FormGResEncoder::OnPaintSource,this,wxID_CANVAS_SRC);
	canvasRes->SetDoubleBuffered(true);
	canvasRes->Bind(wxEVT_PAINT,&FormGResEncoder::OnPaintResult,this,wxID_CANVAS_RES);
	palette->SetDoubleBuffered(true);
	palette->Bind(wxEVT_PAINT,&FormGResEncoder::OnPaintPalette,this,wxID_PALETTE);
	

	const int ss_w[] = {150,150,150,100,150,-1};
	sbar->SetFieldsCount(6,ss_w);
	
}

FormGResEncoder::~FormGResEncoder()
{
}

void FormGResEncoder::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormGResEncoder::OnCloseClick(wxCommandEvent& event)
{
	Close();
}



//------------------------------------------------------------------------------------------------------------------------
// Graphic resource meta file loader
//------------------------------------------------------------------------------------------------------------------------
FormGResEncoder::SpellGresInfo::SpellGresInfo()
{
	Clear();
}

// clear metadata
void FormGResEncoder::SpellGresInfo::Clear()
{
	path = L"";
	info_name = "";
	name = "";
	img_name = "";
	pal_name = "";
	colors_str = "";
	x_size = 0;
	y_size = 0;
	is_transparent = false;
}

// is loaded?
bool FormGResEncoder::SpellGresInfo::isLoaded()
{
	return(!path.empty() && !info_name.empty() && !name.empty() && !img_name.empty() && !pal_name.empty() && x_size && y_size);
}

// try load metadata from info file
int FormGResEncoder::SpellGresInfo::LoadInfo(std::wstring path)
{
	Clear();

	// try read meta file
	ifstreamext fr(path.c_str(),ios::in);
	if(!fr.is_open())
		return(1);
	auto info = fr.read_str();
	fr.close();

	this->path = path;
	info_name = std::filesystem::path(path).filename().string();

	name = info_get_string(info,"name");
	if(name.empty())
	{
		Clear();
		return(1);
	}
	img_name = info_get_string(info,"image");
	if(img_name.empty())
	{
		Clear();
		return(1);
	}
	pal_name = info_get_string(info,"palette");
	if(pal_name.empty())
	{
		Clear();
		return(1);
	}
	colors_str = info_get_string(info,"colors");
	if(colors_str.empty())
	{
		Clear();
		return(1);
	}
	auto x_size_str = info_get_string(info,"xsize");
	auto y_size_str = info_get_string(info,"ysize");
	if(x_size_str.empty() || y_size_str.empty())
	{
		Clear();
		return(1);
	}
	x_size = std::atoi(x_size_str.c_str());
	y_size = std::atoi(y_size_str.c_str());
	bool m_is_transparent = std::atoi(info_get_string(info,"transparent").c_str());
	return(0);
}

// load new resource
int FormGResEncoder::LoadResource(std::wstring path)
{
	m_source = wxBitmap();
	m_pal.Clear();
	m_gres.Clear();
	m_info.Clear();
	
	// try read meta file
	m_info.LoadInfo(path);

	// try load palette
	auto pal_path = std::filesystem::path(path).parent_path().append(m_info.pal_name).wstring();
	if(m_pal.LoadInfo(pal_path))
		return(1);
	//m_pal.m_name = m_info.pal_name;

	// try read image file
	auto image_path = std::filesystem::path(path).parent_path().append(m_info.img_name).wstring();
	if(!m_source.LoadFile(image_path,wxBITMAP_TYPE_PNG))
		return(1);

	SetStatusText(m_info.info_name,0);
	SetStatusText(m_info.name,1);
	SetStatusText(string_format("size = %d x %d",m_source.GetWidth(),m_source.GetHeight()),2);
	SetStatusText((m_info.is_transparent)?"transparent":"solid",3);
	SetStatusText(m_info.pal_name,4);
	SetStatusText(m_info.colors_str,5);
	

	return(0);
}

// open glyph resource
void FormGResEncoder::OnOpenClick(wxCommandEvent& event)
{
	// cleanup
	m_source = wxBitmap();
	m_pal.Clear();
	m_gres.Clear();
	m_info.Clear();
	lboxList->Clear();

	// force redraw when done
	canvasSrc->Refresh();
	canvasRes->Refresh();
	palette->Refresh();

	// show save dialog
	wxFileDialog openFileDialog(this,"Open glyph resource",spell_data->export_path,L"","Graphic resource file (*.png)|*.png|Graphic resource meta file (*.info)|*.info",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring dir = openFileDialog.GetDirectory().ToStdWstring();
	spell_data->export_path = dir;
	wstring path = wstring(openFileDialog.GetPath().ToStdWstring());	
	std::string ext = std::filesystem::path(path).extension().string();	
	if(iequals(ext,".info"))
	{
		// is info meta file
	}
	else if(iequals(ext,".png"))
	{
		// is image file, look for info meta file
		auto png_name = std::filesystem::path(path).filename().string();
		auto info_name = std::filesystem::path(path).stem().concat(".info").wstring();
		path = std::filesystem::path(dir).append(info_name).wstring();
		if(!std::filesystem::exists(path))
		{
			wxMessageDialog msg(NULL,string_format("Cannot find matching graphic resource meta file:\n%ls",path.c_str()),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}
		SpellGresInfo info;
		if(info.LoadInfo(path) || info.img_name.compare(png_name) != 0)			
		{
			wxMessageDialog msg(NULL,string_format("Cannot find matching graphic resource meta file:\n%ls",path.c_str()),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}
	}
	else
	{
		wxMessageDialog msg(NULL,string_format("Unknown file type:\n%ls!",path.c_str()),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
		msg.ShowModal();
		return;
	}	
	
	// load resource
	LoadResource(path);
	
	// load all other resources with shares palette
	lboxList->Clear();
	lboxList->Freeze();
	int select_id = -1;
	for(const auto& entry : std::filesystem::directory_iterator(std::filesystem::path(path).parent_path()))
	{
		auto name = entry.path().filename().string();
		if(!wildcmp("*.info",name.c_str()))
			continue;
		SpellGresInfo info;
		if(info.LoadInfo(entry.path().wstring()))
			continue;
		if(m_info.pal_name.compare(info.pal_name) != 0)
			continue;
		lboxList->Append(name);
		if(name.compare(m_info.info_name) == 0)
			select_id = lboxList->GetCount() - 1;
	}
	lboxList->Thaw();
	lboxList->Select(select_id);

	// force encoding
	wxCommandEvent evt;
	OnRegenClick(evt);

}

// select resoutce from list
void FormGResEncoder::OnSelectClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	auto id = lboxList->GetSelection();
	if(id < 0)
		return;
	
	auto path = std::filesystem::path(m_info.path).parent_path().append(lboxList->GetString(id).ToStdString());
	LoadResource(path);

	// force encoding
	wxCommandEvent evt;
	OnRegenClick(evt);
}

// regenerate result
void FormGResEncoder::OnRegenClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	// re-encode
	m_gres.Encode(m_source,m_info.name,&m_pal,slideMinDither->GetValue());

	canvasSrc->Refresh();
	canvasRes->Refresh();
	palette->Refresh();
}


// export glyph
void FormGResEncoder::OnSaveClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	std::string name = m_info.name;

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Export glyph resource"),spell_data->export_path,name,"LZ resource file (*.LZ)|*.LZ",
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());
	spell_data->export_path = saveFileDialog.GetDirectory().ToStdWstring();

	if(m_gres.Export(path))
	{
		wxMessageDialog msg(NULL,string_format("Exporting graphic resource failed!"),"Export glyph resource",wxOK| wxICON_EXCLAMATION);
		msg.ShowModal();
		return;
	}

}

// export all glyphs
void FormGResEncoder::OnSaveAllClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	// show save dialog
	wxDirDialog saveDirDialog(this,"Export multiple resources",spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring dir = wstring(saveDirDialog.GetPath().ToStdWstring());
	spell_data->export_path = dir;

	// rather ask for permission
	wxMessageDialog msg(NULL,"Files in the selected folder might be overwritten! Continue?","Export glyphs",wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if(msg.ShowModal() != wxID_YES)
		return;

	// for each listed resource
	for(auto &item: lboxList->GetStrings())
	{
		// item path
		auto info_path = std::filesystem::path(dir).append(item.ToStdString()).wstring();

		// try load
		if(LoadResource(info_path))
			continue;

		// force encode
		OnRegenClick(event);

		auto path = std::filesystem::path(dir).append(m_info.name).wstring();
		if(m_gres.Export(path))
			continue;
	}

}

// export glyph
void FormGResEncoder::OnSavePalClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	// directory dialog
	wxDirDialog saveDirDialog(this,"Export palette file(s)",spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring dir = wstring(saveDirDialog.GetPath().ToStdWstring());
	spell_data->export_path = dir;

	// rather ask for permission
	wxMessageDialog msg(NULL,"Files in the selected folder might be overwritten! Continue?","Export palette(s)",wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if(msg.ShowModal() != wxID_YES)
		return;
		
	if(m_pal.SaveChunks(dir))
	{
		wxMessageDialog msg(NULL,string_format("Exporting palette(s) failed!"),"Export glyph palette(s)",wxOK| wxICON_EXCLAMATION);
		msg.ShowModal();
		return;
	}

}


// regenerate palette
void FormGResEncoder::OnRegenPaletteClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	auto dir = std::filesystem::path(m_info.path).parent_path().wstring();
		
	
	std::vector<uint32_t> pixels;

	// for each listed resource
	for(auto& item: lboxList->GetStrings())
	{
		// item path
		auto info_path = std::filesystem::path(dir).append(item.ToStdString()).wstring();

		// try load
		if(LoadResource(info_path))
			continue;
		
		// collect source pixels
		int x_size = m_source.GetWidth();
		int y_size = m_source.GetHeight();		
		if(m_source.HasAlpha())
		{
			// scan 32bit RGBA bmp data to buffer
			typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
			PixelData data(m_source);
			PixelData::Iterator p(data);
			for(int y = 0; y < y_size; ++y)
			{
				uint32_t* scan = (uint32_t*)p.m_ptr;
				for(int x = 0; x < x_size; x++)
					pixels.push_back(*scan++);				
				p.OffsetY(data,1);
			}
		}
		else
		{
			// scan 24bit RGB bmp data to buffer
			wxNativePixelData data(m_source);
			wxNativePixelData::Iterator p(data);
			int depth = m_source.GetDepth();
			if(depth != 24)
				continue;
			for(int y = 0; y < y_size; ++y)
			{
				uint8_t* scan = p.m_ptr;
				for(int x = 0; x < x_size; x++)
				{
					union{
						uint8_t db[4];
						uint32_t dw;						
					}pix;
					pix.db[0] = *scan++;
					pix.db[1] = *scan++;
					pix.db[2] = *scan++;
					pix.db[3] = 0xFF;
					pixels.push_back(pix.dw);
				}
				p.OffsetY(data,1);
			}
		}
	}

	// sort pixels
	std::sort(pixels.begin(),pixels.end());

	// find uniques
	std::vector<uint32_t> uniques(pixels.size());
	auto uniques_end = std::unique_copy(pixels.begin(),pixels.end(),uniques.begin());
	uniques.resize(uniques_end - uniques.begin());

	// count uniques
	class UniquePixel{
		public:
		uint32_t pix;
		uint32_t count;
	};
	std::vector<UniquePixel> list;
	for(auto &pix: uniques)
	{
		UniquePixel item;
		item.pix = pix;
		item.count = std::count(pixels.begin(), pixels.end(), pix);
		list.push_back(item);
	}

	// sort uniques by counts
	std::sort(list.begin(),list.end(),[](UniquePixel &a,UniquePixel &b) {return a.count > b.count;});

	m_pal.m_used[0] = 0;
	auto max_count = std::count(m_pal.m_used.begin(), m_pal.m_used.end(), 1);
	
	// place black/transparent
	m_pal.m_pal.assign(3*256,0);
	
	// assign new colors
	auto used = m_pal.m_used;
	for(int k = 0; k < max_count; k++)
	{
		if(k >= list.size())
			break;
		auto item = std::find(used.begin(), used.end(),1);
		if(item == used.end())
			break;
		union{
			uint8_t db[4];
			uint32_t dw;
		} color;
		color.dw = list[k].pix;
		int cid = item - used.begin() + 1;
		m_pal.m_pal[cid*3 + 0] = color.db[2];
		m_pal.m_pal[cid*3 + 1] = color.db[1];
		m_pal.m_pal[cid*3 + 2] = color.db[0];
		*item = 0;
	}

	// refresh
	OnRegenClick(event);
}



// render source
void FormGResEncoder::OnPaintSource(wxPaintEvent& event)
{
	
	int x_surf = canvasSrc->GetClientSize().GetWidth();
	int y_surf = canvasSrc->GetClientSize().GetHeight();
	int x_size = m_source.GetWidth();
	int y_size = m_source.GetHeight();
	
	if(x_size && y_size && x_surf >= x_size && y_surf >= y_size)
	{
		// source fits canvas
		
		// blit to screen
		wxPaintDC pdc(canvasSrc);

		// make background		
		int checker_step = 32;
		for(int y = 0; y < y_surf; y += checker_step)
		{
			for(int x = 0; x < x_surf; x += checker_step)
			{
				bool tile = ((x/checker_step)^(y/checker_step)) & 1;
				auto color = wxColor(tile?0x00888888:0x00AAAAAA);
				pdc.SetBrush(wxBrush(color,wxBRUSHSTYLE_SOLID));
				pdc.SetPen(wxPen(color,1,wxPENSTYLE_SOLID));					
				auto to = wxPoint(min(x + checker_step,x_surf - 1),min(y + checker_step,y_surf - 1));
				pdc.DrawRectangle(wxRect(wxPoint(x,y),to));
			}
		}

		// render image
		int x_ofs = (x_surf - x_size)/2;
		int y_ofs = (y_surf - y_size)/2;
		pdc.DrawBitmap(m_source,wxPoint(x_ofs,y_ofs));
	}

	
}


// render result
void FormGResEncoder::OnPaintResult(wxPaintEvent& event)
{	
	if(!m_gres.pixels.empty())
	{
		int surf_x = canvasRes->GetClientSize().GetWidth();
		int surf_y = canvasRes->GetClientSize().GetHeight();
		auto bmp = m_gres.Render(surf_x,surf_y);

		// blit to screen
		wxPaintDC pdc(canvasRes);
		pdc.DrawBitmap(*bmp,wxPoint(0,0));
		delete bmp;
	}	
}

// render palette preview
void FormGResEncoder::OnPaintPalette(wxPaintEvent& event)
{
	// render palette
	wxBitmap bmp(palette->GetClientSize(),24);
	m_pal.Render(bmp);
	
	// blit to screen
	wxPaintDC pdc(palette);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}


