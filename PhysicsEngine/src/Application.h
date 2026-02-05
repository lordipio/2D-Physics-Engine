#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics.h"
#include "physics/Body.h"
#include "physics/Constants.h"
#include "physics/Force.h"
#include "physics/CollisionDetection.h"
#include "physics/World.h"
#include <vector>

class Application
{
private:
	bool isRunning = false;
	bool isInDebugMode = false;

	World* world;
	// std::vector<Body*> bodies;

	Vec2 PushForce = Vec2(0.f, 0.f);

	SDL_Rect FluidRect;

	Vec2 mouseCursor = Vec2(0, 0);
	bool rightMouseButtonDown = false;

	Vec2 anchor;
	const float K = 300.f;
	float g = 9.8f;
	const float restLength = 10.f;
	const int numberOfBodys = 1;
	const float mass = 2;
	void HandleUI();
	// Application.h (add members)
	// float uiGravity = 9.8f;   // meters / s^2
	bool uiPaused = false;
	bool uiShowDemoWindow = false;
public:
	Application() = default;
	~Application() = default;

	bool IsRunning();
	void Setup();
	void Update();
	void Input();
	void Render();
	void Destroy();
};

#endif // !APPLICATION_H