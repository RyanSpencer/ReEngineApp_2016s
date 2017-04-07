#include "AppClass.h"


void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("SLERP - YOUR USER NAME GOES HERE"); // Window Name

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);
}

void AppClass::InitVariables(void)
{
	//Setting the position in which the camera is looking and its interest point
	m_pCamera->SetPosition(vector3(0.0f, 0.0f, 15.0f));
	m_pCamera->SetTarget(vector3(0.0f, 0.0f, 0.0f));
	m_pCamera->SetUp(vector3(0.0f, 1.0f, 0.0f));

	//Setting the color to black
	m_v4ClearColor = vector4(0.0f);

	//Setting the days duration
	m_fDay = 1.0f;
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Getting the time between calls
	double fCallTime = m_pSystem->LapClock();
	//Counting the cumulative time
	static double fRunTime = 0.0f;
	fRunTime += fCallTime;

}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	PrimitiveClass cone = PrimitiveClass();
	cone.GenerateCone(2.0f, 3.0f, 4, RERED);
	cone.Render(m_pCamera->GetProjection(false), m_pCamera->getView(), IDENTITY_M4); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}