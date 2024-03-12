#ifndef _WEAPON_H_
#define _WEAPON_H_
#include "CMOGO.h"


struct GameData;
class Weapon :
	public CMOGO
{

public:
	Weapon(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, GameObject& owner);
	~Weapon();

	virtual void Tick(GameData* _GD) override;
	Vector3 CalculateDirectionToMiddleOfScreen(GameData* _GD);
	float derekszog;

	GameObject* m_owner;


protected:


};


#endif
