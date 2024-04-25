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
	Player(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF);
	~Player();

	virtual void Tick(GameData* _GameData) override;

	void SetBullets(std::vector<std::shared_ptr<Bullet>> _bullets) { m_bullets = _bullets; }

	bool GetIsGrounded(){ return m_isGrounded; }
	void SetIsGrounded(bool _grounded) { m_isGrounded = _grounded; };

	void SetTilePos(Vector2 _tilePos) { m_tilePos = _tilePos;};
	Vector2 GetTilePos(){ return m_tilePos;}

	int GetHealth() { return m_health;}
	void TakeDamage(int m_damage);
	bool IsHealthChanged() { return m_healthChanged;}
	void SetHealthChanged(bool _changed) { m_healthChanged = _changed;}

protected:
	bool m_isGrounded;
	float m_gravity = 9.8f;
	float m_jumpSpeed = 200.0f;
	float m_moveSpeed = 10000;
	int m_health = 100;
	bool m_healthChanged = false;
	Vector2 m_tilePos;
	std::vector<std::shared_ptr<Bullet>> m_bullets;
};

#endif