/*-----------------------------------------------------------------------------+
| File name: gl_text.cpp						  						       |
| Date:		 13 December 2005		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Basic functions / classes for text drawing in OpenGL Applications            |
| Implementation...                                                            |
+-----------------------------------------------------------------------------*/

#include "gl_text.h"

/*-----------------------------------------------------------------------------+
|                      the GLfont class implementation                         |
+-----------------------------------------------------------------------------*/

// Init2d method --------------------------------------------------------------+
bool CGLFont::Init2d(HDC hdcContext, FontMode fm, HFONT hFont, GLuint iWidth, GLuint iHeigth, GLuint iTexture) {
	if (m_FontMode!= fmNone) return false;
	
	HFONT hFontOld;
	BOOL bRet = TRUE;
	float fCx, fCy;
	
	// size of characters... in 2d mode should be fixed size
	m_fSize[0][0] = (float)iWidth;
	m_fSize[0][1] = (float)iHeigth;

	// build appropriate font:
	m_FontMode = fm;
	switch ( m_FontMode ) {
	case fmBitmap: {
			m_iList = glGenLists(256);

			hFontOld = (HFONT)SelectObject(hdcContext, hFont);
			bRet = wglUseFontBitmaps(hdcContext, 0, 255, m_iList);
			SelectObject(hdcContext, hFontOld);

			DeleteObject(hFont);
			break;
		}
	case fmTexture: {
			m_iList = glGenLists(256);								

			m_iTexture = iTexture;
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, m_iTexture);			
	
			// create 256 textured flat quads:
			for (GLuint i = 0; i < 256; ++i) {
				fCx = (float)(i%16)/16.0f;						// X Position Of Current Character
				fCy = (float)(i/16)/16.0f;						// Y Position Of Current Character

				glNewList(m_iList + i, GL_COMPILE);
					glBegin(GL_QUADS);
						glTexCoord2f(fCx, 1-fCy-0.0625f);
						glVertex2i(0, iWidth);
						glTexCoord2f(fCx + 0.0625f, 1 - fCy - 0.0625f);
						glVertex2i(iWidth, iWidth);
						glTexCoord2f(fCx + 0.0625f, 1 - fCy);
						glVertex2i(iWidth, 0);
						glTexCoord2f(fCx, 1 - fCy);
						glVertex2i(0, 0);
				glEnd();
				glTranslated(10, 0, 0);
			glEndList();
			}
			break;
		}
	case fm3d:	break;			   // one must call Init3d!
	default: m_FontMode = fmNone;
	}

	// any errors?
	if (bRet == FALSE) 	{
		glDeleteLists(m_iList, 256);
		return false;
	}

	return true;	 // success!
}

// Init3d method --------------------------------------------------------------+
bool CGLFont::Init3d(HDC hdcContext, HFONT hFont, float fDeviation, float fExtrusion, int iFormat) {
	if (m_FontMode != fmNone) return false;
	
	HFONT hFontOld;
	GLYPHMETRICSFLOAT gmf[256];
	BOOL bRet;

	m_FontMode = fm3d;

	m_iList = glGenLists(256);

	// select font and create 3d characters:
	hFontOld = (HFONT)SelectObject(hdcContext, hFont);
	bRet = wglUseFontOutlines(hdcContext, 0, 255, m_iList, fDeviation, 
		                      fExtrusion, iFormat, gmf);
	SelectObject(hdcContext, hFontOld);

	DeleteObject(hFont);

    // ok?
	if (bRet == FALSE) {
		glDeleteLists(m_iList, 256);
		return false;
	}

	// copy sizes of the characters:
	for (int i = 0; i < 256; ++i) {
		m_fSize[i][0] = gmf[i].gmfCellIncX;
		m_fSize[i][1] = gmf[i].gmfCellIncY;
	}

	return true;
}

// TextWidth method -----------------------------------------------------------+
float CGLFont::TextWidth(const char *strText) {
	// has font fixed size?
	if (m_FontMode != fm3d)
		return (float)strlen(strText)*m_fSize[0][0];

	// 3d font has variant width for each character 
	float fLen = 0.0f;
	GLuint iLen = (GLuint)strlen(strText);
	for (GLuint i = 0; i < iLen; ++i)
		fLen += m_fSize[i][0];

	return fLen;
}

// Delete method --------------------------------------------------------------+
void CGLFont::Delete() {
	glDeleteLists(m_iList, 256);
	if (m_FontMode == fmTexture) 
		glDeleteTextures(1, &m_iTexture);

	m_FontMode = fmNone;
}

// Create2dCourierFont static method ------------------------------------------+
bool CGLFont::Create2dCourierFont(const CGLApp *glApp, CGLFont &glFont, GLuint iWidth, GLuint iHeight) {
	HFONT hFont = CreateFont(iWidth, iHeight,		// heigth and width
							 0, 0,					// Escapement and Orientation
							 FW_BOLD,				// font weigth
							 FALSE, FALSE, FALSE,	// italic, underline, strikeout
							 ANSI_CHARSET,			
							 OUT_TT_PRECIS,			// TrueType fonts
							 CLIP_DEFAULT_PRECIS,	// clipping
							 ANTIALIASED_QUALITY,	// quality
							 FIXED_PITCH,			// pitch
							 "Courier New");


	if (glFont.Init2d(const_cast<CGLApp *>(glApp)->GetHdc(), fmBitmap, hFont, iWidth, iHeight) == false)
		return false;
	else
		return true;
}

/*-----------------------------------------------------------------------------+
|                                glBeginText							       |
+------------------------------------------------------------------------------+
| Disables lighting, texture 2d, depth tests... etc...                         |
+-----------------------------------------------------------------------------*/
void glBeginText(int iScrW, int iScrH) {	
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	
	// change projection matrix
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, (float)iScrW, (float)iScrH, 0.0f, -1.0f, 1.0f);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

/*-----------------------------------------------------------------------------+
|                                  glEndText							       |
+------------------------------------------------------------------------------+
| Restores parameters that were before text drawing...                         |
+-----------------------------------------------------------------------------*/
void glEndText() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glPopAttrib();
}

/*-----------------------------------------------------------------------------+
|                                 glTextout                                    |
+-----------------------------------------------------------------------------*/
void glTextout(const CGLFont *font, const char *strText, float x, float y) {
	GLuint b = font->m_iList;
	
	glPushMatrix();
	glRasterPos2i((int)x, (int)y);
	
	glTranslatef(x, y, 0.0f);

	if (font->m_FontMode == fmTexture) { 
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, font->m_iTexture); b-=32;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	glPushAttrib(GL_LIST_BIT);
	glListBase(b);
	glCallLists((GLsizei)strlen(strText), GL_UNSIGNED_BYTE, strText);
	glPopAttrib();
	glPopMatrix();
}

/*-----------------------------------------------------------------------------+
|                               glTextPrintf                                   |
+-----------------------------------------------------------------------------*/
void glTextPrintf(const CGLFont *font, float x, float y, const char *msg, ...) {
	char		text[256];				// Holds Our String
	va_list		ap;

	if (msg == NULL) return;

	va_start(ap, msg);					// Parses The String For Variables
	    vsprintf_s(text, msg, ap);		// And Converts Symbols To Actual Numbers
	va_end(ap);

	glTextout(font, text, x, y);
}

// end of file ----------------------------------------------------------------+