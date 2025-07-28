#ifndef FORCE_H
#define FORCE_H

#include "Vec2.h"
#include "Particle.h"

struct Force
{
	static Vec2 GenerateDragForce(const Vec2& Velocity, float k);
	static Vec2 GenerateFrictionForce(const Vec2& Velocity, float k);
	static Vec2 GenerateGravitationalForce(const Particle& ParticleA, const Particle& Particle, float G);
};

#endif // !FORCE_H
