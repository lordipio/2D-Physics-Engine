#include "CollisionDetection.h"
#include "E:\Projects\PhysicsEngine\PhysicsEngine\src\Graphics.h"
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

	if (isBody1PolygonOrBox && isBody2Circle)
	{
		return IsCollidingPolygonCircle(body1, body2, contact);
	}

	if (isBody1Circle && isBody2PolygonOrBox)
	{
		return IsCollidingPolygonCircle(body2, body1, contact);
	}

	return false;
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

bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact)
{
	PolygonShape* shapeA = (PolygonShape*)a->shape;
	PolygonShape* shapeB = (PolygonShape*)b->shape;

	Vec2 pointA;
	Vec2 axisA;
	float SepAB;
	
	Vec2 pointB;
	Vec2 axisB;
	float SepBA;

	SepAB = shapeA->FindMinSeparation(shapeB, axisA, pointA);
	
	if (SepAB > 0)
		return false;
	

	SepBA = shapeB->FindMinSeparation(shapeA, axisB, pointB);

	if (SepBA > 0)
		return false;


	contact.body1 = a;
	contact.body2 = b;
	if (SepAB > SepBA)
	{
		contact.depth = -SepAB;
		contact.normal = axisA.Normal();
		printf("PointA.x: %f		PointB.y: %f\n", pointA.x, pointA.y);
		contact.start = pointA;
		contact.end = pointA + contact.normal * contact.depth;
	}
	else
	{
		contact.depth = -SepBA;
		contact.normal = -axisB.Normal();
		printf("PointB.x: %f		PointB.y: %f\n", pointB.x, pointB.y);
		contact.end = pointB;
		contact.start = pointB - contact.normal * contact.depth;
	}

	return true;

}

bool CollisionDetection::IsCollidingPolygonCircle(Body* polygonBody, Body* circleBody, Contact& contact)
{
	PolygonShape* polygonShape = (PolygonShape*)polygonBody->shape;
	CircleShape* circleShape = (CircleShape*)circleBody->shape;
	std::vector<Vec2> vertices = polygonShape->worldVertices;

	Vec2 currentVertex;
	Vec2 nextVertex;
	Vec2 currentNormal;
	Vec2 currentEdge;

	Vec2 minMainVertex;
	Vec2 minNextVertex;
	Vec2 nearestEdgeNormal;
	Vec2 nearestEdge;


	bool isOutside = false;
	float smallestDistanceBetweenCircleCenterAndEdge;
	float distanceBetweenCircleCenterAndEdge;

	int verticesNumber = vertices.size();
	for (int i = 0; i < verticesNumber; i++)
	{
		currentVertex = vertices[i];
		nextVertex = vertices[ (i + 1) % verticesNumber];
		currentEdge = nextVertex - currentVertex;
		currentNormal = currentEdge.Normal();
		distanceBetweenCircleCenterAndEdge = (circleBody->Position - currentVertex).Dot(currentNormal);

		if (i == 0)
			smallestDistanceBetweenCircleCenterAndEdge = distanceBetweenCircleCenterAndEdge;

		if (distanceBetweenCircleCenterAndEdge > 0)
		{
			smallestDistanceBetweenCircleCenterAndEdge = distanceBetweenCircleCenterAndEdge;
			minMainVertex = currentVertex;
			minNextVertex = nextVertex;
			nearestEdgeNormal = currentNormal;
			nearestEdge = currentEdge;
			isOutside = true;
			break;
		}
		// circle is inside of the polygon
		else if (smallestDistanceBetweenCircleCenterAndEdge <= distanceBetweenCircleCenterAndEdge)
		{
			smallestDistanceBetweenCircleCenterAndEdge = distanceBetweenCircleCenterAndEdge;
			minMainVertex = currentVertex;
			minNextVertex = nextVertex;
			nearestEdgeNormal = currentNormal;
			nearestEdge = currentEdge;
		}
	}

	if (isOutside)
	{
		Vec2 centerToMainVertex = circleBody->Position - minMainVertex;
		Vec2 centerToNextVertex = circleBody->Position - minNextVertex;

		if (currentEdge.Dot(centerToMainVertex) < 0) // A
		{
			if (centerToMainVertex.MagnitudeSquared() <= circleShape->radius * circleShape->radius)
			{
				contact.body1 = polygonBody;
				contact.body2 = circleBody;
				contact.normal = centerToMainVertex.Normalize();
				contact.start = circleBody->Position - contact.normal * circleShape->radius;
				contact.end = minMainVertex;
				contact.depth = (contact.start - contact.end).Magnitude();
			}

			else
				return false;
		}

		
		else if (currentEdge.Dot(centerToNextVertex) > 0) // B
			{
				if (centerToNextVertex.MagnitudeSquared() <= circleShape->radius * circleShape->radius)
				{
					contact.body1 = polygonBody;
					contact.body2 = circleBody;
					contact.normal = centerToNextVertex.Normalize();
					contact.start = circleBody->Position - contact.normal * circleShape->radius;
					contact.end = minNextVertex;
					contact.depth = (contact.start - contact.end).Magnitude();
				}
				else
					return false;
			}
		

		else // C
		{
			float circleCenterToEdgeDist = nearestEdgeNormal.Dot(centerToMainVertex);
			if (circleCenterToEdgeDist < circleShape->radius)
			{
				contact.body1 = polygonBody;
				contact.body2 = circleBody;
				contact.normal = nearestEdgeNormal;
				contact.start = circleBody->Position - contact.normal * circleShape->radius;
				contact.end = circleBody->Position - contact.normal * circleCenterToEdgeDist;
				contact.depth = (contact.start - contact.end).Magnitude();
			}
			else
				return false;
		}
	}
	else // inside
	{
		float circleCenterToEdgeDist = nearestEdgeNormal.Dot(circleBody->Position - minMainVertex);
		contact.body1 = polygonBody;
		contact.body2 = circleBody;
		contact.normal = nearestEdgeNormal;
		contact.start = circleBody->Position - contact.normal * circleShape->radius;
		contact.end = circleBody->Position - contact.normal * circleCenterToEdgeDist;
		contact.depth = (contact.start - contact.end).Magnitude();
	}

	return true;
}
