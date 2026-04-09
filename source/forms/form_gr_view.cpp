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
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

#include <filesystem>
#include <string>

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
	mmClose = new wxMenuItem(mmFile,wxID_MM_CLOSE,wxString(wxT("Close")) + wxT('\t') + wxT("ESC"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmClose);

	wxMenuItem* mmExport;
	mmExport = new wxMenuItem(mmFile,wxID_MM_EXPORT,wxString(wxT("Export")) + wxT('\t') + wxT("Ctrl+S"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExport);

	wxMenuItem* mmExportAll;
	mmExportAll = new wxMenuItem(mmFile,wxID_MM_EXPORT_ALL,wxString(wxT("Export all")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExportAll);

	m_menubar5->Append(mmFile,wxT("File"));

	this->SetMenuBar(m_menubar5);

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SB);
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer27;
	bSizer27 = new wxBoxSizer(wxVERTICAL);

	m_staticText104 = new wxStaticText(this,wxID_ANY,wxT("Source:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText104->Wrap(-1);
	bSizer27->Add(m_staticText104,0,wxLEFT|wxTOP,5);

	wxArrayString chSourceChoices;
	chSource = new wxChoice(this,wxID_CH_SOURCE,wxDefaultPosition,wxDefaultSize,chSourceChoices,0);
	chSource->SetSelection(0);
	bSizer27->Add(chSource,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText97 = new wxStaticText(this,wxID_ANY,wxT("Filter (wildcard: *?):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText97->Wrap(-1);
	bSizer27->Add(m_staticText97,0,wxLEFT|wxTOP,5);

	txtFilter = new wxTextCtrl(this,wxID_TXT_FILTER,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	bSizer27->Add(txtFilter,0,wxEXPAND|wxLEFT|wxRIGHT,5);

	m_staticText31 = new wxStaticText(this,wxID_ANY,wxT("Files:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText31->Wrap(-1);
	bSizer27->Add(m_staticText31,0,wxTOP|wxRIGHT|wxLEFT,5);

	lboxFiles = new wxListBox(this,wxID_LB_FILES,wxDefaultPosition,wxSize(180,-1),0,NULL,0|wxALWAYS_SHOW_SB|wxVSCROLL);
	bSizer27->Add(lboxFiles,1,wxLEFT|wxRIGHT,5);

	m_staticText32 = new wxStaticText(this,wxID_ANY,wxT("Width:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText32->Wrap(-1);
	bSizer27->Add(m_staticText32,0,wxTOP|wxRIGHT|wxLEFT,5);

	spinWidth = new wxSpinCtrl(this,wxID_SPIN_W,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxSP_ARROW_KEYS,0,10,0);
	bSizer27->Add(spinWidth,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	cbTransparent = new wxCheckBox(this,wxID_CB_TRANSPARENT,wxT("Is transparent?"),wxDefaultPosition,wxDefaultSize,0);
	bSizer27->Add(cbTransparent,0,wxALL,5);


	bSizer26->Add(bSizer27,0,wxEXPAND,5);

	m_staticline8 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer26->Add(m_staticline8,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* bSizer97;
	bSizer97 = new wxBoxSizer(wxVERTICAL);

	canvas = new wxPanel(this,wxID_CANVAS,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	bSizer97->Add(canvas,1,wxALL|wxEXPAND,5);

	m_staticline32 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer97->Add(m_staticline32,0,wxEXPAND | wxALL,5);

	m_staticText98 = new wxStaticText(this,wxID_ANY,wxT("Palette:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText98->Wrap(-1);
	bSizer97->Add(m_staticText98,0,wxLEFT|wxRIGHT,5);

	palette = new wxPanel(this,wxID_PALETTE,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL);
	palette->SetMaxSize(wxSize(-1,50));

	bSizer97->Add(palette,1,wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT,5);


	bSizer26->Add(bSizer97,1,wxEXPAND,5);


	this->SetSizer(bSizer26);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===

		// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);
	
	// close
	Bind(wxEVT_CLOSE_WINDOW, &FormGResView::OnClose, this, this->m_windowId);
	Bind(wxEVT_MENU,&FormGResView::OnCloseClick,this,wxID_MM_CLOSE);
	Bind(wxEVT_MENU,&FormGResView::OnExportClick,this,wxID_MM_EXPORT);
	Bind(wxEVT_MENU,&FormGResView::OnExportAllClick,this,wxID_MM_EXPORT_ALL);
	
	// canvas stuff:	
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormGResView::OnPaintCanvas,this,wxID_CANVAS);
	palette->SetDoubleBuffered(true);
	palette->Bind(wxEVT_PAINT,&FormGResView::OnPaintPalette,this,wxID_PALETTE);
	

	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormGResView::OnSelectFile,this,wxID_LB_FILES);
	Bind(wxEVT_SPINCTRL,&FormGResView::OnWidthChange,this,wxID_SPIN_W);

	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormGResView::OnChangeFilter,this,wxID_TXT_FILTER);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormGResView::OnChangeFilter,this,wxID_CH_SOURCE);

	chSource->Clear();
	chSource->Append("COMMON");
	chSource->Append("UNITS");
	chSource->Select(0);
	
	const int ss_w[] = {120,120,120,80,290,-1};
	sbar->SetFieldsCount(6,ss_w);

	// fill list
	//LoadFileList();
	txtFilter->SetValue("*");
	LoadGrpList();

	// default width
	spinWidth->SetMin(1);
	spinWidth->SetMax(1024);
	spinWidth->SetValue(16);
	m_width_dir = 0;
	m_width_old = 16;
}

FormGResView::~FormGResView()
{
}

void FormGResView::OnClose(wxCloseEvent& ev)
{
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormGResView::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

// change wildcard filter
void FormGResView::OnChangeFilter(wxCommandEvent& event)
{
	LoadGrpList();
}


// export glyph
void FormGResView::OnExportClick(wxCommandEvent& event)
{	
	// select resource
	if(lboxFiles->GetSelection() < 0 || lboxFiles->IsEmpty())
		return;
	std::string name = lboxFiles->GetString(lboxFiles->GetSelection()).ToStdString();

	// images source
	auto source = GetSource();
	if(!source)
		return;

	// image
	auto grpi = source->GetResource(name.c_str());
	if(!grpi)
		return;

	/*if(!grpi->x_size)
	{
		wxMessageDialog wxMessageDialog(NULL,"Cannot export glyph of undefined size!","Export glyph",wxOK| wxICON_EXCLAMATION);
		wxMessageDialog.ShowModal();
		return;
	}*/

	
	// default name
	auto wname = std::filesystem::path(name).concat(".png").wstring();

	// show save dialog
	wxFileDialog saveFileDialog(this,_("Export glyph image"),spell_data->export_path,wname,"PNG image file (*.png)|*.png",
		wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(saveFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveFileDialog.GetPath().ToStdWstring());
	spell_data->export_path = saveFileDialog.GetDirectory().ToStdWstring();
		
	// render glyph
	auto bmp = Render(name,true);
	if(!bmp)
		return;
	bmp->SaveFile(path,wxBITMAP_TYPE_PNG);
	delete bmp;		

	// save palette file
	auto pal_info_path = std::filesystem::path(path).parent_path().append(std::filesystem::path(grpi->palette->m_name).stem().string()).concat(".palinfo").wstring();
	grpi->palette->SaveInfo(pal_info_path);

	// save info file
	auto info_path = std::filesystem::path(path).parent_path().append(name).concat(".info").wstring();
	grpi->ExportInfo(info_path,std::filesystem::path(path).filename().wstring());
}


// export all listed glyph
void FormGResView::OnExportAllClick(wxCommandEvent& event)
{	
	// show save dialog
	wxDirDialog saveDirDialog(this,"Export multiple glyphs",spell_data->export_path,wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring dir = wstring(saveDirDialog.GetPath().ToStdWstring());
	spell_data->export_path = dir;

	// rather ask for permission
	wxMessageDialog msg(NULL,"Files in the selected folder might be overwritten! Continue?","Export glyphs", wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
	if(msg.ShowModal() != wxID_YES)
		return;

	// images source
	auto source = GetSource();
	if(!source)
		return;
		
	for(auto &item: lboxFiles->GetStrings())
	{			
		// remove extension
		//auto name = std::filesystem::path(item.ToStdString()).stem().string();
		auto name = item.ToStdString();

		auto grpi = source->GetResource(name.c_str());
		if(!grpi)
			return;
		
		// skip undefined glyphs
		if(!grpi->x_size)
			continue;

		// render glyph
		auto bmp = Render(item.ToStdString(),true);
		if(!bmp)
			return;
		auto image_path = std::filesystem::path(dir).append(name).concat(".png");
		bmp->SaveFile(image_path.wstring(),wxBITMAP_TYPE_PNG);
		delete bmp;
		
		// save palette file
		auto pal_info_path = std::filesystem::path(dir).append(std::filesystem::path(grpi->palette->m_name).stem().string()).concat(".palinfo").wstring();
		grpi->palette->SaveInfo(pal_info_path);

		// save info file
		auto info_path = std::filesystem::path(dir).append(name).concat(".info").wstring();
		grpi->ExportInfo(info_path,image_path.filename().wstring());

	}
}




// fill list of loaded graphics
void FormGResView::LoadGrpList()
{	
	auto source = GetSource();
	if(!source)
		return;

	lboxFiles->Freeze();
	lboxFiles->Clear();	
	for(int k = 0; k < source->Count(); k++)
	{
		auto *res = source->GetResource(k);
		if(!txtFilter->GetValue().IsEmpty() && !wildcmp(txtFilter->GetValue(),res->full_name.c_str()))
			continue;
		lboxFiles->Append(res->full_name);
	}
	
	
	lboxFiles->Thaw();
}

// file selected
void FormGResView::OnSelectFile(wxCommandEvent& event)
{
	canvas->Refresh();
	palette->Refresh();
}

// change width
void FormGResView::OnWidthChange(wxSpinEvent& event)
{	
	int width = spinWidth->GetValue();
	m_width_dir = min(max(width - m_width_old,-1),+1);
	m_width_old = width;
	canvas->Refresh();
}

// get graphics source
SpellGraphics*FormGResView::GetSource()
{
	std::vector<SpellGraphics*> sources = {&spell_data->gres, &spell_data->gres_info};
	if(chSource->GetSelection() < 0)
		return(NULL);
	return(sources[chSource->GetSelection()]);
}


// render glyph
wxBitmap *FormGResView::Render(std::string name, bool for_export)
{	
	auto source = GetSource();
	if(!source)
		return(NULL);
	auto grpi = source->GetResource(name.c_str());
	if(!grpi)
		return(NULL);

	SetStatusText(name,0);
	
	bool known_size = false;
	int len = grpi->pixels.size();

	SetStatusText(string_format("size = %d",len),1);
	SetStatusText(string_format("palette = %s (colors = %s)",grpi->palette->m_name.c_str(),grpi->palette->GetRangeString().c_str()),4);
	SetStatusText(string_format("used colors = %s",grpi->GetColorRangeString().c_str()),5);

	if(grpi->x_size)
	{
		// known exact size
		m_width_old = grpi->x_size;
		spinWidth->SetValue(grpi->x_size);
		known_size = true;

		SetStatusText(string_format("w = %d",grpi->x_size),2);
		SetStatusText(string_format("h = %d",grpi->y_size),3);
	}
	else
	{
		// unknown size
		int w = spinWidth->GetValue();
		while(len % w)
		{
			if(m_width_dir > 0)
				w++;
			else
				w--;
			if(w >= len || w <= 1)
				break;
		}
		m_width_old = w;
		spinWidth->SetValue(w);

		SetStatusText(string_format("w = %d (manual)",w),2);
		SetStatusText(string_format("h = %d",len/w),3);
	}

	spinWidth->Enable(!known_size);
	cbTransparent->SetValue(grpi->is_transparent);
			
		
	// input data dimensions
	int x_size = spinWidth->GetValue();
	int y_size = len/x_size + ((len%x_size)?1:0);
		
	// canvas size
	int surf_x = canvas->GetClientSize().GetWidth();
	int surf_y = canvas->GetClientSize().GetHeight();
	if(for_export)
	{
		surf_x = x_size;
		surf_y = y_size;			
	}

	if(!known_size)
	{
		// fake size for render
		grpi->x_size = x_size;
		grpi->y_size = y_size;
	}

	// render
	auto bmp = grpi->Render(surf_x,surf_y,for_export);

	if(!known_size)
	{
		// restore size
		grpi->x_size = 0;
		grpi->y_size = 0;
	}

	return(bmp);

}

// render preview
void FormGResView::OnPaintCanvas(wxPaintEvent& event)
{	
	// select resource
	if(lboxFiles->GetSelection() < 0 || lboxFiles->IsEmpty())
		return;
	std::string name = lboxFiles->GetString(lboxFiles->GetSelection()).ToStdString();
	
	// render glyph
	auto bmp = Render(name);
	if(!bmp)
		return;
	
	// blit to screen
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(*bmp,wxPoint(0,0));
	delete bmp;

}

// render palette preview
void FormGResView::OnPaintPalette(wxPaintEvent& event)
{
	auto source = GetSource();
	if(!source)
		return;

	// select resource
	if(lboxFiles->GetSelection() < 0 || lboxFiles->IsEmpty())
		return;
	std::string name = lboxFiles->GetString(lboxFiles->GetSelection()).ToStdString();

	auto grpi = source->GetResource(name.c_str());
	if(!grpi)
		return;
	auto pal = grpi->palette;
	if(!pal)
		return;

	// render palette
	wxBitmap bmp(palette->GetClientSize(),24);
	pal->Render(bmp);
	
	// blit to screen
	wxPaintDC pdc(palette);
	pdc.DrawBitmap(bmp,wxPoint(0,0));
}


