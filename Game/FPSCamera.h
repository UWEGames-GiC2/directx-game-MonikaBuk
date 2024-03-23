
#ifndef _FPS_CAMERA_
#define _FPS_CAMERA_
#include "camera.h"
class FPSCamera : public Camera
{
public:
	FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, Vector3 _up, Vector3 _dpos, float _width, float _height);
	virtual ~FPSCamera();

	virtual void Tick(GameData* _GD) override;

	float		GetNewPitch() { return newPitch; }

	float		GetNewYawn() { return newYawn; }

	Matrix		GetRot() { return rotCam; }

	Vector3		GetForwardVector() { return m_forwardVector; }
	Vector3 GetCenterOfScreen(GameData* _GD);

protected:
	GameObject* m_targetPosObject;
	GameObject* m_targetRotObject;
	float maxPitch = XMConvertToRadians(90);
	float minPitch = XMConvertToRadians(0);
	POINT GetCursorPosition(POINT cursorPos);

	float m_width;
	float m_height;
	

	float newPitch = maxPitch / 2;

	Matrix rotCam;

	float newYawn;

	Vector3	m_forwardVector;
	
	//I'm following this object
	Vector3	m_dpos; //I'll lurk this far behind and away from it
};



#endif