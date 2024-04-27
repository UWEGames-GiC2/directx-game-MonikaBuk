#include "pch.h"
#include "Enemy.h"
#include <iostream>
#include "GameData.h"




Enemy::Enemy(const std::string& _fileName, ID3D11Device* _pd3dDevice, IEffectFactory* _EF, Vector3 _pos, float _pitch, float _yaw, float _roll, Vector3 _scale, std::shared_ptr<Player> _target, std::shared_ptr<GameMap> _map)
    : CMOGO(_fileName, _pd3dDevice, _EF)
{
	m_pos = _pos;
	m_pitch = _pitch;
	m_roll = _roll;
	m_yaw = _yaw;
	m_scale = _scale;
	target = _target;
    m_map = _map;
	SetVisibility(true);
    prevousGoal = Vector2(1,1);
    speed = 50.0f;
	
}

Enemy::~Enemy()
{
}

void Enemy::Tick(GameData* _GameData)
{
    if (!_GameData->isPaused) {
        distanceToTarget = (target->GetPos() - m_pos).Length();

        if (reloadTime <= 5.0f)
        {
            reloadTime += _GameData->m_DeltaTime;
        }

        if (distanceToTarget < 300.0f)
        {
            if (reloadTime >= 5.0f)
            {

                if (!m_bullet->IsShot())
                {
                    m_bullet->Fire();

                }
            }
            reloadTime = 0;
        }
        if (distanceToTarget < 400)
        {
            MoveTo(target->GetPos(), _GameData);
        }
        else if (distanceToTarget < 600)
        {
            MoveAlongPath(target->GetPos(), _GameData);
        }
    }
    // Call the base class Tick function
    CMOGO::Tick(_GameData);
}


void Enemy::MoveTo(Vector3 targetPos, GameData* _GameData)
{
    Vector3 direction = targetPos - m_pos;
    Vector3 diff = previousDirection - direction;
    float diffLength = diff.Length();
    if (diffLength > 0.1) {
        direction.Normalize();

        m_yaw = atan2f(direction.x, direction.z);
        m_pitch = 0.0f;
        m_roll = 0.0f;

        float minDistance = 50.0f;
        if (distanceToTarget > minDistance)
        {
            m_pos += direction * speed * _GameData->m_DeltaTime;
        }
        previousDirection = direction;
    }
 
}
void Enemy::MoveAlongPath(Vector3 targetPos, GameData* _GameData)
{
    Vector2 startTile = m_tilePos;
    Vector2 goalTile = target->GetTilePos();

    if (prevousGoal != goalTile)
    {
        prevousGoal = goalTile;

        std::vector<Vector2> path = m_map->FindPath(startTile, goalTile);
        if (!path.empty())
        {
            if (path.size() > 1) {
                path.erase(path.begin());
                nextTile = path.front();
                nextPos = m_map->TileToWorld(nextTile);
            }
        }
    }
    MoveTo(Vector3(nextPos.x, 50, nextPos.y), _GameData);
}