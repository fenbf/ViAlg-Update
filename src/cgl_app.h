/*-----------------------------------------------------------------------------+
| File name: cgl_app.hpp													   |
| Date:		 4 October 2005													   |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Declaration of the CGLApp class...										   |
| Based on the NeHe tutorials and code to the "Focus on 3D Terrain Programming"|
| by Trent Polack.															   |
+-----------------------------------------------------------------------------*/

#ifndef CGL_APP_HPP
	#define CGL_APP_HPP

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "ctimer.h"

/*-----------------------------------------------------------------------------+
| The CGLApp class															   |
+-----------------------------------------------------------------------------*/
class CGLApp {
public:
	CGLApp();
	~CGLApp();

	// methods:
	// core:
	bool Init(WORD hMenu = NULL, bool (*procCommand)(WORD, HMENU) = NULL, WORD hIcon = NULL);
	bool Destroy();
	static void SetRenderProc(void (*procRender)()) { m_procRender = procRender; }

	// input:
	inline bool KeyDown(unsigned char chKey) { return m_bKeys[chKey]; }
	inline bool KeyUp(unsigned char chKey)   { return !m_bKeys[chKey]; }
	inline void SetKey(unsigned char chKey, bool bState) { m_bKeys[chKey] = bState; } 
	inline int  GetMouseX()                  { return m_iMouseX; }
	inline int  GetMouseY()                  { return m_iMouseY; }
	inline bool MouseBtnDown(WORD wBtn)      { return (m_iMouseBtn & wBtn ? true : false ); }
	inline bool MouseBtnUp(WORD wBtn)        { return (m_iMouseBtn & wBtn ? false : true ); }

	inline bool IsActive()                   { return m_bActive; }

	inline HWND GetHwnd()                    { return m_hWnd; }
	inline HINSTANCE GetInstance()           { return m_hInstance; }
	inline HDC GetHdc()                      { return m_hDC; }

	// static methods:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);
	static int CheckWindowMessages();

	// windows variables:
	static char m_szTitle[64];
	static int m_iWidth;
	static int m_iHeight;
	static int m_iBpp;
	static HMENU m_hMenu;

	// input:
	static int m_iMouseX;
	static int m_iMouseY;
	static int m_iMouseBtn;
	static bool m_bKeys[256];
	
	static bool m_bActive;
	static bool m_bChangeSize;
	static int m_menuCommand;

	static bool m_bFullscreen;

	static bool (*m_procCommand)(WORD wId, HMENU hMenu);
	static void (*m_procRender)();
private:
	// windows variables:
	HINSTANCE m_hInstance;
	HWND	  m_hWnd;
	HDC		  m_hDC;
	HGLRC	  m_hRC;
};

#endif // CGL_APP_HPP

// end of file ----------------------------------------------------------------+