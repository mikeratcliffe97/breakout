#pragma once
#include <string>
#include <Engine\OGLGame.h>

class Vector

{
public:

	Vector(float x_vec_val, float y_vec_val);
	~Vector();

	float get_x();
	float get_y();

	void x_set(float x_vec_val);
	void y_set(float y_vec_val);

private:
	float x;
	float y;
};