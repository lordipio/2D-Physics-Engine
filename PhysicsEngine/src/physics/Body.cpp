#include "Body.h"

Body::Body(const Shape& shape, Vec2 Position, float Mass, float restitution)
{
	this->restitution = restitution;
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
	if (IsStatic())
		return;

	this->Acceleration = sumForces *  this->InverseMass;
	this->Velocity += this->Acceleration * dt;
	this->Position += this->Velocity * dt;

	ClearLinearForce();
}

void Body::IntegrateAngular(float dt)
{
	if (IsStatic())
		return;

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

void Body::Update(float deltaTime)
{
	
	IntegrateLinear(deltaTime);
	IntegrateAngular(deltaTime);
	

	bool isPolygon = shape->GetType() == POLYGON || shape->GetType() == BOX;
	if (isPolygon)
	{
		PolygonShape* polygonBody = (PolygonShape*)shape;
		polygonBody->UpdateVertices(rotation, Position);
	}
}

bool Body::IsStatic() const
{
	return InverseMass < 0.005f;
}


void Body::ApplyImpulse(Vec2 J)
{
	Velocity += J * InverseMass;
}