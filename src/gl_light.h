/*-----------------------------------------------------------------------------+
| File name: gl_light.h			     										   |
| Date:		 13th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Simple CGLLight class for Open GL based applications                         |															   |
+-----------------------------------------------------------------------------*/

#ifndef GL_LIGHT_H
#define GL_LIGHT_H

#include <windows.h>
#include <gl/gl.h>

// Very simple light - in fact it is directional light ------------------------+
// wrapper class for OpenGL Lights
class CGLLight {
public:
	CGLLight() { }
	~CGLLight() { }

	void SetAmbientColor(float r, float g, float b);
	void SetDiffuseColor(float r, float g, float b);
	void SetSpecularColor(float r, float g, float b);
	void SetPos(float x, float y, float z);
};

#endif // GL_LIGHT_H

// end of file ----------------------------------------------------------------+
