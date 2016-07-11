/*-----------------------------------------------------------------------------+
| File name: gl_shapes.h						  						       |
| Date:		 21st February 2006		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Some basic shapes - box, sphere, cylinder...                                 |
+-----------------------------------------------------------------------------*/

#ifndef GL_SHAPES_H
#define GL_SHAPES_H

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "tg_math.h"

void DrawCube(float size);
void DrawBox(float sizex, float sizey, float sizez);
void DrawPyramid(float width, float heigth);
void DrawSphere(float rx, float ry, float rz, int nx, int ny, bool spread = true, bool inverse = false);
void DrawCylinder(float r, float h, int nx, int ny, bool spread = true, bool top = true, bool bottom = true);

#endif // GL_SHAPES_H

// end of file ----------------------------------------------------------------+