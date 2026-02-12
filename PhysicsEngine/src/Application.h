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
	Vec2 PushForce = Vec2(0.f, 0.f);
	SDL_Rect FluidRect;
	Vec2 mousePos = Vec2(0, 0);
	bool rightMouseButtonDown = false;
	Vec2 anchor;
	const float K = 300.f;
	float g = 9.8f;
	const float restLength = 10.f;
	const int numberOfBodys = 1;
	const float mass = 2;
	void HandleUI();
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

	SDL_Texture* ballTex;
	SDL_Texture* crateTex;
	SDL_Texture* bowlingTex;
	SDL_Texture* jointTex;
	SDL_Texture* BackgroundImageTex;

	enum class SpawnTool {
		None,
		Box,
		Ball,
		BowlingBall,
		JointConstraint
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
		case SpawnTool::JointConstraint: return "Spring";
		default:                     return "Unknown";
		}
	}

	bool ImageButtonWithOutline(const char* name, ImTextureID tex, bool selected, ImVec2 size);

	void HandleSpawnTool(SpawnTool tool, Vec2 pos);

	enum class MouseState
	{
		None,
		Hold_Right,
		Hold_Left
	};

	MouseState mouseState = MouseState::None;
	Body* GetBodyAtPosition(const Vec2& mousePos);
	Body* grabbedBody = nullptr;
	Vec2 mouseVel;
	Vec2 mousePreviousPos;
	const float mouseVelSmoothing = 0.5f;

	

	// std::vector<Body*> selectedJointBodies;
	Body* selectedJointBody1;
	Body* selectedJointBody2;

	bool uiStatic = false;
	float uiObjectScale = 50.f;
	float uiObjectMass = 1.f;
	float uiObjectRestitution = 0.5f;
	float uiObjectFriction = 0.5f;

	void SetSelectedJointBodiesToDefault();

};

#endif // !APPLICATION_H