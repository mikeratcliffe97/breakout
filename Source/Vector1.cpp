#include "Vector1.h"
#include <string>
#include <Engine/Platform.h>



Vector::Vector(float x_vec_val, float y_vec_val)
{
	x = x_vec_val;
	y = y_vec_val;
}


Vector::~Vector()
{
}


void Vector::x_set(float x_vec_val)
{
	x = x_vec_val;
}

void Vector::y_set(float y_vec_val)
{
	y = y_vec_val;
}

float Vector::get_x()
{
	return x;
}

float Vector::get_y()
{
	return y;
}