#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Vec2.h"
#include "Body.h"

class World
{
private:
	float gravity;
	std::vector<Body*> bodies;
	std::vector<Vec2> forces;
	std::vector<float> torques;

public:
	World(float gravity = -9.8);
	~World();

	std::vector<Body*>* GetBodies();

	void AddBody(Body* body);
	void AddForce(Vec2 force);
	void AddTorque(float torque);

	void Update(float deltaTime);
	void CheckCollisions();
};

#endif // !WORLD_H
