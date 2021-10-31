#include "MyForm.h"
#include "windows.h"
#include "spellcross.h"
#include "map.h"

#include <thread>
#include <chrono>
#include <vector>

using namespace std;
using namespace System;
using namespace System::Windows::Forms;



Map *map;
SpellData* spelldata;

// --- tick timer thread
int tick_thread(int *exit)
{	
	auto t_start = std::chrono::high_resolution_clock::now();	
	while (!*exit)
	{
		// measure delta time from last cycle
		auto t_now = std::chrono::high_resolution_clock::now();
		double delta = std::chrono::duration<double, std::milli>(t_now - t_start).count();
		
		// tick?
		if (delta > 150.0)
		{
			// yaha - update map
			if (map->Tick())
			{
				//form->Canvas->Refresh();
				HWND handle;
				handle = FindWindow(NULL, WIN_TITLE);
				if(handle)
					PostMessage(handle, WM_USER, NULL, NULL);
			}
			t_start = t_now;
		}
		else
			Sleep(1);
	}

	return(0);

}


[STAThread]

void main(void) {
	
	// spellcross data root path
	wstring spelldata_path = L"D:\\hry\\SPELCROS\\data";
		
	// load spellcross data
	spelldata = new SpellData(spelldata_path);

	// special data folder
	wstring spec_folder = L"D:\\hry\\SPELCROS\\data\\spec";

	// load special sprites
	spelldata->LoadSpecialLand(spec_folder);

	// load some map
	//wchar_t* map_path = L"D:\\hry\\SPELCROS\\data\\data\\common\\M01_01.DTA";	
	wstring map_path = L"D:\\hry\\SPELCROS\\data\\data\\common\\M01_01B.DEF";
	//wstring map_path = L"D:\\hry\\SPELCROS\\data\\data\\common\\M04_12A.DEF";
	map = new Map();
	if (map->Load(map_path, spelldata))
	{
		delete map;
		return;
	}
	map->SetGamma(1.3);

	
	// init Forms application
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	CLRformstest::MyForm form;

	// create tick timer thread
	int th_exit = 0;
	std::thread th_timer(tick_thread, &th_exit);
	
	// run GUI
	Application::Run(% form);
	
	// terminate tick timer thread
	th_exit = 1;
	th_timer.join();
}