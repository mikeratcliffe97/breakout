#pragma once
struct vector2
{
	// construction
	vector2(float x_vec_value, float y_vec_value_);
	vector2(const vector2& rhs);
	
	// operations
	vector2 operator*(float scalar);
	void normalise();
	
	float get_x();
	float get_y();

	void x_set(float x_vec_val);
	void y_set(float y_vec_val);

private:
	float x;
	float y;
};