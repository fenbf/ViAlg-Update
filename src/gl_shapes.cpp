/*-----------------------------------------------------------------------------+
| File name: gl_shapes.h						  						       |
| Date:		 21st February 2006		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Some basic shapes - box, sphere, cylinder...                                 |
| all of those shapes are drawn in the center of the coordinate system         |
+-----------------------------------------------------------------------------*/

#include "gl_shapes.h"

/*-----------------------------------------------------------------------------+
|                                  DrawCube                                    |
+-----------------------------------------------------------------------------*/
void DrawCube(float size)
{
	size*=0.5f;

	glBegin(GL_QUADS);

		// front wall:
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, size);

		// right wall:
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, -size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, -size);

		// back wall:
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(size, size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, size, -size);

		// left wall:
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-size, -size, size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, size, size);

		// top wall:
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-size, size, size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, size, size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(size, size, -size);

		// bottom wall:
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-size, -size, -size);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(size, -size, -size);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(size, -size, size);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-size, -size, size);
	glEnd();
}

/*-----------------------------------------------------------------------------+
|                                  DrawCube                                    |
+-----------------------------------------------------------------------------*/
void DrawBox(float sizex, float sizey, float sizez) {
	// the code is almost the same as in the DrawCube function...
	sizex*=0.5f;
	sizey*=0.5f;
	sizez*=0.5;

	glBegin(GL_QUADS);

		// front wall:
    	glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizex, sizey, sizez);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizex, -sizey, sizez);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(sizex, -sizey, sizez);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(sizex, sizey, sizez);

		// right wall:
		glNormal3f(1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(sizex, sizey, sizez);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(sizex, -sizey, sizez);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(sizex, -sizey, -sizez);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(sizex, sizey, -sizez);

		// back wall:
		glNormal3f(0.0f, 0.0f, -1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(sizex, sizey, -sizez);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(sizex, -sizey, -sizez);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-sizex, -sizey, -sizez);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-sizex, sizey, -sizez);

		// left wall:
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizex, sizey, -sizez);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizex, -sizey, -sizez);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-sizex, -sizey, sizez);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-sizex, sizey, sizez);

		// top wall:
		glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizex, sizey, -sizez);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-sizex, sizey, sizez);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(sizex, sizey, sizez);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(sizex, sizey, -sizez);

		// bottom wall:
		glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-sizex, -sizey, -sizez);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(sizex, -sizey, -sizez);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(sizex, -sizey, sizez);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-sizex, -sizey, sizez);
	glEnd();
}

/*-----------------------------------------------------------------------------+
|                                  DrawPyramid                                 |
+-----------------------------------------------------------------------------*/
void DrawPyramid(float width, float height) {
	width*=0.5f;

	glBegin(GL_TRIANGLE_FAN);
	    // top
	    glNormal3f(0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.5f, 0.0f);
		glVertex3f(0.0f, height, 0.0f);

		glNormal3f(-0.7071f, 0.0f, 0.7071f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-width, 0.0f, width);

		glNormal3f(0.7071f, 0.0f, 0.7071f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(width, 0.0f, width);

		glNormal3f(0.7071f, 0.0f, -0.7071f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0f, -width);

		glNormal3f(-0.7071f, 0.0f, -0.7071f);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-width, 0.0f, -width);

		glNormal3f(-0.7071f, 0.0f, 0.7071f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(-width, 0.0f, width);
	glEnd();

	// floor:
	glBegin(GL_QUADS);
	    glNormal3f(0.0f, -1.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(width, 0.0f, -width);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(width, 0.0f, width);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(-width, 0.0f, width);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(-width, 0.0f, -width);
	glEnd();
}

/*-----------------------------------------------------------------------------+
|                                DrawSphere                                    |
+------------------------------------------------------------------------------+
| Creates sphere n point (0, 0, 0) withh radius: rx on X axis, ry on y axis	   |
| and rz on Z axis, nx and ny are numbers of walls...                          |
+------------------------------------------------------------------------------+
| Obtained form: "OpenGL sphere with texture coordinates" by Paul Bourke       |
+-----------------------------------------------------------------------------*/
void DrawSphere(float rx, float ry, float rz, int nx, int ny, bool spread, bool inverse)
{
   int i,j;
   double theta1,theta2,theta3;
   float ex, ey, ez, px, py, pz;
   float s[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
   float t[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
   int tid;

   if (inverse == true) glFrontFace(GL_CW);

   ny<<=1;

   for (j=0;j<ny>>1;j++)
   {
	  theta1 = (double)j * M_2_PI/(double)ny - M_PI*0.5f;
      theta2 = (double)(j + 1) * M_2_PI /(double)ny - M_PI*0.5f;

	  tid = 0;

      glBegin(GL_TRIANGLE_STRIP);
      for (i=0;i<=nx;i++) {
         theta3 = (double)i * M_2_PI/ (double)nx;

         ex = (float)(cos(theta2) * cos(-theta3));
         ey = (float)sin(theta2);
         ez = (float)(cos(theta2) * sin(-theta3));
         px = rx * ex;
         py = ry * ey;
         pz = rz * ez;

         if (inverse == false) glNormal3f(ex, ey, ez);
		 else glNormal3f(-ex, -ey, -ez);
         
		 if (spread == true) glTexCoord2f((float)i/(float)nx,2*(float)(j+1)/(float)ny);
		 else glTexCoord2f(s[tid], t[tid]);

         glVertex3f(px,py,pz);

		 tid++;

         ex = (float)(cos(theta1) * cos(-theta3));
         ey = (float)sin(theta1);
         ez = (float)(cos(theta1) * sin(-theta3));
         px = rx * ex;
         py = ry * ey;
         pz = rz * ez;

         if (inverse == false) glNormal3f(ex,ey,ez);
		 else glNormal3f(-ex, -ey, -ez);
         
		 if (spread == true) glTexCoord2f((float)i/(float)nx,2*(float)j/(float)ny);
		 else glTexCoord2f(s[tid], t[tid]);
         
		 glVertex3f(px,py,pz);

		 tid++;
		 if (tid == 4) tid = 0;
      }
      glEnd();
   }

   if (inverse == true) glFrontFace(GL_CCW);
}

/*-----------------------------------------------------------------------------+
|                                DrawCylinder                                  |
+-----------------------------------------------------------------------------*/
void DrawCylinder(float r, float h, int nx, int ny, bool spread, bool top, bool bottom)
{
	double ang, sa;
	float sy;
	float px, py, pz;
	float hbuf = h*0.5f;
	int i, j;

	sa = M_2_PI/(double)nx;
	sy = h/(float)ny;
	h*=0.5f;

	for (i = 0; i < ny; i++)
	{
		ang = 0.0f;

		glBegin(GL_TRIANGLE_STRIP);
		for (j = 0; j <= nx; j++)
		{
			px = (float)cos(ang);
			py = h;
			pz = (float)sin(-ang);

			glNormal3f(px, 0.0f, pz);
			if (spread == true) glTexCoord2f((float)(ang/M_2_PI), sy*(float)i);
			else glTexCoord2f((j % 2 == 1 ? 1.0f : 0.0f), sy*(float)i);
			glVertex3f(px*r, py, pz*r);

			glNormal3f(px, 0.0f, pz);
			if (spread == true) glTexCoord2f((float)(ang/M_2_PI), sy*(float)(i+1));
			else glTexCoord2f((j % 2 == 1 ? 1.0f : 0.0f), sy*(float)(i+1)); 
			glVertex3f(px*r, h-sy, pz*r);

			ang += sa;
		}
		glEnd();

		h-=sy;
	}

	if (top == true) {
        ang = 0.0;
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.0f);
		    glVertex3f(0.0f, hbuf, 0.0f);
		    for (i = 0; i <= nx; ++i) {
			    px = (float)cos(ang);
			    py = h;
			    pz = (float)sin(-ang);
				glNormal3f(px, 0.0f, pz);
				if (spread == true) glTexCoord2f((float)ang/M_2_PI, 1);
			    else glTexCoord2f((i % 2 == 0 ? 1.0f : 0.0f), 1);
				glVertex3f(px*r, hbuf, pz*r);
				ang+=sa;
			}
		glEnd();
	}

	if (top == true) {
        ang = M_2_PI;
		glBegin(GL_TRIANGLE_FAN);
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.5f, 0.0f);
		    glVertex3f(0.0f, -hbuf, 0.0f);
		    for (i = 0; i <= nx; ++i) {
			    px = (float)cos(ang);
			    py = h;
			    pz = (float)sin(-ang);
				glNormal3f(px, 0.0f, pz);
				if (spread == true) glTexCoord2f((float)(ang/M_2_PI), 1);
			    else glTexCoord2f((i % 2 == 0 ? 1.0f : 0.0f), 1);
				glVertex3f(px*r, -hbuf, pz*r);
				ang-=sa;
			}
		glEnd();
	}
}