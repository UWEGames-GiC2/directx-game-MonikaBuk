#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "CMOGO.h"
#include "Bullet.h"

//=================================================================
//Base Player Class (i.e. a GO the player controls)
//=================================================================

class Player : public CMOGO
{

public:
	Player(string _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~Player();

	virtual void Tick(GameData* _GameData) override;
	std::vector<std::shared_ptr<Bullet>> bullets;
	bool GetIsGrounded(){ return isGrounded; }
	void SetIsGrounded(bool grounded) { isGrounded = grounded; };


protected:
	bool isGrounded;
	float gravity = 9.8f;
	float jumpSpeed = 50.0f;

};

#endif