#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Vec2.h"
#include "Body.h"
#include "Constraint.h"

class World
{
private:
	float gravity;
	std::vector<Body*> bodies;
	std::vector<Constraint*> constraints;
	std::vector<Vec2> forces;
	std::vector<float> torques;

public:
	World(float gravity = -9.8);
	~World();

	std::vector<Body*>& GetBodies();
	std::vector<Constraint*>& GetConstraints();

	void AddConstraint(Constraint* constraint);
	void AddBody(Body* body);
	void AddForce(const Vec2& force);
	void AddTorque(float torque);

	void Update(float deltaTime);
	// void CheckCollisions();
};

#endif // !WORLD_H
