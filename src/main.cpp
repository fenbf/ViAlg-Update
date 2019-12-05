/*-----------------------------------------------------------------------------+
| File name: main.cpp			     										   |
| Date:		 4th October 2005				  							       |
| Author:	 Fen															   |
+------------------------------------------------------------------------------+
| The Main File of the ViAlg project                                           |
| The Framework is based on the NeHe tutorials and code to the "Focus on 3D    |
| Terrain Programming" by Trent Polack.							               |
+-----------------------------------------------------------------------------*/

#include <stdio.h>
#include <exception>
#include <typeinfo>

#include "cgl_app.h"
#include "clog.h"
#include "tg_math.h"
#include "ctimer.h"

#include "gl_text.h"
#include "gl_camera.h"
#include "gl_shapes.h"
#include "gl_light.h"
#include "gl_texture.h"

#include "av_system.h"
#include "av_data.h"
#include "calgorithms.h"

#include "resource.h"

using namespace std;

const double fTempoDelta = 500.0;  // beats per minute (bpm)
const int iElementsDelta = 10;
const int iElementsMin = 20;

// global variables/objects ---------------------------------------------------+
CGLApp g_glApp;
CLog g_Log;
CTimer g_Timer;
CGLFont g_Font;
CCamera g_Camera;
CGLLight g_Light;

// textures' ids:
GLuint g_iTexFloor;
GLuint g_iTexFrame;

HANDLE hTimer = NULL;

// switches:
bool g_bLight;
bool g_bTextured;
bool g_bReflection;
bool g_bFloor;
bool g_bInfo;
bool g_bRegenerate;

// algorithms:
#define NUM_ALGORITHMS 6
#define ABUBBLE_SORT 0
#define ASHAKER_SORT 1
#define ASELECTION_SORT 2
#define AINSERTION_SORT 3
#define ASHELL_SORT 4
#define AQUICK_SORT 5

CAlgorithm *g_Algorithms[NUM_ALGORITHMS];
CAlgManager *g_algManager;
CAVSystem *g_avSystem;

// functions' prototypes ------------------------------------------------------+
void InitApp();
void InitGL();
void CleanUp();
void ResizeGLScene(int iWidth, int iHeight, bool bView = false);
void RenderScene();
void UpdateAnimation();
void DrawFloor();
bool ProcessKeyboard();
bool OnMenuCommand(WORD iId, HMENU hMenu);
bool OnExit();

/*-----------------------------------------------------------------------------+
|                                    MAIN                                      |
+-----------------------------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE current_in, HINSTANCE prev_in, LPSTR cmdl, int n_show) {
	
	g_Log.Init("log.html");
	try {
		InitApp();
	}
	catch (exception &e) {
		g_Log.AddMsg(lmError, "Exception was thrown: type: %s - message: %s", typeid(e).name(), e.what());
		MessageBox(NULL, e.what(), "Exception!", MB_OK | MB_ICONSTOP);
		CleanUp();
		return 1;
	}

	LARGE_INTEGER liDueTime;

	liDueTime.QuadPart = -100000LL; // 1ms

	int iMsg;
	while (1) {
		SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0);

		iMsg = CGLApp::CheckWindowMessages();
		
		// Quit?
		if (iMsg == 0 && !OnExit()) break;
		
		// any Massages for the window ?
		if (iMsg > 0) continue;
		
		// is application window active?
		if (g_glApp.IsActive() == false) continue;
			
		if (!ProcessKeyboard() && !OnExit()) break;

		UpdateAnimation();
		RenderScene(); 

		//glFlush();
		SwapBuffers(g_glApp.GetHdc());

		if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
			printf("WaitForSingleObject failed (%d)\n", GetLastError());
	}

	CleanUp();

	return 0;
}

/*-----------------------------------------------------------------------------+
|                                  InitApp                                     |
+------------------------------------------------------------------------------+
| Description:																   |
|    Initialises all of the application.                                       | 
+-----------------------------------------------------------------------------*/
void InitApp() {	
	CGLApp::m_iBpp = 32;
	CGLApp::m_iWidth = 1024;
	CGLApp::m_iHeight = 768;
	CGLApp::m_bFullscreen = false;
	sprintf_s(CGLApp::m_szTitle, "ViAlg");
	
	if (!g_glApp.Init(IDR_MENU1, &OnMenuCommand)) throw exception("Failed in initialising the Application");
	else g_Log.AddMsg(lmSuccess, "Main window of the application has been created!");

	if (!g_Timer.Init()) throw exception("High Performance Timer not available!");
	else g_Log.AddMsg(lmSuccess, "High Performance Timer available!");

	g_glApp.SetRenderProc(&RenderScene);

    BuildSinCosTables();
	srand((unsigned int)time(NULL));

	g_Algorithms[ABUBBLE_SORT] = new CBubbleSortAlgorithm();
	g_Algorithms[ASHAKER_SORT] = new CShakerSortAlgorithm();
	g_Algorithms[ASELECTION_SORT] = new CSelectionSortAlgorithm();
	g_Algorithms[AINSERTION_SORT] = new CInsertionSortAlgorithm();
	g_Algorithms[ASHELL_SORT] = new CShellSortAlgorithm();
	g_Algorithms[AQUICK_SORT] = new CQuickSortAlgorithm();
	g_algManager = new CAlgManager();
	g_algManager->SetTempo(3000.0);
	g_algManager->SetNumOfElements(100);
	g_algManager->GenerateData(doSpecialRandomized);
	g_algManager->SetAlgorithm(g_Algorithms[ABUBBLE_SORT]);

	g_avSystem = new CAVSystem();
	g_avSystem->SetMaxSize(5.0f, 1.0f, 1.0f);

	g_iTexFloor = LoadTextureFromBmp("data\\checker64.bmp", GL_BGR_EXT, GL_LINEAR_MIPMAP_LINEAR);
	g_iTexFrame = LoadTextureFromBmp("data\\frame.bmp", GL_BGR_EXT, GL_LINEAR_MIPMAP_LINEAR);

	g_avSystem->SetDiagramBlockInfo(btBox, VECTOR3D(0.2f, 0.3f, 0.7f),       // normal color
		                                  VECTOR3D(0.2f, 1.0f, 0.2f),        // marked
										  VECTOR3D(0.4f, 0.6f, 0.99f));      // highlighted 
	g_avSystem->SetOutlook(VECTOR3D(0.8f, 0.7f, 0.3f), g_iTexFrame);
	g_avSystem->m_bTextured = false;

    // switches:
	g_bLight = true;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_LIGHT, MF_CHECKED);
	g_bTextured = false;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_TEXTURED, MF_UNCHECKED);
	g_bReflection = true;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_REFLECTION, MF_CHECKED);
	g_bFloor = true;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_FLOOR, MF_CHECKED);
	g_avSystem->m_bFrame = true;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_FRAME, MF_CHECKED);
	g_avSystem->m_bHoriz = false;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_HORIZ, MF_UNCHECKED);
	g_bInfo = true;
	CheckMenuItem(g_glApp.m_hMenu, ID_VIEW_INFO, MF_CHECKED);
	g_bRegenerate = true;
	CheckMenuItem(g_glApp.m_hMenu, ID_DATAORDER_REGENERATE, MF_CHECKED);
	g_Log.AddMsg(lmSuccess, "App's menu fixed!");

	InitGL();

	// Create an unnamed waitable timer.
	hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
	if (NULL == hTimer)
		throw exception("CreateWaitableTimer failed");

	g_Log.AddMsg(lmSuccess, "App initialised!");
}

/*-----------------------------------------------------------------------------+
|                                   InitGL                                     |
+------------------------------------------------------------------------------+
| Description:																   |
|    Does initialization of the OpenGL                                         | 
+-----------------------------------------------------------------------------*/
void InitGL() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	

	glEnable(GL_CULL_FACE);
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	glShadeModel(GL_SMOOTH);

	if (CGLFont::Create2dCourierFont(&g_glApp, g_Font, 16, 8)) g_Log.AddMsg(lmSuccess, "Courier Font Created!");
	else throw exception("Faild to create the courier font!");

	// camera:
	g_Camera.InitView(1.0f, 1000.0f, 90.0f, 1.33f);
	g_Camera.m_fRad = 4.0f;
	g_Camera.m_fYaw = M_PI;
	g_Camera.m_fPitch = DegToRad(30.0f);

    // set lighting:
	glEnable(GL_LIGHT0);
	g_Light.SetAmbientColor(0.2f, 0.2f, 0.2f);
	g_Light.SetDiffuseColor(0.9f, 0.9f, 0.9f);
	g_Light.SetSpecularColor(1.0f, 1.0f, 1.0f);
	g_Light.SetPos(0.0f, 0.0f, 10.0f);

	glEnable(GL_COLOR_MATERIAL);

	ResizeGLScene(g_glApp.m_iWidth, g_glApp.m_iHeight);

	g_Log.AddMsg(lmSuccess, "OpenGL initialised!");
}

/*-----------------------------------------------------------------------------+
|                                  CleanUp                                     |
+-----------------------------------------------------------------------------*/
void CleanUp() {
	g_Log.AddMsg(lmNormal, "Closing the application...");

	glDeleteTextures(1, &g_iTexFloor);
	glDeleteTextures(1, &g_iTexFrame);
	g_Log.AddMsg(lmNormal, "Textures were deleted...");

	delete g_Algorithms[ABUBBLE_SORT];
	delete g_Algorithms[ASHAKER_SORT];
	delete g_Algorithms[ASELECTION_SORT];
	delete g_Algorithms[AINSERTION_SORT];
	delete g_Algorithms[ASHELL_SORT];
	delete g_Algorithms[AQUICK_SORT];

	delete g_avSystem;
	delete g_algManager;

	g_Font.Delete();
	g_glApp.Destroy();

	g_Log.AddMsg(lmSuccess, "Application was closed properly");
}

/*-----------------------------------------------------------------------------+
|                               ResizeGLScene                                  |
+-----------------------------------------------------------------------------*/
void ResizeGLScene(int iWidth, int iHeight, bool bView) {
	glViewport(0, 0, iWidth, iHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(90.0, (float)iWidth/(float)iHeight, 0.1, 1000.0);
	//g_Camera.SetProjectionMatrix();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/*-----------------------------------------------------------------------------+
|                                RenderScene                                   |
+-----------------------------------------------------------------------------*/
void RenderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	
	if (g_bLight) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);

	// set camera:
	glLoadIdentity();
	g_Camera.BuildMatrix(cmSpherical);
	g_Camera.SetViewMatrix();

	if (g_bReflection) {
		double eqr[] = { 0.0f, -1.0f, 0.0f, 0.0f };		// plane equation

		glColorMask(0, 0, 0, 0);

		glEnable(GL_STENCIL_TEST);						// Enable Stencil Buffer For "marking" The Floor
		glStencilFunc(GL_ALWAYS, 1, 1);					// Always Passes, 1 Bit Plane, 1 As Mask
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);		// We Set The Stencil Buffer To 1 Where We Draw Any Polygon
													    // Keep If Test Fails, Keep If Test Passes But Buffer Test Fails
													    // Replace If Test Passes
		glDisable(GL_DEPTH_TEST);						// Disable Depth Testing
		DrawFloor();

		glEnable(GL_DEPTH_TEST);						// Enable Depth Testing
		glColorMask(1, 1, 1, 1);						// Set Color Mask to TRUE, TRUE, TRUE, TRUE
		glStencilFunc(GL_EQUAL, 1, 1);					// We Draw Only Where The Stencil Is 1
													    // (I.E. Where The Floor Was Drawn)
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glEnable(GL_CLIP_PLANE0);						// Enable Clip Plane For Removing Artifacts
													    // (When The Object Crosses The Floor)
		glClipPlane(GL_CLIP_PLANE0, eqr);
		glPushMatrix();
			glScalef(1.0f, -1.0f, 1.0f);
			//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);		
			glFrontFace(GL_CW);
			glTranslatef(0.0f, 0.3f, 0.0);
			g_algManager->Render(g_avSystem);
			glFrontFace(GL_CCW);
		glPopMatrix();
		glDisable(GL_CLIP_PLANE0);						// Disable Clip Plane For Drawing The Floor
		glDisable(GL_STENCIL_TEST);	

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.6f, 0.7f, 1.0f, 0.5f);
		DrawFloor();
		glDisable(GL_BLEND);
	}
	else if (g_bFloor) {
		glColor4f(0.6f, 0.7f, 1.0f, 0.5f);
		DrawFloor();
	}

	// "Draw" algorithm...
	glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glTranslatef(0.0f, 0.3f, 0.0);
	    g_algManager->Render(g_avSystem);
	glPopMatrix();

	// draw text: 
	if (g_bInfo) {
		glBeginText(CGLApp::m_iWidth, CGLApp::m_iHeight);
	        glColor3f(0.0f, 0.0f, 0.0f);
		    glTextPrintf(&g_Font, 10.0f, 20.0f, "Fps: %2.3f", g_Timer.GetFps());
			if (g_Timer.IsFreezed()) glTextout(&g_Font, "Algorithm paused", 500.0f, 20.0f);
			else glTextPrintf(&g_Font, 500.0f, 20.0f, "Update tempo: %3.1f", g_algManager->GetTempo()/100.0);
		    glTextPrintf(&g_Font, 10.0f, 34.0f, "Algorithm: %s", g_algManager->GetAlgorithmName());
		    glTextPrintf(&g_Font, 10.0f, 48.0f, "Num of Elements: %d", g_algManager->GetNumOfElements());
			glTextPrintf(&g_Font, 10.0f, 62.0f, "Elements order: %s", g_algManager->GetDataOrderName());
		    glTextPrintf(&g_Font, 10.0f, 76.0f, "Comparisions: %d", CAlgorithm::GetNumOfComparisions());
		    glTextPrintf(&g_Font, 10.0f, 90.0f, "Exchanges: %d", CAlgorithm::GetNumOfExchanges());
			glTextPrintf(&g_Font, 10.0f, 104.0f, "Iterations: %d", CAlgorithm::GetNumOfIterations());
	    glEndText();
	}
}

/*-----------------------------------------------------------------------------+
|                                   DrawFloor                                  |
+-----------------------------------------------------------------------------*/
void DrawFloor()
{
	float x, z;

	if (g_bTextured) glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, g_iTexFloor);
	glBegin(GL_QUADS);
	for (x = -4.0f; x < 4.0f; x+=1.0f)
		for (z = -4.0f; z < 4.0f; z+=1.0f)
		{
				glNormal3f(0.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 0.0f);
				glVertex3f(x, 0.0f, z);
				glTexCoord2f(0.0f, 1.0f);
				glVertex3f(x, 0.0f, z+0.9f);
				glTexCoord2f(1.0f, 1.0f);
				glVertex3f(x+0.9f, 0.0f, z+0.9f);
				glTexCoord2f(1.0f, 0.0f);
				glVertex3f(x+0.9f, 0.0f, z);
		}
	glEnd();
	if (g_bTextured) glDisable(GL_TEXTURE_2D);
}

/*-----------------------------------------------------------------------------+
|                              UpdateAnimation                                 |
+-----------------------------------------------------------------------------*/
void UpdateAnimation() {
	g_Timer.Update();

	if (g_Timer.IsFreezed()) return;

	g_algManager->Update();
}

/*-----------------------------------------------------------------------------+
|                               ProcessKeyboard                                |
+------------------------------------------------------------------------------+
| Returns "false" if user presses ESC key                                      |
+-----------------------------------------------------------------------------*/
bool ProcessKeyboard() {
	if (g_glApp.KeyDown(VK_ESCAPE)) return false;
	
	// restore defaults:
	if (g_glApp.KeyDown(VK_RETURN)) {
		g_Camera.m_fPitch = DegToRad(30.0f);
		g_Camera.m_fRad = 4.0f;
		g_Camera.m_fYaw = M_PI;
		g_Camera.m_fRoll = 0.0f;
		g_Camera.SetNewFov(45.0f);
	}

	if (g_glApp.KeyDown(VK_SPACE)) {
		if (g_Timer.IsFreezed()) g_Timer.Unfreeze();
		else g_Timer.Freeze();
		g_glApp.SetKey(VK_SPACE, false);
	}

	// virtual camera:
	if (g_glApp.KeyDown(VK_LEFT)) g_Camera.m_fYaw += DegToRad(45.0f*(float)g_Timer.GetDelta());
	if (g_glApp.KeyDown(VK_RIGHT)) g_Camera.m_fYaw -= DegToRad(45.0f*(float)g_Timer.GetDelta());
	if (g_glApp.KeyDown(VK_UP)) {
		if (g_Camera.m_fPitch < M_PI*0.45f) 
			g_Camera.m_fPitch += DegToRad(45.0f*(float)g_Timer.GetDelta());
	}
	if (g_glApp.KeyDown(VK_DOWN)) {
		if (g_Camera.m_fPitch > 0.1f) 
			g_Camera.m_fPitch -= DegToRad(45.0f*(float)g_Timer.GetDelta());
	}
	if (g_glApp.KeyDown('Z')) {
		if (g_Camera.m_fRad < 10.0f) 
			g_Camera.m_fRad += 2.0f*(float)g_Timer.GetDelta();
	}
	if (g_glApp.KeyDown('X')) {
		if (g_Camera.m_fRad > 2.0f) 
			g_Camera.m_fRad -= 2.0f*(float)g_Timer.GetDelta();
	}

	if (g_glApp.KeyDown('Q')) g_Camera.SetNewFov(g_Camera.GetFov() + 5.0f*(float)g_Timer.GetDelta());
	if (g_glApp.KeyDown('E')) g_Camera.SetNewFov(g_Camera.GetFov() + 5.0f*(float)g_Timer.GetDelta());

	return true;
}

/*-----------------------------------------------------------------------------+
|                                OnMenuCommand                                 |
+------------------------------------------------------------------------------+
| Returns "false" if user wants to exit                                        |
+-----------------------------------------------------------------------------*/
bool OnMenuCommand(WORD iId, HMENU hMenu) {
	switch ( iId ) {
		case ID_FILE_EXIT: return false;
		case ID_DIAGRAMTYPE_POINTS:    { g_avSystem->SetBlockType(btPoint); break; }
		case ID_DIAGRAMTYPE_BOXES:     { g_avSystem->SetBlockType(btBox); break; }
	    case ID_DIAGRAMTYPE_PYRAMIDS:  { g_avSystem->SetBlockType(btPyramid); break; }
	    case ID_DIAGRAMTYPE_CYLINDERS: { g_avSystem->SetBlockType(btCylinder); break; }
		case ID_ALGORITHM_RUNAGAIN:    { 
			    if (g_bRegenerate) g_algManager->RegenerateData(); 
				g_algManager->RunAgain(); 
				break; 
			}
		case ID_METHOD_BUBBLESORT: {
				g_algManager->SetAlgorithm(g_Algorithms[ABUBBLE_SORT]);
				if (g_bRegenerate) g_algManager->RegenerateData();
				g_algManager->RunAgain();
				break;
			}
	    case ID_METHOD_SHAKERSORT: {
				g_algManager->SetAlgorithm(g_Algorithms[ASHAKER_SORT]);
				if (g_bRegenerate) g_algManager->RegenerateData();
				g_algManager->RunAgain();
				break;
			}
	    case ID_METHOD_SELECTIONSORT: {
				g_algManager->SetAlgorithm(g_Algorithms[ASELECTION_SORT]);
				if (g_bRegenerate) g_algManager->RegenerateData();
				g_algManager->RunAgain();
				break;
			}
	    case ID_METHOD_INSERTIONSORT: {
				g_algManager->SetAlgorithm(g_Algorithms[AINSERTION_SORT]);
				if (g_bRegenerate) g_algManager->RegenerateData();
				g_algManager->RunAgain();
				break;
			}
		case ID_METHOD_SHELLSORT: {
				g_algManager->SetAlgorithm(g_Algorithms[ASHELL_SORT]);
				if (g_bRegenerate) g_algManager->RegenerateData();
				g_algManager->RunAgain();
				break;
			}
		case ID_METHOD_QUICKSORT: {
			g_algManager->SetAlgorithm(g_Algorithms[AQUICK_SORT]);
			if (g_bRegenerate) g_algManager->RegenerateData();
			g_algManager->RunAgain();
			break;
		}
		case ID_DATAORDER_SORTED: { 
			    g_algManager->GenerateData(doSorted); 
				g_algManager->RunAgain(); 
				break; 
		    }
		case ID_DATAORDER_REVERSED: { 
			    g_algManager->GenerateData(doReversed); 
				g_algManager->RunAgain(); 
				break; 
			}
	    case ID_DATAORDER_RANDOMIZED: { 
			    g_algManager->GenerateData(doRandomized); 
				g_algManager->RunAgain(); 
				break; 
			}
	    case ID_DATAORDER_SPECIALRANDOMIZED: { 
			    g_algManager->GenerateData(doSpecialRandomized); 
				g_algManager->RunAgain(); 
				break; 
			}
        case ID_DATAORDER_REGENERATE: {
			    g_bRegenerate = !g_bRegenerate;
				CheckMenuItem(hMenu, iId, (g_bRegenerate ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_ELEMENTS_SETTO100: {
				g_algManager->SetNumOfElements(100);
				g_algManager->RunAgain();
				break;
			}
		case ID_ELEMENTS_SETTO20: {
				g_algManager->SetNumOfElements(20);
				g_algManager->RunAgain();
				break;
			}
	    case ID_ELEMENTS_SETTO200: {
				g_algManager->SetNumOfElements(200);
				g_algManager->RunAgain();
				break;
			}
	    case ID_ELEMENTS_INCREASE: {
				g_algManager->SetNumOfElements(g_algManager->GetNumOfElements()+iElementsDelta);
				g_algManager->RunAgain();
				break;
			}
	    case ID_ELEMENTS_DECREASE: {
			if (g_algManager->GetNumOfElements() > iElementsMin)
				    g_algManager->SetNumOfElements(g_algManager->GetNumOfElements()-iElementsDelta);
				g_algManager->RunAgain();
				break;
			}
		case ID_VIEW_LIGHT: {
			    g_bLight = !g_bLight;
				CheckMenuItem(hMenu, iId, (g_bLight ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_VIEW_TEXTURED: {
			    g_bTextured = !g_bTextured;
				g_avSystem->m_bTextured = g_bTextured;
				CheckMenuItem(hMenu, iId, (g_bTextured ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
	    case ID_VIEW_REFLECTION: {
			    g_bReflection = !g_bReflection;
				CheckMenuItem(hMenu, iId, (g_bReflection ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_VIEW_FLOOR: {
			    g_bFloor = !g_bFloor;
				CheckMenuItem(hMenu, iId, (g_bFloor ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_VIEW_FRAME: {
				g_avSystem->m_bFrame = !g_avSystem->m_bFrame;
				CheckMenuItem(hMenu, iId, (g_avSystem->m_bFrame ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_VIEW_HORIZ: {
				g_avSystem->m_bHoriz = !g_avSystem->m_bHoriz;
				CheckMenuItem(hMenu, iId, (g_avSystem->m_bHoriz ? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_VIEW_INFO: {
				g_bInfo = !g_bInfo;
				CheckMenuItem(hMenu, iId, (g_bInfo? MF_CHECKED : MF_UNCHECKED));
				break;
			}
		case ID_FLOW_PAUSE: {
				g_algManager->SwapPause();
				break;
			}
		case ID_FLOW_INCREASETEMPO: { g_algManager->SetTempo(g_algManager->GetTempo()+fTempoDelta); break; }
		case ID_FLOW_DECREASETEMPO: { g_algManager->SetTempo(g_algManager->GetTempo()-fTempoDelta); break; }
		case ID_FLOW_SETTOSLOW: { g_algManager->SetTempo(fTempoDelta); break; }
		case ID_FLOW_SETTOFAST: { g_algManager->SetTempo(fTempoDelta*12); break; }
		case ID_HELP_ABOUT: {
				MessageBox(NULL, "Created by Bart \"Fen\" Filipek\n9th April 2006, Updated in December 2019", "Info", MB_OK | MB_ICONQUESTION);
				break;
			}
	}

	return true;
}

bool OnExit() {
	if (MessageBox(NULL, "Do you really want to exit?", "Exit?", MB_YESNO | MB_ICONQUESTION) == IDYES)
		return false;

	return true;
}

// end of file ----------------------------------------------------------------+
