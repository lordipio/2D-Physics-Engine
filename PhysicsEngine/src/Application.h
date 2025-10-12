#ifndef APPLICATION_H
#define APPLICATION_H

#include "Graphics.h"
#include "physics/Body.h"
#include "physics/Constants.h"
#include "physics/Force.h"
#include <vector>

class Application
{
private:
	bool isRunning = false;

	std::vector<Body*> bodies;

	Vec2 PushForce = Vec2(0.f, 0.f);

	SDL_Rect FluidRect;

	Vec2 mouseCursor = Vec2(0, 0);
	bool rightMouseButtonDown = false;

	Vec2 anchor;
	const float K = 300.f;
	const float g = 9.8f;
	const float restLength = 10.f;
	const int numberOfBodys = 1;
	const float mass = 2;

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