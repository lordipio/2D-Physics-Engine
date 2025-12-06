#include "World.h"
#include "Constants.h"
#include "Contact.h"
#include "CollisionDetection.h"

World::World(float gravity)
{
	this->gravity = -gravity;
}

World::~World()
{
	for (Body* body : bodies)
		delete body;
}

std::vector<Body*>* World::GetBodies()
{
	return &bodies;
}

void World::AddBody(Body* body)
{
	bodies.push_back(body);
}

void World::AddForce(Vec2 force)
{
	forces.push_back(force);
}

void World::AddTorque(float torque)
{
	torques.push_back(torque);
}

void World::Update(float deltaTime)
{
	Vec2 weight = Vec2(0.f, 1.f);
	
	for (Body* body : bodies)
	{
		weight = Vec2(0.f, body->Mass * gravity * PIXEL_PER_METER);
		body->AddForce(weight);

		for (Vec2 force : forces)
			body->AddForce(force);

		for (float torque : torques)
			body->AddTorque(torque);

		body->Update(deltaTime);
	}

	for (int n = 0; n < 20; n++) // for avoiding jitter effect
		CheckCollisions();
}

void World::CheckCollisions()
{

	for (int i = 0; i < bodies.size() - 1; i++)
		for (int j = i + 1; j < bodies.size(); j++)
		{
			bodies[i]->isCollided = false;
			bodies[j]->isCollided = false;
		}


	for (int i = 0; i <= bodies.size() - 1; i++)
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Contact contact;

			if (CollisionDetection::isColliding(bodies[i], bodies[j], contact))
			{
				//if (isInDebugMode)
				//{
				//	Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFF00FFFF);
				//	Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFF00FFFF);
				//	Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 20, contact.start.y + contact.normal.y * 20, 0xFF00FFFF);
				//}
				bodies[i]->isCollided = true;
				bodies[j]->isCollided = true;
				contact.ResolveCollision();
			}
		}
}
