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

	SDL_Texture* BallTex;
	SDL_Texture* CrateTex;
	SDL_Texture* BowlingTex;

	enum class SpawnTool {
		None,
		Box,
		Ball,
		BowlingBall
	};

	SpawnTool currentTool = SpawnTool::None;


	const char* SpawnToolToString(SpawnTool tool)
	{
		switch (tool)
		{
		case SpawnTool::None:        return "None";
		case SpawnTool::Box:         return "Box";
		case SpawnTool::Ball:        return "Ball";
		case SpawnTool::BowlingBall: return "Bowling Ball";
		default:                     return "Unknown";
		}
	}

	bool ImageButtonWithOutline(const char* name, ImTextureID tex, bool selected, ImVec2 size);

	void CreateBodyBaseOnSpawnTool(SpawnTool tool, Vec2 pos);

};

#endif // !APPLICATION_H