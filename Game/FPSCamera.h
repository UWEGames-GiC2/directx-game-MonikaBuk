
#ifndef _FPS_CAMERA_
#define _FPS_CAMERA_
#include "camera.h"
class FPSCamera : public Camera
{
public:
	FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, std::shared_ptr<GameObject> _targetPos, Vector3 _up, Vector3 _dpos, float _width, float _height);
	virtual ~FPSCamera();

	virtual void Tick(GameData* _GameData) override;

	float		GetNewPitch() { return m_newPitch; }

	float		GetNewYawn() { return m_newYawn; }

	Matrix		GetRot() { return m_rotCam; }

	Vector3		GetForwardVector() { return m_forwardVector; }
	Vector3 GetCenterOfScreen(GameData* _GameData);

protected:
	std::shared_ptr<GameObject> m_targetPosObject;
	float m_maxPitch = XMConvertToRadians(65);
	float m_minPitch = XMConvertToRadians(15);
	float offsetX = XMConvertToRadians(40);
	float offsetY = XMConvertToRadians(30);
	POINT GetCursorPosition(POINT cursorPos);

	float m_width;
	float m_height;
	

	float m_newPitch = m_maxPitch / 2;
	Matrix m_rotCam;
	float m_newYawn;
	Vector3	m_forwardVector = Vector3::Forward;
	
	//I'm following this object
	Vector3	m_dpos; //I'll lurk this far behind and away from it
	float Lerp(float a, float b, float t)
	{
		return a + t * (b - a);
	}
	float Clamp(float value, float min, float max)
	{
		return value < min ? min : (value > max ? max : value);
	}
};



#endif