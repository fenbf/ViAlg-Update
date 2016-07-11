/*-----------------------------------------------------------------------------+
| File name: gl_light.h			     										   |
| Date:		 13th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Simple CGLLight class for Open GL based applications                         |
| implementation...                                                            |
+-----------------------------------------------------------------------------*/

#include "gl_light.h"

/*-----------------------------------------------------------------------------+
|                   Implementation of the CGLLight class                       |
+-----------------------------------------------------------------------------*/

// SetAmbientColor method -----------------------------------------------------+
void CGLLight::SetAmbientColor(float r, float g, float b) {
	float fAmp[] = { r, g, b, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, fAmp);
}

// SetDiffuseColor method -----------------------------------------------------+
void CGLLight::SetDiffuseColor(float r, float g, float b) {
	float fDiff[] = { r, g, b, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiff);
}

// SetSpecularColor method ----------------------------------------------------+
void CGLLight::SetSpecularColor(float r, float g, float b) {
	float fSpec[] = { r, g, b, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, fSpec);
}

// SetPos method --------------------------------------------------------------+
void CGLLight::SetPos(float x, float y, float z) {
	float fPos[] = { x, y, z, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, fPos);
}

// end of file ----------------------------------------------------------------+