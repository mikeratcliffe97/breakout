#pragma once
#include <string>
#include <Engine/OGLGame.h>

#include "GameObject.h"
#include "Rect.h"
#include "Vector1.h"

/**
*  An OpenGL Game based on ASGE.
*/
class BreakoutGame :
	public ASGE::OGLGame
{
public:
	BreakoutGame();
	~BreakoutGame();
	virtual bool init() override;

private:
	void keyHandler(const ASGE::SharedEventData data);
	void clickHandler(const ASGE::SharedEventData data);
	void setupResolution();

	virtual void update(const ASGE::GameTime &) override;
	void BrickCollider();
	void updateBall(float &x_pos, const ASGE::GameTime & us, float &y_pos);
	virtual void render(const ASGE::GameTime &) override;

	void BlockUpdate();

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	void win();
	void lose();

	//Add your GameObjects
	GameObject paddle;
	ASGE::Sprite* paddle_sprite;
	
//Paddle movement bool
	bool paddle_left = false;
	bool paddle_right = false;
	
	
GameObject ball;
	ASGE::Sprite* ball_sprite;
	vector2 ball_direction{1, 1 };
	void reset(float& x_pos, float& y_pos);

	//Block objects and data
	GameObject blocks[50] = {};
	ASGE::Sprite* blocks_sprites[50] = {};
	
	GameObject gems[5] = {};
	ASGE::Sprite* gem_sprites[5] = {};

	int max_sprites = 50;
	int blocks_hit = 0;
	int block_width = 64;
	int block_height = 32;
	
	int max_gems = 5;
	int gem_width = 48;
	int gem_height = 48;

	//menu options
	bool in_menu = true;
	int player_life = 3;
	
};