/*-----------------------------------------------------------------------------+
| File name: gl_camera.cpp						   						       |
| Date:		 19th January 2006		  									       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| Class that represents camera which can be used in OpenGL applications...	   |
+-----------------------------------------------------------------------------*/

#include "gl_camera.h"

/*-----------------------------------------------------------------------------+
|                   Implementation of the CCamera class                        |
+-----------------------------------------------------------------------------*/

// constructor ----------------------------------------------------------------+
CCamera::CCamera():
	m_fNear(0.0f),
	m_fFar(0.0f),
	m_fFov(0.0f),
	m_fRad(0.0f),
	m_fAspectRatioInv(0.0f),
	m_fViewplaneW(0.0f),
	m_fViewplaneH(0.0f),
	m_fViewDistance(0.0f)
{

}

// destructor -----------------------------------------------------------------+
CCamera::~CCamera() {

}

// Init method ----------------------------------------------------------------+
void CCamera::InitView(float fNear, float fFar, float fFov, float fViewAspect) {
	m_fNear = fNear;
	m_fFar = fFar;
	m_fFov = fFov;
	m_fAspectRatioInv = 1.0f/fViewAspect;

	// viewplane size:
	m_fViewplaneW = 2.0;
	m_fViewplaneH = 2.0f * m_fAspectRatioInv;

	// view distance:
	m_fViewDistance = (m_fViewplaneW*0.5f)/tan(DegToRad(m_fFov*0.5f));
}
// BuildMatrix method ---------------------------------------------------------+
void CCamera::BuildMatrix(CameraMode cmMode) {
	switch (cmMode) {
		case cmYawPitchRoll: {
			float r = cos(m_fPitch);
			float x = r*sin(m_fYaw);
			float y = sin(m_fPitch);
			float z = -r*cos(m_fYaw);
			m_vTarget = VECTOR3D(x, y, z);
			m_vUp = VECTOR3D(sin(m_fRoll), cos(m_fRoll), 0.0f);
			break;
		}
		case cmSpherical: {
			float r = cos(m_fPitch)*m_fRad;
			float x = r*sin(m_fYaw);
			float y = sin(m_fPitch)*m_fRad;
			float z = -r*cos(m_fYaw);
			m_vPos = VECTOR3D(x, y, z) + m_vTarget;
			m_vUp = VECTOR3D(sin(m_fRoll), cos(m_fRoll), 0.0f);    
			break;
		}
	}
	glPushMatrix();
	    // faster than calculatin it "manually" :)
	    glLoadIdentity();
	    gluLookAt(m_vPos.x, m_vPos.y, m_vPos.z,
              m_vTarget.x, m_vTarget.y, m_vTarget.z,
			  m_vUp.x, m_vUp.y, m_vUp.z);
	    glGetFloatv(GL_MODELVIEW_MATRIX, (float *)m_matView.M);
	glPopMatrix();
}
// CalcPlanes method ----------------------------------------------------------+
void CCamera::CalcPlanes() {
	VECTOR3D vBuf;
	
	m_fHNear = m_fNear*tan(DegToRad(m_fFov*0.5f));
	m_fHFar = m_fFar*tan(DegToRad(m_fFov*0.5f));
	float fWNear = m_fHNear/m_fAspectRatioInv;
	float fWFar = m_fHFar/m_fAspectRatioInv;

	// direction, up and right unit vector
	VECTOR3D vDir = m_vTarget - m_vPos;
	Vec3dNormalize(&vDir);
	
	VECTOR3D vUp = m_vUp;
	Vec3dNormalize(&vUp);

	VECTOR3D vRight = Vec3dCrossProduct(&vDir, &vUp);
	Vec3dNormalize(&vRight);

	vUp = Vec3dCrossProduct(&vRight, &vDir);
	Vec3dNormalize(&vUp);

	// front plane:
	m_pNear.p0 = m_vPos + m_fNear*vDir;
	m_pNear.n = -vDir;

	// far plane:
	m_pFar.p0 = m_vPos + m_fFar*vDir;
	m_pFar.n = vDir;

	// right plane:
	m_pRight.p0 = m_pNear.p0 - vRight*fWNear;
	vBuf = (m_pNear.p0 - vRight*fWNear)-m_vPos;
	Vec3dNormalize(&vBuf);
	m_pRight.n = Vec3dCrossProduct(&vUp, &vBuf);
	Vec3dNormalize(&m_pRight.n);
	
	// left plane:
	m_pLeft.p0 = m_pNear.p0 + vRight*fWNear;
	vBuf = (m_pNear.p0 + vRight*fWNear)-m_vPos;
	Vec3dNormalize(&vBuf);
	m_pLeft.n = Vec3dCrossProduct(&vBuf, &vUp);
	Vec3dNormalize(&m_pLeft.n);

	// bottom plane:
	m_pBottom.p0 = m_pNear.p0 - vUp*m_fHNear;
	vBuf = (m_pNear.p0 - vUp*m_fHNear)-m_vPos;
	Vec3dNormalize(&vBuf);
	m_pBottom.n = Vec3dCrossProduct(&vBuf, &vRight);
	Vec3dNormalize(&m_pBottom.n);

	// bottom plane:
	m_pTop.p0 = m_pNear.p0 + vUp*m_fHNear;
	vBuf = (m_pNear.p0 + vUp*m_fHNear)-m_vPos;
	Vec3dNormalize(&vBuf);
	m_pTop.n = Vec3dCrossProduct(&vRight, &vBuf);
	Vec3dNormalize(&m_pTop.n);
}

// IsPointInFrustum method ---------------------------------------------------------+
bool CCamera::IsPointInFrustum(const POINT3D& pt, int iPlaneMode) {
	if (iPlaneMode & PM_NEAR_FAR) {
		if (DistanceToPlane3d(&m_pNear, &pt) > 0.0 ||
			DistanceToPlane3d(&m_pFar, &pt) > 0.0f) 
			return false;
		}
	if (iPlaneMode & PM_LEFT_RIGHT) {
		if (DistanceToPlane3d(&m_pLeft, &pt) > 0.0 ||
			DistanceToPlane3d(&m_pRight, &pt) > 0.0f) 
			return false;
		}
	if (iPlaneMode & PM_BOTTOM_TOP) {
		if (DistanceToPlane3d(&m_pBottom, &pt) > 0.0 ||
			DistanceToPlane3d(&m_pTop, &pt) > 0.0f) 
			return false;
		}
	return true;
	}
// IsSphereInFrustum method ---------------------------------------------------+
bool CCamera::IsSphereInFrustum(const POINT3D& pt, float fRadius, int iPlaneMode) {
	if (iPlaneMode & PM_NEAR_FAR) {
		if (DistanceToPlane3d(&m_pNear, &pt) > fRadius ||
			DistanceToPlane3d(&m_pFar, &pt) > fRadius) 
			return false;
		}
	if (iPlaneMode & PM_LEFT_RIGHT) {
		if (DistanceToPlane3d(&m_pLeft, &pt) > fRadius ||
			DistanceToPlane3d(&m_pRight, &pt) > fRadius) 
			return false;
		}
	if (iPlaneMode & PM_BOTTOM_TOP) {
		if (DistanceToPlane3d(&m_pBottom, &pt) > fRadius ||
			DistanceToPlane3d(&m_pTop, &pt) > fRadius) 
			return false;
		}
	return true;
	}

// CalcFrutumPoints -----------------------------------------------------------+
void CCamera::CalcFrustumPoints(POINT3D *pFPoints) {
	POINT3D pCenter;
	
	m_fHNear = m_fNear*tan(DegToRad(m_fFov*0.5f));
	m_fHFar = m_fFar*tan(DegToRad(m_fFov*0.5f));
	float fWNear = m_fHNear/m_fAspectRatioInv;
	float fWFar = m_fHFar/m_fAspectRatioInv;

	// direction, up and right unit vector
	VECTOR3D vDir = m_vTarget - m_vPos;
	Vec3dNormalize(&vDir);
	
	VECTOR3D vUp = m_vUp;
	Vec3dNormalize(&vUp);

	VECTOR3D vRight = Vec3dCrossProduct(&vDir, &vUp);
	Vec3dNormalize(&vRight);

	vUp = Vec3dCrossProduct(&vRight, &vDir);
	Vec3dNormalize(&vUp);

	// near plane:
	pCenter = m_vPos + m_fNear*vDir;
	pFPoints[0] = pCenter + vUp*m_fHNear - vRight*fWNear;
	pFPoints[1] = pCenter - vUp*m_fHNear - vRight*fWNear;
	pFPoints[2] = pCenter - vUp*m_fHNear + vRight*fWNear;
	pFPoints[3] = pCenter + vUp*m_fHNear + vRight*fWNear;
	pFPoints[4] = pCenter;

	// far plane:
	pCenter = m_vPos + m_fFar*vDir;
	pFPoints[5] = pCenter + vUp*m_fHFar - vRight*fWFar;
	pFPoints[6] = pCenter - vUp*m_fHFar - vRight*fWFar;
	pFPoints[7] = pCenter - vUp*m_fHFar + vRight*fWFar;
	pFPoints[8] = pCenter + vUp*m_fHFar + vRight*fWFar;
	pFPoints[9] = pCenter;
}

// DrawFrustum method ---------------------------------------------------------+
void CCamera::DrawFrustum() {
	POINT3D pFPoints[11];
	VECTOR3D vBuf;

	CalcFrustumPoints(pFPoints);
	pFPoints[10] = m_vPos;

	// near plane:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3fv(pFPoints[0].m);
		glVertex3fv(pFPoints[1].m);
		glVertex3fv(pFPoints[2].m);
		glVertex3fv(pFPoints[3].m);
	glEnd();

    glBegin(GL_LINES);
		vBuf = 3.0f*m_pNear.n + m_pNear.p0;
		glVertex3fv(vBuf.m);
		glVertex3fv(m_pNear.p0.m);
	glEnd();

	// far plane:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3fv(pFPoints[5].m);
		glVertex3fv(pFPoints[6].m);
		glVertex3fv(pFPoints[7].m);
		glVertex3fv(pFPoints[8].m);
	glEnd();

	glBegin(GL_LINES);
		vBuf = 3.0f*m_pFar.n + m_pFar.p0;
		glVertex3fv(vBuf.m);
		glVertex3fv(m_pFar.p0.m);
	glEnd();

	// left plane:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3fv(pFPoints[10].m);
		glVertex3fv(pFPoints[5].m);
		glVertex3fv(pFPoints[10].m);
		glVertex3fv(pFPoints[6].m);
	glEnd();

	glBegin(GL_LINES);
		vBuf = 3.0f*m_pLeft.n + m_pLeft.p0;
		glVertex3fv(vBuf.m);
		glVertex3fv(m_pLeft.p0.m);
	glEnd();

	// right plane:
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		glVertex3fv(pFPoints[10].m);
		glVertex3fv(pFPoints[7].m);
		glVertex3fv(pFPoints[10].m);
		glVertex3fv(pFPoints[8].m);
	glEnd();

	glBegin(GL_LINES);
		vBuf = 3.0f*m_pRight.n + m_pRight.p0;
		glVertex3fv(vBuf.m);
		glVertex3fv(m_pRight.p0.m);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		vBuf = 3.0f*m_pTop.n + m_pTop.p0;
		glVertex3fv(vBuf.m);
		glVertex3fv(m_pTop.p0.m);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
		vBuf = 3.0f*m_pBottom.n + m_pBottom.p0;
		glVertex3fv(vBuf.m);
		glVertex3fv(m_pBottom.p0.m);
	glEnd();
}

// end of file ----------------------------------------------------------------+