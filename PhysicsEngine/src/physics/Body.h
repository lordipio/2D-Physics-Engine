#ifndef BODY_H
#define BODY_H

#include "Vec2.h"
#include "Shape.h"

struct Body
{
	Body(const Shape& shape, Vec2 Position, float Mass, float restitution);
	~Body();

	bool isCollided = false;

	Vec2 Position;
	Vec2 Acceleration;
	Vec2 Velocity;

	float rotation;
	float angularVelocity;
	float angularAcceleration;

	Vec2 sumForces;
	float sumTorques;

	Shape* shape;

	float Mass;
	float InverseMass;
	
	float I;
	float inverseI;

	float restitution;

	void IntegrateLinear(float dt);
	void IntegrateAngular(float dt);

	void AddForce(const Vec2& Force);
	void AddTorque(const float& Torque);


	void ClearLinearForce();
	void ClearAngularForce();

	void Update(float deltaTime);

	void ApplyImpulse(Vec2 J);

	bool IsStatic() const;

};

#endif // !BODY_H
