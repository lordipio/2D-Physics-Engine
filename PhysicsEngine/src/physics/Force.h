#ifndef FORCE_H
#define FORCE_H

#include "Vec2.h"

struct Force
{
	static Vec2 GenerateDragForce(const Vec2& Velocity, float k);
};

#endif // !FORCE_H
