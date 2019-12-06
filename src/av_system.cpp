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
	m_vColHighlighted(0.6f, 0.6f, 0.95f),
	m_iTex(0),
	m_bTextured(false),
	m_bDrawing(false),
	m_bHoriz(true),
	m_bFrame(true),
	m_vFrameCol(0.7f, 0.5f, 0.1f),
	m_vMaxSize(0.0f, 0.0f, 0.0f),
	m_vSizeAsp(0.0f, 0.0f, 0.0f)
    {
	CLog::Instance()->AddMsg(LogMode::Info, "%s initialisation", typeid(*this).name());
}

// destructor:
CAVSystem::~CAVSystem() {
	CLog::Instance()->AddMsg(LogMode::Info, "%s cleanup", typeid(*this).name());
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
				DrawSphere(m_vSizeAsp.x*0.75f, m_vSizeAsp.x*0.75f, m_vSizeAsp.x*0.75f, m_iLod, m_iLod);
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

void CAVSystem::BeginDrawing(double fMaxData, int nCount)
{
	static VECTOR3D vFrame = VECTOR3D(0.2f, 0.2f, 0.2f);

	if (m_bDrawing == true) return;
	m_bDrawing = true;

	m_vSizeAsp.y = m_vMaxSize.y / (float)fMaxData;
	m_vSizeAsp.x = m_vMaxSize.x / (float)nCount;
	m_vSizeAsp.z = m_vMaxSize.z;

	// simple Level Of Details function:
	m_iLod = 6;
	if (nCount <= 150) m_iLod = 8;
	if (nCount <= 100) m_iLod = 12;
	if (nCount <= 50) m_iLod = 16;
	if (nCount <= 30) m_iLod = 32;

	glPushMatrix();
	if (m_bHoriz == true) glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

	if (m_bFrame == true) {
		if (m_bTextured) {
			//glEnable(GL_TEXTURE_2D);
			//glBindTexture(GL_TEXTURE_2D, m_iTex);
		}
		glColor3fv(m_vFrameCol.m);
		glPushMatrix();
		glTranslatef(-m_vMaxSize.x * 0.5f - vFrame.x, vFrame.y + m_vMaxSize.y * 0.5f, 0.0f);
		DrawBox(vFrame.x, m_vMaxSize.y + vFrame.y * 1.0f, vFrame.z);
		glTranslatef(m_vMaxSize.x + vFrame.x * 2.0f, 0.0f, 0.0f);
		DrawBox(vFrame.x, m_vMaxSize.y + vFrame.y * 1.0f, vFrame.z);

		glTranslatef(-m_vMaxSize.x * 0.5f - vFrame.x, -m_vMaxSize.y * 0.5f - vFrame.y, 0.0f);
		DrawBox(m_vMaxSize.x + vFrame.x * 1.0f, vFrame.y, vFrame.z);
		glTranslatef(0.0f, m_vMaxSize.y + vFrame.y * 2.0f, 0.0f);
		DrawBox(m_vMaxSize.x + vFrame.x * 1.0f, vFrame.y, vFrame.z);
		glPopMatrix();
		if (m_bTextured) { glDisable(GL_TEXTURE_2D); }
	}

	glTranslatef(-m_vMaxSize.x * 0.5f + m_vSizeAsp.x * 0.5f, vFrame.y, 0.0f);
}

void CAVSystem::EndDrawing() {
	if (m_bDrawing == true) {
		glPopMatrix();
		m_bDrawing = false;
	}
}

// end of file ----------------------------------------------------------------+
