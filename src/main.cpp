/*
**  VROverlay -- OpenVR window overlay management tool
**		Copyright(C) 2016 Joshua New
**
**	This program is free software : you can redistribute it and / or modify
**		it under the terms of the GNU General Public License as published by
**		the Free Software Foundation, either version 3 of the License, or
**		(at your option) any later version.
**
**		This program is distributed in the hope that it will be useful,
**		but WITHOUT ANY WARRANTY; without even the implied warranty of
**		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
**		GNU General Public License for more details.
**
**		You should have received a copy of the GNU General Public License
**		along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib, "hid.lib")


#include "StaticOverlay.h"
#include "WindowOverlay.h"
#include "OverlayManager.h"
#include "OverlayTexture.h"
#include "RenderEnvironment.h"
#include "VertexObject.h"
#include <openvr.h>
#include <Psapi.h>
#include <PathCch.h>
#include <shellapi.h>

#include "../UniversalVROverlay/mainwindow.h"
#include <QApplication>

extern "C"
{
#include <hidsdi.h>
}

#include <CommCtrl.h>



//Will Need to move to WindowManager class
#include <vector>
#include "../UniversalVROverlay/WindowDescriptor.h"


#pragma comment(lib, "Pathcch.lib")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
HWND  hWnd;										// Main Window HWND
HFONT hFont;

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND targethWnd;
HWND targethWnd2;

//D3D env
RenderEnvironment* env;

//OverlayMgr
OverlayManager* mgr;

//Selected Overlay Index
int selectedIndex;

//VR System
vr::IVRSystem *pVRSystem;


std::vector<WindowDescriptor> wndVec;

												// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL    CALLBACK    EnumProc(HWND hWnd, LPARAM lParam);




int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	//Init values
	selectedIndex = -1;
	//AllocConsole();
	
	//TODO: add VR hooking to it's own class

	// Perform application initialization:
	/*if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}*/
	MSG msg;

	
	//----VR Init-------------------------------------------------------
	vr::HmdError m_eLastHmdError;
	pVRSystem = vr::VR_Init(&m_eLastHmdError, vr::VRApplication_Overlay);

	if (m_eLastHmdError != vr::VRInitError_None)
	{

		return 1;
	}

	//----Overlay D3dInit-----------------------------------------------

	
	//env = new RenderEnvironment(adapterIndex);
	mgr = new OverlayManager();
	QApplication a(__argc, __argv);
	MainWindow w(0,mgr, pVRSystem);
	w.show();
	hWnd = (HWND)w.effectiveWinId();
		
	//Setup viewport
	/*D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(1280);
	viewport.Height = static_cast<float>(720);
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;*/

	////D3D11
	//env->getContext()->RSSetViewports(1, &viewport);

	RAWINPUTDEVICE Rid[3];

	Rid[0].usUsagePage = 0x01;
	Rid[0].usUsage = 0x04;
	Rid[0].dwFlags = RIDEV_INPUTSINK;                 // adds joystick
	Rid[0].hwndTarget = hWnd;

	//Rid[1].usUsagePage = 0x01;
	//Rid[1].usUsage = 0x05;
	//Rid[1].dwFlags = RIDEV_INPUTSINK;                 // adds game pad
	//Rid[1].hwndTarget = hWnd;

	Rid[1].usUsagePage = 0x01;
	Rid[1].usUsage = 0x06;
	Rid[1].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;                 // adds hid mouse
	Rid[1].hwndTarget = hWnd;

	//Rid[3].usUsagePage = 0x01;
	//Rid[3].usUsage = 0x06;
	//Rid[3].dwFlags = RIDEV_NOLEGACY | RIDEV_INPUTSINK;                 // adds hid keyboard
	//Rid[3].hwndTarget = hWnd;

	/*if (RegisterRawInputDevices(Rid, 2, sizeof(Rid[0])) == FALSE)
	{
		
		MessageBox(NULL, L"Failed Raw", boost::lexical_cast<std::wstring>(GetLastError()).c_str(), MB_OK);
		
	}
		*/

	//------------------------------------------------------------------
	// Main message loop:
	return a.exec();
}

BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
	TCHAR title[500] = L"";
	TCHAR wndTitle[500] = L"";
	ZeroMemory(title, sizeof(title));

	if (IsWindowVisible(hWnd))
	{
		//GetWindowText(hWnd, title, sizeof(title) / sizeof(title[0]));
		DWORD PID = NULL;
		GetWindowThreadProcessId(hWnd, &PID);
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);
		GetModuleFileNameEx(hProcess, NULL, title, sizeof(title) / sizeof(title[0]));
		//GetModuleFileName()
		//GetWindowModuleFileName(hWnd, title, sizeof(title) / sizeof(title[0]));
		
		TCHAR *name = wcsrchr(title, L'\\');
		
		if (GetWindowText(hWnd, wndTitle, 500) > 0 && name != NULL)
		{
			//--------------------------------------------------
			wndVec.push_back(WindowDescriptor(hWnd, std::wstring(wndTitle), std::wstring(name)));
		}
		//--------------------------------------------------
		CloseHandle(hProcess);
		
	}

	
	return TRUE;
}

