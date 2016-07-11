/*-----------------------------------------------------------------------------+
| File name: gl_camera.hpp						   						       |
| Date:		 19th January 2006		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Class that represents camera which can be used in OpenGL applications...	   |
+-----------------------------------------------------------------------------*/

#ifndef GL_CAMERA_HPP
	#define GL_CAMERA_HPP

#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include "tg_math.h"

// the CCamera class ----------------------------------------------------------+
#define PM_NEAR_FAR   1
#define PM_LEFT_RIGHT 2
#define PM_BOTTOM_TOP 4
#define PM_ALL 7

enum CameraMode { cmUVN, cmYawPitchRoll, cmSpherical };

class CCamera {
public:	
	CCamera();
	~CCamera();

	 void InitView(float fNear, float fFar, float fFov, float fViewAspect);
	 void BuildMatrix(CameraMode cmMode);
	 void CalcPlanes();
	 bool IsPointInFrustum(const POINT3D& pt, int iPlaneMode = PM_ALL);
	 bool IsSphereInFrustum(const POINT3D& pt, float fRadius, int iPlaneMode = PM_ALL);
	 
	 void SetNewFov(float fFov) { InitView(m_fNear, m_fFar, fFov, 1.0f/m_fAspectRatioInv); }
	 float GetFov() { return m_fFov; }

	 // additional:
	 void CalcFrustumPoints(POINT3D *pFPoints);
	 void DrawFrustum();

	 // inline methods:
	 void SetProjectionMatrix() { 
		 glFrustum(-m_fViewplaneW*0.5f, m_fViewplaneW*0.5f, -m_fViewplaneH*0.5f, m_fViewplaneH*0.5f, 
			       m_fViewDistance, m_fFar); 
	 }
	 void SetViewMatrix() { 
		 gluLookAt(m_vPos.x, m_vPos.y, m_vPos.z, 
			       m_vTarget.x, m_vTarget.y, m_vTarget.z, 
				   m_vUp.x, m_vUp.y, m_vUp.z); 
	 }

    // variables:
	VECTOR3D m_vPos;
	VECTOR3D m_vTarget;
	VECTOR3D m_vUp;
	float m_fYaw;   // y axis, in radians
	float m_fPitch; // x axis, in radians
	float m_fRoll;  // z axis, in radians
	float m_fRad;   // radius of the sphere, used in cmSpherical cam mode
private:
	PLANE3D m_pNear;
	PLANE3D m_pFar;
	PLANE3D m_pRight;
	PLANE3D m_pLeft;
	PLANE3D m_pBottom;
	PLANE3D m_pTop;
	float m_fNear;
	float m_fFar;
	float m_fFov;
	float m_fAspectRatioInv;
	float m_fViewplaneW;
	float m_fViewplaneH;
	float m_fViewDistance;
	float m_fHNear;
	float m_fHFar;
	MATRIX4X4 m_matView;
};

#endif // GL_CAMERA_HPP

// end of file ----------------------------------------------------------------+