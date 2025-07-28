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

Vec2 Force::GenerateGravitationalForce(const Particle* ParticleA, const Particle* ParticleB, float G, float DistanceMin, float DistanceMax)
{
	Vec2 distance = (ParticleB->Position - ParticleA->Position);

	float distanceSquaredMagnitude = distance.MagnitudeSquared();
	distanceSquaredMagnitude = std::clamp(distanceSquaredMagnitude, DistanceMin, DistanceMax);

	Vec2 normalizedDistance = distance.UnitVector();

	return (normalizedDistance * (G * ParticleA->Mass * ParticleB->Mass)) / distanceSquaredMagnitude;
}

