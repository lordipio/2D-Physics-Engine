#include "Application.h"
#include "physics/Contact.h"

bool Application::IsRunning()
{
	return isRunning;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();

	//Body* body = new Body(CircleShape(50), Vec2(Graphics::Width() / 2.0, Graphics::Height() / 2.0), 1.0);
	//Body* body = new Body(BoxShape(200, 100), Vec2(Graphics::Width() / 2.0, Graphics::Height() / 2.0), 1.0);
	//Body* circle1 = new Body(CircleShape(100.f), Vec2(300, 300), 1, 1);
	//Body* circle2 = new Body(CircleShape(200.f), Vec2(600, 600), 0, 1);

	Body* boxA = new Body(BoxShape(200.f, 100.f), Vec2(300, 300), 1, 1);
	Body* boxB = new Body(BoxShape(400.f, 200.f), Vec2(600, 600), 1, 1);

	boxA->angularVelocity = -1.f;
	boxB->angularVelocity = 0.5f;

	bodies.push_back(boxA);
	bodies.push_back(boxB);
	
	// anchor = Vec2(Graphics::Width() / 2, 10.f);

	//float accumulativeWeight = 0;

	//for (int i = 1; i <= numberOfBodys; i++)
	//{
	//	accumulativeWeight += numberOfBodys - i + 1;
	//	bodies.push_back(new Body(CircleShape(60), Vec2(anchor.x, anchor.y + ((g * PIXEL_PER_METER/K) * mass * (accumulativeWeight)) + i * restLength), 2, 6.f));

	//}

//		bodys.push_back(new Body(Vec2(anchor.x, anchor.y + ((g * PIXEL_PER_METER/K) * 2 * (numberOfBodys - 1 + 2)) + 3 * restLength), 2, 6.f));
		//bodys.push_back(new Body(Vec2(anchor.x, anchor.y + ((g * PIXEL_PER_METER/K) * 2 * (numberOfBodys - 1 + 2)) + 2 * restLength), 2, 6.f));
	//	bodys.push_back(new Body(Vec2(anchor.x, 9.8 + 15 + 10), 2, 6.f));

	//FluidRect.x = 0;
	//FluidRect.y = Graphics::Height() / 2.f;
	//FluidRect.w = Graphics::Width();
	//FluidRect.h = Graphics::Height() / 2.f;
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
			break;

		//case SDL_MOUSEBUTTONDOWN:
		//	int x, y;
		//	SDL_GetMouseState(&x, &y);
		//	bodies.push_back(new Body(CircleShape(20.f), Vec2(x, y), 1.f, 1.f));
		//	break;
		case SDL_MOUSEMOTION:
			int x, y;
			SDL_GetMouseState(&x, &y);
			bodies[0]->Position = Vec2(x, y);
			break;
		}
	}

	//SDL_Event event;
	//while (SDL_PollEvent(&event))
	//{
	//	switch (event.type)
	//	{
	//	case SDL_QUIT:
	//		isRunning = false;
	//		break;

	//	case SDL_KEYDOWN:
	//		if (event.key.keysym.sym == SDLK_ESCAPE)
	//			isRunning = false;

	//		if (event.key.keysym.sym == SDLK_UP)
	//			PushForce.y = -50 * PIXEL_PER_METER;

	//		if (event.key.keysym.sym == SDLK_DOWN)
	//			PushForce.y =  50 * PIXEL_PER_METER;

	//		if (event.key.keysym.sym == SDLK_RIGHT)
	//			PushForce.x =  50 * PIXEL_PER_METER;

	//		if (event.key.keysym.sym == SDLK_LEFT)
	//			PushForce.x = -50 * PIXEL_PER_METER;

	//		break;


	//	case SDL_KEYUP:
	//		if (event.key.keysym.sym == SDLK_UP)
	//			PushForce.y = 0.f;

	//		if (event.key.keysym.sym == SDLK_DOWN)
	//			PushForce.y = 0.f;

	//		if (event.key.keysym.sym == SDLK_RIGHT)
	//			PushForce.x = 0.f;

	//		if (event.key.keysym.sym == SDLK_LEFT)
	//			PushForce.x = 0.f;

	//		break;

	//	case SDL_MOUSEBUTTONDOWN:
	//		if (event.button.button == SDL_BUTTON_LEFT)
	//		{
	//			int x, y;
	//			SDL_GetMouseState(&x, &y);
	//			bodies.push_back(new Body(CircleShape(50), Vec2(x, y), 1.f, 10));
	//		}

	//		if (!rightMouseButtonDown && event.button.button == SDL_BUTTON_RIGHT) {
	//			rightMouseButtonDown = true;
	//			int x, y;
	//			SDL_GetMouseState(&x, &y);
	//			mouseCursor.x = x;
	//			mouseCursor.y = y;
	//		}
	//		break;

	//	case SDL_MOUSEMOTION:
	//		mouseCursor.x = event.motion.x;
	//		mouseCursor.y = event.motion.y;
	//		break;

	//	case SDL_MOUSEBUTTONUP:
	//		if (rightMouseButtonDown && event.button.button == SDL_BUTTON_RIGHT) {
	//			rightMouseButtonDown = false;
	//			Vec2 impulseDirection = (bodies[numberOfBodys - 1]->Position - mouseCursor).UnitVector();
	//			float impulseMagnitude = (bodies[numberOfBodys - 1]->Position - mouseCursor).Magnitude() * 5.0;
	//			bodies[numberOfBodys - 1]->Velocity = impulseDirection * impulseMagnitude;
	//		}
	//		break;
	//	}


	//}
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

	// Vec2 springForce = Force::GenerateSpringForce(bodies[0], anchor, restLength, K);
	// bodies[0]->AddForce(springForce);

	//for (int i = 1; i < numberOfBodys; i++)
	//{
	//	Body* currentBody = bodies[i];
	//	Body* previousBody = bodies[i-1];

	//	springForce = Force::GenerateSpringForce(currentBody, previousBody, restLength, K);
	//	
	//	currentBody->AddForce(springForce);
	//	previousBody->AddForce(-springForce);

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
	//}

	Vec2 windForce = Vec2(10 * PIXEL_PER_METER, 0);
	Vec2 weight = Vec2(0.f, 1.f);
	for (Body* body : bodies)
	{
		//body->AddForce(PushForce);
		//Vec2 friction = Force::GenerateFrictionForce(body->Velocity, 0.005f);
		//body->AddForce(friction);
		//body->AddTorque(600.f);
		//body->AddForce(Vec2(0.f, g) * body->Mass * PIXEL_PER_METER);
		
		//weight = Vec2(0.f, body->Mass * 9.8 *PIXEL_PER_METER);
		
		//body->AddForce(weight);
		//body->AddForce(windForce);

		body->Update(deltaTime);

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
	

	for (int i = 0; i < bodies.size() - 1; i++)
		for (int j = i + 1; j < bodies.size(); j++)
		{
			bodies[i]->isCollided = false;
			bodies[j]->isCollided = false;
		}


	for(int i = 0; i <= bodies.size() - 1; i++)
		for (int j = i + 1; j < bodies.size(); j++)
		{
			Contact contact;

			if (CollisionDetection::isColliding(bodies[i], bodies[j], contact))
			{
				//Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFF00FFFF);
				//Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFF00FFFF);
				//Graphics::DrawLine(contact.body1->Position.x, contact.body1->Position.y, contact.body1->Position.x + contact.normal.x * 20, contact.body1->Position.y + contact.normal.y * 20, 0xFF00FFFF);

				bodies[i]->isCollided = true;
				bodies[j]->isCollided = true;
				//contact.ResolveCollision();
			}
		}

}


void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);


	for (auto body : bodies)
	{
		Uint32 color = body->isCollided ? 0xFF0000FF : 0xFFFFFFFF;

		if (body->shape->GetType() == CIRCLE)
		{
			CircleShape* circleShape = (CircleShape*)body->shape;
			body->isCollided ? Graphics::DrawCircle(body->Position.x, body->Position.y, circleShape->radius, body->rotation, color) : Graphics::DrawCircle(body->Position.x, body->Position.y, circleShape->radius, body->rotation, 0xFFFFFFFF);
		}
		else if (body->shape->GetType() == BOX)
		{
			BoxShape* boxShape = (BoxShape*)body->shape;
			Graphics::DrawPolygon(body->Position.x, body->Position.y, boxShape->worldVertices, color);
		}
	}



	// Graphics::DrawFillRect(FluidRect.x + FluidRect.w / 2, FluidRect.y + FluidRect.h / 2, FluidRect.w, FluidRect.h, 0xFFFF0000);

	//Graphics::DrawFillCircle(anchor.x, anchor.y, 5, 0xFF000000); // anchor

	//Graphics::DrawLine(anchor.x, anchor.y, bodies[0]->Position.x, bodies[0]->Position.y, 0xFFFF0000);
	

	//for (int i = 0; i < numberOfBodys - 1; i++)
	//{
	//	Body* body = bodies[i];
	//	if (body->shape->GetType() == CIRCLE)
	//	{
	//		CircleShape* circleShape = (CircleShape*)body->shape;
	//		Graphics::DrawLine(body->Position.x, body->Position.y, bodies[i+1]->Position.x, bodies[i+1]->Position.y, 0xFFFF0000);
	//		Graphics::DrawCircle(body->Position.x, body->Position.y, circleShape->radius, body->rotation, 0xFFFF3333);
	//	}
	//}
	//if (bodies[numberOfBodys - 1]->shape->GetType() == CIRCLE)
	//{
	//	CircleShape* circleShape = (CircleShape*)bodies[numberOfBodys - 1]->shape;
	//	Graphics::DrawCircle(bodies[numberOfBodys - 1]->Position.x, bodies[numberOfBodys - 1]->Position.y, circleShape->radius, bodies[numberOfBodys - 1]->rotation, 0xFFFF3333);
	//}

	//if (rightMouseButtonDown) {
	//	Graphics::DrawLine(bodies[numberOfBodys-1]->Position.x, bodies[numberOfBodys - 1]->Position.y, mouseCursor.x, mouseCursor.y, 0xFFFF00FF);
	//}
	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	for (Body* body : bodies)
		delete body;

	Graphics::CloseWindow();
}
