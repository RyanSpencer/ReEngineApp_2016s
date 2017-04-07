#include "CameraManager.h";

matrix4 CameraManager::getView(void)
{
	viewMatrix = glm::lookAt(eye, center, up);
	return viewMatrix;
}

matrix4 CameraManager::GetProjection(bool bOrtographic)
{
	if (bOrtographic) {
		return ortholgicalMatrix;
	}
	else {
		return perspectiveMatrix;
	}
}

void CameraManager::SetPosition(vector3 v3Position)
{
	eye = v3Position;
}

void CameraManager::SetTarget(vector3 v3Target)
{
	center = v3Target;
}

void CameraManager::SetUp(vector3 v3Up)
{
	up = v3Up;
}

void CameraManager::MoveForward(float fIncrement)
{
	eye.z -= fIncrement;
	center.z -= fIncrement;
}

void CameraManager::MoveSideways(float fIncrement)
{
	eye.x += fIncrement;
	center.x += fIncrement;
}

void CameraManager::MoveVertical(float fIncrement)
{
	eye.y += fIncrement;
	center.y += fIncrement;
}

void CameraManager::ChangePitch(float fIncrement)
{
	viewMatrix *= ToMatrix4(glm::angleAxis(fIncrement, vector3(1.0f, 0.0f, 0.0f)));
}

void CameraManager::ChangeRoll(float fIncrement)
{
	viewMatrix *= ToMatrix4(glm::angleAxis(fIncrement, vector3(0.0, 1.0f, 0.0f)));
}

void CameraManager::ChangeYaw(float fIncrement)
{
	viewMatrix *= ToMatrix4(glm::angleAxis(fIncrement, vector3(0.0f, 0.0f, 1.0f)));
}

CameraManager::CameraManager(void)
{
	Init();
}

CameraManager::~CameraManager(void)
{
	Release();
}

void CameraManager::Release(void)
{
}

void CameraManager::Init(void)
{
	viewMatrix = matrix4();
	ortholgicalMatrix = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, 0.01f, 1000.0f);
	perspectiveMatrix = glm::perspective(45.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
	eye = vector3(0.0f, 0.0f, 0.0f);
	center = vector3(0.0f, 0.0f, 0.0f);
	up = vector3(0.0f, 0.0f, 0.0f);
}