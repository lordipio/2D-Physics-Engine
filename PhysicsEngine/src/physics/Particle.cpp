#include "Particle.h"

Particle::Particle(Vec2 Position, float Mass, float Radius)
{
	this->Position = Position;
	this->Mass = Mass;
	this->Radius = Radius;
	this->sumForces = Vec2(0, 0);

	if (Mass != 0)
		InverseMass = 1.f / Mass;
	else
		InverseMass = 0.f;
}

Particle::~Particle()
{

}

void Particle::Integrate(float dt)
{
	this->Acceleration = sumForces *  this->InverseMass;
	this->Velocity += this->Acceleration * dt;
	this->Position += this->Velocity * dt;

	ClearForce();
}

void Particle::AddForce(const Vec2& Force)
{
	sumForces += Force;
}

void Particle::ClearForce()
{
	this->sumForces = Vec2(0, 0);
}