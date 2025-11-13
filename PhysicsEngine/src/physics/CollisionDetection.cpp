#include "CollisionDetection.h"
#include <limits>

bool CollisionDetection::isColliding(Body* body1, Body* body2, Contact& contact)
{
	
	bool isBody1Circle = body1->shape->GetType() == CIRCLE;
	bool isBody2Circle = body2->shape->GetType() == CIRCLE;

	bool isBody1PolygonOrBox = body1->shape->GetType() == BOX || body1->shape->GetType() == POLYGON;
	bool isBody2PolygonOrBox = body2->shape->GetType() == BOX || body2->shape->GetType() == POLYGON;


	if (isBody1Circle && isBody2Circle)
	{
		return isCollidingCircleCircle(body1, body2, contact);
	}

	if (isBody1PolygonOrBox && isBody2PolygonOrBox)
	{
		return IsCollidingPolygonPolygon(body1, body2, contact);
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


float CollisionDetection::FindMinSeparation(PolygonShape* a, PolygonShape* b)
{
	float separation = std::numeric_limits<float>::lowest();

	for (int i = 0; i < a->worldVertices.size(); i++)
	{
		Vec2 va = a->worldVertices[i];
		Vec2 normal = a->EdgeAt(i).Normal();
		
		float minStep = std::numeric_limits<float>::max();

		for (int j = 0; j < b->worldVertices.size(); j++)
		{
			Vec2 vb = b->worldVertices[j];
			minStep = std::min(minStep, normal.Dot(vb - va));
		}

		separation = std::max(separation, minStep);
	}

	return separation;
}

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact contact)
{
	PolygonShape* shapeA = (PolygonShape*)a->shape;
	PolygonShape* shapeB = (PolygonShape*)b->shape;

	return FindMinSeparation(shapeA, shapeB) <= 0 && FindMinSeparation(shapeB, shapeA) <= 0;
}