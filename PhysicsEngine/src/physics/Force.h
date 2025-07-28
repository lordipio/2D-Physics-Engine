#ifndef FORCE_H
#define FORCE_H

#include "Vec2.h"
#include "Particle.h"
#include <algorithm>

struct Force
{
	static Vec2 GenerateDragForce(const Vec2& Velocity, float k);
	static Vec2 GenerateFrictionForce(const Vec2& Velocity, float k);
	static Vec2 GenerateGravitationalForce(const Particle* ParticleA, const Particle* ParticleB, float G, float DistanceMin, float DistanceMax);
	static Vec2 GenerateSpringForce(const Particle* particle, const Vec2& anchor, const float& rest,  const float& K);
};

#endif // !FORCE_H
