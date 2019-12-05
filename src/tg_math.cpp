/*-----------------------------------------------------------------------------+
|  _________    _______                                                        |
| |___   ___| /  ______|                                                       |
|     | |     | |   ___                                                        |
|     | |     | |  |_  |   ENGINE made by FEN                                  |
|     | |     | |____| |                                                       |
|     |_|     \ ______/    based on "Tricks of the 3D game programming Gurus"  |
|                                   by Andre LaMothe                           |
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+ 
|                               tg_math.cpp                                    |
+------------------------------------------------------------------------------+
| 20th July 2004, 18:37                                                        |
+-----------------------------------------------------------------------------*/

#include "tg_math.h"

// declaration of externs variables -------------------------------------------+
float cos_look[361]; // 1 extra so we can store 0-360 inclusive
float sin_look[361]; // 1 extra so we can store 0-360 inclusive

/*-----------------------------------------------------------------------------+
|                   Implementation of the MATRIX2X2 class                      |
+-----------------------------------------------------------------------------*/

const float MATRIX2X2::IDENTITY[2][2] = { 1.0f, 0.0f, 
										  0.0f, 1.0f };

// overloaded operator * ------------------------------------------------------+
MATRIX2X2 MATRIX2X2::operator* (const MATRIX2X2& mt) const
{
	return MATRIX2X2(M00*mt.M00 + M01*mt.M10, 
					 M00*mt.M01 + M01*mt.M11,
					 M10*mt.M00 + M11*mt.M10,
					 M10*mt.M01 + M11*mt.M11);
} 

// overloaded operator *= -----------------------------------------------------+
void MATRIX2X2::operator*= (const MATRIX2X2& mt)
{
	MATRIX2X2 buf(M00*mt.M00 + M01*mt.M10, 
				  M00*mt.M01 + M01*mt.M11,
				  M10*mt.M00 + M11*mt.M10,
				  M10*mt.M01 + M11*mt.M11);
	memcpy((void *)M, (const void *)buf.M, sizeof(M));
} 

/*-----------------------------------------------------------------------------+
|                   Implementation of the MATRIX3X3 class                      |
+-----------------------------------------------------------------------------*/

const float MATRIX3X3::IDENTITY[3][3] = { 1.0f, 0.0f, 0.0f,
										  0.0f, 1.0f, 0.0f,
										  0.0f, 0.0f, 1.0f };

// overloaded operator * ------------------------------------------------------+
MATRIX3X3 MATRIX3X3::operator* (const MATRIX3X3& mt) const
{
	MATRIX3X3 buf;
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			buf.M[i][j] = M[0][j]*mt.M[i][0] +  M[1][j]*mt.M[i][1] +  M[2][j]*mt.M[i][2];

	return buf;
}

// overloaded operator * ------------------------------------------------------+
MATRIX3X3 MATRIX3X3::operator* (float v) const
{
	MATRIX3X3 buf;
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			buf.M[i][i] = M[i][i]*v;

	return buf;
}

// overloaded operator + ------------------------------------------------------+
MATRIX3X3 MATRIX3X3::operator+ (const MATRIX3X3& mt) const
{
	MATRIX3X3 buf;
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			buf.M[i][j] = M[i][i] + mt.M[i][j];

	return buf;
}

// overloaded operator *= -----------------------------------------------------+
void MATRIX3X3::operator*= (const MATRIX3X3& mt)
{
	MATRIX3X3 buf;
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			buf.M[i][j] = M[0][j]*mt.M[i][0] +  M[1][j]*mt.M[i][1] +  M[2][j]*mt.M[i][2];

	memcpy((void *)M, (const void *)buf.M, sizeof(MATRIX3X3));
}

// overloaded operator *= -----------------------------------------------------+
void MATRIX3X3::operator*= (float v)
{
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			M[i][j] *= v;
}

// overloaded operator += -----------------------------------------------------+
void MATRIX3X3::operator+= (const MATRIX3X3& mt)
{
	int i, j;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			M[i][j] += mt.M[i][j];
}

/*-----------------------------------------------------------------------------+
|                   Implementation of the MATRIX4X4 class                      |
+-----------------------------------------------------------------------------*/

const float MATRIX4X4::IDENTITY[4][4] = { 1.0f, 0.0f, 0.0f, 0.0f,
										  0.0f, 1.0f, 0.0f, 0.0f,
										  0.0f, 0.0f, 1.0f, 0.0f,
										  0.0f, 0.0f, 0.0f, 1.0f };

// overloaded operator * ------------------------------------------------------+
MATRIX4X4 MATRIX4X4::operator* (const MATRIX4X4& mt) const
{
	MATRIX4X4 buf;
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			buf.M[i][j] = M[0][j]*mt.M[i][0] +  M[1][j]*mt.M[i][1] +  M[2][j]*mt.M[i][2] + M[3][j]*mt.M[i][3];

	return buf;
}

// overloaded operator * ------------------------------------------------------+
MATRIX4X4 MATRIX4X4::operator* (float v) const
{
	MATRIX4X4 buf;
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			buf.M[i][i] = M[i][i]*v;

	return buf;
}

// overloaded operator + ------------------------------------------------------+
MATRIX4X4 MATRIX4X4::operator+ (const MATRIX4X4& mt) const
{
	MATRIX4X4 buf;
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			buf.M[i][j] = M[i][j] + mt.M[i][j];

	return buf;
}

// overloaded operator *= -----------------------------------------------------+
void MATRIX4X4::operator*= (const MATRIX4X4& mt)
{
	MATRIX4X4 buf;
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			buf.M[i][j] = M[0][j]*mt.M[i][0] +  M[1][j]*mt.M[i][1] +  M[2][j]*mt.M[i][2] + M[3][j]*mt.M[i][3];;

	memcpy((void *)M, (const void *)buf.M, sizeof(MATRIX4X4));
}

// overloaded operator *= -----------------------------------------------------+
void MATRIX4X4::operator*= (float v)
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			M[i][j] *= v;
}

// overloaded operator += -----------------------------------------------------+
void MATRIX4X4::operator+= (const MATRIX4X4& mt)
{
	int i, j;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			M[i][j] += mt.M[i][j];
}

/*-----------------------------------------------------------------------------+
|                  Implementation of the QUATERNION class                      |
+-----------------------------------------------------------------------------*/

// overloaded operator * ------------------------------------------------------+
QUATERNION QUATERNION::operator* (const QUATERNION& q)
{
	QUATERNION qprod;
	// standard version:
	//qprod.w = w*q.w - x*q.x - y*q.y - z*q.z;
	//qprod.x = w*q.x + x*q.w + y*q.z - z*q.y;
	//qprod.y = w*q.y - x*q.z + y*q.w - z*q.x;
	//qprod.z = w*q.z + x*q.y - y*q.x + z*q.w;

	// optimalized version:
	float prd_0 = (z - y) * (q.y - q.z);
	float prd_1 = (w + x) * (q.w + q.x);
	float prd_2 = (w - x) * (q.y + q.z);
	float prd_3 = (y + z) * (q.w - q.x);
	float prd_4 = (z - x) * (q.x - q.y);
	float prd_5 = (z + x) * (q.x + q.y);
	float prd_6 = (w + y) * (q.w - q.z);
	float prd_7 = (w - y) * (q.w + q.z);

	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5f * (prd_4 + prd_8);

	qprod.w = prd_0 + prd_9 - prd_5;
	qprod.x = prd_1 + prd_9 - prd_8;
	qprod.y = prd_2 + prd_9 - prd_7;
	qprod.z = prd_3 + prd_9 - prd_6;

	return qprod;
}

// overloaded operator * ------------------------------------------------------+
void QUATERNION::operator*= (const QUATERNION& q)
{
	// standard version:
	//QUATERNION qprod;
	//qprod.w = w*q.w - x*q.x - y*q.y - z*q.z;
	//qprod.x = w*q.x + x*q.w + y*q.z - z*q.y;
	//qprod.y = w*q.y - x*q.z + y*q.w - z*q.x;
	//qprod.z = w*q.z + x*q.y - y*q.x + z*q.w;
	//memcpy((void *)m, (const void *)qprod.m, sizeof(QUATERNION));
	
	// optimalized version:
	float prd_0 = (z - y) * (q.y - q.z);
	float prd_1 = (w + x) * (q.w + q.x);
	float prd_2 = (w - x) * (q.y + q.z);
	float prd_3 = (y + z) * (q.w - q.x);
	float prd_4 = (z - x) * (q.x - q.y);
	float prd_5 = (z + x) * (q.x + q.y);
	float prd_6 = (w + y) * (q.w - q.z);
	float prd_7 = (w - y) * (q.w + q.z);

	float prd_8 = prd_5 + prd_6 + prd_7;
	float prd_9 = 0.5f * (prd_4 + prd_8);

	w = prd_0 + prd_9 - prd_5;
	x = prd_1 + prd_9 - prd_8;
	y = prd_2 + prd_9 - prd_7;
	z = prd_3 + prd_9 - prd_6;
}

/*-----------------------------------------------------------------------------+
|                         Function impelementations                            |
+-----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------+
|                              Vec2dTransform                                  |
+-----------------------------------------------------------------------------*/
VECTOR2D Vec2dTransform(const VECTOR2D_PTR v, const MATRIX3X3_PTR mt)
{
	VECTOR2D buf;
	buf.x = v->x * mt->M00 + v->y * mt->M10 + mt->M20;
	buf.y = v->x * mt->M01 + v->y * mt->M11 + mt->M21;
	return buf;
}

/*-----------------------------------------------------------------------------+
|                              Vec3dTransform                                  |
+-----------------------------------------------------------------------------*/
VECTOR3D Vec3dTransform(const VECTOR3D_PTR v, const MATRIX4X4_PTR mt)
{
	VECTOR3D buf;
	buf.x = v->x*mt->M00 + v->y*mt->M10 + v->z*mt->M20 + mt->M30;
	buf.y = v->x*mt->M01 + v->y*mt->M11 + v->z*mt->M21 + mt->M31;
	buf.z = v->x*mt->M02 + v->y*mt->M12 + v->z*mt->M22 + mt->M32;
	return buf;
}

/*-----------------------------------------------------------------------------+
|                              Vec34Transform                                  |
+-----------------------------------------------------------------------------*/
VECTOR4D Vec4dTransform(const VECTOR4D_PTR v, const MATRIX4X4_PTR mt)
{
	VECTOR4D buf;
	buf.x = v->x*mt->M00 + v->y*mt->M10 + v->z*mt->M20 + mt->M30;
	buf.y = v->x*mt->M01 + v->y*mt->M11 + v->z*mt->M21 + mt->M31;
	buf.z = v->x*mt->M02 + v->y*mt->M12 + v->z*mt->M22 + mt->M32;
	//buf.w = v->x*mt->M03 + v->y*mt->M13 + v->z*mt->M23 + mt->M33;
	buf.w = 1.0f;
	return buf;
}

/*-----------------------------------------------------------------------------+
|                            Polar2dToPoint2d                                  |
+-----------------------------------------------------------------------------*/
POINT2D Polar2dToPoint2d(const POLAR2D_PTR p)
{
	return POINT2D(p->r*cosf(p->theta), p->r*sinf(p->theta));
}

/*-----------------------------------------------------------------------------+
|                            Point2dToPolar2d                                  |
+-----------------------------------------------------------------------------*/
POLAR2D Point2dToPolar2d(const POINT2D_PTR p)
{
	POLAR2D buf;
	buf.r = Vec2dLength(p);
	buf.theta = atanf(p->y/p->x);		 // division by 0 may happen!
	return buf;
}

/*-----------------------------------------------------------------------------+
|                          Spherical3dToPoint3D                                |
+-----------------------------------------------------------------------------*/
POINT3D Spherical3dToPoint3d(const SPHERICAL3D_PTR s)
{
	POINT3D pt;
	float r;

	r = s->p*sinf(s->phi);
	pt.z = s->p*cosf(s->phi);

	pt.x = r*cosf(s->theta);
	pt.y = r*sinf(s->theta);
	return pt;
}

/*-----------------------------------------------------------------------------+
|                          Point3dToSpherical3d                                |
+-----------------------------------------------------------------------------*/
SPHERICAL3D Point3dToSpherical3d(const POINT3D_PTR p)
{
	SPHERICAL3D sph;

	sph.p = Vec3dLength(p);

	sph.theta = atanf(p->y/p->x);

	float r = sph.p*sinf(sph.phi);

	sph.phi   = asinf(r/sph.p);
	
	return sph;
}

/*-----------------------------------------------------------------------------+
|                         Cylindrical3dToPoint3d                               |
+-----------------------------------------------------------------------------*/
POINT3D Cylindrical3dToPoint3d(const CYLINDRICAL3D_PTR s)
{
	return POINT3D(s->r*cosf(s->theta), s->r*sinf(s->theta), s->z);
}

/*-----------------------------------------------------------------------------+
|                         Point3dToCylindrical3d                               |
+-----------------------------------------------------------------------------*/
CYLINDRICAL3D Point3dToCylindrical3d(const POINT3D_PTR p)
{
	CYLINDRICAL3D c;
	c.r = Vec3dLength(p);
	c.theta = atanf(p->y/p->x);
	c.z = p->z;
	return c;
}

/*-----------------------------------------------------------------------------+
|                                 Mat2x2Inverse                                |
+-----------------------------------------------------------------------------*/
int Mat2x2Inverse(const MATRIX2X2_PTR mt, MATRIX2X2_PTR out)
{
	float d = Mat2x2Det(mt);

	if (ABS(d) < EPSILON_E5)
		return 0;
	
	d = 1.0f/d;
	out->M00 = d*mt->M11;
	out->M01 = -d*mt->M01;
	out->M10 = -d*mt->M10;
	out->M11 = d*mt->M00;

	return 1;
}

/*-----------------------------------------------------------------------------+
|                                 Mat3x3Det                                    |
+-----------------------------------------------------------------------------*/
float Mat3x3Det(const MATRIX3X3_PTR mt)
{
	return (mt->M00*(mt->M11*mt->M22 - mt->M21*mt->M12) - 
			mt->M01*(mt->M10*mt->M22 - mt->M20*mt->M12) + 
			mt->M02*(mt->M10*mt->M21 - mt->M20*mt->M11) );
}

/*-----------------------------------------------------------------------------+
|                                 Mat3x3Inverse                                |
+-----------------------------------------------------------------------------*/
int Mat3x3Inverse(const MATRIX3X3_PTR mt, MATRIX3X3_PTR out)
{
	float d = Mat3x3Det(mt);

	if (ABS(d) < EPSILON_E5)
		return 0;
	
	d = 1.0f/d;
	out->M00 =  d*(mt->M11*mt->M22 - mt->M21*mt->M12);
	out->M10 = -d*(mt->M10*mt->M22 - mt->M20*mt->M12);
	out->M20 =  d*(mt->M10*mt->M21 - mt->M20*mt->M11);

	out->M01 = -d*(mt->M01*mt->M22 - mt->M21*mt->M02);
	out->M11 =  d*(mt->M00*mt->M22 - mt->M20*mt->M02);
	out->M21 = -d*(mt->M00*mt->M21 - mt->M20*mt->M01);

	out->M02 =  d*(mt->M01*mt->M12 - mt->M11*mt->M02);
	out->M12 = -d*(mt->M00*mt->M12 - mt->M10*mt->M02);
	out->M22 =  d*(mt->M00*mt->M11 - mt->M10*mt->M01);

	return 1;
}

/*-----------------------------------------------------------------------------+
| 		                     Mat3x3Transformation			                   |
+-----------------------------------------------------------------------------*/
MATRIX3X3 Mat3x3Transformation(float a, float sx, float sy,  float x, float y)
{
	return MATRIX3X3(sx*cosf(a),  sy*sinf(a), 0.0f,
					 sx*-sinf(a), sy*cosf(a), 0.0f,
					 sy*x,        sy*y,       1.0f);
}

/*-----------------------------------------------------------------------------+
| 		                        Mat4x4Transpose  		                       |
+-----------------------------------------------------------------------------*/
MATRIX4X4 Mat4x4Transpose(const MATRIX4X4_PTR mt)
{
	return MATRIX4X4(mt->M00, mt->M10, mt->M20, mt->M30, 
					 mt->M01, mt->M11, mt->M21, mt->M31, 
					 mt->M02, mt->M12, mt->M22, mt->M32, 
					 mt->M03, mt->M13, mt->M22, mt->M33);
}

/*-----------------------------------------------------------------------------+
|                                Mat4x4Inverse                                 |
+-----------------------------------------------------------------------------*/
int Mat4x4Inverse(MATRIX4X4_PTR m, MATRIX4X4_PTR mi)
{
	// calc det assuming that the last row of matrix m is {0, 0, 0, 1)
	float det =  ( m->M00 * ( m->M11 * m->M22 - m->M12 * m->M21 ) -
				   m->M01 * ( m->M10 * m->M22 - m->M12 * m->M20 ) +
				   m->M02 * ( m->M10 * m->M21 - m->M11 * m->M20 ) );

	
	// are we able to find inverse matrix?
	if (fabs(det) < EPSILON_E5)
		return 0;

	det = 1.0f/det;

	mi->M00 =  det * ( m->M11 * m->M22 - m->M12 * m->M21 );
	mi->M01 = -det * ( m->M01 * m->M22 - m->M02 * m->M21 );
	mi->M02 =  det * ( m->M01 * m->M12 - m->M02 * m->M11 );
	mi->M03 = 0.0f; // always 0

	mi->M10 = -det * ( m->M10 * m->M22 - m->M12 * m->M20 );
	mi->M11 =  det * ( m->M00 * m->M22 - m->M02 * m->M20 );
	mi->M12 = -det * ( m->M00 * m->M12 - m->M02 * m->M10 );
	mi->M13 = 0.0f; // always 0

	mi->M20 =  det * ( m->M10 * m->M21 - m->M11 * m->M20 );
	mi->M21 = -det * ( m->M00 * m->M21 - m->M01 * m->M20 );
	mi->M22 =  det * ( m->M00 * m->M11 - m->M01 * m->M10 );
	mi->M23 = 0.0f; // always 0

	mi->M30 = -( m->M30 * mi->M00 + m->M31 * mi->M10 + m->M32 * mi->M20 );
	mi->M31 = -( m->M30 * mi->M01 + m->M31 * mi->M11 + m->M32 * mi->M21 );
	mi->M32 = -( m->M30 * mi->M02 + m->M31 * mi->M12 + m->M32 * mi->M22 );
	mi->M33 = 1.0f; // always 0

	return 1;
}

/*-----------------------------------------------------------------------------+
| 		                       Mat4x4RotationX			                       |
+-----------------------------------------------------------------------------*/
MATRIX4X4 Mat4x4RotationX(float ang)
{
	float s = sinf(ang);
	float c = cosf(ang);
 	return MATRIX4X4(1.0f, 0.0f,  0.0f,  0.0f,
		             0.0f, c,     s,     0.0f,
					 0.0f, -s,    c,     0.0f,
					 0.0f, 0.0f,  0.0f,  1.0f);
}

/*-----------------------------------------------------------------------------+
| 		                       Mat4x4RotationY			                       |
+-----------------------------------------------------------------------------*/
MATRIX4X4 Mat4x4RotationY(float ang)
{
	float s = sinf(ang);
	float c = cosf(ang);
 	return MATRIX4X4(c,    0.0f, -s,   0.0f,
		             0.0f, 1.0f, 0.0f, 0.0f, 
					 s,    0.0f, c,    0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

/*-----------------------------------------------------------------------------+
| 		                       Mat4x4RotationZ			                       |
+-----------------------------------------------------------------------------*/
MATRIX4X4 Mat4x4RotationZ(float ang)
{
	float s = sinf(ang);
	float c = cosf(ang);
 	return MATRIX4X4(c,    s,    0.0f, 0.0f,
		             -s,   c,    0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,
					 0.0f, 0.0f, 0.0f, 1.0f);
}

/*-----------------------------------------------------------------------------+
| 		                     Mat4x4VectorRotation			                   |
+------------------------------------------------------------------------------+
| "vn" is the axis vector, it must be normalised!							   |
+-----------------------------------------------------------------------------*/
MATRIX4X4 Mat4x4VectorRotation(float ang, const VECTOR3D_PTR vn)
{
	MATRIX4X4 ret;
	float sinA, cosA, invCosA;
	float x, y, z;
	float xSq, ySq, zSq;

	sinA = sinf(ang);
	cosA = cosf(ang);
	invCosA = 1.0f - cosA;

	x = vn->x;
	y = vn->y;
	z = vn->z;

	xSq = x * x;
	ySq = y * y;
	zSq = z * z;

	ret.M[0][0] = (invCosA * xSq) + (cosA);
	ret.M[1][0] = (invCosA * x * y) - (sinA * z );
	ret.M[2][0] = (invCosA * x * z) + (sinA * y );
	ret.M[3][0] = 0.0f;
  
	ret.M[0][1] = (invCosA * x * y) + (sinA * z);
	ret.M[1][1] = (invCosA * ySq) + (cosA);
	ret.M[2][1] = (invCosA * y * z) - (sinA * x);
	ret.M[3][1] = 0.0f;

	ret.M[0][2] = (invCosA * x * z) - (sinA * y);
	ret.M[1][2] = (invCosA * y * z) + (sinA * x);
	ret.M[2][2] = (invCosA * zSq) + (cosA);
	ret.M[3][2] = 0.0f;

	ret.M[0][3] = 0.0F;
	ret.M[1][3] = 0.0F;
	ret.M[2][3] = 0.0F;
	ret.M[3][3] = 1.0f;

	return ret;
}

/*-----------------------------------------------------------------------------+
| 		                     Mat4x4Transformation			                   |
+-----------------------------------------------------------------------------*/
MATRIX4X4 Mat4x4Transformation(float rx, float ry, float rz, 
							   float sx, float sy, float sz, 
							   float x, float y, float z)
{
	return MATRIX4X4(1.0f, 0.0f, 0.0f, 0.0f,
					 0.0f, 1.0f, 0.0f, 0.0f,
					 0.0f, 0.0f, 1.0f, 0.0f,	   // not implemented yet
					 0.0f, 0.0f, 0.0f, 1.0f);
}

/*-----------------------------------------------------------------------------+
| 		                       QuaternionNormalize			                   |
+-----------------------------------------------------------------------------*/
void QuaternionNormalize(const QUATERNION_PTR q)
{
	float l = 1.0f/(sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z));

	q->w=q->w*l;
	q->x=q->x*l;
	q->y=q->y*l;
	q->z=q->z*l;	
}

/*-----------------------------------------------------------------------------+
| 		                       QuaternionInverse			                   |
+-----------------------------------------------------------------------------*/
QUATERNION QuaternionInverse(const QUATERNION_PTR q)
{
	float l = 1.0f/(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);

	return QUATERNION(q->w*l, -q->x*l, -q->y*l, -q->z*l);
}

/*-----------------------------------------------------------------------------+
|                          QuaternionAxisRotation                              |
+-----------------------------------------------------------------------------*/
QUATERNION QuaternionAxisRotation(const VECTOR3D_PTR axis, float rot)
{
	float t = 0.5f*rot;

	float st = sinf(t);

	return QUATERNION(cosf(t), st*axis->x, st*axis->y, st*axis->z);
}

/*-----------------------------------------------------------------------------+
|                          QuaternionZYXRotation                               |
+-----------------------------------------------------------------------------*/
QUATERNION QuaternionZYXRotation(float rx, float ry, float rz)
{
	float cos_z_2 = 0.5f*cosf(rz);
	float cos_y_2 = 0.5f*cosf(ry);
	float cos_x_2 = 0.5f*cosf(rx);

	float sin_z_2 = 0.5f*sinf(rz);
	float sin_y_2 = 0.5f*sinf(ry);
	float sin_x_2 = 0.5f*sinf(rx);

	return QUATERNION(cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2,
				cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2,
				cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2,
				sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2);
}

/*-----------------------------------------------------------------------------+
|                       QuaternionToVector3dAndAngle                           |
+-----------------------------------------------------------------------------*/
void QuaternionToVector3dAndAngle(const QUATERNION_PTR q, VECTOR3D_PTR axis, float &rot)
{
	rot = acosf(q->w);

	float buf = 1.0f/sinf(rot);

	axis->x = q->x*buf;
	axis->y = q->y*buf;
	axis->z = q->z*buf;

	rot*=2.0f;
}

/*-----------------------------------------------------------------------------+
|                            ComputePointOnLine2d                              |
+-----------------------------------------------------------------------------*/
POINT2D ComputePointOnLine2d(const PARAMLINE2D_PTR line, float t)
{
	return POINT2D(line->p0.x + line->v.x*t, 
				   line->p0.y + line->v.y*t);
}

/*-----------------------------------------------------------------------------+
|                            ComputePointOnLine3d                              |
+-----------------------------------------------------------------------------*/
POINT3D ComputePointOnLine3d(const PARAMLINE3D_PTR line, float t)
{
	return POINT3D(line->p0.x + line->v.x*t, 
				   line->p0.y + line->v.y*t,
				   line->p0.z + line->v.z*t);
}

/*-----------------------------------------------------------------------------+
|                          IntersectLine2dAndLine2d                            |
+-----------------------------------------------------------------------------*/
int IntersectLine2dAndLine2d(const PARAMLINE2D_PTR l1, const PARAMLINE2D_PTR l2,
							 float& t1, float& t2)
{	
	float det = l1->v.x*l2->v.y - l1->v.y*l2->v.x;
	if (ABS(det) < EPSILON_E5)
	{
		return INTERSECT_NO;
	}

	t1 = (l2->v.x*(l1->p0.y - l2->p0.y) - l2->v.y*(l1->p0.x - l2->p0.x))/det;

	t2 = (l1->v.x*(l1->p0.y - l2->p0.y) - l1->v.y*(l1->p0.x - l2->p0.x))/det;

	if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
		return INTERSECT_IN_SEGMENT;
	else
		return INTERSECT_OUT_SEGMENT;
}

/*-----------------------------------------------------------------------------+
|                          IntersectLine2dAndLine2d                            |
+-----------------------------------------------------------------------------*/
int IntersectLine2dAndLine2d(const PARAMLINE2D_PTR l1, const PARAMLINE2D_PTR l2, POINT2D_PTR pt)
{	
	float det = l1->v.x*l2->v.y - l1->v.y*l2->v.x;
	if (ABS(det) < EPSILON_E5)
	{
		return INTERSECT_NO;
	}

	float t1 = (l2->v.x*(l1->p0.y - l2->p0.y) - l2->v.y*(l1->p0.x - l2->p0.x))/det;

	float t2 = (l1->v.x*(l1->p0.y - l2->p0.y) - l1->v.y*(l1->p0.x - l2->p0.x))/det;

	pt->x = l1->p0.x + l1->v.x*t1;
	pt->y = l1->p0.y + l1->v.y*t1;

	if (t1 >= 0.0f && t1 <= 1.0f && t2 >= 0.0f && t2 <= 1.0f)
		return INTERSECT_IN_SEGMENT;
	else
		return INTERSECT_OUT_SEGMENT;
}

/*-----------------------------------------------------------------------------+
|                             DistanceToPlane3d                                |
+-----------------------------------------------------------------------------*/
float DistanceToPlane3d(const PLANE3D* plane, const POINT3D* p)
{
	return (plane->n.x * (p->x - plane->p0.x) +
			plane->n.y * (p->y - plane->p0.y) +
			plane->n.z * (p->z - plane->p0.z));
}

/*-----------------------------------------------------------------------------+
|                         IntersectLine3dAndPlane3d                            |
+-----------------------------------------------------------------------------*/
int IntersectLine3dAndPlane3d(const PARAMLINE3D_PTR line, 
							  const PLANE3D_PTR plane, float &t, POINT3D_PTR pt)
{
	float plane_dot_line = Vec3dDotProduct(&line->v, &plane->n);

	if (ABS(plane_dot_line) <= EPSILON_E5)
    {   
		if (ABS(DistanceToPlane3d(plane, &line->p0)) <= EPSILON_E5)
			return INTERSECT_EVERYWHERE;
		else
			return INTERSECT_NO;
	}

	t = -(plane->n.x*line->p0.x + 
		  plane->n.y*line->p0.y + 
          plane->n.z*line->p0.z -
          plane->n.x*plane->p0.x - 
          plane->n.y*plane->p0.y - 
          plane->n.z*plane->p0.z) / (plane_dot_line);
   
	pt->x = line->p0.x + line->v.x*t;
	pt->y = line->p0.y + line->v.y*t;
	pt->z = line->p0.z + line->v.z*t;

	if (t >= 0.0f && t <= 1.0f)
		return INTERSECT_IN_SEGMENT;
	else
		return INTERSECT_OUT_SEGMENT;
}

/*-----------------------------------------------------------------------------+
|                              BuildSinCosTables                               |
+-----------------------------------------------------------------------------*/
void BuildSinCosTables()
{
	for (int ang = 0; ang <= 360; ang++)
	{
		// convert ang to radians
		float theta = DegToRad((float)ang);

		cos_look[ang] = cosf(theta);
		sin_look[ang] = sinf(theta);
    }
}

/*-----------------------------------------------------------------------------+
|                               Solve2x2System                                 |
+------------------------------------------------------------------------------+
| "a" holds factors of the equation, "b" holds consts, "x" is the result...    |
+-----------------------------------------------------------------------------*/
int Solve2x2System(const MATRIX2X2_PTR a, const VECTOR2D_PTR b, VECTOR2D_PTR x)
{
	float det_a = Mat2x2Det(a);

	if (ABS(det_a) < EPSILON_E5)
		return 0;

	MATRIX2X2 buf;

	// find x->x:
	buf = *a;
	buf.M00 = b->x;
	buf.M10 = b->y;
	
	x->x = Mat2x2Det(&buf)/det_a;

	// find x->y:
	Mat2x2Copy(&buf, a);
	buf.M01 = b->x;
	buf.M11 = b->y;
	
	x->y = Mat2x2Det(&buf)/det_a;

	return 1;
} 

/*-----------------------------------------------------------------------------+
|                              Solve3x3System                                  |
+------------------------------------------------------------------------------+
| "a" holds factors of the equation, "b" holds consts, "x" is the result...    |
+-----------------------------------------------------------------------------*/
int Solve3x3System(const MATRIX3X3_PTR a, const VECTOR3D_PTR b, VECTOR3D_PTR x)
{
	float det_a = Mat3x3Det(a);

	if (ABS(det_a) < EPSILON_E5)
		return 0;

	MATRIX3X3 buf;

	// find x->x:
	Mat3x3Copy(&buf, a);
	buf.M00 = b->x;
	buf.M10 = b->y;
	buf.M20 = b->z;
	x->x = Mat3x3Det(&buf)/det_a;

	// find x->y:
	Mat3x3Copy(&buf, a);
	buf.M01 = b->x;
	buf.M11 = b->y;
	buf.M21 = b->z;
	x->y = Mat3x3Det(&buf)/det_a;

	// find x->x:
	Mat3x3Copy(&buf, a);
	buf.M02 = b->x;
	buf.M12 = b->y;
	buf.M22 = b->z;
	x->z = Mat3x3Det(&buf)/det_a;

	return 1;
}

/*-----------------------------------------------------------------------------+
|                                  FastSin                                     |
+------------------------------------------------------------------------------+
| This function uses precalculated sinus array, so one must call			   |
| BuildSinCosTable before using it                                             |
+-----------------------------------------------------------------------------*/
float FastSin(float ang)
{
	ang = (float)((int)(ang)%360);
	if (ang < 0) ang+=360.0f;

	int a = (int)ang;
	float f = ang-(float)a;

	return (sin_look[a] + f*(sin_look[a+1] - sin_look[a]));
}

/*-----------------------------------------------------------------------------+
|                                  FastCos                                     |
+------------------------------------------------------------------------------+
| This function uses precalculated cosinus array, so one must call			   |
| BuildSinCosTable before using it                                             |
+-----------------------------------------------------------------------------*/
float FastCos(float ang)
{
	ang = (float)((int)(ang)%360);
	if (ang < 0) ang+=360.0f;

	int a = (int)ang;
	float f = ang-(float)a;

	return (cos_look[a] + f*(cos_look[a+1] - cos_look[a]));
}

/*-----------------------------------------------------------------------------+
|                                FastDistance2d                                | 
+------------------------------------------------------------------------------+
| this function computes the distance from 0,0 to x,y with 3.5% error          |
+-----------------------------------------------------------------------------*/
int FastDistance2d(int x, int y)
{
	// first compute the absolute value of x,y
	x = ABS(x);
	y = ABS(y);

	// compute the minimum of x,y
	int mn = Min(x, y);

	// return the distance
	return (x+y-(mn>>1)-(mn>>2)+(mn>>4));
}

/*-----------------------------------------------------------------------------+
|                                FastDistance3d                                | 
+------------------------------------------------------------------------------+
| this function computes the distance from 0,0,0  to x, y, z with 8% error     |
+-----------------------------------------------------------------------------*/
float FastDistance3d(float fx, float fy, float fz)
{
	int x, y, z; // used for algorithm

	// make sure values are all positive
	x = (int)(ABS(fx)) * 1024;
	y = (int)(ABS(fy)) * 1024;
	z = (int)(ABS(fz)) * 1024;

	// sort values
	if (y < x) Swap(x,y);

	if (z < y) Swap(y,z);

	if (y < x) Swap(x,y);

	int dist = (z + 11 * (y >> 5) + (x >> 2) );

	return ((float)(dist >> 10));
}

// end of file ----------------------------------------------------------------+
