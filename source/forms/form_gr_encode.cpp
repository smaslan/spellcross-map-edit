///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_gr_encode.h"
#include "sprites.h"
#include "other.h"
#include "wx_other.h"
#include "LZ_spell.h"
#include "graphics.h"

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
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormGResEncoder' on 2026-09-16 21:16:37
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_menubar12 = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem( mmFile, wxID_MM_OPEN, wxString( wxT("Open resource") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOpen );
	
	wxMenuItem* mmSave;
	mmSave = new wxMenuItem( mmFile, wxID_MM_SAVE, wxString( wxT("Save resource") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSave );
	
	wxMenuItem* mmExportAll;
	mmExportAll = new wxMenuItem( mmFile, wxID_MM_SAVE_ALL, wxString( wxT("Save All Resource") ) + wxT('\t') + wxT("Ctrl+Shift+S"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExportAll );
	
	wxMenuItem* mmSavePal;
	mmSavePal = new wxMenuItem( mmFile, wxID_MM_SAVE_PAL, wxString( wxT("Save palette") ) + wxT('\t') + wxT("Ctrl+P"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmSavePal );
	
	mmFile->AppendSeparator();
	
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( mmFile, wxID_MM_EXIT, wxString( wxT("Exit") ) + wxT('\t') + wxT("ESC"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmExit );
	
	m_menubar12->Append( mmFile, wxT("File") );
	
	this->SetMenuBar( m_menubar12 );
	
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	wxBoxSizer* bSizer98;
	bSizer98 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText103 = new wxStaticText( this, wxID_ANY, wxT("Others sharing palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText103->Wrap( -1 );
	bSizer104->Add( m_staticText103, 0, wxLEFT|wxTOP, 5 );
	
	lboxList = new wxListBox( this, wxID_LB_LIST, wxDefaultPosition, wxDefaultSize, 0, NULL, 0|wxVSCROLL );
	lboxList->SetMinSize( wxSize( 150,-1 ) );
	
	bSizer104->Add( lboxList, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer101->Add( bSizer104, 0, wxEXPAND, 5 );
	
	m_staticline37 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer101->Add( m_staticline37, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer99;
	bSizer99 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText99 = new wxStaticText( this, wxID_ANY, wxT("Source:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText99->Wrap( -1 );
	bSizer99->Add( m_staticText99, 0, wxLEFT|wxTOP, 5 );
	
	canvasSrc = new wxPanel( this, wxID_CANVAS_SRC, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer99->Add( canvasSrc, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer101->Add( bSizer99, 1, wxEXPAND, 5 );
	
	m_staticline33 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer101->Add( m_staticline33, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer100;
	bSizer100 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText100 = new wxStaticText( this, wxID_ANY, wxT("Output:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText100->Wrap( -1 );
	bSizer100->Add( m_staticText100, 0, wxLEFT|wxTOP, 5 );
	
	canvasRes = new wxPanel( this, wxID_CANVAS_RES, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	bSizer100->Add( canvasRes, 1, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	
	bSizer101->Add( bSizer100, 1, wxEXPAND, 5 );
	
	
	bSizer98->Add( bSizer101, 1, wxEXPAND, 5 );
	
	m_staticline34 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer98->Add( m_staticline34, 0, wxBOTTOM|wxEXPAND, 5 );
	
	m_staticText102 = new wxStaticText( this, wxID_ANY, wxT("Palette:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText102->Wrap( -1 );
	bSizer98->Add( m_staticText102, 0, wxLEFT, 5 );
	
	palette = new wxPanel( this, wxID_PALETTE, wxDefaultPosition, wxSize( -1,-1 ), wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	palette->SetMaxSize( wxSize( -1,50 ) );
	
	bSizer98->Add( palette, 1, wxEXPAND | wxALL, 5 );
	
	m_staticline36 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer98->Add( m_staticline36, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer102;
	bSizer102 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer103;
	bSizer103 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText101 = new wxStaticText( this, wxID_ANY, wxT("Min color distance for dithering:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText101->Wrap( -1 );
	bSizer103->Add( m_staticText101, 0, wxLEFT, 5 );
	
	slideMinDither = new wxSlider( this, wxID_SLIDE_MIN_DITHER, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS|wxSL_HORIZONTAL|wxSL_LABELS );
	slideMinDither->SetMinSize( wxSize( 200,-1 ) );
	
	bSizer103->Add( slideMinDither, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer102->Add( bSizer103, 0, wxEXPAND, 5 );
	
	m_staticline35 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer102->Add( m_staticline35, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* bSizer114;
	bSizer114 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText115 = new wxStaticText( this, wxID_ANY, wxT("Extra x-offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText115->Wrap( -1 );
	bSizer114->Add( m_staticText115, 0, wxRIGHT|wxLEFT, 5 );
	
	spinExtraXoffset = new wxSpinCtrl( this, wxID_SPIN_EX_OFS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -200, 200, 0 );
	spinExtraXoffset->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer114->Add( spinExtraXoffset, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	m_staticText116 = new wxStaticText( this, wxID_ANY, wxT("Extra y-offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText116->Wrap( -1 );
	bSizer114->Add( m_staticText116, 0, wxRIGHT|wxLEFT, 5 );
	
	spinExtraYoffset = new wxSpinCtrl( this, wxID_SPIN_EY_OFS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -200, 200, 0 );
	spinExtraYoffset->SetMinSize( wxSize( 100,-1 ) );
	
	bSizer114->Add( spinExtraYoffset, 0, wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer102->Add( bSizer114, 0, wxEXPAND, 5 );
	
	m_staticline42 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer102->Add( m_staticline42, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer128;
	bSizer128 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText136 = new wxStaticText( this, wxID_ANY, wxT("Resource meta data:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText136->Wrap( -1 );
	bSizer128->Add( m_staticText136, 0, wxRIGHT|wxLEFT, 5 );
	
	pgProperties = new wxPropertyGrid(this, wxID_PG_PROPS, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	pgProperties->SetMinSize( wxSize( 500,-1 ) );
	
	bSizer128->Add( pgProperties, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer102->Add( bSizer128, 1, wxEXPAND, 5 );
	
	
	bSizer102->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnRegen = new wxButton( this, wxID_BTN_REGEN, wxT("Regenerate"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer102->Add( btnRegen, 0, wxALL|wxEXPAND, 5 );
	
	btnRegenPalette = new wxButton( this, wxID_BTN_REGEN_PAL, wxT("Regenerate\nPalette"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer102->Add( btnRegenPalette, 0, wxALL|wxEXPAND, 5 );
	
	
	bSizer98->Add( bSizer102, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer98 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormGResEncoder' on 2026-09-16 21:16:37
	// === AUTO GENERATED END ===
	m_thread_active = 0;

	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	AssignSVGresourceToMenu(mmOpen, "IDR_OPEN3");
	AssignSVGresourceToMenu(mmSave,"IDR_SAVE");
	AssignSVGresourceToMenu(mmSavePal,"IDR_SAVE");
	AssignSVGresourceToMenu(mmExportAll,"IDR_SAVE_ALL");
	AssignSVGresourceToMenu(mmExit,"IDR_CLOSE");
	
		
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
	Bind(wxEVT_PG_CHANGED,&FormGResEncoder::OnPropChange,this,wxID_PG_PROPS);

	Bind(wxEVT_THREAD,&FormGResEncoder::OnThreadEvent,this,wxID_PROC_THREAD);	
	
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







// encoder thread constructor
ProcTh::ProcTh(wxFrame* parent,Params& config)
{
	m_parent = parent;
	m_config = config;
}

// encoder thread entry point
wxThread::ExitCode ProcTh::Entry()
{
	
	while(true)
	{
		// try get task
		std::filesystem::path path;
		m_config.mutex->lock();
		if(!m_config.list->empty())
		{
			path = m_config.list->back();
			m_config.list->pop_back();
		}
		m_config.mutex->unlock();
		// done?
		if(path.empty())
			break;

		// try read meta file
		SpellGresInfo info;
		if(info.LoadInfo(path))
		{
			m_config.mutex->lock();
			if(!m_config.list->empty())
				m_config.failed_list->push_back(path.filename().string());
			m_config.mutex->unlock();			
			continue;
		}

		// try load palette
		SpellPalette pal;
		auto pal_path = std::filesystem::path(path).parent_path().append(info.pal_name).wstring();
		if(pal.LoadInfo(pal_path))
		{
			m_config.mutex->lock();
			if(!m_config.list->empty())
				m_config.failed_list->push_back(path.filename().string());
			m_config.mutex->unlock();
			continue;
		}

		// for each frame in case of animations:
		std::vector<std::unique_ptr<SpellGraphicItem>> gres_list;
		std::vector<std::string> img_names = {info.img_name};
		if(info.isPNM())
			img_names = info.img_names;
		for(auto &img_name: img_names)
		{		
			int frame_id = &img_name - img_names.data();
			
			// try read image file
			wxBitmap source;
			auto image_path = std::filesystem::path(path).parent_path().append(img_name).wstring();
			if(!source.LoadFile(image_path,wxBITMAP_TYPE_PNG))
			{
				m_config.mutex->lock();
				if(!m_config.list->empty())
					m_config.failed_list->push_back(path.filename().string());
				m_config.mutex->unlock();
				continue;
			}

			// show status
			std::vector<std::string> status;
			if(info.isPNM())
				status.push_back(string_format("%s (frame %d)",info.info_name.c_str(),frame_id));
			else
				status.push_back(info.info_name);
			status.push_back(string_format("size = %d x %d",source.GetWidth(),source.GetHeight()));
			status.push_back((info.is_transparent)?"transparent":"solid");
			status.push_back(info.pal_name);
			status.push_back(info.colors_str);
			SetStatusCallback(status, &source);

			// encode image
			int* shadow_color = NULL;
			if(info.isUnitsFSU())
				shadow_color = info.shadow_color;			
			gres_list.push_back(std::make_unique<SpellGraphicItem>());
			auto &gres = gres_list.back();
			gres->Encode(source,info.name,&pal,info.gamma,info.x_size,info.y_size,(wxImageResizeQuality)info.resampling,m_config.dither_randomize,info.alpha_threshold,shadow_color,0xFD);

			// just collect frames for animations
			if(info.isPNM() && frame_id < info.img_names.size() - 1)
				continue;

			// auto y-offset for trees?
			int y_offset = info.y_offset;
			if(info.is_tree_auto_y_offset)
				y_offset -= (gres->y_size - 32);

			// auto centering to given width?
			int x_offset = info.x_offset;
			if(info.center_width)
				x_offset += (info.center_width - gres->x_size)/2;

			// save to file
			auto save_path = std::filesystem::path(m_config.target_dir).append(info.name).wstring();
			if(info.isPNM())
			{
				// PNM animation: all frames encoded
				
				auto err = AnimPNM::Encode(save_path, gres_list);
				if(err)
				{
					m_config.mutex->lock();
					if(!m_config.list->empty())
						m_config.failed_list->push_back(path.filename().string());
					m_config.mutex->unlock();
					continue;
				}

				/*std::vector<uint8_t> pnm_data;
				loaddata(save_path,pnm_data);

				AnimPNM pnm;
				pnm.Decode(pnm_data.data(), "test.pnm");

				
				auto frame = pnm.frames[0];

				int x_size = pnm.x_max;
				int y_size = pnm.y_max;
				int y_ref = 0;
				if(pnm.y_min < 0)
				{
					y_ref = -pnm.y_min;
					y_size += (-pnm.y_min);
				}				
				
				
				// make indexed buffer
				std::vector<uint8_t> buf(x_size*y_size,0);
				uint8_t* buf_end = buf.data() + buf.size();

				// render tile
				frame->Render(buf.data(),buf_end,0,y_ref,x_size);

				// render 24bit RGB data to raw bmp buffer
				auto ppal = (uint8_t(*)[3])pal.m_pal.data();

				// render with alpha channel to bitmap
				wxBitmap bmp(x_size,y_size,32);
				bmp.UseAlpha(true);
				typedef wxPixelData<wxBitmap,wxAlphaPixelFormat> PixelData;
				PixelData data(bmp);
				PixelData::Iterator p(data);
				for(int y = 0; y < y_size; ++y)
				{
					uint8_t* scan = p.m_ptr;
					uint8_t* src = &buf[y*x_size];
					for(int x = 0; x < x_size; x++)
					{
						*scan++ = ppal[*src][2];
						*scan++ = ppal[*src][1];
						*scan++ = ppal[*src][0];
						*scan++ = (*src)?255:0;
						src++;
					}
					p.OffsetY(data,1);
				}

				SetStatusCallback(status,&bmp);*/

			}
			else if(info.isUnitsFSU())
			{
				// UNITS.FSU sprite
				auto x_ofs = m_config.x_offset;
				auto y_ofs = info.y_offset + m_config.y_offset;
				auto err = FSU_sprite::SaveSprite(save_path,gres->pixels,gres->x_size,x_ofs,y_ofs,0xFD);
				if(err)
				{
					m_config.mutex->lock();
					if(!m_config.list->empty())
						m_config.failed_list->push_back(path.filename().string());
					m_config.mutex->unlock();
					continue;
				}
			}
			else if(info.isDTA())
			{
				// sprite DTA

				

				auto err = Sprite::SaveSprite(save_path, gres->pixels, gres->x_size, x_offset, y_offset, info.land_type);
				if(err)
				{
					m_config.mutex->lock();
					if(!m_config.list->empty())
						m_config.failed_list->push_back(path.filename().string());
					m_config.mutex->unlock();
					continue;
				}
			}
			else
			{
				// general graphic resource
				if(gres->Export(save_path))
				{
					m_config.mutex->lock();
					if(!m_config.list->empty())
						m_config.failed_list->push_back(path.filename().string());
					m_config.mutex->unlock();
					continue;
				}
			}

		}
	}

	// signalize we are done
	auto evt = new wxThreadEvent(wxEVT_THREAD,FormGResEncoder::wxID_PROC_THREAD);
	evt->SetInt(Event::DONE);
	auto handler = m_parent->GetEventHandler();
	handler->QueueEvent(evt);

	return(0);
}

// status bar callback
void ProcTh::SetStatusCallback(std::vector<std::string>& info, wxBitmap *src)
{
	auto evt = new wxThreadEvent(wxEVT_THREAD,FormGResEncoder::wxID_PROC_THREAD);
	evt->SetInt(Event::STATUS);	
	auto payload = new Status();
	payload->status = info;
	if(src)
		payload->src = *src;
	evt->SetPayload(payload);
	auto handler = m_parent->GetEventHandler();
	handler->QueueEvent(evt);
}




// mod builder event
void FormGResEncoder::OnThreadEvent(wxThreadEvent& event)
{
	auto what = event.GetInt();
	if(what == ProcTh::Event::DONE)
	{
		// processing done		
		m_thread_active--;
		if(!m_thread_active)
		{
			std::string msg = "Encoding done!";
			if(!m_task_failed_list.empty())
			{
				msg += " Encoding of following resources failed:\n";
				for(auto &item: m_task_failed_list)
					msg += string_format(" %s\n",item.c_str());
			}
			wxMessageBox(msg, "Encoding graphics resources");
		}
	}
	else if(what == ProcTh::Event::STATUS)
	{
		// status bar		
		auto *data = event.GetPayload<ProcTh::Status*>();
		if(!data)
			return;
		for(auto k = 0; k < data->status.size(); k++)
			sbar->SetStatusText(data->status.at(k), k);

		if(data->src.IsOk())
		{
			m_source_mutex.lock();
			m_source = data->src;
			m_source_mutex.unlock();
		}
		canvasSrc->Refresh();

		delete data;
	}

}




//------------------------------------------------------------------------------------------------------------------------
// Graphic resource meta file loader
//------------------------------------------------------------------------------------------------------------------------
const std::map<int,std::string> SpellGresInfo::c_resampling = {
	{wxIMAGE_QUALITY_NEAREST,"Nearest"},
	{wxIMAGE_QUALITY_BILINEAR,"Bilinear"},
	{wxIMAGE_QUALITY_BICUBIC,"Bicubic"}
};

SpellGresInfo::SpellGresInfo()
{
	Clear();
}

// clear metadata
void SpellGresInfo::Clear()
{
	path = "";
	info_name = "";
	name = "";
	raw_name = "";
	img_name = "";
	raw_img_name = "";
	pal_name = "";
	colors_str = "";
	x_size = 0;
	y_size = 0;
	is_transparent = false;
	y_offset = 0;
	format = "";
	shadow_color[0] = -1;
	shadow_color[1] = -1;
	shadow_color[2] = -1;
	gamma = 1.0;
}

// is loaded?
bool SpellGresInfo::isLoaded()
{
	return(!path.empty() && !info_name.empty() && !name.empty() && !img_name.empty() && !pal_name.empty());
}

// try load metadata from info file
int SpellGresInfo::LoadInfo(std::filesystem::path path)
{
	Clear();

	// try read meta file
	std::string infostr;
	if(loadstr(path, infostr))
		return(1);
	auto info = get_text_lines(infostr, true);
	
	this->path = path;
	info_name = std::filesystem::path(path).filename().string();
	std::string stem = path.stem().string();

	// resource file name
	raw_name = name = info_get_string(info,"name");
	if(name.empty())
	{
		Clear();
		return(1);
	}
	name = strrep(name,"*",stem);

	// resource image name
	raw_img_name = img_name = info_get_string(info,"image");
	if(img_name.empty())
	{
		Clear();
		return(1);
	}
	img_name = strrep(img_name,"*",stem);

	// palette name (palinfo)
	pal_name = info_get_string(info,"palette");
	if(pal_name.empty())
	{
		Clear();
		return(1);
	}

	// used colors list
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
	is_transparent = std::atoi(info_get_string(info,"transparent").c_str());
	format = info_get_string(info,"format");

	auto x_offset_str = info_get_string(info,"xoffset");
	x_offset = std::atoi(x_offset_str.c_str());
	auto y_offset_str = info_get_string(info,"yoffset");
	y_offset = std::atoi(y_offset_str.c_str());

	// resampling mode
	auto res_str = info_get_string(info, "resampling","nearest");	
	auto res_it = std::ranges::find_if(c_resampling,[res_str](const auto &pair){return(iequals(pair.second,res_str));});
	if(res_it == c_resampling.end())
	{
		Clear();
		return(1);
	}
	resampling = res_it->first;

	// preprocessing gamma correction
	gamma = info_get_real(info,"gamma",1.0);

	// non-zero to enable image centering to given width
	center_width = info_get_int(info,"center_to_width",0);

	// enable automatic y-offset for trees (y_offset is then just extra relative offset to automatic value)
	is_tree_auto_y_offset = !!info_get_int(info,"tree_auto_y_offset",0);
	
	// sprite land type code
	auto landtype_str = info_get_string(info,"landtype");
	land_type = std::atoi(landtype_str.c_str());

	// alpha channel threshold
	alpha_threshold = info_get_int(info,"alpha_threshold",128);

	auto shadow_color_str = info_get_string(info,"shadow_color");
	auto shadow_colors_list = get_text_lines(shadow_color_str,true,',');
	if(!shadow_color_str.empty() && shadow_colors_list.size() != 3)
	{
		Clear();
		return(1);
	}
	if(!shadow_color_str.empty())
		for(auto &colstr: shadow_colors_list)
			shadow_color[&colstr - shadow_colors_list.data()] = std::atoi(colstr.c_str());

	// try read image names (optional for PNM format)
	img_names = info_get_text_vector(info, "images");

	return(0);
}

// try save metadata to info file
int SpellGresInfo::SaveInfo(std::filesystem::path path)
{
	if(path.empty())
		path = this->path;

	std::string info = "// Spellcross graphics resource meta data file (autogenerated by Spellcross Map Editor)\n";

	info += info_make_string("name",raw_name);
	info += info_make_string("image",raw_img_name);
	info += info_make_string("format",format);
	
	info += info_make_int("xsize",x_size);
	info += info_make_int("ysize",y_size);
	info += info_make_int("xoffset",x_offset);
	info += info_make_int("yoffset",y_offset);

	auto res_it = c_resampling.find(resampling);
	if(res_it == c_resampling.end())
		return(1);
	info += info_make_string("resampling",res_it->second);

	info += info_make_real("gamma",gamma);

	info += info_make_int("center_to_width",center_width);
	info += info_make_int("tree_auto_y_offset",!!is_tree_auto_y_offset);
	info += info_make_int("landtype",land_type);
	info += info_make_int("transparent",!!is_transparent);
	info += info_make_int("alpha_threshold",alpha_threshold);

	std::vector<int> sh_colors(std::begin(shadow_color),std::end(shadow_color));	
	info += info_make_string("shadow_color",merge_vector(sh_colors,","));

	info += info_make_string("palette",pal_name);
	info += info_make_string("colors",colors_str);

	if(!img_names.empty())
	{
		info += info_make_text_vector("images",img_names);
	}

	// try save
	return(savestr(path,info));
}

// load new resource
int FormGResEncoder::LoadResource(std::filesystem::path path,int frame_id)
{
	m_source = wxBitmap();
	pgProperties->Clear();
	m_pal.Clear();
	m_gres.Clear();
	m_info.Clear();
	
	
	// try read meta file
	m_info.LoadInfo(path);

	// try load palette
	auto pal_path = std::filesystem::path(path).parent_path().append(m_info.pal_name);
	if(m_pal.LoadInfo(pal_path))
		return(1);
	//m_pal.m_name = m_info.pal_name;

	// pick frame of animation?
	bool is_pnm = m_info.isPNM();
	if(is_pnm && (frame_id < 0 || frame_id >= m_info.img_names.size()))
		return(1);
	auto img_name = m_info.img_name;
	if(is_pnm && frame_id >= 0)
		img_name = m_info.img_names[frame_id];

	// try read image file
	auto image_path = std::filesystem::path(path).parent_path().append(img_name).wstring();
	if(!m_source.LoadFile(image_path,wxBITMAP_TYPE_PNG))
		return(1);

	SetStatusText(m_info.info_name,0);
	SetStatusText(m_info.name,1);
	SetStatusText(string_format("size = %d x %d",m_source.GetWidth(),m_source.GetHeight()),2);
	SetStatusText((m_info.is_transparent)?"transparent":"solid",3);
	SetStatusText(m_info.pal_name,4);
	SetStatusText(m_info.colors_str,5);

	pgProperties->Freeze();
	pgProperties->Clear();
	pgProperties->Append(new wxStringPropertyExt(wxT("Resource name"),wxT(""),&m_info.raw_name));
	pgProperties->Append(new wxStringPropertyExt(wxT("Image name"),wxT(""),&m_info.raw_img_name));
	pgProperties->Append(new wxStringPropertyExt(wxT("Palette name"),wxT(""),&m_info.pal_name));
	pgProperties->Append(new wxStringPropertyExt(wxT("Format"),wxT(""),&m_info.format));
	pgProperties->Append(new wxIntPropertyExt(wxT("x-size"),wxT(""),&m_info.x_size,-1));
	pgProperties->Append(new wxIntPropertyExt(wxT("y-size"),wxT(""),&m_info.y_size,-1));
	pgProperties->Append(new wxIntPropertyExt(wxT("x-offset"),wxT(""),&m_info.x_offset));
	pgProperties->Append(new wxIntPropertyExt(wxT("y-offset"),wxT(""),&m_info.y_offset));
	pgProperties->Append(new wxEnumPropertyExt(wxT("Resampling mode"),wxT(""),MapToPGenumChoices(SpellGresInfo::c_resampling),&m_info.resampling));
	pgProperties->Append(new wxRealPropertyExt(wxT("Gamma correction"),wxT(""),&m_info.gamma,2,0.1,3.0));
	pgProperties->Append(new wxIntPropertyExt(wxT("Alpha threshold"),wxT(""),&m_info.alpha_threshold,0,255));
	pgProperties->Append(new wxIntPropertyExt(wxT("Centering width"),wxT(""),&m_info.center_width,-1));
	pgProperties->Append(new wxBoolPropertyExt(wxT("Auto y-offset for tree"),wxT(""),&m_info.is_tree_auto_y_offset));
	pgProperties->Append(new wxIntPropertyExt(wxT("Land type"),wxT(""),&m_info.land_type,0,13));
	pgProperties->Append(new wxBoolPropertyExt(wxT("Transparent"),wxT(""),&m_info.is_transparent));
	pgProperties->Append(new wxStringPropertyExt(wxT("Colors range"),wxT(""),&m_info.colors_str));

	pgProperties->Thaw();
	pgProperties->FitColumns();
	setPGsize(pgProperties,10);
	pgProperties->GetParent()->Layout();
	

	return(0);
}

// edit resource properties
void FormGResEncoder::OnPropChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		// update
		obj->Update(prop);

		// try save changes
		m_info.SaveInfo();

		wxCommandEvent evt;
		OnRegenClick(evt);
	}
}


// open glyph resource
void FormGResEncoder::OnOpenClick(wxCommandEvent& event)
{
	if(m_thread_active)
	{
		wxMessageBox("Encoding resources seems to be still in progress!","Encode resources");
		return;
	}

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

	// show open dialog
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
	LoadResource(path,0);
	
	// load all other resources with shared palette
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

// select resource from list
void FormGResEncoder::OnSelectClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;
	if(m_thread_active)
	{
		wxMessageBox("Encoding resources seems to be still in progress!","Encode resources");
		return;
	}

	pgProperties->Clear();

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
	if(m_thread_active)
	{
		wxMessageBox("Encoding resources seems to be still in progress!","Encode resources");
		return;
	}

	int *shadow_color = NULL; 
	if(m_info.format == "UNITS.FSU")
		shadow_color = m_info.shadow_color;


	/*wxImage img = m_source.ConvertToImage();
	wxImage resizedImg = img.Rescale(newWidth,newHeight,wxIMAGE_QUALITY_HIGH);
	wxBitmap bmp = resizedImg;*/

	// re-encode
	m_gres.Encode(m_source,m_info.name,&m_pal, m_info.gamma, m_info.x_size,m_info.y_size,(wxImageResizeQuality)m_info.resampling,slideMinDither->GetValue(),m_info.alpha_threshold,shadow_color,0xFD);

	canvasSrc->Refresh();
	canvasRes->Refresh();
	palette->Refresh();
}


// export glyph
void FormGResEncoder::OnSaveClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;
	if(m_thread_active)
	{
		wxMessageBox("Encoding resources seems to be still in progress!","Encode resources");
		return;
	}

	std::string name = m_info.name;

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Export glyph resource"),spell_data->export_path,name,"LZ resource file (*.LZ)|*.LZ",
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());
	spell_data->export_path = saveFileDialog.GetDirectory().ToStdWstring();

	// save to file
	if(m_info.isUnitsFSU())
	{
		// UNITS.FSU sprite
		auto x_ofs = spinExtraXoffset->GetValue();
		auto y_ofs = m_info.y_offset + spinExtraYoffset->GetValue();
		if(FSU_sprite::SaveSprite(path,m_gres.pixels,m_gres.x_size,x_ofs,y_ofs,0xFD))
		{
			wxMessageDialog msg(NULL,string_format("Exporting graphic resource failed!"),"Export glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}
	}
	else
	{
		if(m_gres.Export(path))
		{
			wxMessageDialog msg(NULL,string_format("Exporting graphic resource failed!"),"Export glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}		
	}
}

// export all glyphs
void FormGResEncoder::OnSaveAllClick(wxCommandEvent& event)
{
	if(!m_info.isLoaded())
		return;

	if(m_thread_active)
	{
		wxMessageBox("Encoding resources seems to be still in progress!", "Encode resources");
		return;
	}

	// show save dialog
	wxDirDialog saveDirDialog(this,"Export multiple resources",spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring dir = wstring(saveDirDialog.GetPath().ToStdWstring());
	spell_data->export_path = dir;

	// rather ask for permission
	wxMessageDialog msg(NULL,"Files in the selected folder might be overwritten! Continue?","Export glyphs",wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
	if(msg.ShowModal() != wxID_YES)
		return;

	// for each listed resource
	m_task_failed_list.clear();
	m_task_list.clear();
	for(auto &item: lboxList->GetStrings())
	{
		// item path
		auto info_path = std::filesystem::path(dir).append(item.ToStdString()).wstring();
		m_task_list.push_back(info_path);
	}

	// build tasks
	ProcTh::Params params;
	params.x_offset = spinExtraXoffset->GetValue();
	params.y_offset = spinExtraYoffset->GetValue();
	params.dither_randomize = slideMinDither->GetValue();
	params.target_dir = std::filesystem::path(dir);
	params.mutex = &m_mutex;
	params.list = &m_task_list;
	params.failed_list = &m_task_failed_list;

	// start processign threads	
	auto cores = std::min(wxThread::GetCPUCount(),8);
	//auto cores = 1;
	m_threads.clear();
	m_thread_active = 0;
	for(int k = 0; k < cores; k++)
	{
		auto th_proc = new ProcTh(this,params);
		m_threads.push_back(th_proc);
		if(th_proc->Create() != wxTHREAD_NO_ERROR)
		{
			wxMessageBox(_("Couldn't create processing thread!"));
			for(auto& th: m_threads)
				delete th;
			return;
		}
		m_thread_active++;
	}
	for(auto& th: m_threads)
	{		
		if(th->Run() != wxTHREAD_NO_ERROR)
		{
			wxMessageBox(_("Couldn't run processing thread!"));
			// ###todo: somehow get rid of other threads?
			return;
		}
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

	
	std::vector<ImgQuantize::Pixel> pixels;

	// for each listed resource
	std::filesystem::path prev_path;
	for(auto& item: lboxList->GetStrings())
	{
		// item path
		auto info_path = std::filesystem::path(dir).append(item.ToStdString()).wstring();
		if(item == lboxList->GetStringSelection())
			prev_path = info_path;

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
				p.MoveTo(data,0,y);
				for(int x = 0; x < x_size; x++)
				{
					if(p.Alpha() != 0)
						pixels.push_back({p.Red(),p.Green(),p.Blue()});
					p++;
				}
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
				p.MoveTo(data,0,y);
				for(int x = 0; x < x_size; x++)
				{
					pixels.push_back({p.Red(),p.Green(),p.Blue()});
					p++;
				}
				
			}
		}
	}

	// reload original resource
	if(!prev_path.empty() && lboxList->GetCount() > 1)
		LoadResource(prev_path);
		

	m_pal.m_used[0] = 0;
	auto max_count = std::count(m_pal.m_used.begin(),m_pal.m_used.end(),1);

	auto pal = ImgQuantize::GenMedianCutPalette(pixels, max_count);

	// place black/transparent
	m_pal.m_pal.assign(3*256,0);

	// assign new colors
	auto used = m_pal.m_used;
	for(int k = 0; k < max_count; k++)
	{
		if(k >= pal.size())
			break;
		auto item = std::find(used.begin(), used.end(), 1);
		if(item == used.end())
			break;
		int cid = item - used.begin() + 1;
		m_pal.m_pal[cid*3 + 0] = pal[k].r;
		m_pal.m_pal[cid*3 + 1] = pal[k].g;
		m_pal.m_pal[cid*3 + 2] = pal[k].b;
		*item = 0;
	}
		
	
	/*std::vector<uint32_t> pixels;

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
	}*/

	// refresh
	OnRegenClick(event);
}



// render source
void FormGResEncoder::OnPaintSource(wxPaintEvent& event)
{
	m_source_mutex.lock();

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

	m_source_mutex.unlock();
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


