/*-----------------------------------------------------------------------------+
| File name: gl_texture.h   											       |
| Date:		 20th March 2006				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Texture managment unit.                                                      |
+-----------------------------------------------------------------------------*/

#include "gl_texture.h"

/*-----------------------------------------------------------------------------+
|	                         LoadTextureFromBmp                                |
+------------------------------------------------------------------------------+
|  - format and filter as in OpenGL (fe. GL_RGB, GL_LINEAR, etc)               |
| Returns index of the texture...                                              |
+-----------------------------------------------------------------------------*/
GLuint LoadTextureFromBmp(const char *fname, GLenum format, GLuint filter)
{
	HBITMAP hBmp;
	BITMAP Bmp;
	char buf[40];
	GLuint t;
	int n;

	// number of data components
	switch ( format )
	{
	case GL_RGB: 
	case GL_BGR_EXT: n = 3; break;
	case GL_RGBA:
	case GL_BGRA_EXT: n = 4; break;
	case GL_LUMINANCE: n = 1; break;
	default: n = 1;
	}

	glGenTextures(1, &t);

	// load data
	hBmp = (HBITMAP)LoadImage(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBmp == NULL)
	{
		sprintf_s(buf, "Cannot load: %s file!", fname);
		throw std::exception(buf);
		return 0;
	}

	GetObject(hBmp, sizeof(Bmp), &Bmp);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, t);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// create texture using appropriate filter
	switch ( filter )
	{
	case GL_NEAREST:
	case GL_LINEAR:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			glTexImage2D(GL_TEXTURE_2D, 0, n, Bmp.bmWidth, Bmp.bmHeight,
						 0, format, GL_UNSIGNED_BYTE, Bmp.bmBits);
			break;
		}
	case GL_LINEAR_MIPMAP_NEAREST:
	case GL_LINEAR_MIPMAP_LINEAR:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
			gluBuild2DMipmaps(GL_TEXTURE_2D, n, Bmp.bmWidth, Bmp.bmHeight,
						      format, GL_UNSIGNED_BYTE, Bmp.bmBits);
			break;
		}
	}

	DeleteObject(hBmp);

	return t;
}

// end of file ----------------------------------------------------------------+