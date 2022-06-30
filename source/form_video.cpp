///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_video.h"
//#include "sprites.h"
//#include "other.h"


#include <wx/rawbmp.h>
#include <wx/dirdlg.h>

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

FormVideo::FormVideo(wxWindow* parent,SpellData* spell_data,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	m_spell_data = spell_data;

	// === AUTO GENERATED START ===

	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

	mmenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem(mmFile,wxID_MM_OPEN,wxString(wxT("Open Video File")) + wxT('\t') + wxT("Ctrl+O"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmOpen);

	wxMenuItem* mmSaveFrames;
	mmSaveFrames = new wxMenuItem(mmFile,wxID_SAVE_FRAMES,wxString(wxT("Save raw frames")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSaveFrames);

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(wxT("Exit")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	mmenu->Append(mmFile,wxT("File"));

	this->SetMenuBar(mmenu);

	wxBoxSizer* bSizer58;
	bSizer58 = new wxBoxSizer(wxVERTICAL);

	m_staticText64 = new wxStaticText(this,wxID_ANY,wxT("Frame view:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText64->Wrap(-1);
	bSizer58->Add(m_staticText64,0,wxTOP|wxRIGHT|wxLEFT,5);

	canvas = new wxPanel(this,wxID_CANVAS,wxDefaultPosition,wxSize(320,200),wxTAB_TRAVERSAL);
	canvas->SetMinSize(wxSize(320,200));
	canvas->SetMaxSize(wxSize(320,200));

	bSizer58->Add(canvas,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	m_staticText65 = new wxStaticText(this,wxID_DATA_POS,wxT("Offset:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText65->Wrap(-1);
	bSizer58->Add(m_staticText65,0,wxALL,5);

	slideOffset = new wxSlider(this,wxID_SLIDE_OFS,0,0,100,wxDefaultPosition,wxDefaultSize,wxSL_HORIZONTAL);
	bSizer58->Add(slideOffset,0,wxALL|wxEXPAND,5);


	this->SetSizer(bSizer58);
	this->Layout();
	m_statusBar8 = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_ANY);

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===
	
	// close
	Bind(wxEVT_CLOSE_WINDOW,&FormVideo::OnClose,this,this->m_windowId);
	Bind(wxEVT_MENU,&FormVideo::OnCloseClick,this,wxID_MM_EXIT);

	// canvas stuff:
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormVideo::OnPaintCanvas,this);

	Bind(wxEVT_MENU,&FormVideo::OnOpenClick,this,wxID_MM_OPEN);
	Bind(wxEVT_COMMAND_SLIDER_UPDATED,&FormVideo::OnChangeOffst,this,wxID_SLIDE_OFS);
	Bind(wxEVT_MENU,&FormVideo::OnSaveFramesClick,this,wxID_SAVE_FRAMES);

	m_data = NULL;

}

FormVideo::~FormVideo()
{
}

void FormVideo::OnClose(wxCloseEvent& ev)
{
	// loose old data
	if(m_data)
		delete m_data;
	m_data = NULL;
	
	wxPostEvent(GetParent(), ev);
	ev.Skip();
	Destroy();
}

// close form
void FormVideo::OnCloseClick(wxCommandEvent& event)
{
	Close();
}

// open file
void FormVideo::OnOpenClick(wxCommandEvent& event)
{
	// split path to folder and file
	std::filesystem::path last_path = "D:\\hry\\spellcd\\data\\data\\movie\\L_01.CAN";
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show open dialog
	wxFileDialog openFileDialog(this,_("Open Spellcross Video File"),dir,name,"CAN file (*.CAN)|*.CAN|Packed file (*.DPK)|*.DPK",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(openFileDialog.GetPath().ToStdWstring());

	// loose old data
	if(m_data)
		delete m_data;
	m_data = NULL;
	
	// load new data
	m_data = new SpellCAN(path);
	
	slideOffset->SetMax(m_data->GetFramesCount() - 1);
	canvas->Refresh();
}

// open file
void FormVideo::OnSaveFramesClick(wxCommandEvent& event)
{
	if(!m_data)
		return;
	
	// split path to folder and file
	std::filesystem::path last_path = "D:\\hry\\spellcd\\data\\data\\movie\\L_01.CAN";
	wstring dir = last_path.parent_path(); dir += wstring(L"\\");
	wstring name = last_path.filename();

	// show open dialog
	wxDirDialog saveDirDialog(this,_("Save video frames"),dir,wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if(saveDirDialog.ShowModal() == wxID_CANCEL)
		return;
	wstring path = wstring(saveDirDialog.GetPath().ToStdWstring());

	// save frames
	m_data->SaveFrames(path);	
}

void FormVideo::OnChangeOffst(wxCommandEvent& event)
{
	canvas->Refresh();
}


// render preview
void FormVideo::OnPaintCanvas(wxPaintEvent& event)
{
	// make render buffer
	wxBitmap bmp(canvas->GetClientSize(),24);
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();		

	/*if(!m_data.empty())
	{
		// render 24bit RGB data to raw bmp buffer
		uint8_t* buf = &m_data[slideOffset->GetValue()];
		wxNativePixelData pdata(bmp);
		wxNativePixelData::Iterator p(pdata);
		for(int y = 0; y < surf_y; y++)
		{
			uint8_t* scan = p.m_ptr;
			for(int x = 0; x < surf_x; x++)
			{
				uint8_t pix = *buf++;
				*scan++ = pix;
				*scan++ = pix;
				*scan++ = pix;
			}
			p.OffsetY(pdata,1);
		}
	}*/

	// blit to screen
	wxPaintDC pdc(canvas);
	pdc.DrawBitmap(bmp,wxPoint(0,0));

}


// fill common file list
/*void FormGResView::LoadFileList()
{	
	lboxFiles->Freeze();
	lboxFiles->Clear();
	for(int k = 0; k < common->Count(); k++)
	{
		std::filesystem::path name = string(common->GetFileName(k));		

		if( name.extension().compare(".LZ") == 0 ||
			name.extension().compare(".LZ0") == 0 ||
			name.extension().compare(".ICO") == 0 ||
			name.extension().compare(".BTN") == 0 || 
			wildcmp("RAM*.DTA",name.string().c_str()))
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


*/


