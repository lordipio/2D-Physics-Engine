#include "CollisionDetection.h"

bool CollisionDetection::isColliding(Body* body1, Body* body2, Contact& contact)
{
	if (body1->shape->GetType() == CIRCLE && body2->shape->GetType() == CIRCLE)
	{
		return isCollidingCircleCircle(body1, body2, contact);
	}
}

bool CollisionDetection::isCollidingCircleCircle(Body* circleBody1, Body* circleBody2, Contact& contact)
{
	CircleShape* circleShape1 = (CircleShape*)circleBody1->shape;
	CircleShape* circleShape2 = (CircleShape*)circleBody2->shape;

	Vec2 circle2To1 = circleBody2->Position - circleBody1->Position;
	float distanceSquare = circle2To1.MagnitudeSquared();

	float radiusSum = circleShape1->radius + circleShape2->radius;

	if (distanceSquare > radiusSum * radiusSum)
		return false;

	contact.body1 = circleBody1;
	contact.body2 = circleBody2;

	contact.normal = circle2To1.Normalize();

	contact.start = circleBody2->Position - contact.normal * circleShape2->radius;
	contact.end = circleBody1->Position + contact.normal * circleShape1->radius;

	contact.depth = (contact.start - contact.end).Magnitude();

	return true;
}
