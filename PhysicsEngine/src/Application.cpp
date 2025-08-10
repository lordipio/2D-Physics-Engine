#include "Application.h"

bool Application::IsRunning()
{
	return isRunning;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();

	anchor = Vec2(Graphics::Width() / 2, 10.f);

	float accumulativeWeight = 0;

	for (int i = 1; i <= numberOfBodys; i++)
	{
		accumulativeWeight += numberOfBodys - i + 1;
		bodys.push_back(new Body(CircleShape(60), Vec2(anchor.x, anchor.y + ((g * PIXEL_PER_METER/K) * mass * (accumulativeWeight)) + i * restLength), 2, 6.f));

	}

//		bodys.push_back(new Body(Vec2(anchor.x, anchor.y + ((g * PIXEL_PER_METER/K) * 2 * (numberOfBodys - 1 + 2)) + 3 * restLength), 2, 6.f));
		//bodys.push_back(new Body(Vec2(anchor.x, anchor.y + ((g * PIXEL_PER_METER/K) * 2 * (numberOfBodys - 1 + 2)) + 2 * restLength), 2, 6.f));
	//	bodys.push_back(new Body(Vec2(anchor.x, 9.8 + 15 + 10), 2, 6.f));

	FluidRect.x = 0;
	FluidRect.y = Graphics::Height() / 2.f;
	FluidRect.w = Graphics::Width();
	FluidRect.h = Graphics::Height() / 2.f;
}

void Application::Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;

			if (event.key.keysym.sym == SDLK_UP)
				PushForce.y = -50 * PIXEL_PER_METER;

			if (event.key.keysym.sym == SDLK_DOWN)
				PushForce.y =  50 * PIXEL_PER_METER;

			if (event.key.keysym.sym == SDLK_RIGHT)
				PushForce.x =  50 * PIXEL_PER_METER;

			if (event.key.keysym.sym == SDLK_LEFT)
				PushForce.x = -50 * PIXEL_PER_METER;

			break;


		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_UP)
				PushForce.y = 0.f;

			if (event.key.keysym.sym == SDLK_DOWN)
				PushForce.y = 0.f;

			if (event.key.keysym.sym == SDLK_RIGHT)
				PushForce.x = 0.f;

			if (event.key.keysym.sym == SDLK_LEFT)
				PushForce.x = 0.f;

			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				bodys.push_back(new Body(CircleShape(50), Vec2(x, y), 1.f, 10));
			}

			if (!rightMouseButtonDown && event.button.button == SDL_BUTTON_RIGHT) {
				rightMouseButtonDown = true;
				int x, y;
				SDL_GetMouseState(&x, &y);
				mouseCursor.x = x;
				mouseCursor.y = y;
			}
			break;

		case SDL_MOUSEMOTION:
			mouseCursor.x = event.motion.x;
			mouseCursor.y = event.motion.y;
			break;

		case SDL_MOUSEBUTTONUP:
			if (rightMouseButtonDown && event.button.button == SDL_BUTTON_RIGHT) {
				rightMouseButtonDown = false;
				Vec2 impulseDirection = (bodys[numberOfBodys - 1]->Position - mouseCursor).UnitVector();
				float impulseMagnitude = (bodys[numberOfBodys - 1]->Position - mouseCursor).Magnitude() * 5.0;
				bodys[numberOfBodys - 1]->Velocity = impulseDirection * impulseMagnitude;
			}
			break;
		}


	}
}


void Application::Update()
{
	static int timePreviousFrame;
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
	if (timeToWait > 0)
		SDL_Delay(timeToWait);

	float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;

	timePreviousFrame = SDL_GetTicks();

	// body->Acceleration = Vec2(0.f, 9.8f * PIXEL_PER_METER);

	//Vec2 wind = Vec2(0.4f * PIXEL_PER_METER, 0);
	
	// Vec2 gravitationalForce = Force::GenerateGravitationalForce(bodys[0], bodys[1], 1000.f, 10, 50);
	//bodys[0]->AddForce(gravitationalForce);
	//bodys[1]->AddForce(-gravitationalForce);

	Vec2 springForce = Force::GenerateSpringForce(bodys[0], anchor, restLength, K);
	bodys[0]->AddForce(springForce);

	for (int i = 1; i < numberOfBodys; i++)
	{
		Body* currentBody = bodys[i];
		Body* previousBody = bodys[i-1];

		springForce = Force::GenerateSpringForce(currentBody, previousBody, restLength, K);
		
		currentBody->AddForce(springForce);
		previousBody->AddForce(-springForce);

		//currentBody->AddForce(PushForce);
		//body->AddForce(Vec2(0.f, 9.8f) * body->Mass * PIXEL_PER_METER);
		//Vec2 friction = Force::GenerateFrictionForce(currentBody->Velocity, 0.002f);
		//currentBody->AddForce(friction);
		//currentBody->Integrate(deltaTime);

		//if (body->Position.y > Graphics::Height() - FluidRect.h)
		//{
		//	Vec2 dragForce = Force::GenerateDragForce(body->Velocity, 0.06f);
		//	body->AddForce(dragForce);
		//}
		//else
			//body->AddForce(wind);




		//if (body->Position.x + body->Radius <= 0)
		//{
		//	body->Position.x = body->Radius;
		//	body->Velocity.x *= -1.f;
		//}

		//else if (body->Position.x + body->Radius >= Graphics::Width())
		//{
		//	body->Position.x = Graphics::Width() - body->Radius;
		//	body->Velocity.x *= -1.f;
		//}

		//if (body->Position.y + body->Radius <= 0)
		//{
		//	body->Position.y = body->Radius;
		//	body->Velocity.y *= -1.f;
		//}

		//else if (body->Position.y + body->Radius >= Graphics::Height())
		//{
		//	body->Position.y = Graphics::Height() - body->Radius;
		//	body->Velocity.y *= -1.f;
		//}
	}



	for (Body* body : bodys)
	{
		body->AddTorque(100.f);
		body->AddForce(PushForce);
		body->AddForce(Vec2(0.f, g) * body->Mass * PIXEL_PER_METER);
		Vec2 friction = Force::GenerateFrictionForce(body->Velocity, 0.005f);
		body->AddForce(friction);

		body->IntegrateLinear(deltaTime);
		body->IntegrateAngular(deltaTime);


		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circleShape = (CircleShape*)body->shape;
			if (body->Position.x + circleShape->radius <= 0)
			{
				body->Position.x = circleShape->radius;
				body->Velocity.x *= -1.f;
			}

			else if (body->Position.x + circleShape->radius >= Graphics::Width())
			{
				body->Position.x = Graphics::Width() - circleShape->radius;
				body->Velocity.x *= -1.f;
			}

			if (body->Position.y + circleShape->radius <= 0)
			{
				body->Position.y = circleShape->radius;
				body->Velocity.y *= -1.f;
			}

			else if (body->Position.y + circleShape->radius >= Graphics::Height())
			{
				body->Position.y = Graphics::Height() - circleShape->radius;
				body->Velocity.y *= -1.f;
			}
		}
	}



}


void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);



	// Graphics::DrawFillRect(FluidRect.x + FluidRect.w / 2, FluidRect.y + FluidRect.h / 2, FluidRect.w, FluidRect.h, 0xFFFF0000);

	Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF000000); // anchor

	Graphics::DrawLine(anchor.x, anchor.y, bodys[0]->Position.x, bodys[0]->Position.y, 0xFFFF0000);
	

	for (int i = 0; i < numberOfBodys - 1; i++)
	{
		Body* body = bodys[i];
		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circleShape = (CircleShape*)body->shape;
			Graphics::DrawLine(body->Position.x, body->Position.y, bodys[i+1]->Position.x, bodys[i+1]->Position.y, 0xFFFF0000);
			Graphics::DrawCircle(body->Position.x, body->Position.y, circleShape->radius, body->rotation, 0xFFFF3333);
		}
	}
	if (bodys[numberOfBodys - 1]->shape->GetType() == CIRCLE)
	{
		CircleShape* circleShape = (CircleShape*)bodys[numberOfBodys - 1]->shape;
		Graphics::DrawCircle(bodys[numberOfBodys - 1]->Position.x, bodys[numberOfBodys - 1]->Position.y, circleShape->radius, bodys[numberOfBodys - 1]->rotation, 0xFFFF3333);
	}

	if (rightMouseButtonDown) {
		Graphics::DrawLine(bodys[numberOfBodys-1]->Position.x, bodys[numberOfBodys - 1]->Position.y, mouseCursor.x, mouseCursor.y, 0xFFFF00FF);
	}
	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	for (Body* body : bodys)
		delete body;

	Graphics::CloseWindow();
}
