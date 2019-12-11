/*-----------------------------------------------------------------------------+
| File name: cgl_app.hpp													   |
| Date:		 4 October 2005													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Implementation of the CGLApp class from the "cgl_app.hpp" file.              |
| Based on the NeHe tutorials and code to the "Focus on 3D Terrain Programming"|
| by Trent Polack.															   |
+-----------------------------------------------------------------------------*/

#include "cgl_app.h"

#define WND_CLASS_NAME "GLWindow"

/*-----------------------------------------------------------------------------+
|                       Implementation of the CGLApp                           |
+-----------------------------------------------------------------------------*/

// variables:

// windows variables:
char CGLApp::m_szTitle[64] = "window";
bool CGLApp::m_bKeys[256];
int CGLApp::m_iWidth  = 0;
int CGLApp::m_iHeight = 0;
int CGLApp::m_iBpp    = 0;
HMENU CGLApp::m_hMenu = 0;

// input:
int CGLApp::m_iMouseX     = 0;
int CGLApp::m_iMouseY     = 0;
int CGLApp::m_iMouseBtn   = 0;
	
bool CGLApp::m_bActive     = false;
bool CGLApp::m_bChangeSize = false;
int CGLApp::m_menuCommand  = 0;

bool CGLApp::m_bFullscreen = false;

std::any CGLApp::s_param;
bool (*CGLApp::m_procCommand)(WORD, HMENU, std::any& param) = NULL;
void (*CGLApp::m_procRender)() = NULL;

// default constructor:
CGLApp::CGLApp():
	m_hWnd(NULL),
	m_hInstance(NULL),
	m_hDC(NULL),
	m_hRC(NULL)
{

}

// destructor:
CGLApp::~CGLApp() {
	Destroy();
}

/*-----------------------------------------------------------------------------+
|                                    Init                                      |
+------------------------------------------------------------------------------+
| Description:																   |
|    Initialises the window, sets up the OpenGL Rendering Context and the like.|
|    Fill static variables of the class before using thie method!			   |
| Arguments:																   |
|  - WORD hIcon - ID of the Window icon   (optional)   					       |
|  - WORD hMenu - ID of the Menu Resource (optional)					       |
| Return value:																   |
|    true if everything goes ok, unless false								   |
+-----------------------------------------------------------------------------*/
bool CGLApp::Init(WORD hMenu, bool (*procCommand)(WORD, HMENU, std::any&), WORD hIcon) {
	WNDCLASS wc;
	DWORD wStyle, wStyleEx;
	RECT rectWin;

	m_hInstance = GetModuleHandle(NULL);

	// fill wc structure:
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hbrBackground = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(NULL, MAKEINTRESOURCE(hIcon));
	wc.lpszMenuName = MAKEINTRESOURCE(hMenu);
	wc.lpszClassName = WND_CLASS_NAME;

	// register class:
	if (RegisterClass(&wc) == 0) {
		return false;
	}

	// Set full screen mode?
	if (m_bFullscreen == true) {
		DEVMODE dm;
		ZeroMemory(&dm, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmBitsPerPel = m_iBpp;
		dm.dmPelsWidth = m_iWidth;
		dm.dmPelsHeight = m_iHeight;
		dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSWIDTH;

		if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL) {
			return false;
		}

		// special styles of window:
		wStyle = WS_POPUP;
		wStyleEx = WS_EX_APPWINDOW;
		ShowCursor(FALSE);
	}
	else {
		wStyle = WS_POPUPWINDOW | WS_CAPTION | WS_MINIMIZEBOX;
		wStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	}

	// window width and heigth:
	rectWin.left = 0;
	rectWin.right = m_iWidth;
	rectWin.top = 0;
	rectWin.bottom = m_iHeight;

	// client area in windowed mode will be gla_win.w and gla_win.h
	AdjustWindowRectEx(&rectWin, wStyle, (hMenu != NULL ? TRUE : FALSE), wStyleEx);

	// create window:
	m_hWnd = CreateWindowEx(wStyleEx, WND_CLASS_NAME, m_szTitle, wStyle,
								 0, 0, rectWin.right - rectWin.left, rectWin.bottom - rectWin.top,
								 NULL, NULL, m_hInstance, NULL);
	
	if (m_hWnd == NULL) {
		return false;
	}

	// get DC
	m_hDC= GetDC(m_hWnd);
	if (m_hDC == NULL) {
		return FALSE;
	}

	// set pixel format:
	int pf;

	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		static_cast<BYTE>(m_iBpp),					// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		1,											// Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	pf = ChoosePixelFormat(m_hDC, &pfd);

	if (pf == 0)
		return false;

	if (SetPixelFormat(m_hDC, pf, &pfd) == FALSE)
		return false;

	// Rendering context:
	m_hRC = wglCreateContext(m_hDC);
	if (m_hRC == NULL)
		return false;

	if (wglMakeCurrent(m_hDC, m_hRC) == FALSE)
		return false;

	// Show Window and set to be foreground:
	ShowWindow(m_hWnd,SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	m_hMenu = GetMenu(m_hWnd);

    // set callback function for the WM_COMMAND event:
	m_procCommand = procCommand;

	// success! :)
	return true;
}

/*-----------------------------------------------------------------------------+
|                                  Destroy                                     |
+------------------------------------------------------------------------------+
| Description:																   |
|    Cleans up and closes the window   			                               |
| Arguments:																   |
|					        												   |
| Return value:																   |
|    true if everything goes ok, unless false								   |
+-----------------------------------------------------------------------------*/
bool CGLApp::Destroy() {
	if (m_bFullscreen == true) {
		// back to desktop:
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(TRUE);
	}

	// Rendering Context:
	if (m_hRC != nullptr) {
		wglMakeCurrent(m_hDC, nullptr);
		wglDeleteContext(m_hRC);
		m_hRC = NULL;
	}

	// Device Context:
	if (m_hDC != NULL) { 
		DeleteDC(m_hDC);
		m_hDC = NULL;
	}

	// Window:
	if (m_hWnd!= NULL) {
		DestroyWindow(m_hWnd);
		m_hWnd = NULL;
	}

	// Class:
	UnregisterClass(WND_CLASS_NAME, m_hInstance);

	return true;
}

/*-----------------------------------------------------------------------------+
|                                 WindowProc                                   |
+------------------------------------------------------------------------------+
| Description:																   |
|    Window Procedure for the application - handles WinApi messages.           |
| Arguments:																   |
|  - hWnd - handle to the window											   |
|  - uiMsg - system message													   |
|  - wParam, lParam - special parameters, they depend on the message		   |
| Return value:																   |
|    0 - message was served, if sth else the system must serve it...           |
+-----------------------------------------------------------------------------*/
LRESULT CALLBACK CGLApp::WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	//PAINTSTRUCT ps; 
    //HDC hdc; 
	
	switch( uiMsg )	{
		case WM_CREATE: {
			    
			    return 0;
		    }
		case WM_ACTIVATE: {
				if(!HIWORD(wParam))
					m_bActive = true;
				else
					m_bActive = false;
				return 0;
				//break;
			}
		case WM_SYSCOMMAND: {
				switch( wParam ) {
					case SC_SCREENSAVE:
					case SC_MONITORPOWER:
					return 0;
				}
				break;
			}
		case WM_COMMAND: {
				m_menuCommand = LOWORD(wParam);
				if (m_procCommand != NULL) {
					if (m_procCommand(m_menuCommand, m_hMenu, s_param) == false)
						PostQuitMessage(0);
				}
				return 0;
			}
		case WM_KEYDOWN: {
				m_bKeys[wParam]= true;
				return 0;
				break;
			}
		case WM_KEYUP: {
				m_bKeys[wParam]= false;
				return 0;
				break;
			}
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP: 
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:   {
				//get the current mouse position
				m_iMouseX = (int)LOWORD(lParam);
				m_iMouseY = (int)HIWORD(lParam);

				//get the current mouse button being pressed
				m_iMouseBtn = (int)wParam;

				break;
			}
		case WM_SIZE: {
				m_iWidth  = LOWORD(lParam);
				m_iHeight = HIWORD(lParam);

				m_bChangeSize = true;
				return 0;
				break;
			}
		/*case WM_MOVING: {
				g_Timer.Freeze();
				return TRUE;
				break;
			}
		case WM_MOVE: {
			    g_Timer.Unfreeze();
			    return 0;
			    break;
			}*/
		/*case WM_PAINT: { 
                hdc = BeginPaint(hWnd, &ps); 
                //TextOut(hdc, 0, 0, "Hello, Windows!", 15); 
                m_procRender();
				EndPaint(hWnd, &ps); 
                return 0;
			}*/
	    case WM_CLOSE: {
				PostQuitMessage(0);
				return 0;
				break;
			}
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

/*-----------------------------------------------------------------------------+
|                             CheckWindowMessages                              |
+------------------------------------------------------------------------------+
| Description:                                                                 |
|     Checks if there is any message for the main window of the                |
|     Application and send the message to the window procedure.                |
| Return Value:                                                                |
|     - 0 if there were a WM_QUIT Message                                      |
|     - 1 if there were any message for the window                             |
|     - -1 if there weren't any messages                                       |
+-----------------------------------------------------------------------------*/
int CGLApp::CheckWindowMessages() {
	static MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {		 // Is There A Message Waiting?
			if (msg.message == WM_QUIT)	return 0;        // Have We Received A Quit Message?
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);				
			}
			return 1;
		}
	return -1;
}

// end of file ----------------------------------------------------------------+