#include <math.h>
#include "Vector2.h"

/**
*   @brief   Constructor.
*   @details Requires values for it's x and y magnitudes.
*   @return  void
*/
vector2::vector2(float x_, float y_) : x(x_), y(y_)
{

};

/**
*   @brief   Copies a vector.
*   @details Constructs a new vector2 from an existing.
*   @return  void
*/
vector2::vector2(const vector2 & rhs)
{
	this->x = rhs.x;
	this->y = rhs.y;
}

/**
*   @brief   Normalises vector.
*   @details Turns the vector into a unit vector.
*   @return  void
*/
void vector2::normalise()
{
	float magnitude = sqrtf((x*x) + (y*y));

	if (!magnitude)
		return;

	x /= magnitude;
	y /= magnitude;
}

/**
*   @brief   Scales the vector.
*   @details Uses a single scalar value to adjust the vector.
*   @return  void
*/
vector2 vector2::operator*(float scalar)
{
	vector2 vec(*this);
	vec.x *= scalar;
	vec.y *= scalar;
	return vec;
}

void vector2::x_set(float x_vec_val)
{
	x = x_vec_val;
}

void vector2::y_set(float y_vec_val)
{
	y = y_vec_val;
}

float vector2::get_x()
{
	return x;
}

float vector2::get_y()
{
	return y;
}