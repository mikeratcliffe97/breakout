#include <string>

#include <Engine/Keys.h>
#include <Engine/Input.h>
#include <Engine/InputEvents.h>
#include <Engine/Sprite.h>

#include "Game.h"
#include "Vector2.h"
//#include "Vector1.h"

/**
*   @brief   Default Constructor.
*   @details Consider setting the game's width and height
             and even seeding the random number generator.
*/
BreakoutGame::BreakoutGame()
{
	srand(time(NULL));

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

	mouse_callback_id = inputs->addCallbackFnc(
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
	ball_sprite = ball.spriteComponent()->getSprite();
	ball_sprite->yPos(game_height / 2);
	ball_sprite->xPos(game_width / 2);


	for (int i = 0; i < max_sprites; i++)
	{

		if (!blocks[i].addSpriteComponent(renderer.get(), ".\\Resources\\Textures\\puzzlepack\\png\\element_red_rectangle.png"))
		{
			return false;
		}
		ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
		
	}

	for (int i = 0; i < max_gems; i++)
	{
		if
			(!gems[i].addSpriteComponent(renderer.get(), ".\\Resources\\Textures\\puzzlepack\\png\\element_blue_diamond.png"))
		{
			return false;
		}
		
		gems[i].setVisibility(false);
		ASGE::Sprite* gem_sprite = gems[i].spriteComponent()->getSprite();
		gem_sprite->yPos(rand() % 1 + -2000);
		gem_sprite->xPos(1 + rand() % game_width - gem_width);

	}

	gems[0].setVisibility(true);
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
	game_height = 940;
	
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

			elapsed_time += (us.game_time.count() / 1000);
			int gem_drop = 10;
		

		auto paddle_pos = paddle_sprite->xPos();

		auto y_pos = ball_sprite->yPos();
		auto x_pos = ball_sprite->xPos();

		BallCollider(x_pos, y_pos);


		BrickCollider();


		PaddleMovement(paddle_pos, us);

		updateBall(x_pos, us, y_pos);

		for (int i = 0; i < max_gems; i++)
		{

			if (elapsed_time >= gem_drop)
			{

				if (gems[i].isvisible())
				{

					ASGE::Sprite* gem_sprite = gems[i].spriteComponent()->getSprite();

					float gem_y_pos = gem_sprite->yPos();

					gem_y_pos += 150 * (us.delta_time.count() / 1000.f);
					gem_sprite->yPos(gem_y_pos);


					if (gems[i].spriteComponent()->getBoundingBox().isInside(
						paddle.spriteComponent()->getBoundingBox()) == true
						&& gems[i].isvisible())
					{
						gems[i].spriteComponent()->getSprite()->xPos(rand() % 1 + game_width - gem_width);
						gems[i].spriteComponent()->getSprite()->yPos(rand() % 1 + -200);

						score += 10000;


						break;
					}

					if (gem_sprite->yPos() >= game_height)
					{
						gems[i].spriteComponent()->getSprite()->xPos(rand() % 1 + game_width - gem_width);
						gems[i].spriteComponent()->getSprite()->yPos(rand() % 1 + -200);

						break;

					}

					elapsed_time = 0;
				}
			}
		}
	}
}

void BreakoutGame::BallCollider(float &x_pos, float &y_pos)
{
	//Wall Collision
	if ((ball_sprite->xPos() + ball_sprite->width() >= game_width) ||
		(ball_sprite->xPos() < 0))
	{
		ball_direction.x_set(ball_direction.get_x() * -1);
	}

	//Ceiling Collision
	if (ball_sprite->yPos() < 0)
	{
		ball_direction.y_set(ball_direction.get_y() * -1);
	}

	//Floor Reset
	if (ball_sprite->yPos() > game_height + 50)
	{
		reset(x_pos, y_pos);
		--player_life;
	}

	//Paddle Collision
	if (ball.spriteComponent()->getBoundingBox().isInside(
		paddle.spriteComponent()->getBoundingBox()))
	{
		ball_direction.y_set(ball_direction.get_y() * -1);
		ball_direction.x_set(ball_direction.get_x() * 1);
	}
}

void BreakoutGame::BrickCollider()
{
	//Block Collision
	for (int i = 0; i < max_sprites; i++)
	{
		if (blocks[i].spriteComponent()->getBoundingBox().isInside(
			ball.spriteComponent()->getBoundingBox()) == true
			&& blocks[i].isvisible())
		{
			ball_direction.y_set(ball_direction.get_y() * -1);
			ball_direction.x_set(ball_direction.get_x() * 1);
			ball_direction.normalise();
			blocks[i].setVisibility(false);
			blocks_hit++;
			score+=150;
			break;
		}
		
	}
}

void BreakoutGame::PaddleMovement(float &paddle_pos, const ASGE::GameTime & us)
{
	//Paddle Movement speed
	if (paddle_left)
	{
		if (paddle_sprite, paddle_pos >= 0)
		{
			paddle_pos -= paddle.velocity * (us.delta_time.count() / 1000.f);
		}

		else
		{
			paddle_left = false;
		}
	}

	if (paddle_right)
	{
		if (paddle_sprite, paddle_pos + paddle_sprite->width() <= game_width)
		{
			paddle_pos += paddle.velocity * (us.delta_time.count() / 1000.f);
		}

		else
		{
			paddle_right = false;
		}

	}


	//Position Updates
	paddle_sprite->xPos(paddle_pos);
}


void BreakoutGame::updateBall(float &x_pos, const ASGE::GameTime & us, float &y_pos)
{
	
	//Ball Speed
	x_pos += (ball.velocity / 2) * ball_direction.get_x() * (us.delta_time.count() / 1000.f);
	y_pos += (ball.velocity / 2) * ball_direction.get_y() * (us.delta_time.count() / 1000.f);
	ball_sprite->xPos(x_pos);
	ball_sprite->yPos(y_pos);
}

/**
*   @brief   Renders the scene
*   @details Renders all the game objects to the current frame.
	         Once the current frame is has finished the buffers are
			 swapped accordingly and the image shown.
*   @return  void
*/
void BreakoutGame::render(const ASGE::GameTime & us)
{
	//int rand_pos = (rand() % game_width);
	
	

	renderer->setFont(0);

	if (in_menu)
	{
		renderer->renderText("\tWelcome to Breakout \nBreak all bricks on screen. \nYou have 3 lives. \nLives are lost when you miss the ball.\nPress Enter to start game",
			200, 200, 1.0, ASGE::COLOURS::WHITE);
	}
	else if (player_life > 0 && blocks_hit != 50)
	{
		std::string life_str = "LIVES: " + std::to_string(player_life);
		std::string score_str = "SCORE: " + std::to_string(score);
		renderer->renderText(score_str, 500, 850, 1.0, ASGE::COLOURS::WHITE);
		renderer->renderText(life_str, 500, 900, 1.0, ASGE::COLOURS::WHITE);
		renderer->renderSprite(*paddle_sprite);
		paddle_sprite->xPos(); //predefined in init
		paddle_sprite->yPos(game_height - 100); //umnoving

		renderer->renderSprite(*ball_sprite);

		BlockUpdate();
	

		for (int i = 0; i < max_gems; i++)
		{
			if (gems[i].isvisible())
			{
				ASGE::Sprite* gem_sprite = gems[i].spriteComponent()->getSprite();
				renderer->renderSprite(*gem_sprite);
			}
		}
	}

	else if (player_life <= 0)
	{
		lose();
	}

	else
	{
		win();
	}
}

void BreakoutGame::BlockUpdate()
{
	int x_block_total = 0;
	int y_block_total = 0;

	for (int i = 0; i < max_sprites; i++)
	{
		ASGE::Sprite* block_sprite = blocks[i].spriteComponent()->getSprite();
		
		blocks[i].spriteComponent()->getSprite()->xPos(x_block_total * block_width);
		blocks[i].spriteComponent()->getSprite()->yPos(y_block_total * block_height);
		
		if (blocks_hit >= 10)
		{
			blocks[i].spriteComponent()->getSprite()->yPos((y_block_total * block_height) + 30);
		}

		if (blocks_hit >= 20)
		{
			blocks[i].spriteComponent()->getSprite()->yPos((y_block_total * block_height) + 60);
		}

		if (blocks_hit >= 30)
		{
			blocks[i].spriteComponent()->getSprite()->yPos((y_block_total * block_height) + 90);
		}

	
		x_block_total++;
		if (i == 9 || i == 19 || i == 29 || i == 39 || i == 49)
		{
			x_block_total = 0;
			y_block_total++;
		}
		
		if (blocks[i].isvisible())
		{
			renderer->renderSprite(*blocks[i].spriteComponent()->getSprite());
			
		}

	}
	
}


void BreakoutGame::reset(float& x_pos, float& y_pos)
{
	
	x_pos = game_width / 2;
	y_pos = game_height / 2;
	
}

void BreakoutGame::win()
{
	renderer->renderText("CONGRATULATIONS \nYOU WIN", 100, 400, 2.0, ASGE::COLOURS::RED);
	std::string score_str = "SCORE WAS:  " + std::to_string(score);
	renderer->renderText(score_str, 100, 450, 1.0, ASGE::COLOURS::RED);
}

void BreakoutGame::lose()
{
	renderer->renderText("YOU LOST ALL YOUR LIVES \n Press Esc to Exit Game", 100, 400, 2.0, ASGE::COLOURS::TOMATO);
}