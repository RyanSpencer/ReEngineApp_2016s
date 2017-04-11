#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;

	//Set up each new array
	lerpPoints = new vector3[11];
	sphereMat = new matrix4[11];
	
	//Fill the points array with each point on the list
	lerpPoints[0] = vector3(-4.0f, -2.0f, 5.0f);
	lerpPoints[1] = vector3(1.0f, -2.0f, 5.0f);
	lerpPoints[2] = vector3(-3.0f, -1.0f, 3.0f);
	lerpPoints[3] = vector3(2.0f, -1.0f, 3.0f);
	lerpPoints[4] = vector3(-2.0f, 0.0f, 0.0f);
	lerpPoints[5] = vector3(3.0f, 0.0f, 0.0f);
	lerpPoints[6] = vector3(-1.0f, 1.0f, -3.0f);
	lerpPoints[7] = vector3(4.0f, 1.0f, -3.0f);
	lerpPoints[8] = vector3(0.0f, 2.0f, -5.0f);
	lerpPoints[9] = vector3(5.0f, 2.0f, -5.0f);
	lerpPoints[10] = vector3(1.0f, 3.0f, -5.0f);

	for (int i = 0; i < 11; i++) {
		//Each point gets its own matrix and sphere of size 0.1
		sphereMat[i] = IDENTITY_M4;
		sphereMat[i] = glm::translate(lerpPoints[i]);
		//Generate the sphere then move it to each point
		m_pMeshMngr->InstanceSphere(0.1f, 5, RERED, std::to_string(i));
		m_pMeshMngr->SetModelMatrix(sphereMat[i], i + 1);
	}
	//Establish the animated sprite at the first point
	matrix4 mat = IDENTITY_M4;
	mat = glm::translate(lerpPoints[0]);

	m_pMeshMngr->SetModelMatrix(mat, "WallEye");

}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan; 
#pragma endregion

#pragma region Your Code goes here
	//If we've completed one section of movement
	if (fRunTime > fDuration) {
		//Reset runtime and increased the loop counter
		fRunTime = 0;
		loopNum++;
		//If we just hit the last point
		if (loopNum >= 11) {
			//Reset the loop counter 
			loopNum = 0;
		}
	}
	//Grab the model matrix and set it to a variable
	matrix4 modelMesh = m_pMeshMngr->GetModelMatrix("WallEye");
	//grab your fpercent variable going from the number of loops that should be in an average section
	float fpercent = MapValue(static_cast<float>(fRunTime / fTimeSpan), 0.0f, static_cast<float>(fDuration / fTimeSpan), 0.0f, 1.0f);
	//Create your lerp vector
	vector3 lerpVec(fpercent, fpercent, fpercent);
	//Translate from the current point you've been at to the next one using the lerp vector
	if (loopNum == 10) 	modelMesh = glm::translate(IDENTITY_M4, glm::lerp(lerpPoints[loopNum], lerpPoints[0], lerpVec));
	else modelMesh = glm::translate(IDENTITY_M4, glm::lerp(lerpPoints[loopNum], lerpPoints[loopNum + 1], lerpVec));
	//overrite the old matrix
	m_pMeshMngr->SetModelMatrix(modelMesh, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	delete[] sphereMat;
	delete[] lerpPoints;
	super::Release(); //release the memory of the inherited fields
}