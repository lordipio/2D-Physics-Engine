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
	bool uiPaused = false;
	bool uiStatic = false;

	const float mouseVelSmoothing = 0.5f;
	float uiObjectScale = 50.f;
	float uiObjectMass = 1.f;
	float uiObjectRestitution = 0.5f;
	float uiObjectFriction = 0.5f;
	float g = 9.8f;

	Vec2 mousePos = Vec2(0, 0);
	Vec2 mousePreviousPos;
	Vec2 mouseVel;

	World* world;

	Body* grabbedBody = nullptr;
	Body* selectedJointBody1;
	Body* selectedJointBody2;
	Body* GetBodyAtPosition(const Vec2& mousePos);

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
	enum class MouseState
	{
		None,
		Hold_Right,
		Hold_Left
	};

	MouseState mouseState = MouseState::None;

	void HandleUI();
	void SetSelectedJointBodiesToDefault();
	bool ImageButtonWithOutline(const char* name, ImTextureID tex, bool selected, ImVec2 size);
	void HandleSpawnTool(SpawnTool tool, Vec2 pos);

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