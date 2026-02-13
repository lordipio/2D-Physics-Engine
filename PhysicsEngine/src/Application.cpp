#include "Application.h"
#include "physics/Contact.h"
#include <SDL_image.h>


bool Application::IsRunning()
{
	return isRunning;
	SDL_Texture* t;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();
	world = new World();

	crateTex = Graphics::GetTexture("./assets/crate.png");
	ballTex = Graphics::GetTexture("./assets/basketball.png");
	bowlingTex = Graphics::GetTexture("./assets/bowlingball.png");
	jointTex = Graphics::GetTexture("./assets/joint.png");
	BackgroundImageTex = Graphics::GetTexture("./assets/background.png");

	Body* floor = new Body(BoxShape(Graphics::Width(), 100.f), Vec2(Graphics::Width()/2, Graphics::Height() - 82.f), 0, 0.1f, 1.f);
	Body* ceil = new Body(BoxShape(Graphics::Width(), 100.f), Vec2(Graphics::Width()/2, -50.f), 0, 0.1f, 1.f);
	Body* wall1 = new Body(BoxShape(10.f, Graphics::Height() * 2), Vec2(-10.f, Graphics::Height() / 2), 0, 0.1f, 1.f);
	Body* wall2 = new Body(BoxShape(10.f, Graphics::Height() * 2), Vec2(Graphics::Width() + 10.f, Graphics::Height() / 2), 0, 0.1f, 1.f);

	floor->isInvisible = true;
	ceil->isInvisible = true;
	wall1->isInvisible = true;
	wall2->isInvisible = true;

	world->AddBody(floor);
	world->AddBody(ceil);
	world->AddBody(wall1);
	world->AddBody(wall2);
}

void Application::Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;

			if (event.key.keysym.sym == SDLK_d)
				isInDebugMode = !isInDebugMode;

			break;

		case SDL_MOUSEMOTION:
			mousePreviousPos = mousePos;
			mousePos = Vec2(event.motion.x, event.motion.y);
			mouseVel = mouseVel * mouseVelSmoothing;
			mouseVel = mouseVel + mousePos - mousePreviousPos;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					mouseState = MouseState::Hold_Left;

					grabbedBody = GetBodyAtPosition(Vec2(x, y));
				}

				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					HandleSpawnTool(currentTool, Vec2(x, y));

				}


				if (event.button.button == SDL_BUTTON_MIDDLE)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);

				}
			}

			break;

		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				mouseState = MouseState::None;
				
				if (grabbedBody)
				{
					grabbedBody->SetToMovable();	

					if (!uiPaused)
					{
						grabbedBody->ApplyImpulseLinear(mouseVel * 4.f * grabbedBody->mass);
					}
					
					grabbedBody = nullptr;
				}
			}
			break;
		}
	}

}


void Application::Update()
{
	Graphics::ClearScreen(0xFF056263);

	static int timePreviousFrame;
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
	if (timeToWait > 0)
		SDL_Delay(timeToWait);

	float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;

	timePreviousFrame = SDL_GetTicks();

	if (grabbedBody)
	{
		grabbedBody->SetToStatic();
		grabbedBody->position = mousePos;
	}
		
	if (!uiPaused)
		world->Update(deltaTime);


}


void Application::Render()
{
	// start ImGui frame
	Graphics::ImGuiNewFrame(Graphics::window); // if Graphics::window is public; otherwise pass stored window ptr

	Uint32 color;
	Graphics::DrawTexture(0, 0, 1920 * 2, 1080 * 2, 0, BackgroundImageTex);

	for (Constraint* constraint : world->GetConstraints())
	{
		Vec2 aPos = constraint->a->position;
		Vec2 bPos = constraint->b->position;

		Graphics::DrawThickLine(aPos.x, aPos.y, bPos.x, bPos.y, 0xFF11FFFF, 3);

	}

	for (auto body : world->GetBodies())
	{
		if (body->isInvisible)
			continue;
		
		if (isInDebugMode)
			color = body->isCollided ? 0xFF0000FF : 0xFFFFFFFF;
		else
			color = 0xFFFFFFFF;

		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circleShape = (CircleShape*)body->shape;
			if (isInDebugMode || !body->texture) 
				Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, color);
			else
				Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);

		}

		else if (body->shape->GetType() == BOX)
		{
			BoxShape* boxShape = (BoxShape*)body->shape;
			if (isInDebugMode || !body->texture)
				Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, color);

			else
				Graphics::DrawTexture(body->position.x, body->position.y,boxShape->width,boxShape->height, body->rotation, body->texture);

		}

		else if (body->shape->GetType() == POLYGON)
		{
			PolygonShape* polygonShape = (PolygonShape*)body->shape;

			if (isInDebugMode)
				Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, color);
			
			else
				Graphics::DrawFillPolygon(body->position.x, body->position.y, polygonShape->worldVertices, color);

		}
	}


	HandleUI();

	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	delete world;

	Graphics::CloseWindow();
}

void Application::HandleUI()
{
	// === ImGui UI ===
	ImGui::Begin("Simulation Controls");
	ImGui::Text("Physics");
	// slider controls gravity (in m/s^2)
	if (ImGui::SliderFloat("Gravity (m/s^2)", &g, -20.0f, 20.0f))
		world->gravity = g;
	if (ImGui::Button(uiPaused ? "Resume" : "Pause")) {
		uiPaused = !uiPaused;
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear Bodies")) {
		world->ClearBodies(); // implement a method to remove dynamic bodies if not present yet
	}
	ImGui::SameLine();

	ImGui::Checkbox("Show debug mode", &isInDebugMode);


	
	ImGui::End();



	ImGui::Begin("Spawn Tools", nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize
	);

	ImGui::Text("Click to select:");

	if (ImageButtonWithOutline("Crate", (ImTextureID)crateTex, currentTool == SpawnTool::Box, ImVec2(48, 48)))
		currentTool = SpawnTool::Box;
	ImGui::SameLine();
	if (ImageButtonWithOutline("Ball", (ImTextureID)ballTex, currentTool == SpawnTool::Ball, ImVec2(48, 48)))
		currentTool = SpawnTool::Ball;

	ImGui::SameLine();
	if (ImageButtonWithOutline("Bowling", (ImTextureID)bowlingTex, currentTool == SpawnTool::BowlingBall, ImVec2(48, 48)))
		currentTool = SpawnTool::BowlingBall;

	ImGui::SameLine();
	if (ImageButtonWithOutline("Joint Tool", (ImTextureID)jointTex, currentTool == SpawnTool::JointConstraint, ImVec2(48, 48)))
		currentTool = SpawnTool::JointConstraint;
	
	if (currentTool != SpawnTool::JointConstraint)
		SetSelectedJointBodiesToDefault();

	ImGui::Separator();
	ImGui::Text("Current tool: %s", SpawnToolToString(currentTool));

	if (mouseState == MouseState::Hold_Left)
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

	ImGui::Separator();
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Spawn Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
		ImGui::PushItemWidth(200);

		ImGui::Spacing();
		ImGui::Text("Object Properties");
		ImGui::Separator();

		ImGui::SliderFloat("Scale", &uiObjectScale, 1.0f, 150.0f);
		ImGui::SliderFloat("Mass", &uiObjectMass, 0.1f, 1000.0f);
		ImGui::SliderFloat("Restitution", &uiObjectRestitution, 0.001f, 1.0f);
		ImGui::SliderFloat("Friction", &uiObjectFriction, 0.0f, 1.0f);
		ImGui::Checkbox("Is Object Static?", &uiStatic);

		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
	}

	ImGui::End();
}


bool Application::ImageButtonWithOutline(
	const char* name,
	ImTextureID tex,
	bool selected,
	ImVec2 size
) {
	// Force hover style when selected
	if (selected) {
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::PushStyleColor(ImGuiCol_Button, style.Colors[ImGuiCol_ButtonHovered]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, style.Colors[ImGuiCol_ButtonHovered]);
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, style.Colors[ImGuiCol_ButtonHovered]);
	}

	bool clicked = ImGui::ImageButton(name, tex, size);

	// Draw selection border AFTER the button exists
	if (selected) {
		ImDrawList* draw = ImGui::GetWindowDrawList();
		draw->AddRect(
			ImGui::GetItemRectMin(),
			ImGui::GetItemRectMax(),
			IM_COL32(255, 160, 255, 200),
			1.0f,
			0,
			5.0f
		);
	}

	if (selected) {
		ImGui::PopStyleColor(3);
	}

	return clicked;
}

void Application::HandleSpawnTool(SpawnTool tool, Vec2 pos)
{
	Body* body;
	switch (tool)
	{
		case SpawnTool::BowlingBall:
			body = new Body(CircleShape(uiObjectScale / 2.f), pos, uiStatic ? 0 : uiObjectMass, uiObjectRestitution, uiObjectFriction);
			body->SetTexture("./assets/bowlingball.png");
			world->AddBody(body);
			break;

		case SpawnTool::Ball:
			body = new Body(CircleShape(uiObjectScale / 2.f), pos, uiStatic ? 0 : uiObjectMass, uiObjectRestitution, uiObjectFriction);
			body->SetTexture("./assets/basketball.png");
			world->AddBody(body);
			break;

		case SpawnTool::Box:
			body = new Body(BoxShape(uiObjectScale, uiObjectScale), pos, uiStatic ? 0 : uiObjectMass, uiObjectRestitution, uiObjectFriction);
			body->SetTexture("./assets/crate.png");
			world->AddBody(body);
			break;


		case SpawnTool::JointConstraint:
			if (Body* selectedBody = GetBodyAtPosition(pos))
			{
				if (!selectedJointBody1)
				{
					selectedJointBody1 = selectedBody;
					selectedJointBody1->SetTextureColor(140, 140, 255);
				}

				else
				{
					selectedJointBody2 = selectedBody;
					selectedJointBody2->SetTextureColor(140, 140, 255);

					world->AddConstraint(new JointConstraint(selectedJointBody1, selectedJointBody2, (selectedJointBody1->position + selectedJointBody2->position)* 0.5f));
					// world->AddConstraint(new JointConstraint(selectedJointBody1, selectedJointBody2, selectedJointBody1->position));
					// world->AddConstraint(new JointConstraint(selectedJointBody2, selectedJointBody1, selectedJointBody2->position));
					SetSelectedJointBodiesToDefault();
				}
			}

			else
				SetSelectedJointBodiesToDefault();
	}
}

void Application::SetSelectedJointBodiesToDefault()
{
	if (selectedJointBody1)
	{
		selectedJointBody1->ResetTextureColor();
		selectedJointBody1 = nullptr;
	}

	if (selectedJointBody2)
	{
		selectedJointBody2->ResetTextureColor();
		selectedJointBody2 = nullptr;
	}
}

Body* Application::GetBodyAtPosition(const Vec2& mousePos)
{
	for (Body* body : world->GetBodies())
	{
		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circle = (CircleShape*)body->shape;
			Vec2 diff = mousePos - body->position;
			if (diff.MagnitudeSquared() <= circle->radius * circle->radius)
				return body;
		}
		else
		{
			// Simple AABB check for box
			BoxShape* box = (BoxShape*)body->shape;

			float halfW = box->width * 0.5f;
			float halfH = box->height * 0.5f;

			if (mousePos.x >= body->position.x - halfW &&
				mousePos.x <= body->position.x + halfW &&
				mousePos.y >= body->position.y - halfH &&
				mousePos.y <= body->position.y + halfH)
			{
				return body;
			}
		}
	}
	return nullptr;
}