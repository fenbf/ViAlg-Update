/*-----------------------------------------------------------------------------+
| File name: av_system.h													   |
| Date:		 13th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorith Visualization System.     										   |
| It Provides classes, methods, for easy representing way of work of           |
| algorithms. It is a layer between the CAlgorithm class and the graphics      |
| framework...                                                                 |
+-----------------------------------------------------------------------------*/

#ifndef AV_SYSTEM_H
	#define AV_SYSTEM_H

#include <typeinfo.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

#include "clog.h"
#include "tg_math.h"
#include "gl_shapes.h"

// the CAVSystem class --------------------------------------------------------+
enum BlockType { btPoint, btBox, btPyramid, btCylinder };
enum ColorType { ctNormal, ctMarked, ctHighlighted };

class CAVSystem {
public:
	CAVSystem();
	~CAVSystem();

	void SetDiagramBlockInfo(BlockType bType, const VECTOR3D &vCol, const VECTOR3D &vColMarked, 
		                     const VECTOR3D &vColHighlighted);
	template <class Iter> void BeginDrawing(Iter iBegin, Iter iEnd, double fMaxData, int nCount);
	void DrawDiagramBlock(double fValue, ColorType cType);
	void EndDrawing();
	void SetMaxSize(float fWidth, float fHeight, float fDepth) { m_vMaxSize = VECTOR3D(fWidth, fHeight, fDepth); }
	void SetOutlook(const VECTOR3D &vFrameCol, GLuint iFrameTex);
	void SetBlockType(BlockType bType) { m_bType = bType; }
	bool m_bHoriz;
	bool m_bFrame;
	bool m_bTextured;
private:
	BlockType m_bType;
	VECTOR3D m_vCol;
	VECTOR3D m_vColMarked;
	VECTOR3D m_vColHighlighted;
	GLuint m_iTex;
	bool m_bDrawing;
	VECTOR3D m_vFrameCol;
	VECTOR3D m_vMaxSize;
	VECTOR3D m_vSizeAsp;
	int m_iLod;
};

// in order of error in C++ implementation I must define templated methods here:
template <class Iter>
void CAVSystem::BeginDrawing(Iter iBegin, Iter iEnd, double fMaxData, int nCount) {
	static VECTOR3D vFrame = VECTOR3D(0.2f, 0.2f, 0.2f);
	
	if (m_bDrawing == true) return;
	m_bDrawing = true;

	m_vSizeAsp.y = m_vMaxSize.y/(float)fMaxData;
	m_vSizeAsp.x = m_vMaxSize.x/(float)nCount;
	m_vSizeAsp.z = m_vMaxSize.z;

	// simple Level Of Details function:
	m_iLod = 6;
	if (nCount <= 150) m_iLod = 8;
	if (nCount <= 100) m_iLod = 12;
    if (nCount <= 50) m_iLod = 16; 

	glPushMatrix();
		if (m_bHoriz == true) glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

		if (m_bFrame == true) {
			if (m_bTextured) { 
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, m_iTex);
			}
			glColor3fv(m_vFrameCol.m);
			glPushMatrix();
			    glTranslatef(-m_vMaxSize.x*0.5f-vFrame.x, vFrame.y+m_vMaxSize.y*0.5f, 0.0f);
				DrawBox(vFrame.x, m_vMaxSize.y+vFrame.y*1.0f, vFrame.z);
				glTranslatef(m_vMaxSize.x+vFrame.x*2.0f, 0.0f, 0.0f);
				DrawBox(vFrame.x, m_vMaxSize.y+vFrame.y*1.0f, vFrame.z);
				
				glTranslatef(-m_vMaxSize.x*0.5f-vFrame.x, -m_vMaxSize.y*0.5f-vFrame.y, 0.0f);
				DrawBox(m_vMaxSize.x+vFrame.x*1.0f, vFrame.y, vFrame.z);
				glTranslatef(0.0f, m_vMaxSize.y+vFrame.y*2.0f, 0.0f);
				DrawBox(m_vMaxSize.x+vFrame.x*1.0f, vFrame.y, vFrame.z);
			glPopMatrix();
			if (m_bTextured) { glDisable(GL_TEXTURE_2D); }
		}

	    glTranslatef(-m_vMaxSize.x*0.5f+m_vSizeAsp.x*0.5f, vFrame.y, 0.0f);
}

#endif // AV_SYSTEM_HPP

// end of file ----------------------------------------------------------------+