#ifndef _GAME_OBJECT_2D_H_
#define _GAME_OBJECT_2D_H_

//=================================================================
//the base Game Object 2D
//=================================================================

#include <d3d11_1.h>
#include "SimpleMath.h"
#include <string>
using namespace DirectX;
using namespace SimpleMath;
using namespace std;

enum VisibleState
{
	MENU = 1,
	FPS_ONLY = 2,
	TPS_ONLY = 3,
	GAME = 4,
	GAME_WON = 5,
	GAME_LOSE = 6,
	ALL = 7
};

struct GameData;
struct DrawData2D;

class GameObject2D
{
public:
	GameObject2D();
	virtual ~GameObject2D() {};

	virtual void Tick(GameData* _GD);
	virtual void Draw(DrawData2D* _DD);

	 

	//setters

	void SetPos(Vector2 _pos) { m_pos = _pos; }
	void SetRot(float _rot) { m_rotation = _rot; }
	void SetColour(Color _colour) { m_colour = _colour; }
	void SetScale(Vector2 _scale) { m_scale = _scale; }
	void SetScale(float _scale) { m_scale = _scale * Vector2::One; }
	void SetOrigin(Vector2 _origin) { m_origin = _origin; }
	void SetVisibilityState(VisibleState _state) { m_visibilityState = _state; m_visibilityChanged = true;}

protected:
	Vector2 m_pos;
	float m_rotation;
	Vector2 m_scale;
	Color m_colour;
	Vector2 m_origin;
	bool m_isVisible;
	VisibleState m_visibilityState;
	bool m_visibilityChanged = true;

};


#endif