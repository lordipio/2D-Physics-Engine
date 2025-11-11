#ifndef CONTACT_H
#define CONTACT_H

#include "Body.h"

struct Contact
{
	Body* body1;
	Body* body2;

	Vec2 start;
	Vec2 end;

	float depth;
	Vec2 normal;

	void ResolveCollision();
	void ResolvePeneteration();
};

#endif // !CONTACT_H
