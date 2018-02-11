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
	virtual void render(const ASGE::GameTime &) override;

	int  key_callback_id = -1;	        /**< Key Input Callback ID. */
	int  mouse_callback_id = -1;        /**< Mouse Input Callback ID. */

	//Add your GameObjects
	GameObject paddle;
	ASGE::Sprite* paddle_sprite;
	int velocity = 700;
//Paddle movement bool
	bool paddle_left = false;
	bool paddle_right = false;
	
	
GameObject ball;
	ASGE::Sprite* ball_sprite;
	Vector ball_direction{1, -2 };
	void reset();

	//Block objects and data
	GameObject blocks[50] = {};
	ASGE::Sprite* blocks_sprites[50] = {};
	

	int max_sprites = 50;
	int block_width = 64;
	int block_height = 32;
	

	//menu options
	bool in_menu = true;

	
};