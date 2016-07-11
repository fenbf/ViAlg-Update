/*-----------------------------------------------------------------------------+
| File name: gl_text.hpp						  						       |
| Date:		 13 December 2005		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Basic functions / classes for text drawing in OpenGL Applications            |
+-----------------------------------------------------------------------------*/

/* to do:
 - Loading texture from a specific file - CGLFont takes care for the font texture
   so there will be no need to initialise the texture outside the Font class.

*/

#ifndef CTEXT_HPP
    #define CTEXT_HPP

#include <windows.h>
#include <stdarg.h>
#include <stdio.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "cgl_app.h"

// the Font class -------------------------------------------------------------+
enum FontMode { fmNone = 0, fmBitmap, fmTexture, fm3d };

class CGLFont {
friend void glTextout(const CGLFont *, const char *, float, float);
friend void glTextPrintf(const CGLFont *, float, float, const char *, ...);

public:
	CGLFont(): m_FontMode(fmNone), m_iList(0), m_iTexture(0) { }
	~CGLFont() { Delete(); }

	// methods:
	bool Init2d(HDC hdcContext, FontMode fm, HFONT hFont, GLuint iWidth, GLuint iHeigth, GLuint iTexture = 0);
	bool Init3d(HDC hdcContext, HFONT hFont, float fDeviation, float fExtrusion, int iFormat);
	float TextWidth(const char *strText);
	float TextHeight() { return m_fSize[0][1]; }
	void Delete();

	// some additional static methods:
	static bool Create2dCourierFont(const CGLApp *glApp, CGLFont &glFont, GLuint iWidth, GLuint iHeight);
private:
	FontMode m_FontMode;
	GLuint m_iList;			 // font's display list
	GLuint m_iTexture;		 // texture id if necessary
	float m_fSize[256][2];	 // instead of the GLYPHMETRICSFLOAT structure
};

// functions for text drawing -------------------------------------------------+
void glBeginText(int iScrW, int iScrH);
void glEndText();
void glTextout(const CGLFont *font, const char *strText, float x = 0.0f, float y = 0.0f);
void glTextPrintf(const CGLFont *font, float x, float y, const char *strMsg, ...);

#endif // CTEXT_HPP

// end of file ----------------------------------------------------------------+