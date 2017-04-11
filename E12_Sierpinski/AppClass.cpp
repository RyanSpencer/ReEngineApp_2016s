#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E12 - Instance Rendering");
	m_v4ClearColor = vector4(REBLACK, 1.0f);
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), vector3(0.0f, 0.0f, 0.0f), REAXISY);

	m_pMesh = new MyMesh();
	
	//Creating the Mesh points
	m_pMesh->AddVertexPosition(vector3(0.0f, 0.0f, 0.0f));
	m_pMesh->AddVertexColor(REBLUE);
	m_pMesh->AddVertexPosition(vector3(-0.5f,  -1.0f, 0.0f));
	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexPosition(vector3(0.5f, -1.0f, 0.0f));
	m_pMesh->AddVertexColor(RERED);

	int a[30][30]; // rows then collumns
	int i, j; 
	int size = 30;

	for (i = 0; i < size; i++) {
		for (j = 0; j < size; j++) {
			a[i][j] = 0;
		}
	}

	//First of each row should be 1
	for (i = 0; i <= size; i++) {
		a[i][0] = 1;
	}

	//Generate the full Triangle
	for (i = 1; i <= size; i++) {
		for (j = 1; j <= size - i; j++) {
			//Else the point = the left one and the top one
			a[i][j] = a[i - 1][j] + a[i - 1][j - 1];
		}
	}

	//Compiling the mesh
	m_pMesh->CompileOpenGL3X();

	m_fMatrixArray = new float[m_nObjects * 16];
	int k = 1;

	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 30; j++) {
			if (a[i][j] % 2 != 0) {
					const float* m4MVP = glm::value_ptr(glm::translate(vector3(1.0f * (((-0.5f * i) + j)) , 1.0f * -i, 0.0f)));
					memcpy(&m_fMatrixArray[k * 16], m4MVP, 16 * sizeof(float));
					k++;
			}
		}
	}
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCameraMngr->CalculateView();

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	printf("FPS: %d            \r", nFPS);//print the Frames per Second
										  //Print info on the screen
	m_pMeshMngr->PrintLine("");//Add a line on top
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	ClearScreen();
	
	//Matrices from the camera
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	m_pMesh->RenderList(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), m_fMatrixArray, m_nObjects);//Rendering nObjects

	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}