#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"
#include <SDL.h>
#include <SDL_image.h>

struct Body
{
	Body(const Shape& shape, Vec2 Position, float Mass, float restitution = 0.1f, float friction = 1.f);
	~Body();

	bool isCollided = false;

	Vec2 position;
	Vec2 acceleration;
	Vec2 velocity;

	float rotation;
	float angularVelocity;
	float angularAcceleration;

	Vec2 sumForces;
	float sumTorques;

	Shape* shape;

	float mass;
	float InverseMass;
	
	float I;
	float inverseI;

	float restitution;
	float friction;

	SDL_Texture* texture = nullptr;

	void SetTexture(const char* texturePath);

	void IntegrateLinear(float dt);
	void IntegrateAngular(float dt);

	void AddForce(const Vec2& Force);
	void AddTorque(const float& Torque);


	void ClearLinearForce();
	void ClearAngularForce();

	void ApplyImpulseLinear(const Vec2& J);
	void ApplyImpulseAngular(const float J);
	void ApplyImpulseAtPoint(const Vec2& J, const Vec2& r);

	void IntegrateForces(float dt);
	void IntegrateVelocities(float dt);

	bool IsStatic() const;

	Vec2 LocalToWorld(const Vec2& point) const;

	Vec2 WorldToLocal(const Vec2& point) const;
};

#endif // !BODY_H
