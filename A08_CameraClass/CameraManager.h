#ifndef __CAMERAMANAGER_H_
#define __CAMERAMANAGER_H_

#include "RE\ReEng.h"
#include <SFML\Graphics.hpp>

using namespace ReEng;

class ReEngDLL CameraManager
{
public:
	CameraManager(void);

	~CameraManager(void);

	matrix4 getView(void);

	matrix4 GetProjection(bool bOrtographic);

	void SetPosition(vector3 v3Position);

	void SetTarget(vector3 v3Target);

	void SetUp(vector3 v3Up);

	void MoveForward(float fIncrement);

	void MoveSideways(float fIncrement);

	void MoveVertical(float fIncrement);

	void ChangePitch(float fIncrement);

	void ChangeRoll(float fIncrement);

	void ChangeYaw(float fIncrement);
private:
	matrix4 viewMatrix;

	matrix4 ortholgicalMatrix;

	matrix4 perspectiveMatrix;

	vector3 eye;
	
	vector3 center;

	vector3 up;

	void Release(void);

	void Init(void);
};
#endif
