/*-----------------------------------------------------------------------------+
| File name: gl_texture.h   											       |
| Date:		 20th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Texture managment unit.                                                      |
+-----------------------------------------------------------------------------*/

#ifndef GL_TETURE_H
	#define GL_TEXTURE_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <exception>
#include <cstdio>

// this is only a temporary unit, in the future it must be improved!
// normally it should have cTexManager class that manages all textures in App
// but unfortunalely there is no time for implementing that ;/

GLuint LoadTextureFromBmp(char *fname, GLenum format, GLuint filter);

#endif // GL_TEXTURE_H

// end of file ----------------------------------------------------------------+