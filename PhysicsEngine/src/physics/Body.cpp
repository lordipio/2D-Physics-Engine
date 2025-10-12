#include "Body.h"

Body::Body(const Shape& shape, Vec2 Position, float Mass)
{
	this->shape = shape.Clone();
	this->Position = Position;
	this->Velocity = Vec2(0, 0);
	this->Acceleration= Vec2(0, 0);
	this->rotation = 0.f;
	this->angularAcceleration = 0.f;
	this->angularVelocity = 0.f;
	this->sumForces = Vec2(0, 0);
	this->sumTorques = 0.f;
	this->Mass = Mass;
	if (Mass != 0)
		InverseMass = 1.f / Mass;
	else
		InverseMass = 0.f;

	I = shape.GetMomentOfInertia() * Mass;
	if (I != 0)
		this->inverseI = 1.0 / I;
	else
		this->inverseI = 0.0;
}

Body::~Body()
{
	delete this->shape;
}

void Body::IntegrateLinear(float dt)
{
	this->Acceleration = sumForces *  this->InverseMass;
	this->Velocity += this->Acceleration * dt;
	this->Position += this->Velocity * dt;

	ClearLinearForce();
}

void Body::IntegrateAngular(float dt)
{
	this->angularAcceleration = sumTorques * this->inverseI;
	this->angularVelocity += this->angularAcceleration * dt;
	this->rotation += this->angularVelocity * dt;

	ClearAngularForce();
}

void Body::AddForce(const Vec2& Force)
{
	sumForces += Force;
}

void Body::AddTorque(const float& Torque)
{
	sumTorques += Torque;
	printf("rotation: %f", this->rotation);
}

void Body::ClearLinearForce()
{
	this->sumForces = Vec2(0, 0);
}

void Body::ClearAngularForce()
{
	this->sumTorques = 0;
}

