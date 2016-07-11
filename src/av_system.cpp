/*-----------------------------------------------------------------------------+
| File name: av_system.h													   |
| Date:		 13th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Algorith Visualization System.     										   |
| It Provides classes, methods, for easy representing way of work of           |
| algorithms. It is a layer between the CAlgorithm class and the graphics      |
| framework...                                                                 |
| Implementation...                                                            |
+-----------------------------------------------------------------------------*/

#include "av_system.h"

/*-----------------------------------------------------------------------------+
|                     Implementation of the CAVSystem                          |
+-----------------------------------------------------------------------------*/

// constructor:
CAVSystem::CAVSystem():
    m_bType(btBox),
	m_vCol(0.2f, 0.4f, 0.9f),
	m_vColMarked(0.2f, 0.9f, 0.2f),
	m_vColHighlighted(0.3f, 0.4f, 0.9f),
	m_iTex(0),
	m_bTextured(false),
	m_bDrawing(false),
	m_bHoriz(true),
	m_bFrame(true),
	m_vFrameCol(0.7f, 0.5f, 0.1f),
	m_vMaxSize(0.0f, 0.0f, 0.0f),
	m_vSizeAsp(0.0f, 0.0f, 0.0f)
    {
	g_Log.AddMsg(lmNormal, "%s initialisation", typeid(*this).name());
}

// destructor:
CAVSystem::~CAVSystem() {
	g_Log.AddMsg(lmNormal, "%s cleanup", typeid(*this).name());
}

void CAVSystem::SetDiagramBlockInfo(BlockType bType, const VECTOR3D &vCol, const VECTOR3D &vColMarked, 
									const VECTOR3D &vColHighlighted) {
	m_bType = bType;
	m_vCol = vCol;
	m_vColMarked = vColMarked;
	m_vColHighlighted = vColHighlighted;
}

void CAVSystem::SetOutlook(const VECTOR3D &vFrameCol, GLuint iFrameTex) {
	m_vFrameCol = vFrameCol;
	m_iTex = iFrameTex;
}

void CAVSystem::DrawDiagramBlock(double fValue, ColorType cType) {
	if (m_bDrawing == false) return;

	switch (cType) {
		case ctNormal:      glColor3fv(m_vCol.m); break;
		case ctHighlighted: glColor3fv(m_vColHighlighted.m); break;
		case ctMarked:      glColor3fv(m_vColMarked.m); break;
	}

	switch ( m_bType ) {
		case btPoint: {
			glPushMatrix();
			    glTranslatef(0.0f, m_vSizeAsp.y*(float)fValue, 0.0f);
				DrawSphere(m_vSizeAsp.x*0.75f, m_vSizeAsp.x*0.75f, m_vSizeAsp.x*0.75f, m_iLod/2, m_iLod/3);
			glPopMatrix();
			break;
			}
		case btBox: {
			glPushMatrix();
				glTranslatef(0.0f, m_vSizeAsp.y*(float)fValue*0.5f, 0.0f);
				DrawBox(m_vSizeAsp.x, m_vSizeAsp.y*(float)fValue, m_vSizeAsp.x);
			glPopMatrix();
			break;
			}
		case btPyramid: {
			DrawPyramid(m_vSizeAsp.x, m_vSizeAsp.y*(float)fValue);
			break;
			}
		case btCylinder: {
			glPushMatrix();
				glTranslatef(0.0f, m_vSizeAsp.y*(float)fValue*0.5f, 0.0f);
				DrawCylinder(m_vSizeAsp.x*0.5f, m_vSizeAsp.y*(float)fValue, m_iLod, 1, false);
			glPopMatrix();
			break;
			}
	}

	glTranslatef(m_vSizeAsp.x, 0.0f, 0.0f);
}

void CAVSystem::EndDrawing() {
	if (m_bDrawing == true) {
		glPopMatrix();
		m_bDrawing = false;
	}
}

// end of file ----------------------------------------------------------------+
