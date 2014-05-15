#pragma once

class Vector2 {
public:

	Vector2(void);
	Vector2(Vector2 &copy);
	~Vector2(void);

protected:

	float x;                                                                                                           
	float y;
};