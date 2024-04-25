#ifndef _ENEMY_H_
#define _ENEMY_H_
#include "terrain.h"
#include "Player.h"
#include "GameMap.h"
#include "EnemyBullets.h"

class Enemy : public CMOGO
{

public:
	Enemy(const std::string&, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, std::shared_ptr<Player> _target, std::shared_ptr<GameMap> m_Map);
	~Enemy();

	virtual void Tick(GameData* _GameData) override;

	void SetTilePos(Vector2 _tilePos) { m_tilePos = _tilePos; };
	Vector2 GetTilePos() { return m_tilePos; }

	void MoveTo(Vector3 targetPos, GameData* _GameData);
	void MoveAlongPath(Vector3 targetPos, GameData* _GameData);

	void SetBullet(std::shared_ptr<EnemyBullets> bullet) { m_bullet = bullet;}


protected:

	//variables for pathfinging
	std::shared_ptr<Player> target;
	std::shared_ptr<GameMap> m_map;
	Vector2 m_tilePos;
	Vector2 prevousGoal;
	int pathIndex;
	Vector2 nextTile;
	Vector2 nextPos;
	float distanceToTarget;
	
	//other variables
	float speed;
	float reloadTime = 10;
	Vector3 previousDirection;
	std::shared_ptr<EnemyBullets> m_bullet;
};


#endif