
#ifndef _FPS_CAMERA_
#define _FPS_CAMERA_
#include "camera.h"
class FPSCamera : public Camera
{
public:
	FPSCamera(float _fieldOfView, float _aspectRatio, float _nearPlaneDistance, float _farPlaneDistance, GameObject* _target, GameObject* _targetForRor, Vector3 _up, Vector3 _dpos);
	virtual ~FPSCamera();

	virtual void Tick(GameData* _GD) override;

protected:
	GameObject* m_targetPosObject;
	GameObject* m_targetRotObject;
	float maxPitch = XMConvertToRadians(90);
	float minPitch = XMConvertToRadians(0);

	float newPitch = maxPitch / 2;
	
	//I'm following this object
	Vector3	m_dpos; //I'll lurk this far behind and away from it
};



#endif