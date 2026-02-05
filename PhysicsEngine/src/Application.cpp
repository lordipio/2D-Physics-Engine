#include "Application.h"
#include "physics/Contact.h"


bool Application::IsRunning()
{
	return isRunning;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();
	world = new World();


	Body* floor = new Body(BoxShape(Graphics::Width() - 100, 100.f), Vec2(Graphics::Width()/2, Graphics::Height() - 100), 0, 1.f, 0.f);
	Body* boxB = new Body(BoxShape(200.f, 200.f), Vec2(Graphics::Width() / 2, Graphics::Height()/2), 0);
	boxB->SetTexture("./assets/crate.png");
	
	
	world->AddBody(floor);
	world->AddBody(boxB);
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

		case SDL_MOUSEBUTTONDOWN:
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					Body* body = new Body(CircleShape(50.f), Vec2(x, y), 1.f, 0.7f);
					body->SetTexture("./assets/basketball.png");
					world->AddBody(body);
				}

				if (event.button.button == SDL_BUTTON_LEFT)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					Body* body = new Body(CircleShape(50.f), Vec2(x, y), 6.f, 0.1f);
					body->SetTexture("./assets/bowlingball.png");
					world->AddBody(body);
				}


				if (event.button.button == SDL_BUTTON_MIDDLE)
				{
					int x, y;
					SDL_GetMouseState(&x, &y);
					Body* body = new Body(BoxShape(100.f, 100.f), Vec2(x, y), 10.f, 0.f, 1.0);
					body->SetTexture("./assets/crate.png");
					world->AddBody(body);
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

	// Apply UI-controlled gravity (convert meters/s^2 to pixels/s^2 if you use PIXELS_PER_METER)
	if (!uiPaused) {
		world->gravity = g;
	}
	else {
		world->gravity = 0.f;
	}

	world->Update(deltaTime);
}


void Application::Render()
{
	// start ImGui frame
	Graphics::ImGuiNewFrame(Graphics::window); // if Graphics::window is public; otherwise pass stored window ptr

	Uint32 color;

	for (Constraint* constraint : world->GetConstraints())
	{
		Vec2 aPointInWorldSpace = constraint->a->LocalToWorld(constraint->bPoint);
		Vec2 bPointInWorldSpace = constraint->b->LocalToWorld(constraint->bPoint);

		Graphics::DrawLine(aPointInWorldSpace.x, aPointInWorldSpace.y, bPointInWorldSpace.x, bPointInWorldSpace.y, 0xFFFFFF00);
		Graphics::DrawFillRect(aPointInWorldSpace.x, aPointInWorldSpace.y, 10, 10, 0xFF0000FF);
		Graphics::DrawFillRect(bPointInWorldSpace.x, bPointInWorldSpace.y, 10, 10, 0xFFFF0000);
	}

	for (auto body : world->GetBodies())
	{
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
	ImGui::SliderFloat("Gravity (m/s^2)", &g, -50.0f, 50.0f);
	if (ImGui::Button(uiPaused ? "Resume" : "Pause")) {
		uiPaused = !uiPaused;
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear Bodies")) {
		// world->ClearBodies(); // implement a method to remove dynamic bodies if not present yet
	}

	ImGui::Separator();
	ImGui::Checkbox("Show debug mode", &isInDebugMode);
	ImGui::Checkbox("Show ImGui Demo", &uiShowDemoWindow);
	ImGui::End();

	if (uiShowDemoWindow) {
		ImGui::ShowDemoWindow(&uiShowDemoWindow);
	}
}