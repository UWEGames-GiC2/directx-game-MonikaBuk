
#ifndef _FPS_CAMERA_
#define _FPS_CAMERA_
#include "camera.h"
class FPSCamera : public Camera
{
public:
	FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _targetPos, Vector3 _up, Vector3 _dpos, float _width, float _height);
	virtual ~FPSCamera();

	virtual void Tick(GameData* _GameData) override;

	float		GetNewPitch() { return newPitch; }

	float		GetNewYawn() { return newYawn; }

	Matrix		GetRot() { return rotCam; }

	Vector3		GetForwardVector() { return m_forwardVector; }
	Vector3 GetCenterOfScreen(GameData* _GameData);

protected:
	std::shared_ptr<GameObject> m_targetPosObject;
	float maxPitch = XMConvertToRadians(90);
	float minPitch = XMConvertToRadians(0);
	POINT GetCursorPosition(POINT cursorPos);

	float m_width;
	float m_height;
	

	float newPitch = maxPitch / 2;

	Matrix rotCam;

	float newYawn;

	Vector3	m_forwardVector = Vector3::Forward;
	
	//I'm following this object
	Vector3	m_dpos; //I'll lurk this far behind and away from it
};



#endif