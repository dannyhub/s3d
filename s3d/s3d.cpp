// s3d.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "s3d.h"
#include "Window.h"
#ifdef UNITTEST

//int main(int argc, char* argv[]) {
//    return 0;
//}

#else
#include "resource.h"
#define MAX_LOADSTRING 100  

extern
HINSTANCE g_hInstance = NULL;

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
  UNREFERENCED_PARAMETER(nCmdShow);
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

  g_hInstance = hInstance;
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_S3D));

  auto wnd = s3d::Window::create();
  wnd->show();

  
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

#endif