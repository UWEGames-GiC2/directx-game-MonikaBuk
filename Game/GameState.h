#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

//=================================================================
//Possible GameStates
//=================================================================

enum GameState {
	GS_NULL = 0,
	GS_ATTRACT,
	GS_START_MENU,
	GS_PLAY_FPS_CAM,
	GS_PLAY_TPS_CAM,
	GS_WIN,
	GS_GAME_OVER,
	GS_COUNT
};

#endif
