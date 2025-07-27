#ifndef PARTICLE_H
#define PARTICLE_H

#include "Vec2.h"

struct Particle
{
	Particle(Vec2 Position, float Mass, float Radius);
	~Particle();

	Vec2 Position;
	Vec2 Acceleration;
	Vec2 Velocity;

	float Mass;
	float InverseMass;
	float Radius;

	void Integrate(float dt);

	void AddForce(const Vec2& Force);

	void ClearForce();

	Vec2 sumForces;
};

#endif // !PARTICLE_H
