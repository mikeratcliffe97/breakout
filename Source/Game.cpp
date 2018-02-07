#include <string>

#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>

#include "Game.h"
#include "Vector2.h"
#include "Vector1.h"

/**
*   @brief   Default Constructor.
*   @details Consider setting the game's width and height
             and even seeding the random number generator.
*/
BreakoutGame::BreakoutGame()
{
}

/**
*   @brief   Destructor.
*   @details Remove any non-managed memory and callbacks.
*/
BreakoutGame::~BreakoutGame()
{
	this->inputs->unregisterCallback(key_callback_id);
	this->inputs->unregisterCallback(mouse_callback_id);


}

/**
*   @brief   Initialises the game.
*   @details The game window is created and all assets required to
			 run the game are loaded. The keyHandler and clickHandler
			 callback should also be set in the initialise function.
*   @return  True if the game initialised correctly.
*/
bool BreakoutGame::init()
{
	setupResolution();
	if (!initAPI())
	{
		return false;
	}

	toggleFPS();
	renderer->setWindowTitle("Breakout!");
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	// input handling functions
	inputs->use_threads = false;

	key_callback_id = inputs->addCallbackFnc(
		ASGE::E_KEY, &BreakoutGame::keyHandler, this);
	
	mouse_callback_id =inputs->addCallbackFnc(
		ASGE::E_MOUSE_CLICK, &BreakoutGame::clickHandler, this);


	if (!paddle.addSpriteComponent(renderer.get(), ".\\Resources\\Textures\\puzzlepack\\png\\paddleBlue.png"))
	{
		return false;
	}

	paddle_sprite = paddle.spriteComponent()->getSprite();
	paddle_sprite->xPos(game_width / 2);
	
	if (!ball.addSpriteComponent(renderer.get(), ".\\Resources\\Textures\\puzzlepack\\png\\ballGrey.png"))
	{
		return false;
	}
	ball_sprite->xPos(game_height / 2);
	ball_sprite->yPos(game_width / 2);
	ball_sprite = ball.spriteComponent()->getSprite();


	for (int i = 0; i < max_sprites; i++)
	{
		if (!blocks[i].addSpriteComponent(renderer.get(), ".\\Resources\\Textures\\puzzlepack\\png\\element_red_rectangle.png"))
		{
			return false;
		}
		blocks_sprites[i] = blocks[i].spriteComponent()->getSprite();

	}

	
	return true;
}

/**
*   @brief   Sets the game window resolution
*   @details This function is designed to create the window size, any 
             aspect ratio scaling factors and safe zones to ensure the 
			 game frames when resolutions are changed in size.
*   @return  void
*/
void BreakoutGame::setupResolution()
{
	// how will you calculate the game's resolution?
	// will it scale correctly in full screen? what AR will you use?
	// how will the game be framed in native 16:9 resolutions?
	// here are some abritrary values for you to adjust as you see fit
	// https://www.gamasutra.com/blogs/KenanBolukbasi/20171002/306822/Scaling_and_MultiResolution_in_2D_Games.php
	game_width = 640;
	game_height = 920;
}

/**
*   @brief   Processes any key inputs
*   @details This function is added as a callback to handle the game's
			 keyboard input. For this game, calls to this function
			 are thread safe, so you may alter the game's state as you
			 see fit.
*   @param   data The event data relating to key input.
*   @see     KeyEvent
*   @return  void
*/
void BreakoutGame::keyHandler(const ASGE::SharedEventData data)
{
	auto key = static_cast<const ASGE::KeyEvent*>(data.get());
	
	if (key->key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}

	
	if (in_menu)
	{
		if (key->key == ASGE::KEYS::KEY_ENTER)
		{
			in_menu = 0;
		}
	}

	if (!in_menu)
		
		//Bool Statements to make paddle move left
		if (key->key == ASGE::KEYS::KEY_A &&
			key->action == ASGE::KEYS::KEY_PRESSED)
		{
			paddle_left = true;
		}

	if (key->key == ASGE::KEYS::KEY_A &&
		key->action == ASGE::KEYS::KEY_RELEASED)
	{
		paddle_left = false;
	}

	//Bool Statements to make paddle move right
	if (key->key == ASGE::KEYS::KEY_D &&
		key->action == ASGE::KEYS::KEY_PRESSED)
	{
		paddle_right = true;
	}

	if (key->key == ASGE::KEYS::KEY_D &&
		key->action == ASGE::KEYS::KEY_RELEASED)
	{
		paddle_right = false;
	}
	
}

/**
*   @brief   Processes any click inputs
*   @details This function is added as a callback to handle the game's
		     mouse button input. For this game, calls to this function
             are thread safe, so you may alter the game's state as you
             see fit.
*   @param   data The event data relating to key input.
*   @see     ClickEvent
*   @return  void
*/
void BreakoutGame::clickHandler(const ASGE::SharedEventData data)
{
	auto click = static_cast<const ASGE::ClickEvent*>(data.get());

	double x_pos, y_pos;
	inputs->getCursorPos(x_pos, y_pos);
}


/**
*   @brief   Updates the scene
*   @details Prepares the renderer subsystem before drawing the
		     current frame. Once the current frame is has finished
		     the buffers are swapped accordingly and the image shown.
*   @return  void
*/
void BreakoutGame::update(const ASGE::GameTime& us)
{
	if (!in_menu)
	{
		

		auto paddle_pos = paddle_sprite->xPos();


		auto y_pos = ball_sprite->yPos();
		auto x_pos = ball_sprite->xPos();

		if ((ball_sprite->xPos() + ball_sprite->width() >= game_width) ||
			(ball_sprite->xPos() < 0))
		{
			ball_direction.x_set(ball_direction.get_x() * -1);

		}

		if (paddle_left)
		{
			if (paddle_sprite, paddle_pos >= 0)
			{
				paddle_pos -= velocity * (us.delta_time.count() / 1000.f);
			}

			else
			{
				paddle_left = false;
			}
		}


		if (paddle_right)
		{
			if (paddle_sprite, paddle_pos + paddle_sprite->width()  <= game_width)
			{
				paddle_pos += velocity * (us.delta_time.count() / 1000.f);
			}

			else
			{
				paddle_right = false;
			}

		}

		paddle_sprite->xPos(paddle_pos);
		ball_sprite->xPos(x_pos);


		x_pos += velocity * ball_direction.get_x() * (us.delta_time.count() / 1000.f);
		y_pos += velocity * ball_direction.get_y() * (us.delta_time.count() / 1000.f);
		ball_sprite->xPos(x_pos);
		ball_sprite->yPos(y_pos);
	}

}

	

	//make sure you use delta time in any movement calculations!




/**
*   @brief   Renders the scene
*   @details Renders all the game objects to the current frame.
	         Once the current frame is has finished the buffers are
			 swapped accordingly and the image shown.
*   @return  void
*/
void BreakoutGame::render(const ASGE::GameTime &)
{
	renderer->setFont(0);

	if (in_menu)
	{
		renderer->renderText("Welcome to Breakout \nPress Enter to start game", 200, 200, 1.0, ASGE::COLOURS::WHITE);
	}
	else
	{
		
		renderer->renderSprite(*paddle_sprite);
		paddle_sprite->xPos(); //predefined in init
		paddle_sprite->yPos(game_height - 30); //umnoving
		
		
		
		renderer->renderSprite(*ball_sprite);
		


		int x_block_total = 0;
		int y_block_total = 0;

		for (int i = 0; i < max_sprites; i++)
		{
			
			blocks_sprites[i]->xPos(x_block_total * block_width);
			blocks_sprites[i]->yPos(y_block_total * block_height);

			x_block_total++;
			if (i == 9 || i == 19 || i == 29 || i == 39 || i == 49)
			{
				x_block_total = 0;
				y_block_total++;
			}
			
			
				renderer->renderSprite(*blocks_sprites[i]);
					
			

			
		}

	}


}
