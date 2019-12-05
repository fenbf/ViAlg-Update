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
|                               tg_math.hpp                                    |
+------------------------------------------------------------------------------+
| 9th July 2004, 9:17                                                          |
+-----------------------------------------------------------------------------*/

#ifndef TG_MATH_HPP
	#define TG_MATH_HPP
	

// #future: think about replacing that math lib with glm maybe?

#include <cmath>
#include <cassert>
#include <cstring>			// for such functions as memcpy...

#ifndef ABS
	#define ABS(x) ( (x) < 0 ? -(x) : (x) )
#endif

// very small numbers ---------------------------------------------------------+
#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

#define FLOAT_COMPARE_DELTA (float)(1E-5)
	
// math basic constances ------------------------------------------------------+
#ifndef M_PI
	#define M_PI 		3.1415926535897932384626433832795f
	#define M_2_PI 		6.28318530717958647692528676655901f		// PI*2
	//#define M_05_PI     1,57079632679489661923132169163975f   // PI*0.5
	//#define M_025_PI 	0.785398163397448309615660845819876f    // PI*0.25
	#define M_INV_PI	0.318309886183790671537767526745029		// 1/PI
#endif

// for parametric lines and planes --------------------------------------------+
#define INTERSECT_NO 0
#define INTERSECT_IN_SEGMENT 1
#define INTERSECT_OUT_SEGMENT 2
#define INTERSECT_EVERYWHERE 3

// useful inline functions ----------------------------------------------------+
inline float DegToRad(float a) { return a*0.01745329252f; };  // ang * M_PI / 180.0f
inline float RadToDeg(float a) { return a*57.29577951f; };    // rads * 180.0f / M_PI

inline bool AreFloatsEqual(float a, float b) { return ( ABS(a-b) < FLOAT_COMPARE_DELTA ? true : false ); }

template <class T> inline T Min(const T& a, const T& b) { return ( a < b ? a : b ); }
template <class T> inline T Max(const T& a, const T& b) { return ( a > b ? a : b ); }

template <class T> inline void Swap(T& a, T& b) { T t(a); a = b; b = t; }

template <class T> inline T Abs(const T& a) { return ( a < 0 ? -a : a ); }

// extern variables -----------------------------------------------------------+
extern float cos_look[361]; // 1 extra so we can store 0-360 inclusive
extern float sin_look[361]; // 1 extra so we can store 0-360 inclusive
	
/*-----------------------------------------------------------------------------+
|                           class predeclarations                              |
+-----------------------------------------------------------------------------*/
class VECTOR2D;
class VECTOR3D;
class VECTOR4D;

class POLAR2D;
class SPHERICAL3D;
class CYLINDRICAL3D;

class MATRIX2X2;
class MATRIX3X3;
class MATRIX4X4;

class QUATERNION;

class PARAMLINE2D;
class PARAMLINE3D;
class PLANE3D;

// typedefs -------------------------------------------------------------------+
typedef VECTOR2D POINT2D;
typedef VECTOR2D* VECTOR2D_PTR;
typedef VECTOR2D* POINT2D_PTR;

typedef VECTOR3D POINT3D;
typedef VECTOR3D* VECTOR3D_PTR;
typedef VECTOR3D* POINT3D_PTR;	

typedef VECTOR4D POINT4D;
typedef VECTOR4D* VECTOR4D_PTR;
typedef VECTOR4D* POINT4D_PTR;

typedef POLAR2D* POLAR2D_PTR;
typedef SPHERICAL3D* SPHERICAL3D_PTR;
typedef CYLINDRICAL3D* CYLINDRICAL3D_PTR;

typedef MATRIX2X2* MATRIX2X2_PTR;
typedef MATRIX3X3* MATRIX3X3_PTR;
typedef MATRIX4X4* MATRIX4X4_PTR;

typedef QUATERNION* QUATERNION_PTR;

typedef PARAMLINE2D* PARAMLINE2D_PTR;
typedef PARAMLINE3D* PARAMLINE3D_PTR;
typedef PLANE3D* PLANE3D_PTR;

/*-----------------------------------------------------------------------------+
|                             class declarations                               |
+-----------------------------------------------------------------------------*/

// VECTOR2D class -------------------------------------------------------------+
class VECTOR2D
{
public:
	// VECTOR2D(): x(0.0f), y(0.0f) { }
	VECTOR2D() { }
	VECTOR2D(float a, float b): x(a), y(b) { }
	VECTOR2D(const VECTOR2D& v): x(v.x), y(v.y) { }
	~VECTOR2D() { }
	
	// friend operators:
	friend bool operator== (const VECTOR2D& a, const VECTOR2D& b) { return (a.x == b.y && a.y == b.y); }
	friend bool operator!= (const VECTOR2D& a, const VECTOR2D& b) { return (a.x != b.y || a.y != b.y); }

	friend VECTOR2D operator- (const VECTOR2D& a)					 { return VECTOR2D(-a.x, -a.y); }
	friend VECTOR2D operator+ (const VECTOR2D& a, const VECTOR2D& b) { return VECTOR2D(a.x+b.x, a.y+b.y); }
	friend VECTOR2D operator- (const VECTOR2D& a, const VECTOR2D& b) { return VECTOR2D(a.x-b.x, a.y-b.y); }
	friend VECTOR2D operator* (const VECTOR2D& a, float v) 			 { return VECTOR2D(a.x*v, a.y*v); }
	friend VECTOR2D operator* (float v, const VECTOR2D& a) 			 { return VECTOR2D(a.x*v, a.y*v); }
	
	// "normal" operators:
	VECTOR2D &operator= (const VECTOR2D& v) { x = v.x; y = v.y; return *this; }
	
	void operator+= (const VECTOR2D& v)		{ x+=v.x; y+=v.y; }
	void operator-= (const VECTOR2D& v)		{ x-=v.x; y-=v.y; }
	void operator*= (float v)				{ x*=v; y*=v; }

	float &operator [] (unsigned int id)      { assert(id < 2); return m[id]; } 
	float operator [] (unsigned int id) const { assert(id < 2); return m[id]; }  

    // casting to a float array
	//operator float* ()			{ return &x; }
	//operator const float* ()	{ return &x; }

	// members:
	union
	{
		float m[2];
		
		struct
		{
			float x, y;
		};
	}; 
};

// VECTOR3D class -------------------------------------------------------------+
class VECTOR3D
{
public:
	// VECTOR3D(): x(0.0f), y(0.0f), z(0.0f) { }
	VECTOR3D() { }
	VECTOR3D(float a, float b, float c): x(a), y(b), z(c) { }
	VECTOR3D(const VECTOR3D& v): x(v.x), y(v.y), z(v.z) { }
	VECTOR3D(const VECTOR2D& v): x(v.x), y(v.y), z(0.0f) { }
	//VECTOR3D(const VECTOR4D& v): x(v.x), y(v.y), z(v.z) { }
	~VECTOR3D() { }
	
	// friend operators:
	friend bool operator== (const VECTOR3D& a, const VECTOR3D& b) { return (a.x == b.y && a.y == b.y && a.z == b.z); }
	friend bool operator!= (const VECTOR3D& a, const VECTOR3D& b) { return (a.x != b.y || a.y != b.y || a.z != b.z); }

	friend VECTOR3D operator- (const VECTOR3D& a)					 { return VECTOR3D(-a.x, -a.y, -a.z); }
	friend VECTOR3D operator+ (const VECTOR3D& a, const VECTOR3D& b) { return VECTOR3D(a.x+b.x, a.y+b.y, a.z+b.z); }
	friend VECTOR3D operator- (const VECTOR3D& a, const VECTOR3D& b) { return VECTOR3D(a.x-b.x, a.y-b.y, a.z-b.z); }
	friend VECTOR3D operator* (const VECTOR3D& a, float v) 			 { return VECTOR3D(a.x*v, a.y*v, a.z*v); }
	friend VECTOR3D operator* (float v, const VECTOR3D& a) 			 { return VECTOR3D(a.x*v, a.y*v, a.z*v); }
	
	// "normal" operators:
	VECTOR3D &operator= (const VECTOR3D& v) { x = v.x; y = v.y; z = v.z; return *this; }
	VECTOR3D &operator= (const VECTOR2D& v) { x = v.x; y = v.y; z = 0.0f; return *this; }
	
	void operator+= (const VECTOR3D& v)		{ x+=v.x; y+=v.y; z+=v.z; }
	void operator-= (const VECTOR3D& v)		{ x-=v.x; y-=v.y; z-=v.z; }
	void operator*= (float v)				{ x*=v; y*=v; z*=v; }

	float &operator [] (unsigned int id)      { assert(id < 3); return m[id]; } 
	float operator [] (unsigned int id) const { assert(id < 3); return m[id]; }  

    // casting to a float array
	//operator float* ()			{ return &x; }
	//operator const float* ()	{ return &x; }

	// members:
	union
	{
		float m[3];
		
		struct
		{
			float x, y, z;
		};
	}; 
};

// VECTOR4D class -------------------------------------------------------------+
class VECTOR4D
{
public:
	// VECTOR4D(): x(0.0f), y(0.0f), z(0.0f) { }
	VECTOR4D() { }
	VECTOR4D(float a, float b, float c, float d): x(a), y(b), z(c), w(d) { }
	VECTOR4D(const VECTOR4D& v): x(v.x), y(v.y), z(v.z), w(v.w) { }
	VECTOR4D(const VECTOR3D& v): x(v.x), y(v.y), z(v.z), w(1.0f) { }
	VECTOR4D(const VECTOR2D& v): x(v.x), y(v.y), z(0.0f), w(1.0f) { }
	~VECTOR4D() { }
	
	// friend operators:
	friend bool operator== (const VECTOR4D& a, const VECTOR4D& b) { return (a.x == b.y && a.y == b.y && a.z == b.z && a.w == b.w); }
	friend bool operator!= (const VECTOR4D& a, const VECTOR4D& b) { return (a.x != b.y || a.y != b.y || a.z != b.z || a.w != b.w); }

	friend VECTOR4D operator- (const VECTOR4D& a)					 { return VECTOR4D(-a.x, -a.y, -a.z, -a.w); }
	friend VECTOR4D operator+ (const VECTOR4D& a, const VECTOR4D& b) { return VECTOR4D(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
	friend VECTOR4D operator- (const VECTOR4D& a, const VECTOR4D& b) { return VECTOR4D(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }
	friend VECTOR4D operator* (const VECTOR4D& a, float v) 			 { return VECTOR4D(a.x*v, a.y*v, a.z*v, a.w*v); }
	friend VECTOR4D operator* (float v, const VECTOR4D& a) 			 { return VECTOR4D(a.x*v, a.y*v, a.z*v, a.w*v); }
	
	// "normal" operators:
	VECTOR4D &operator= (const VECTOR4D& v) { x = v.x; y = v.y; z = v.z; w = v.w; return *this; }
	VECTOR4D &operator= (const VECTOR3D& v) { x = v.x; y = v.y; z = v.z; w = 0.0f; return *this; }
	VECTOR4D &operator= (const VECTOR2D& v) { x = v.x; y = v.y; z = 0.0f; w = 0.0f; return *this; }
	
	void operator+= (const VECTOR4D& v)		{ x+=v.x; y+=v.y; z+=v.z; w+=v.w; }
	void operator-= (const VECTOR4D& v)		{ x-=v.x; y-=v.y; z-=v.z; w-=v.w; }
	void operator*= (float v)				{ x*=v; y*=v; z*=v; w*=v; }

	float &operator [] (unsigned int id)      { assert(id < 4); return m[id]; } 
	float operator [] (unsigned int id) const { assert(id < 4); return m[id]; }  

    // casting to a float array
	//operator float* ()			{ return &x; }
	//operator const float* ()	{ return &x; }

	// members:
	union
	{
		float m[4];
		
		struct
		{
			float x, y, z, w;
		};
	}; 
};

// POLAR2D class --------------------------------------------------------------+
class POLAR2D
{
public:
	POLAR2D(): r(0.0f), theta(0.0f) { }
	POLAR2D(float a, float b): r(a), theta(b) { }
	~POLAR2D() { }

	float r;
	float theta;
};

// SPHERICAL3D class ----------------------------------------------------------+
class SPHERICAL3D
{
public:
	SPHERICAL3D(): p(0.0f), theta(0.0f), phi(0.0f) { }
	SPHERICAL3D(float a, float b, float c): p(a), theta(b), phi(c) { }
	~SPHERICAL3D() { }

	float p;		// like the radius of the sphere
	float theta;	// angle between Z axis and the line (0, 0, 0)->point
	float phi;		// like in 2D space...
};

// CYLINDRICAL3D class --------------------------------------------------------+
class CYLINDRICAL3D
{
public:
	CYLINDRICAL3D(): r(0.0f), theta(0.0f), z(0.0f) { }
	CYLINDRICAL3D(float a, float b, float c): r(a), theta(b), z(c) { }
	~CYLINDRICAL3D() { }

	float r;
	float theta;
	float z;
};

// MATRIX2X2 class ------------------------------------------------------------+
class MATRIX2X2
{
public:
	MATRIX2X2() { }
	MATRIX2X2(const MATRIX2X2& mt)							{ memcpy((void *)M, (const void *)mt.M, sizeof(M)); }
	MATRIX2X2(float *v)										{ memcpy((void *)M, (const void *)v, sizeof(M)); }
	MATRIX2X2(float m00, float m01, float m10, float m11)	{ M00 = m00; M01 = m01; M10 = m10; M11 = m11; }
	~MATRIX2X2() { }

	MATRIX2X2 operator* (const MATRIX2X2& mt) const;
	MATRIX2X2 operator* (float v) const					{ return MATRIX2X2(M00*v, M01*v, M10*v, M11*v); }
	MATRIX2X2 operator+ (const MATRIX2X2& mt) const		{ return MATRIX2X2(M00+mt.M00, M01+mt.M01, M10+mt.M10, M11+mt.M11); }

	void operator*= (const MATRIX2X2& mt);
	void operator*= (float v)				{ M00*=v; M01*=v; M10*=v; M11*=v; }
	void operator+= (const MATRIX2X2& mt)	{ M00+=mt.M00, M01+=mt.M01, M10+=mt.M10, M11+=mt.M11; }

    // casting to a float array
	operator float* ()		 { return (float *)M; }
	operator const float* () { return (const float *)M; }

	union
	{
		float M[2][2];

		struct
		{
			float M00, M01;
	        float M10, M11;
		};
	};

	static const float IDENTITY[2][2];
};
 
// MATRIX3X3 class ------------------------------------------------------------+
class MATRIX3X3
{
public:
	MATRIX3X3() { }
	MATRIX3X3(const MATRIX3X3& mt)							{ memcpy((void *)M, (const void *)mt.M, sizeof(M)); }
	MATRIX3X3(float *v)										{ memcpy((void *)M, (const void *)v, sizeof(M)); }
	MATRIX3X3(float m00, float m01, float m02, 
			  float m10, float m11, float m12,
			  float m20, float m21, float m22)				{ M00 = m00; M01 = m01; M02 = m02; 
															  M10 = m10; M11 = m11; M12 = m12;
															  M20 = m20; M21 = m21; M22 = m22; }
	~MATRIX3X3() { }

	MATRIX3X3 operator* (const MATRIX3X3& mt) const;
	MATRIX3X3 operator* (float v) const;
	MATRIX3X3 operator+ (const MATRIX3X3& mt) const;

	void operator*= (const MATRIX3X3& mt);
	void operator*= (float v);
	void operator+= (const MATRIX3X3& mt);

    // casting to a float array
	operator float* ()		 { return (float *)M; }
	operator const float* () { return (const float *)M; }

	union
	{
		float M[3][3];

		struct
		{
			float M00, M01, M02;
	        float M10, M11, M12;
			float M20, M21, M22;
		};
	};

	static const float IDENTITY[3][3];
};

// MATRIX4X4 class ------------------------------------------------------------+
class MATRIX4X4
{
public:
	MATRIX4X4() { }
	MATRIX4X4(const MATRIX4X4& mt)							{ memcpy((void *)M, (const void *)mt.M, sizeof(M)); }
	MATRIX4X4(float *v)										{ memcpy((void *)M, (const void *)v, sizeof(M)); }
	MATRIX4X4(float m00, float m01, float m02, float m03, 
			  float m10, float m11, float m12, float m13,
			  float m20, float m21, float m22, float m23,
			  float m30, float m31, float m32, float m33)	{ M00 = m00; M01 = m01; M02 = m02; M03 = m03;
															  M10 = m10; M11 = m11; M12 = m12; M13 = m13;
															  M20 = m20; M21 = m21; M22 = m22; M23 = m23;
															  M30 = m30; M31 = m31; M32 = m32; M33 = m33;}
	~MATRIX4X4() { }

	MATRIX4X4 operator* (const MATRIX4X4& mt) const;
	MATRIX4X4 operator* (float v) const;
	MATRIX4X4 operator+ (const MATRIX4X4& mt) const;

	void operator*= (const MATRIX4X4& mt);
	void operator*= (float v);
	void operator+= (const MATRIX4X4& mt);

    // casting to a float array
	operator float* ()		 { return (float *)M; }
	operator const float* () { return (const float *)M; }

	union
	{
		float M[4][4];

		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;	        
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};

	static const float IDENTITY[4][4];
};

// QUATERNION class -----------------------------------------------------------------+
class QUATERNION
{
public:
	QUATERNION() { }
	QUATERNION(float a, float b, float c, float d) { w = a; x = b; y = c; z = d; }
	//QUATERNION(float a, const VECTOR3D& v) { q0 = a; qv = v; }
	QUATERNION(const QUATERNION& q) { w = q.w; x = q.x; y = q.y; z = q.z; }
	~QUATERNION() { }

	QUATERNION operator+ (const QUATERNION& q) { return QUATERNION(w+q.w, x+q.x, y+q.y, z+q.z); }
	QUATERNION operator- (const QUATERNION& q) { return QUATERNION(w-q.w, x-q.x, y-q.y, z-q.z); }
	QUATERNION operator* (float v) { return QUATERNION(w*v, x*v, y*v, z*v); }
	QUATERNION operator* (const QUATERNION& q);

	void operator+= (const QUATERNION& q) { w+=q.w; x+=q.x; y+=q.y; z+=q.z; }
	void operator-= (const QUATERNION& q) { w-=q.w; x-=q.x; y-=q.y; z-=q.z; }
	void operator*= (float v) { w*=v; x*=v; y*=v; z*=v; }
	void operator*= (const QUATERNION& q);

	union
	{
		float m[4];

		/*struct
		{
			float q0;
			VECTOR3D qv;
		};*/
		struct
		{
			float w, x, y, z;
		};
	};
};


// PARAMLINE2D class ----------------------------------------------------------+
class PARAMLINE2D
{
public:
	PARAMLINE2D() { }
	PARAMLINE2D(const POINT2D& a, const POINT2D& b) { p0 = a; p1 = b; v = b-a; } 
	~PARAMLINE2D() { }

	POINT2D p0;
	POINT2D p1;
	VECTOR2D v;
};

// PARAMLINE3D class ----------------------------------------------------------+
class PARAMLINE3D
{
public:
	PARAMLINE3D() { }
	PARAMLINE3D(const POINT3D& a, const POINT3D& b) { p0 = a; p1 = b; v = b-a; }
	~PARAMLINE3D() { }

	POINT3D p0;
	POINT3D p1;
	VECTOR3D v;
};

// PLANE3D class --------------------------------------------------------------+
class PLANE3D
{
public:
	PLANE3D() { }
	PLANE3D(const POINT3D& p, const VECTOR3D& norm) { p0 = p; n = norm; }
	PLANE3D(float x, float y, float z, float nx, float ny, float nz) { p0 = POINT3D(x, y, z); n = VECTOR3D(nx, ny, nz); }
	~PLANE3D() { }

	POINT3D p0;		// point on plane
	VECTOR3D n;		// normal vector to the plane
};

/*-----------------------------------------------------------------------------+
|                               functions/macros                               |
+-----------------------------------------------------------------------------*/

// "Fast" functions:
float FastSin(float ang);			// ang is in degs!
float FastCos(float ang);			// ang is in degs!
int FastDistance2d(int x, int y);
float FastDistance3d(float fx, float fy, float fz);

// Vector 2D:
inline VECTOR2D Vec2dCalc(const POINT2D_PTR p1, const POINT2D_PTR p2)			{ return VECTOR2D(p2->x - p1->x, p2->y - p1->y); }
inline float Vec2dLength(const VECTOR2D_PTR v)									{ return sqrtf(v->x*v->x + v->y*v->y); }
inline float Vec2dLength2(const VECTOR2D_PTR v)									{ return (v->x*v->x + v->y*v->y); }
inline float Vec2dLengthFast(const VECTOR2D_PTR v)								{ return (float)FastDistance2d((int)v->x, (int)v->y); }
inline float Vec2dDotProduct(const VECTOR2D_PTR v1, const VECTOR2D_PTR v2)		{ return (v1->x * v2->x + v1->y * v2->y); }
inline void Vec2dNormalize(VECTOR2D_PTR v)										{ float l = 1.0f/Vec2dLength(v); v->x*=l; v->y*=l; }
inline float Vec2dCalcCos(const VECTOR2D_PTR v1, const VECTOR2D_PTR v2)			{ return Vec2dDotProduct(v1, v2)/(Vec2dLength(v1)*Vec2dLength(v2)); }
VECTOR2D Vec2dTransform(const VECTOR2D_PTR v, const MATRIX3X3_PTR mt);

// Vector 3D:
inline VECTOR3D Vec3dCalc(const POINT3D_PTR p1, const POINT3D_PTR p2)			{ return VECTOR3D(p2->x - p1->x, p2->y - p1->y, p2->z - p1->z); }
inline float Vec3dLength(const VECTOR3D_PTR v)									{ return sqrtf(v->x*v->x + v->y*v->y +v->z*v->z); }
inline float Vec3dLength2(const VECTOR3D_PTR v)									{ return (v->x*v->x + v->y*v->y +v->z*v->z); }
inline float Vec3dLengthFast(const VECTOR3D_PTR v)								{ return FastDistance3d(v->x, v->y, v->z); }
inline float Vec3dDotProduct(const VECTOR3D_PTR v1, const VECTOR3D_PTR v2)		{ return (v1->x*v2->x + v1->y*v2->y + v1->z*v2->z); }
inline VECTOR3D Vec3dCrossProduct(const VECTOR3D_PTR v1, const VECTOR3D_PTR v2) { return VECTOR3D(v1->y*v2->z - v1->z*v2->y, v1->z*v2->x - v1->x*v2->z, v1->x*v2->y - v1->y*v2->x); }
inline void Vec3dNormalize(VECTOR3D_PTR v)										{ float l = 1.0f/Vec3dLength(v); v->x*=l; v->y*=l; v->z*=l; }
inline float Vec3dCalcCos(const VECTOR3D_PTR v1, const VECTOR3D_PTR v2)			{ return Vec3dDotProduct(v1, v2)/(Vec3dLength(v1)*Vec3dLength(v2)); }
VECTOR3D Vec3dTransform(const VECTOR3D_PTR v, const MATRIX4X4_PTR mt);

// Vector 4D:
inline VECTOR4D Vec4dCalc(const POINT4D_PTR p1, const POINT4D_PTR p2)			{ return VECTOR4D(p2->x - p1->x, p2->y - p1->y, p2->z - p1->z, 1.0f); }
inline float Vec4dLength(const VECTOR4D_PTR v)									{ return sqrtf(v->x*v->x + v->y*v->y +v->z*v->z); }
inline float Vec4dLength2(const VECTOR3D_PTR v)									{ return (v->x*v->x + v->y*v->y +v->z*v->z); }
inline float Vec4dLengthFast(const VECTOR3D_PTR v)								{ return FastDistance3d(v->x, v->y, v->z); }
inline float Vec4dDotProduct(const VECTOR4D_PTR v1, const VECTOR4D_PTR v2)		{ return (v1->x*v2->x + v1->y*v2->y + v1->z*v2->z); }
inline VECTOR4D Vec4dCrossProduct(const VECTOR4D_PTR v1, const VECTOR4D_PTR v2) { return VECTOR4D(v1->y*v2->z - v1->z*v2->y, v1->z*v2->x - v1->x*v2->z, v1->x*v2->y - v1->y*v2->x, 1.0f); }
inline void Vec4dNormalize(VECTOR4D_PTR v)										{ float l = 1.0f/Vec4dLength(v); v->x*=l; v->y*=l; v->z*=l; }
inline float Vec4dCalcCos(const VECTOR4D_PTR v1, const VECTOR4D_PTR v2)			{ return Vec4dDotProduct(v1, v2)/(Vec4dLength(v1)*Vec4dLength(v2)); }
VECTOR4D Vec4dTransform(const VECTOR4D_PTR v, const MATRIX4X4_PTR m);

// Conversions:
POINT2D Polar2dToPoint2d(const POLAR2D_PTR p);
POLAR2D Point2dToPolar2d(const POINT2D_PTR p);
POINT3D Spherical3dToPoint3d(const SPHERICAL3D_PTR s);
SPHERICAL3D Point3dToSpherical3d(const POINT3D_PTR p);
POINT3D Cylindrical3dToPoint3D(const CYLINDRICAL3D_PTR s);
CYLINDRICAL3D Point3dToCylindrical3d(const POINT3D_PTR p);

// Matrix 2x2:
inline void Mat2x2Zero(MATRIX2X2_PTR mt)										{ memset((void *)mt->M, 0, 16); }
inline void Mat2x2Identity(MATRIX2X2_PTR mt)									{ memcpy((void *)mt->M, (const void *)MATRIX2X2::IDENTITY, sizeof(MATRIX2X2)); }
inline void Mat2x2Copy(MATRIX2X2_PTR dest, const MATRIX2X2_PTR src)				{ memcpy((void *)dest->M, (const void *)src->M, sizeof(MATRIX2X2)); }
inline float Mat2x2Det(const MATRIX2X2_PTR mt)									{ return (mt->M00*mt->M11 - mt->M01*mt->M10); }
inline MATRIX2X2 Mat2x2Transpose(const MATRIX2X2_PTR mt)						{ return MATRIX2X2(mt->M00, mt->M10, mt->M01, mt->M11); }
int Mat2x2Inverse(const MATRIX2X2_PTR mt, MATRIX2X2_PTR out);

// Matrix 3x3:
inline void Mat3x3Zero(MATRIX3X3_PTR mt)										{ memset((void *)mt->M, 0, 36); }
inline void Mat3x3Identity(MATRIX3X3_PTR mt)									{ memcpy((void *)mt->M, (const void *)MATRIX3X3::IDENTITY, sizeof(MATRIX3X3)); }
inline void Mat3x3Copy(MATRIX3X3_PTR dest, const MATRIX3X3_PTR src)				{ memcpy((void *)dest->M, (const void *)src->M, sizeof(MATRIX3X3)); }
float Mat3x3Det(const MATRIX3X3_PTR mt);
inline MATRIX3X3 Mat3x3Transpose(const MATRIX3X3_PTR mt)						{ return MATRIX3X3(mt->M00, mt->M10, mt->M20, mt->M01, mt->M11, mt->M21, mt->M02, mt->M12, mt->M22); }
int Mat3x3Inverse(const MATRIX3X3_PTR mt, MATRIX3X3_PTR out);

inline MATRIX3X3 Mat3x3Translation(float x, float y)							{ return MATRIX3X3(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, 1.0f); } 
inline void Mat3x3AddTranslation(const MATRIX3X3_PTR m, float x, float y)		{ m->M20 += x; m->M21 += y; }
inline MATRIX3X3 Mat3x3Scaling(float sx, float sy)								{ return MATRIX3X3(sx, 0.0f, 0.0f, 0.0f, sy, 0.0f, 0.0f, 0.0f, 1.0f); }
inline MATRIX3X3 Mat3x3Rotation(float a)										{ return MATRIX3X3(cosf(a), sinf(a), 0.0f, -sinf(a), cosf(a), 0.0f, 0.0f, 0.0f, 1.0f); }
MATRIX3X3 Mat3x3Transformation(float a, float sx, float sy,  float x, float y);

// Matrix 4x4:
inline void Mat4x4Zero(MATRIX4X4_PTR mt)										{ memset((void *)mt->M, 0, 64); }
inline void Mat4x4Identity(MATRIX4X4_PTR mt)									{ memcpy((void *)mt->M, (const void *)MATRIX4X4::IDENTITY, sizeof(MATRIX4X4)); }
inline void Mat4x4Copy(MATRIX4X4_PTR dest, const MATRIX4X4_PTR src)				{ memcpy((void *)dest->M, (const void *)src->M, sizeof(MATRIX4X4)); }
MATRIX4X4 Mat4x4Transpose(const MATRIX4X4_PTR mt);
int Mat4x4Inverse(const MATRIX4X4_PTR mt, MATRIX4X4_PTR out);

inline MATRIX4X4 Mat4x4Translation(float x, float y, float z)					{ return MATRIX4X4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, x, y, z, 1.0f); }
inline void Mat4x4AddTranslation(const MATRIX4X4_PTR m, float x, float y, float z) { m->M30 += x; m->M31 += y; m->M32 += z; }
inline MATRIX4X4 Mat4x4Scaling(float x, float y, float z)						{ return MATRIX4X4(x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f); }
MATRIX4X4 Mat4x4RotationX(float ang);
MATRIX4X4 Mat4x4RotationY(float ang);
MATRIX4X4 Mat4x4RotationZ(float ang);
MATRIX4X4 Mat4x4VectorRotation(float ang, const VECTOR3D_PTR v);
inline MATRIX4X4 Mat4x4VectorRotation(float ang, float vx, float vy, float vz)  { VECTOR3D vn(vx, vy, vz); return Mat4x4VectorRotation(ang, &vn); }
MATRIX4X4 Mat4x4Transformation(float rx, float ry, float rz, 
							   float sx, float sy, float sz,     // not finished!
							   float x, float y, float z);

// Quaternions:
inline float QuaternionNorm(const QUATERNION_PTR q)								{ return (sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z)); }
inline float QuaternionNorm2(const QUATERNION_PTR q)							{ return ((q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z)); }
void QuaternionNormalize(const QUATERNION_PTR q);
inline QUATERNION QuaternionConjugate(const QUATERNION_PTR q)					{ return QUATERNION(q->w, -q->x, -q->y, -q->z); }
QUATERNION QuaternionInverse(const QUATERNION_PTR q);
QUATERNION QuaternionAxisRotation(const VECTOR3D_PTR axis, float rot);
QUATERNION QuaternionZYXRotation(float rx, float ry, float rz);
void QuaternionToVector3dAndAngle(const QUATERNION_PTR q, VECTOR3D_PTR axis, float &rot);

// Parametric lines:
POINT2D ComputePointOnLine2d(const PARAMLINE2D_PTR line, float t);
POINT3D ComputePointOnLine3d(const PARAMLINE3D_PTR line, float t);
int IntersectLine2dAndLine2d(const PARAMLINE2D_PTR l1, const PARAMLINE2D_PTR l2, float& t1, float& t2);
int IntersectLine2dAndLine2d(const PARAMLINE2D_PTR l1, const PARAMLINE2D_PTR l2, POINT2D_PTR pt);

// Planes 3d:
float DistanceToPlane3d(const PLANE3D* plane, const POINT3D* p);
int IntersectLine3dAndPlane3d(const PARAMLINE3D_PTR line, const PLANE3D_PTR plane, float &t, POINT3D_PTR pt);

// Various functions:
void BuildSinCosTables();

int Solve2x2System(const MATRIX2X2_PTR a, const VECTOR2D_PTR b, VECTOR2D_PTR x);
int Solve3x3System(const MATRIX3X3_PTR a, const VECTOR3D_PTR b, VECTOR3D_PTR x);

#endif // TG_MATH_HPP

// end of file ----------------------------------------------------------------+
