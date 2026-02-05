#include "Body.h"
#include "D:\Projects\lordipio\2D-Physics-Engine\PhysicsEngine\src\Graphics.h"


Body::Body(const Shape& shape, Vec2 Position, float Mass, float restitution, float friction)
{
	this->friction = friction;
	this->restitution = restitution;
	this->shape = shape.Clone();
	this->position = Position;
	this->velocity = Vec2(0, 0);
	this->acceleration= Vec2(0, 0);
	this->rotation = 0.f;
	this->angularAcceleration = 0.f;
	this->angularVelocity = 0.f;
	this->sumForces = Vec2(0, 0);
	this->sumTorques = 0.f;
	this->mass = Mass;
	if (Mass != 0)
		InverseMass = 1.f / Mass;
	else
		InverseMass = 0.f;

	I = shape.GetMomentOfInertia() * Mass;
	if (I != 0)
		this->inverseI = 1.0 / I;
	else
		this->inverseI = 0.0;

	this->shape->UpdateVertices(rotation, Position);
}

Body::~Body()
{
	delete this->shape;
	SDL_DestroyTexture(texture);
}

void Body::IntegrateLinear(float dt)
{
	if (IsStatic())
		return;

	//this->Acceleration = sumForces *  this->InverseMass;
	//this->Velocity += this->Acceleration * dt;

	//this->Position += this->Velocity * dt;

	//ClearLinearForce();
}

void Body::IntegrateAngular(float dt)
{
	if (IsStatic())
		return;

	//this->angularAcceleration = sumTorques * this->inverseI;
	// this->angularVelocity += this->angularAcceleration * dt;

	//this->rotation += this->angularVelocity * dt;

	//ClearAngularForce();
}

void Body::SetTexture(const char* texturePath)
{
	if (SDL_Surface* surface = IMG_Load(texturePath))
	{
		texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
		SDL_FreeSurface(surface);
	}
}

void Body::AddForce(const Vec2& Force)
{
	sumForces += Force;
}

void Body::AddTorque(const float& Torque)
{
	sumTorques += Torque;
}

void Body::ClearLinearForce()
{
	this->sumForces = Vec2(0, 0);
}

void Body::ClearAngularForce()
{
	this->sumTorques = 0;
}

bool Body::IsStatic() const
{
	return InverseMass < 0.005f;
}


Vec2 Body::LocalToWorld(const Vec2& point) const {
	Vec2 rotated = point.Rotate(rotation);
	return rotated + position;
}

Vec2 Body::WorldToLocal(const Vec2& point) const {
	float translatedX = point.x - position.x;
	float translatedY = point.y - position.y;
	float rotatedX = cos(-rotation) * translatedX - sin(-rotation) * translatedY;
	float rotatedY = cos(-rotation) * translatedY + sin(-rotation) * translatedX;
	return Vec2(rotatedX, rotatedY);
}

void Body::ApplyImpulseLinear(const Vec2& J)
{
	if (IsStatic())
		return;
	velocity += J * InverseMass;
}

void Body::ApplyImpulseAngular(const float J)
{
	if (IsStatic())
		return;
	angularVelocity += J * inverseI;
}

void Body::ApplyImpulseAtPoint(const Vec2& J, const Vec2& r)
{
	if (IsStatic())
		return;

	velocity += J * InverseMass;

	angularVelocity += r.Cross(J) * inverseI;
}

void Body::IntegrateForces(float dt)
{
	if (IsStatic())
		return;

	this->acceleration = sumForces * this->InverseMass;
	this->velocity += this->acceleration * dt;


	this->angularAcceleration = sumTorques * this->inverseI;
	this->angularVelocity += this->angularAcceleration * dt;

	ClearLinearForce();
	ClearAngularForce();
}

void Body::IntegrateVelocities(float dt)
{
	if (IsStatic())
		return;

	this->position += this->velocity * dt;
	this->rotation += this->angularVelocity * dt;

	shape->UpdateVertices(rotation, position);
}
