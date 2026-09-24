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
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormGResEncoder' on 2026-09-21 21:21:11
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	m_menubar12 = new wxMenuBar( 0 );
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem( mmFile, wxID_MM_OPEN, wxString( wxT("Open resource") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOpen );
	
	wxMenuItem* mmOpenBatch;
	mmOpenBatch = new wxMenuItem( mmFile, wxID_MM_OPEN_BATCH, wxString( wxT("Load batch") ) + wxT('\t') + wxT("Ctrl+B"), wxEmptyString, wxITEM_NORMAL );
	mmFile->Append( mmOpenBatch );
	
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
	
	mmView = new wxMenu();
	mmViewFrame = new wxMenuItem( mmView, wxID_MM_VIEW_FRAME, wxString( wxT("View image frame") ) + wxT('\t') + wxT("Alt+F"), wxEmptyString, wxITEM_CHECK );
	mmView->Append( mmViewFrame );
	mmViewFrame->Check( true );
	
	mmViewRef = new wxMenuItem( mmView, wxID_MM_VIEW_REF, wxString( wxT("View references") ) + wxT('\t') + wxT("Alt+R"), wxEmptyString, wxITEM_CHECK );
	mmView->Append( mmViewRef );
	mmViewRef->Check( true );
	
	m_menubar12->Append( mmView, wxT("View") );
	
	this->SetMenuBar( m_menubar12 );
	
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	wxBoxSizer* bSizer98;
	bSizer98 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText103 = new wxStaticText( this, wxID_ANY, wxT("Others resources:"), wxDefaultPosition, wxDefaultSize, 0 );
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
	
	m_staticText136 = new wxStaticText( this, wxID_ANY, wxT("Resource meta data (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
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
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormGResEncoder' on 2026-09-21 21:21:11
	// === AUTO GENERATED END ===
	m_thread_active = 0;

	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	AssignSVGresourceToMenu(mmOpen, "IDR_OPEN3");
	AssignSVGresourceToMenu(mmOpenBatch,"IDR_OPEN3");
	AssignSVGresourceToMenu(mmSave,"IDR_SAVE");
	AssignSVGresourceToMenu(mmSavePal,"IDR_SAVE");
	AssignSVGresourceToMenu(mmExportAll,"IDR_SAVE_ALL");
	AssignSVGresourceToMenu(mmExit,"IDR_CLOSE");
	
		
	Bind(wxEVT_CLOSE_WINDOW, &FormGResEncoder::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU,&FormGResEncoder::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_MENU,&FormGResEncoder::OnOpenClick,this,wxID_MM_OPEN);
	Bind(wxEVT_MENU,&FormGResEncoder::OnOpenBatchClick,this,wxID_MM_OPEN_BATCH);
	Bind(wxEVT_MENU,&FormGResEncoder::OnSaveClick,this,wxID_MM_SAVE);
	Bind(wxEVT_MENU,&FormGResEncoder::OnSaveAllClick,this,wxID_MM_SAVE_ALL);
	Bind(wxEVT_MENU,&FormGResEncoder::OnSavePalClick,this,wxID_MM_SAVE_PAL);

	Bind(wxEVT_MENU,&FormGResEncoder::OnViewClick,this,wxID_MM_VIEW_FRAME);
	Bind(wxEVT_MENU,&FormGResEncoder::OnViewClick,this,wxID_MM_VIEW_REF);
	
	
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormGResEncoder::OnRegenClick,this,wxID_BTN_REGEN);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormGResEncoder::OnRegenPaletteClick,this,wxID_BTN_REGEN_PAL);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormGResEncoder::OnRegenClick,this,wxID_SLIDE_MIN_DITHER);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormGResEncoder::OnSelectClick,this,wxID_LB_LIST);
	Bind(wxEVT_PG_CHANGED,&FormGResEncoder::OnPropChange,this,wxID_PG_PROPS);

	Bind(wxEVT_THREAD,&FormGResEncoder::OnThreadEvent,this,wxID_PROC_THREAD);

	pgProperties->Bind(wxEVT_PG_RIGHT_CLICK,&FormGResEncoder::OnPropPopupClick,this);

	
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

		// will regenerate palette?
		bool regen_pal = info.regen_pal && info.regen_pal_preset != SpellGresInfo::RegenPalPreset::NONE;

		// try load palette
		SpellPalette pal;
		auto pal_path = std::filesystem::path(path).parent_path().append(info.pal_name);
		if(pal.LoadInfo(pal_path) && !regen_pal)
		{
			m_config.mutex->lock();
			if(!m_config.list->empty())
				m_config.failed_list->push_back(path.filename().string());
			m_config.mutex->unlock();
			continue;
		}

		// expand generic file names
		info.ParseNames();
		
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

			if(regen_pal)
			{
				// preprocess image only
				SpellGraphicItem gres;
				std::vector<ImgQuantize::Pixel> buffer;
				gres.Encode(source,buffer,true,info.name,
					&pal,info.gamma,info.saturation,
					info.x_size,info.y_size,(wxImageResizeQuality)info.resampling);

				// remove transparents
				if(info.is_transparent)
					std::erase_if(buffer,[](ImgQuantize::Pixel &pix){ return(pix.isTransparent());});

				// make palette template
				auto pal_name = std::filesystem::path(info.name).stem().concat(".PAL").string();				
				pal.Clear();
				if(info.regen_pal_preset == SpellGresInfo::RegenPalPreset::INFO_FS)
				{					
					pal.InsertBlank(pal_name, 0, 192, info.colors_str);
				}
				int colors_count = pal.GetUsedCount();

				// generate palette
				auto colors = ImgQuantize::GenMedianCutPalette(buffer,colors_count);
				
				// try assign colors
				pal.AssignColors(colors);							
			}

			if(info.save_pal)
			{
				// save palette chunks
				m_config.mutex->lock();
				pal.SaveChunks(m_config.target_dir);
				m_config.mutex->unlock();
			}

			// define usable range of palette
			pal.ClearUserRange();
			pal.AddUserRangeStr(info.colors_str);

			// encode image
			int* shadow_color = NULL;
			if(info.isUnitsFSU())
				shadow_color = info.shadow_color;			
			gres_list.push_back(std::make_unique<SpellGraphicItem>());
			auto &gres = gres_list.back();
			std::vector<ImgQuantize::Pixel> buffer;
			gres->Encode(source,buffer,false,info.name,
				&pal,info.gamma,info.saturation,
				info.x_size,info.y_size,(wxImageResizeQuality)info.resampling,
				m_config.dither_randomize,info.alpha_threshold,shadow_color,0xFD);

			// just collect frames for animations
			if(info.isPNM() && frame_id < info.img_names.size() - 1)
				continue;

			// auto y-offset for trees?
			int y_offset = info.y_offset;
			if(info.is_tree_auto_y_offset)
				y_offset -= (gres->y_size - 26);

			// auto centering to given width?
			int x_offset = info.x_offset;
			if(info.center_width)
				x_offset += (info.center_width - gres->x_size)/2;

			// save to file
			auto save_path = std::filesystem::path(m_config.target_dir).append(info.name);
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
				// sprite DTA sprites

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
const std::map<int,std::string> SpellGresInfo::c_format_menu = {
	{(int)SpellGresInfo::Format::DTA,"Sprite DTA"},
	{(int)SpellGresInfo::Format::GFK,"Projectile GFK"},
	{(int)SpellGresInfo::Format::LZ,"LZ compressed"},
	{(int)SpellGresInfo::Format::FSU,"UNITS.FSU sprites"},
	{(int)SpellGresInfo::Format::PNM,"PNM animation"}
};
const std::map<SpellGresInfo::Format,std::string> SpellGresInfo::c_format = {
	{SpellGresInfo::Format::DTA,"DTA"},
	{SpellGresInfo::Format::GFK,"GFK"},
	{SpellGresInfo::Format::LZ,"LZ"},
	{SpellGresInfo::Format::FSU,"UNITS.FSU"},
	{SpellGresInfo::Format::PNM,"PNM"}
};
const std::map<int,std::string> SpellGresInfo::c_regen_palette_presets = {
	{(int)SpellGresInfo::RegenPalPreset::NONE,"None"},
	{(int)SpellGresInfo::RegenPalPreset::INFO_FS,"INFO.FS"}
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
	colors_str = "0-255";
	format = Format::DTA;
	x_size = 0;
	y_size = 0;
	x_offset = 0;
	y_offset = 0;
	center_width = 0;
	is_tree_auto_y_offset = false;
	is_transparent = false;
	shadow_color[0] = -1;
	shadow_color[1] = -1;
	shadow_color[2] = -1;
	gamma = 1.0;
	saturation = 1.0;
	alpha_threshold = 128;
	resampling = wxIMAGE_QUALITY_NEAREST;	
	regen_pal = false;
	regen_pal_preset = RegenPalPreset::NONE;
	save_pal = false;

	m_last_error.clear();
}

// parse raw names to actual names
void SpellGresInfo::ParseNames()
{
	auto stem = path.stem();
	if(stem.has_extension())
		stem = stem.stem();
	name = strrep(raw_name,"*",stem.string());
	img_name = strrep(raw_img_name,"*",stem.string());
}

// is loaded?
bool SpellGresInfo::isLoaded()
{
	return(!path.empty() && !info_name.empty() && !name.empty() && !img_name.empty() /*&& !pal_name.empty()*/);
}

// try load metadata from info file
int SpellGresInfo::LoadInfo(std::filesystem::path path)
{
	Clear();
	m_last_error.clear();

	// try read meta file
	std::string infostr;
	if(loadstr(path, infostr))
	{
		m_last_error = string_format("Failed loading file \"%s\"!",path);
		return(1);
	}
	auto info = get_text_lines(infostr, true);
	
	this->path = path;
	info_name = std::filesystem::path(path).filename().string();
	std::string stem = path.stem().string();

	// resource file name
	raw_name = name = info_get_string(info,"name");
	if(name.empty())
	{
		Clear();
		m_last_error = string_format("Missing 'name' item in \"%s\"!",path);
		return(1);
	}
	name = strrep(name,"*",stem);

	// resource image name
	raw_img_name = img_name = info_get_string(info,"image");
	if(img_name.empty())
	{
		Clear();
		m_last_error = string_format("Missing 'image' item in \"%s\"!",path);
		return(1);
	}
	img_name = strrep(img_name,"*",stem);

	// palette name (palinfo)
	pal_name = info_get_string(info,"palette");
	/*if(pal_name.empty())
	{
		Clear();
		m_last_error = string_format("Missing 'palette' item in \"%s\"!",path);
		return(1);
	}*/

	// used colors list
	colors_str = info_get_string(info,"colors");
	if(colors_str.empty())
	{
		Clear();
		m_last_error = string_format("Missing 'colors' range item in \"%s\"!",path);
		return(1);
	}
	
	
	auto x_size_str = info_get_string(info,"xsize");
	auto y_size_str = info_get_string(info,"ysize");
	if(x_size_str.empty() || y_size_str.empty())
	{
		Clear();
		m_last_error = string_format("Missing 'xsize' or 'ysize' items in \"%s\"!",path);
		return(1);
	}
	x_size = std::atoi(x_size_str.c_str());
	y_size = std::atoi(y_size_str.c_str());
	is_transparent = std::atoi(info_get_string(info,"transparent").c_str());
	
	auto format_str = info_get_string(info,"format","");
	auto fmt_it = std::ranges::find_if(c_format,[format_str](const auto& pair) {return(iequals(pair.second,format_str));});
	if(fmt_it == c_format.end())
	{
		Clear();
		m_last_error = string_format("Unknown or missing 'format' item value '%s' in \"%s\"!",format_str,path);
		return(1);
	}
	format = fmt_it->first;

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
		m_last_error = string_format("Unknown or missing 'resampling' item value '%s' in \"%s\"!",res_str,path);
		return(1);
	}
	resampling = res_it->first;

	// preprocessing color corrections
	gamma = info_get_real(info,"gamma",1.0);
	saturation = info_get_real(info,"saturation",1.0);

	// non-zero to enable image centering to given width
	center_width = info_get_int(info,"center_to_width",0);

	// enable automatic y-offset for trees (y_offset is then just extra relative offset to automatic value)
	is_tree_auto_y_offset = !!info_get_int(info,"tree_auto_y_offset",0);
	
	// sprite land type code
	auto landtype_str = info_get_string(info,"landtype");
	land_type = std::atoi(landtype_str.c_str());

	// alpha channel threshold
	alpha_threshold = info_get_int(info,"alpha_threshold",128);

	// auto regen palette on export?
	regen_pal = info_get_int(info,"regen_palette",0);

	// regen palette mode
	auto regen_pal_preset_str = info_get_string(info,"regen_palette_preset","none");
	auto regen_pal_preset_it = std::ranges::find_if(c_regen_palette_presets,[regen_pal_preset_str](const auto& pair) {return(iequals(pair.second,regen_pal_preset_str));});
	if(regen_pal_preset_it == c_regen_palette_presets.end())
	{
		Clear();
		m_last_error = string_format("Unknown or missing 'regen_palette_preset' item value '%s' in \"%s\"!",regen_pal_preset_str,path);
		return(1);
	}
	regen_pal_preset = (RegenPalPreset)regen_pal_preset_it->first;

	// auto save palettes on export?
	save_pal = info_get_int(info,"export_palette",0);

	auto shadow_color_str = info_get_string(info,"shadow_color");
	auto shadow_colors_list = get_text_lines(shadow_color_str,true,',');
	if(!shadow_color_str.empty() && shadow_colors_list.size() != 3)
	{
		Clear();
		m_last_error = string_format("Missing or invalid 'shadow_color' item value '%s' in \"%s\"!",shadow_color_str,path);
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
	
	auto fmt_it = c_format.find(format);
	if(fmt_it == c_format.end())
		return(1);
	info += info_make_string("format",fmt_it->second);
	
	info += info_make_int("xsize",x_size);
	info += info_make_int("ysize",y_size);
	info += info_make_int("xoffset",x_offset);
	info += info_make_int("yoffset",y_offset);

	auto res_it = c_resampling.find(resampling);
	if(res_it == c_resampling.end())
		return(1);
	info += info_make_string("resampling",res_it->second);

	info += info_make_real("gamma",gamma);
	info += info_make_real("saturation",saturation);

	info += info_make_int("center_to_width",center_width);
	info += info_make_int("tree_auto_y_offset",!!is_tree_auto_y_offset);
	info += info_make_int("landtype",land_type);
	info += info_make_int("transparent",!!is_transparent);
	info += info_make_int("alpha_threshold",alpha_threshold);

	std::vector<int> sh_colors(std::begin(shadow_color),std::end(shadow_color));	
	info += info_make_string("shadow_color",merge_vector(sh_colors,","));

	info += info_make_string("palette",pal_name);
	info += info_make_int("regen_palette",!!regen_pal);

	auto pal_preset_it = c_regen_palette_presets.find((int)regen_pal_preset);
	if(pal_preset_it == c_regen_palette_presets.end())
		return(1);
	info += info_make_string("regen_palette_preset",pal_preset_it->second);

	info += info_make_int("export_palette",save_pal);


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
	m_last_error.clear();
	m_source = wxBitmap();
	pgProperties->Clear();
	m_pal.Clear();
	m_gres.Clear();
	m_info.Clear();
	
	
	// try read meta file
	if(m_info.LoadInfo(path))
	{
		m_last_error = m_info.m_last_error;
		return(1);
	}

	// try load palette
	auto pal_path = std::filesystem::path(path).parent_path().append(m_info.pal_name);
	if(!m_info.pal_name.empty() && m_pal.LoadInfo(pal_path))
	{
		m_last_error = string_format("Failed loading palette \"%s\"!",pal_path);
		m_info.Clear();
		return(1);
	}
	
	// mark used palette colors
	m_pal.ClearUserRange();
	m_pal.AddUserRangeStr(m_info.colors_str);

	// pick frame of animation?
	bool is_pnm = m_info.isPNM();
	if(is_pnm && (frame_id < 0 || frame_id >= m_info.img_names.size()))
	{
		m_last_error = string_format("Requested frame index %d outside valid range 0 to %d!",frame_id,m_info.img_names.size()-1);
		m_info.Clear();
		return(1);
	}
	auto img_name = m_info.img_name;
	if(is_pnm && frame_id >= 0)
		img_name = m_info.img_names[frame_id];

	// try read image file
	auto image_path = std::filesystem::path(path).parent_path().append(img_name).wstring();
	if(!m_source.LoadFile(image_path,wxBITMAP_TYPE_PNG))
	{
		m_last_error = string_format("Loading image \"%s\" failed!",image_path);
		m_info.Clear();
		return(1);
	}
	SetStatusText(m_info.info_name,0);
	SetStatusText(m_info.name,1);
	SetStatusText(string_format("size = %d x %d",m_source.GetWidth(),m_source.GetHeight()),2);
	SetStatusText((m_info.is_transparent)?"transparent":"solid",3);
	SetStatusText(m_info.pal_name,4);
	SetStatusText(m_info.colors_str,5);

	pgProperties->Freeze();
	pgProperties->Clear();
	pgProperties->Append(new wxStringPropertyExt(wxT("Resource name"),wxT("name"),&m_info.raw_name));
	pgProperties->Append(new wxStringPropertyExt(wxT("Image name"),wxT("image"),&m_info.raw_img_name));
	pgProperties->Append(new wxStringPropertyExt(wxT("Palette name"),wxT(""),&m_info.pal_name));
	pgProperties->Append(new wxEnumPropertyExt(wxT("Format"),wxT(""),MapToPGenumChoices(SpellGresInfo::c_format_menu),(int*)&m_info.format));
	pgProperties->Append(new wxIntPropertyExt(wxT("x-size"),wxT(""),&m_info.x_size,-1));
	pgProperties->Append(new wxIntPropertyExt(wxT("y-size"),wxT(""),&m_info.y_size,-1));
	pgProperties->Append(new wxIntPropertyExt(wxT("x-offset"),wxT(""),&m_info.x_offset));
	pgProperties->Append(new wxIntPropertyExt(wxT("y-offset"),wxT(""),&m_info.y_offset));
	pgProperties->Append(new wxEnumPropertyExt(wxT("Resampling mode"),wxT(""),MapToPGenumChoices(SpellGresInfo::c_resampling),&m_info.resampling));
	pgProperties->Append(new wxRealPropertyExt(wxT("Gamma correction"),wxT(""),&m_info.gamma,2,0.1,3.0));
	pgProperties->Append(new wxRealPropertyExt(wxT("Color saturation"),wxT(""),&m_info.saturation,2,0.0,3.0));
	pgProperties->Append(new wxIntPropertyExt(wxT("Alpha threshold"),wxT(""),&m_info.alpha_threshold,0,255));
	pgProperties->Append(new wxIntPropertyExt(wxT("Centering width"),wxT(""),&m_info.center_width,-1));
	pgProperties->Append(new wxBoolPropertyExt(wxT("Auto y-offset for tree"),wxT(""),&m_info.is_tree_auto_y_offset));
	pgProperties->Append(new wxIntPropertyExt(wxT("Land type"),wxT(""),&m_info.land_type,0,13));
	pgProperties->Append(new wxBoolPropertyExt(wxT("Transparent"),wxT(""),&m_info.is_transparent));
	pgProperties->Append(new wxBoolPropertyExt(wxT("Regenerate palette"),wxT(""),&m_info.regen_pal));
	pgProperties->Append(new wxEnumPropertyExt(wxT("Regenerate palette preset"),wxT(""),MapToPGenumChoices(SpellGresInfo::c_regen_palette_presets),(int*)&m_info.regen_pal_preset));
	pgProperties->Append(new wxBoolPropertyExt(wxT("Export palettes"),wxT(""),&m_info.save_pal));
	pgProperties->Append(new wxStringPropertyExt(wxT("Colors range"),wxT("colors"),&m_info.colors_str));

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

// property popup menu
void FormGResEncoder::OnPropPopupClick(wxPropertyGridEvent& event)
{
	auto prop = event.GetProperty();
	if(!prop)
		return;
	
	wxMenu menu;
	menu.Append((int)POPUP_ACTIONS::COPY_PROP,string_format("Copy property \"%s\" to all resources",prop->GetLabel().ToStdString()));
	menu.Append((int)POPUP_ACTIONS::COPY_ALL,"Copy all properties to all resources");
	
	if(prop->GetName().CmpNoCase("colors") == 0)
	{
		menu.AppendSeparator();
		menu.Append((int)POPUP_ACTIONS::COLOR_INFO,"Set for INFO.FS graphics");
		menu.Append((int)POPUP_ACTIONS::COLOR_UNITS,"Set for UNITS.FSU graphics");
		menu.Append((int)POPUP_ACTIONS::COLOR_TERR,"Set for terrain sprites");
		menu.Append((int)POPUP_ACTIONS::COLOR_TERR_CYCLE,"Set for terrain sprites witch CYCLE.PAL");
	}

	menu.SetClientData(prop);
		
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormGResEncoder::OnRulesPopup),NULL,this);
	PopupMenu(&menu);
}
void FormGResEncoder::OnRulesPopup(wxCommandEvent& event)
{
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;
	auto prop = (wxPGProperty*)menu->GetClientData();
	if(!prop)
		return;
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(!obj)
		return;
	
	auto menu_id = (POPUP_ACTIONS)event.GetId();
	if(menu_id == POPUP_ACTIONS::COLOR_INFO)
	{
		prop->SetValue("0-191");
		obj->Update(prop);
	}
	else if(menu_id == POPUP_ACTIONS::COLOR_UNITS)
	{
		prop->SetValue("128-223");
		obj->Update(prop);
	}
	else if(menu_id == POPUP_ACTIONS::COLOR_TERR)
	{
		prop->SetValue("0-223");
		obj->Update(prop);
	}
	else if(menu_id == POPUP_ACTIONS::COLOR_TERR_CYCLE)
	{
		prop->SetValue("0-223, 240-249");
		obj->Update(prop);
	}
	else if(menu_id == POPUP_ACTIONS::COPY_PROP)
	{
		// for each listed resource:
		auto sel_id = lboxList->GetSelection();
		for(int k = 0; k < lboxList->GetCount(); k++)
		{
			auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(k));
			if(!data)
				continue;
			auto info_path = std::filesystem::path(data->GetData().ToStdWstring());

			// try load resource info
			if(m_info.LoadInfo(info_path))
				continue;
			
			// update property
			obj->Update(prop);

			// try save changes
			m_info.SaveInfo();
		}
		// reload original resource:
		if(sel_id >= 0)
		{
			auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(sel_id));
			if(!data)
				return;
			auto info_path = std::filesystem::path(data->GetData().ToStdWstring());
			if(m_info.LoadInfo(info_path))
				return;
		}
	}
	else if(menu_id == POPUP_ACTIONS::COPY_ALL)
	{		
		// for each listed resource:
		auto sel_id = lboxList->GetSelection();
		for(int k = 0; k < lboxList->GetCount(); k++)
		{
			auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(k));
			if(!data)
				continue;
			auto info_path = std::filesystem::path(data->GetData().ToStdWstring());

			// try load resource info
			if(m_info.LoadInfo(info_path))
				continue;

			// update all properties
			for(wxPropertyGridIterator it = pgProperties->GetIterator(); !it.AtEnd(); ++it)
			{
				wxPGProperty *prop = *it;
				if(!prop)
					continue;
				auto obj = (wxPGobj*)prop->GetClientObject();
				if(!obj)
					continue;
				if(prop->GetName().CmpNoCase("name") == 0 || prop->GetName().CmpNoCase("image") == 0)
					continue;
				obj->Update(prop);
			}
			
			// try save changes
			m_info.SaveInfo();
		}
		// reload original resource:
		if(sel_id >= 0)
		{
			auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(sel_id));
			if(!data)
				return;
			auto info_path = std::filesystem::path(data->GetData().ToStdWstring());
			if(m_info.LoadInfo(info_path))
				return;
		}
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
	pgProperties->Clear();

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
			wxMessageDialog msg(NULL,string_format("Cannot find matching graphic resource meta file:\n%s",path),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}
		SpellGresInfo info;
		if(info.LoadInfo(path))
		{
			wxMessageDialog msg(NULL,string_format("Failed loading graphic resource metadata:\n%s",info.m_last_error),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}
		if(info.img_name.compare(png_name) != 0)			
		{
			wxMessageDialog msg(NULL,string_format("Cannot find matching graphic resource meta file:\n%s",path),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
			msg.ShowModal();
			return;
		}
	}
	else
	{
		wxMessageDialog msg(NULL,string_format("Unknown file type:\n%s!",path),"Open glyph resource",wxOK| wxICON_EXCLAMATION);
		msg.ShowModal();
		return;
	}	
	
	// load resource
	if(LoadResource(path,0))
	{
		wxMessageBox(string_format("Failed loading graphic resource:\n%s",m_last_error),"Open glyph resource",wxICON_EXCLAMATION);
		return;
	}
	
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
		auto info_path = entry.path();
		if(info.LoadInfo(info_path))
		{
			wxMessageBox(string_format("Failed loading graphic resource metadata:\n%s",info.m_last_error),"Open glyph resource",wxICON_EXCLAMATION);
			break;
		}
		if(m_info.pal_name.compare(info.pal_name) != 0)
			continue;
		lboxList->Append(name, new wxStringClientData(info_path.wstring()));
		if(name.compare(m_info.info_name) == 0)
			select_id = lboxList->GetCount() - 1;
	}
	lboxList->Thaw();
	lboxList->Select(select_id);

	// force encoding
	OnSelectClick(event);
	wxCommandEvent evt;
	OnRegenClick(evt);

}


// open glyph resource
void FormGResEncoder::OnOpenBatchClick(wxCommandEvent& event)
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
	pgProperties->Clear();

	// force redraw when done
	canvasSrc->Refresh();
	canvasRes->Refresh();
	palette->Refresh();

	// show open dialog
	wxFileDialog openFileDialog(this,"Open glyph resource(s)",spell_data->export_path,L"","Graphic resource meta file (*.info)|*.info|Graphic resource image file (*.png)|*.png",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	spell_data->export_path = openFileDialog.GetDirectory();
	wxArrayString list;
	std::vector<std::filesystem::path> paths;
	openFileDialog.GetFilenames(list);
	std::transform(list.begin(), list.end(), std::back_inserter(paths),[](wxArrayString::value_type& item) {return(std::filesystem::path(item.ToStdWstring()));});	
	if(paths.empty())
		return;

	// check selection content
	bool has_non_info = std::ranges::any_of(paths, [](auto &item){return(!iequals(item.extension().string(),".info"));});
	bool has_info = std::ranges::any_of(paths,[](auto& item) {return(iequals(item.extension().string(),".info"));});
	if(has_info && has_non_info)
	{
		wxMessageBox(string_format("Select either metadata *.info or images *.png, but not both!"),"Open graphics resources",wxICON_EXCLAMATION);
		return;
	}

	// common directory
	auto dir = paths[0].parent_path();

	// default metadata
	SpellGresInfo info_def;

	// load all resources
	bool was_info_created = false;
	lboxList->Clear();
	lboxList->Freeze();
	for(auto &item: paths)
	{		
		auto is_info = iequals(item.extension().string(),".info");
		auto is_png = iequals(item.extension().string(),".png");
		if(!is_info && !is_png)
		{
			wxMessageBox(string_format("Select either metadata *.info or images *.png! \"%s\" not supported.",item.filename()),"Open graphics resources",wxICON_EXCLAMATION);
			return;
		}

		auto info_path = dir / item.stem().concat(".info");
		if(is_png && !std::filesystem::exists(info_path))
		{
			// is image, info metada not there
			if(!was_info_created)
			{
				// info not created
				wxMessageDialog dlg(this,string_format("Resource \"%s\" has not metadata *.info file! Create default?",item.filename()),"Open graphics resource",wxICON_QUESTION|wxYES_NO|wxYES_DEFAULT);
				if(dlg.ShowModal() != wxID_YES)
					return;

				// select palette file
				std::filesystem::path pal_path;
				while(true)
				{
					wxFileDialog openFileDialog(this,"Select palette matedata file",dir.wstring(),L"","Spellcross palette metadata file (*.palinfo)|*.palinfo",
						wxFD_OPEN|wxFD_FILE_MUST_EXIST);
					if(openFileDialog.ShowModal() == wxID_CANCEL)
						return;
					pal_path = openFileDialog.GetPath().ToStdWstring();
					auto pal_dir = std::filesystem::path(openFileDialog.GetDirectory().ToStdWstring());
					if(dir == pal_dir)
						break;					
					wxMessageDialog dlg(this,string_format("Palette \"%s\" must be in the same folder as selected resources \"%s\"! Try again?",pal_path,dir),"Open graphics resource",wxICON_QUESTION|wxYES_NO|wxYES_DEFAULT);
					if(dlg.ShowModal() != wxID_YES)
						return;
				}
				
				// make some default metadata				
				info_def.raw_name = "*.DTA";
				info_def.raw_img_name = "*.png";
				info_def.pal_name = pal_path.filename().string();
				info_def.format = SpellGresInfo::Format::DTA;
			}
			
			// save path
			if(info_def.SaveInfo(info_path))
			{
				wxMessageBox(string_format("Saving metadata \"%s\" failed!",info_path),"Open graphics resources",wxICON_EXCLAMATION);
				return;
			}
			was_info_created = true;
		}
		
		SpellGresInfo info;
		if(info.LoadInfo(info_path))
		{
			wxMessageBox(string_format("Failed loading graphic resource metadata:\n%s",info.m_last_error),"Open graphics resource",wxICON_EXCLAMATION);
			break;
		}
		lboxList->Append(info_path.filename().wstring(),new wxStringClientData(info_path.wstring()));
	}
	lboxList->Thaw();
	if(!lboxList->IsEmpty())
		lboxList->Select(0);

	// force encoding
	OnSelectClick(event);
	wxCommandEvent evt;
	OnRegenClick(evt);
}


// select resource from list
void FormGResEncoder::OnSelectClick(wxCommandEvent& event)
{
	/*if(!m_info.isLoaded())
		return;*/
	if(m_thread_active)
	{
		wxMessageBox("Encoding resources seems to be still in progress!","Encode resources");
		return;
	}

	pgProperties->Clear();

	// pick resource
	auto sel_id = lboxList->GetSelection();
	if(sel_id < 0)
		return;
	auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(sel_id));
	if(!data)
		return;
	auto info_path = std::filesystem::path(data->GetData().ToStdWstring());

	// try load
	if(LoadResource(info_path))
	{
		wxMessageBox(string_format("Resource has faulty meta data *.info file:\n%s",m_last_error),"Loading resource");
		return;
	}

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
	if(m_info.isUnitsFSU())
		shadow_color = m_info.shadow_color;

	// re-encode
	std::vector<ImgQuantize::Pixel> buffer;
	m_gres.Encode(m_source,buffer,false,
		m_info.name,&m_pal,m_info.gamma,m_info.saturation,
		m_info.x_size,m_info.y_size,(wxImageResizeQuality)m_info.resampling,
		slideMinDither->GetValue(),m_info.alpha_threshold,shadow_color,0xFD);

	// auto y-offset for trees?
	m_gres.aux.y_offset = m_info.y_offset;
	if(m_info.is_tree_auto_y_offset)
		m_gres.aux.y_offset -= (m_gres.y_size - 26);

	// auto centering to given width?
	m_gres.aux.x_offset = m_info.x_offset;
	if(m_info.center_width)
		m_gres.aux.x_offset += (m_info.center_width - m_gres.x_size)/2;



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

	// show save dialog
	wxDirDialog saveDirDialog(this,"Export resource data to directory",spell_data->export_path,wxDD_DIR_MUST_EXIST);
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
	
	// pick resource
	auto sel_id = lboxList->GetSelection();
	if(sel_id < 0)
	{
		wxMessageBox("No resource selected?","Exporting resource",wxICON_EXCLAMATION);
		return;
	}
	auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(sel_id));
	if(!data)
		return;
	auto info_path = std::filesystem::path(data->GetData().ToStdWstring());
	m_task_list.push_back(info_path);

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
	for(int k = 0; k < lboxList->GetCount(); k++)
	{
		// item path
		auto data = static_cast<wxStringClientData*>(lboxList->GetClientObject(k));
		if(!data)
			continue;
		auto info_path = std::filesystem::path(data->GetData().ToStdWstring());
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

	// all resources pixels
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

		// load image pixels with preprocessing
		std::vector<ImgQuantize::Pixel> buffer;
		m_gres.Encode(m_source,buffer,true,"",
			NULL,m_info.gamma,m_info.saturation,
			m_info.x_size,m_info.y_size,(wxImageResizeQuality)m_info.resampling);
		
		// add pixels to collection
		pixels.insert(pixels.end(), buffer.begin(), buffer.end());
	}

	// reload original resource
	if(!prev_path.empty() && lboxList->GetCount() > 1)
		LoadResource(prev_path);

	// target colors count
	m_pal.ClearUserRange();
	m_pal.AddUserRangeStr(m_info.colors_str);
	m_pal.m_used_user[0] = 0;
	auto max_count = std::ranges::count(m_pal.m_used_user,1);
		
	// generate palette
	auto pal = ImgQuantize::GenMedianCutPalette(pixels, max_count);

	// place black/transparent
	m_pal.m_pal.assign(3*256,0);

	// assign new colors
	auto used = m_pal.m_used_user;
	m_pal.m_used.assign(256,0);
	for(int k = 0; k < max_count; k++)
	{
		if(k >= pal.size())
			break;
		auto item = std::ranges::find(used, 1);
		if(item == used.end())
			break;
		int cid = item - used.begin() + 1;
		m_pal.m_pal[cid*3 + 0] = pal[k].r;
		m_pal.m_pal[cid*3 + 1] = pal[k].g;
		m_pal.m_pal[cid*3 + 2] = pal[k].b;
		m_pal.m_used[cid] = 1;
		*item = 0;
	}

	
		
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

// change view setup
void FormGResEncoder::OnViewClick(wxCommandEvent& event)
{
	canvasRes->Refresh();
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
		
				
		int x_ofs = (surf_x - m_gres.x_size)/2;
		int y_ofs = (surf_y - m_gres.y_size)/2;
		if(mmViewFrame->IsChecked())
		{			
			pdc.SetPen(wxPen(wxColor(0xFF0000),2,wxPENSTYLE_SHORT_DASH));
			pdc.SetBrush(*wxTRANSPARENT_BRUSH);
			pdc.DrawRectangle(wxPoint(x_ofs,y_ofs),wxSize(m_gres.x_size,m_gres.y_size));
		}
		if(mmViewRef->IsChecked())
		{			
			x_ofs -= m_gres.aux.x_offset;
			y_ofs -= m_gres.aux.y_offset;
			int x_size = max(m_gres.x_size + m_gres.aux.x_offset, m_info.center_width);		
			pdc.SetPen(wxPen(wxColor(0x0000FF),2,wxPENSTYLE_SHORT_DASH));
			pdc.DrawLine(x_ofs,0,x_ofs,surf_y-1);
			pdc.DrawLine(x_ofs+x_size,0,x_ofs+x_size,surf_y-1);		
			pdc.DrawLine(0,y_ofs,surf_x-1,y_ofs);

			// render tile frame
			if(m_info.format == SpellGresInfo::Format::DTA)
			{
				auto land_type = std::max(std::min(m_info.land_type,13) - 1,0);
				auto& tile = spell_data->special.frame[land_type];
				std::vector<uint8_t> pal(256*3,0);
				pal[3 + 0] = 0x11;
				pal[3 + 1] = 0xFF;
				pal[3 + 2] = 0x11;
				auto frame = tile.Render(pal.data());
				pdc.DrawBitmap(*frame,wxPoint(x_ofs,y_ofs));
				delete frame;
			}
		}
			
		

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


