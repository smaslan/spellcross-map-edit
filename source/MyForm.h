#include <windows.h>
#include <array>
#include <regex>
#include <filesystem>
#include "vcclr.h"
#include "spellcross.h"
#include "map.h"


// very lame way of finding main window handle - name it by this, then look for window of that name :)
#define WIN_TITLE L"Spellcross Map Editor"

extern SpellMap* map;
extern SpellData* spelldata;

struct {
	int xref, yref;
	int dx, dy;
	int state;
}scroll = {0,0,0,0,0};

#pragma once

namespace CLRformstest {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			this->ResizeRedraw = true;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public: System::Windows::Forms::PictureBox^ Canvas; /* changed to public to be callable from outside */
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ btn_open;
	private: System::Windows::Forms::OpenFileDialog^ open_dialog;
	private: System::Windows::Forms::ToolStripMenuItem^ viewToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ cb_wL1;
	private: System::Windows::Forms::ToolStripMenuItem^ cb_wL2;
	private: System::Windows::Forms::ToolStripMenuItem^ cb_wL3;
	private: System::Windows::Forms::ToolStripMenuItem^ cb_wL4;



	private: System::Windows::Forms::ToolStripMenuItem^ cb_wSTCI;
	private: System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ToolStripTextBox^ btn_SetGamma;
	private: System::Windows::Forms::ToolStripMenuItem^ cb_wUnits;
	private: System::Windows::Forms::ToolStripMenuItem^ btn_Exit;








	private: System::ComponentModel::IContainer^ components;


	protected:

	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Canvas = (gcnew System::Windows::Forms::PictureBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->btn_open = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->viewToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cb_wL1 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cb_wL2 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cb_wL3 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cb_wL4 = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cb_wSTCI = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->cb_wUnits = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->btn_SetGamma = (gcnew System::Windows::Forms::ToolStripTextBox());
			this->open_dialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->btn_Exit = (gcnew System::Windows::Forms::ToolStripMenuItem());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Canvas))->BeginInit();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// Canvas
			// 
			this->Canvas->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->Canvas->Location = System::Drawing::Point(12, 41);
			this->Canvas->Name = L"Canvas";
			this->Canvas->Size = System::Drawing::Size(865, 551);
			this->Canvas->TabIndex = 1;
			this->Canvas->TabStop = false;
			this->Canvas->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::Canvas_Paint);
			this->Canvas->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::Canvas_MouseDown);
			this->Canvas->MouseLeave += gcnew System::EventHandler(this, &MyForm::Canvas_MouseLeave);
			this->Canvas->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::Canvas_MouseMove);
			this->Canvas->MouseUp += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::Canvas_MouseUp);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->fileToolStripMenuItem,
					this->viewToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(890, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->btn_open,
					this->btn_Exit
			});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// btn_open
			// 
			this->btn_open->Name = L"btn_open";
			this->btn_open->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::O));
			this->btn_open->Size = System::Drawing::Size(180, 22);
			this->btn_open->Text = L"Open Map";
			this->btn_open->Click += gcnew System::EventHandler(this, &MyForm::btn_open_Click);
			// 
			// viewToolStripMenuItem
			// 
			this->viewToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(8) {
				this->cb_wL1,
					this->cb_wL2, this->cb_wL3, this->cb_wL4, this->cb_wSTCI, this->cb_wUnits, this->toolStripSeparator1, this->btn_SetGamma
			});
			this->viewToolStripMenuItem->Name = L"viewToolStripMenuItem";
			this->viewToolStripMenuItem->Size = System::Drawing::Size(41, 20);
			this->viewToolStripMenuItem->Text = L"View";
			// 
			// cb_wL1
			// 
			this->cb_wL1->Checked = true;
			this->cb_wL1->CheckOnClick = true;
			this->cb_wL1->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_wL1->Name = L"cb_wL1";
			this->cb_wL1->ShortcutKeys = System::Windows::Forms::Keys::F1;
			this->cb_wL1->Size = System::Drawing::Size(253, 22);
			this->cb_wL1->Text = L"Layer 1: Terrain";
			this->cb_wL1->Click += gcnew System::EventHandler(this, &MyForm::cb_wL1_Click);
			// 
			// cb_wL2
			// 
			this->cb_wL2->Checked = true;
			this->cb_wL2->CheckOnClick = true;
			this->cb_wL2->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_wL2->Name = L"cb_wL2";
			this->cb_wL2->ShortcutKeys = System::Windows::Forms::Keys::F2;
			this->cb_wL2->Size = System::Drawing::Size(253, 22);
			this->cb_wL2->Text = L"Layer 2: Objects";
			this->cb_wL2->Click += gcnew System::EventHandler(this, &MyForm::cb_wL1_Click);
			// 
			// cb_wL3
			// 
			this->cb_wL3->Checked = true;
			this->cb_wL3->CheckOnClick = true;
			this->cb_wL3->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_wL3->Name = L"cb_wL3";
			this->cb_wL3->ShortcutKeys = System::Windows::Forms::Keys::F3;
			this->cb_wL3->Size = System::Drawing::Size(253, 22);
			this->cb_wL3->Text = L"Layer 3: ANM animations";
			this->cb_wL3->Click += gcnew System::EventHandler(this, &MyForm::cb_wL1_Click);
			// 
			// cb_wL4
			// 
			this->cb_wL4->Checked = true;
			this->cb_wL4->CheckOnClick = true;
			this->cb_wL4->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_wL4->Name = L"cb_wL4";
			this->cb_wL4->ShortcutKeys = System::Windows::Forms::Keys::F4;
			this->cb_wL4->Size = System::Drawing::Size(253, 22);
			this->cb_wL4->Text = L"Layer 4: PNM animations";
			this->cb_wL4->Click += gcnew System::EventHandler(this, &MyForm::cb_wL1_Click);
			// 
			// cb_wSTCI
			// 
			this->cb_wSTCI->Checked = true;
			this->cb_wSTCI->CheckOnClick = true;
			this->cb_wSTCI->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_wSTCI->Name = L"cb_wSTCI";
			this->cb_wSTCI->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::E));
			this->cb_wSTCI->Size = System::Drawing::Size(253, 22);
			this->cb_wSTCI->Text = L"Start/Escape tiles";
			this->cb_wSTCI->Click += gcnew System::EventHandler(this, &MyForm::cb_wL1_Click);
			// 
			// cb_wUnits
			// 
			this->cb_wUnits->Checked = true;
			this->cb_wUnits->CheckOnClick = true;
			this->cb_wUnits->CheckState = System::Windows::Forms::CheckState::Checked;
			this->cb_wUnits->Name = L"cb_wUnits";
			this->cb_wUnits->ShortcutKeys = static_cast<System::Windows::Forms::Keys>((System::Windows::Forms::Keys::Control | System::Windows::Forms::Keys::U));
			this->cb_wUnits->Size = System::Drawing::Size(253, 22);
			this->cb_wUnits->Text = L"Units";
			this->cb_wUnits->Click += gcnew System::EventHandler(this, &MyForm::cb_wL1_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(250, 6);
			// 
			// btn_SetGamma
			// 
			this->btn_SetGamma->Name = L"btn_SetGamma";
			this->btn_SetGamma->Size = System::Drawing::Size(193, 21);
			this->btn_SetGamma->Text = L"Set Gamma: 1.3";
			this->btn_SetGamma->Leave += gcnew System::EventHandler(this, &MyForm::btn_SetGamma_Leave);
			this->btn_SetGamma->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::btn_SetGamma_KeyDown);
			this->btn_SetGamma->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &MyForm::btn_SetGamma_KeyPress);
			this->btn_SetGamma->Click += gcnew System::EventHandler(this, &MyForm::btn_SetGamma_Click);
			// 
			// open_dialog
			// 
			this->open_dialog->FileName = L"openFileDialog1";
			// 
			// btn_Exit
			// 
			this->btn_Exit->Name = L"btn_Exit";
			this->btn_Exit->Size = System::Drawing::Size(180, 22);
			this->btn_Exit->Text = L"Exit";
			this->btn_Exit->Click += gcnew System::EventHandler(this, &MyForm::btn_Exit_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(890, 604);
			this->Controls->Add(this->Canvas);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->Shown += gcnew System::EventHandler(this, &MyForm::MyForm_Shown);
			this->ResizeBegin += gcnew System::EventHandler(this, &MyForm::MyForm_ResizeBegin);
			this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->Canvas))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		
	

		//[System.Security.Permissions.PermissionSet(System.Security.Permissions.SecurityAction.Demand, Name = "FullTrust")];
		protected:
			virtual void WndProc(Message% m) override {
				if (m.Msg == WM_USER)
				{
					this->Canvas->Invalidate();
				}
				else if (m.Msg == WM_ACTIVATE)
				{
					this->Text = WIN_TITLE;
				}
				Form::WndProc(m);
			}









	private: System::Void pictureBox1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		
		PictureBox^ pic = safe_cast<PictureBox^>(sender);

		
		
	}
		
	private: System::Void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {

		
	}
	
	private: System::Void MyForm_ResizeBegin(System::Object^ sender, System::EventArgs^ e) {

	}


	private: System::Void MyForm_Shown(System::Object^ sender, System::EventArgs^ e) {
		this->DoubleBuffered = true;
		this->UpdateStyles();
	}
	private: System::Void Canvas_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
		PictureBox^ pic = safe_cast<PictureBox^>(sender);
		System::Drawing::Rectangle rect = System::Drawing::Rectangle(0, 0, pic->Width, pic->Height);
		System::Drawing::Bitmap bmp(pic->Width, pic->Height, System::Drawing::Imaging::PixelFormat::Format24bppRgb);

		::map->Render(% bmp, &scroll.dx, &scroll.dy, scroll.xref, scroll.yref);

		Graphics^ gr = e->Graphics;
		gr->DrawImage(% bmp, 0, 0);
	}
	private: System::Void Canvas_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		scroll.xref = e->X;
		scroll.yref = e->Y;

		if(e->Button == System::Windows::Forms::MouseButtons::Right)
			scroll.state = 1;
	}
	private: System::Void Canvas_MouseLeave(System::Object^ sender, System::EventArgs^ e) {
		scroll.state = 0;
	}
	private: System::Void Canvas_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		if (scroll.state)
		{
			scroll.dx -= (e->X - scroll.xref);
			scroll.dy -= (e->Y - scroll.yref);			
		}
		scroll.xref = e->X;
		scroll.yref = e->Y;

		PictureBox^ pic = safe_cast<PictureBox^>(sender);
		pic->Refresh();
	}
	private: System::Void Canvas_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
		scroll.state = 0;
	}
	
	
	// --- Open Map ---
	private: System::Void btn_open_Click(System::Object^ sender, System::EventArgs^ e) {
		
		std::filesystem::path last_path = ::map->GetTopPath();
		wstring dir = last_path.parent_path();
		wstring name = last_path.filename();
						
		open_dialog->InitialDirectory = gcnew System::String(dir.c_str());
		open_dialog->FileName = gcnew System::String(name.c_str());
		open_dialog->Filter = L"Spellcross DEF-map files (*.def)|*.def|Spellcross MAP files (*.dta)|*.dta";
		
		if (open_dialog->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			pin_ptr<const wchar_t> path = PtrToStringChars(open_dialog->FileName);

			// loose old map
			delete ::map;
			::map = new SpellMap();
			// load new one
			::map->Load(wstring(path), spelldata);
			// reset layers visibility
			cb_wL1_Click(sender, e);
			::map->SetGamma(StrToGamma(btn_SetGamma->Text));
		}
	}

	// change view mode
	private: System::Void cb_wL1_Click(System::Object^ sender, System::EventArgs^ e) {
		::map->SetRender(cb_wL1->Checked, cb_wL2->Checked, cb_wL3->Checked, cb_wL4->Checked, cb_wSTCI->Checked, cb_wUnits->Checked);
		this->Canvas->Invalidate();
	}
	
	private: System::Void btn_SetGamma_Click(System::Object^ sender, System::EventArgs^ e) {				
		double gamma = StrToGamma(btn_SetGamma->Text);
		btn_SetGamma->Text = btn_SetGamma->Text->Format("{0}", gamma);
	}

	private: System::Void btn_SetGamma_Leave(System::Object^ sender, System::EventArgs^ e) {
		//btn_SetGamma->Text = btn_SetGamma->Text->Format("Set gamma: {0}", Convert::ToDouble(btn_SetGamma->Text));
	}
	private: System::Void btn_SetGamma_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e) {
		
	}

	private: System::Void btn_SetGamma_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e) {
		if (e->KeyCode == System::Windows::Forms::Keys::Enter)
		{
			double gamma = StrToGamma(btn_SetGamma->Text);
			btn_SetGamma->Text = btn_SetGamma->Text->Format("Set gamma: {0}", gamma);			
			::map->SetGamma(gamma);
		}
	}

	private: double StrToGamma(System::String ^str) {
		pin_ptr<const wchar_t> txt = PtrToStringChars(str);
		double gamma;
		char dump[256];
		if (swscanf(txt, L"%[^:]:%Lf", &dump, &gamma) != 2)
			if (swscanf(txt, L"%Lf", &gamma) != 1)
				gamma = 1.3;
		return(gamma);
	}
	private: System::Void btn_Exit_Click(System::Object^ sender, System::EventArgs^ e) {
		Close();
	}
};
}
