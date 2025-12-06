#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H

#include "Body.h"
#include "Contact.h"

struct CollisionDetection
{
	static bool isColliding(Body* body1, Body* body2, Contact& contact);
	static bool isCollidingCircleCircle(Body* circleBody1, Body* circleBody2, Contact& contact);
	static bool IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact);
	static bool IsCollidingPolygonCircle(Body* polygonBody, Body* circleBody, Contact& contact);
};

#endif // !COLLISION_DETECTION_H
