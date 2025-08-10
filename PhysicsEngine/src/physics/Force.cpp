#include "Force.h"

Vec2 Force::GenerateDragForce(const Vec2& Velocity, float k)
{
	Vec2 DragForce = Vec2(0, 0);
	if (Velocity.MagnitudeSquared() > 0)
	{
		Vec2 DragDirection = Velocity.UnitVector() * -1;

		float DragMagnitude = Velocity.MagnitudeSquared() * k;

		DragForce = DragDirection * DragMagnitude;
	}

	return DragForce;
}

Vec2 Force::GenerateFrictionForce(const Vec2& Velocity, float k)
{
	Vec2 FrictionForce = Vec2(0, 0);

	if (Velocity.MagnitudeSquared() > 0)
	{
		Vec2 DragDirection = Velocity.UnitVector() * -1;

		float DragMagnitude = Velocity.MagnitudeSquared() * k;

		FrictionForce = DragDirection * DragMagnitude;
	}

	return FrictionForce;
}

Vec2 Force::GenerateGravitationalForce(const Body* BodyA, const Body* BodyB, float G, float DistanceMin, float DistanceMax)
{
	Vec2 distance = (BodyB->Position - BodyA->Position);

	float distanceSquaredMagnitude = distance.MagnitudeSquared();
	distanceSquaredMagnitude = std::clamp(distanceSquaredMagnitude, DistanceMin, DistanceMax);

	Vec2 normalizedDistance = distance.UnitVector();

	return (normalizedDistance * (G * BodyA->Mass * BodyB->Mass)) / distanceSquaredMagnitude;
}

Vec2 Force::GenerateSpringForce(const Body* body, const Vec2& anchor, const float& rest, const float& K)
{
	Vec2 distance = body->Position - anchor;

	float displacement = distance.Magnitude() - rest;

	Vec2 springForceDirection = distance.UnitVector();

	Vec2 springForce = springForceDirection * (displacement * -K);

	return springForce;
}

Vec2 Force::GenerateSpringForce(const Body* bodyA, const Body* bodyB, const float& rest, const float& K)
{
	Vec2 distance = bodyA->Position - bodyB->Position;

	float displacement = distance.Magnitude() - rest;

	Vec2 springForceDirection = distance.UnitVector();

	Vec2 springForce = springForceDirection * (displacement * -K);

	return springForce;
}

