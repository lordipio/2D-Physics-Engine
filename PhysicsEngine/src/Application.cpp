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


	Body* floor = new Body(BoxShape(Graphics::Width() - 100, 100.f), Vec2(Graphics::Width()/2, Graphics::Height() - 100), 0, 1.f);
	Body* boxB = new Body(BoxShape(200.f, 200.f), Vec2(Graphics::Width() / 2, Graphics::Height()/2), 0);
	boxB->SetTexture("./assets/crate.png");
	
	// boxB->rotation = -2.f;
	
	world->AddBody(floor);
	world->AddBody(boxB);

	//const int numberOfBodies = 8;
	//for (int i = 0; i < numberOfBodies; i++)
	//{
	//	Body* b = new Body(BoxShape(20, 20), Vec2(Graphics::Width()/2.f + i * 10.f, 50.f + i * 20.f), i == 0 ? 0 : 1);
	//	world->AddBody(b);

	//	if (i > 0)
	//	{
	//		std::vector<Body*> bodies = *world->GetBodies();
	//		JointConstraint* joint = new JointConstraint(bodies[i], bodies[i - 1], bodies[i - 1]->Position);
	//		world->AddConstraint(joint);
	//	}
	//}

	// Add two bodies
	//Body* a = new Body(CircleShape(30), Vec2(Graphics::Width() / 2.0, Graphics::Height() / 5.0), 0.0f);
	//Body* b = new Body(CircleShape(20), Vec2(a->Position.x - 100, a->Position.y), 1.0f);
	//world->AddBody(a);
	//world->AddBody(b);

	// Add a joint constraint

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

			if (event.key.keysym.sym == SDLK_d)
				isInDebugMode = !isInDebugMode;
			break;

		case SDL_MOUSEBUTTONDOWN:
			
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				Body* body = new Body(CircleShape(50.f), Vec2(x, y), 1.f, 0.8f);
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
				//int x, y;
				//SDL_GetMouseState(&x, &y);
				//std::vector<Vec2> vertices = { Vec2(-1, 0) * 100, Vec2(-0.5, -1) * 100, Vec2(0.5, -1) * 100, Vec2(1, 0) * 100, Vec2(0.5, 1) * 100, Vec2(-0.5, 1) * 100 };
				//bodies.push_back(new Body(PolygonShape(vertices), Vec2(x, y), 0.01f, 1.f));
				int x, y;
				SDL_GetMouseState(&x, &y);
				Body* body = new Body(BoxShape(100.f, 100.f), Vec2(x, y), 10.f, 0.01f, 0.0);
				body->SetTexture("./assets/crate.png");
				world->AddBody(body);
			}




			break;

		//case SDL_MOUSEMOTION:
		//	int x, y;
		//	SDL_GetMouseState(&x, &y);
		//	bodies[0]->Position = Vec2(x, y);
		//	//bodies.push_back(new Body(BoxShape(100, 100), Vec2(x, y), 1, 0.1));
		//	//bodies.push_back(new Body(CircleShape(100), Vec2(x, y), 1, 0.1));
		//	break;
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


	Vec2 windForce = Vec2(10 * PIXELS_PER_METER, 0);
	Vec2 weight = Vec2(0.f, 1.f);

	world->Update(deltaTime);

	//for (Body* body : bodies)
	//{
	//	//body->AddForce(PushForce);
	//	//Vec2 friction = Force::GenerateFrictionForce(body->Velocity, 0.005f);
	//	//body->AddForce(friction);
	//	//body->AddTorque(600.f);
	//	//body->AddForce(Vec2(0.f, g) * body->Mass * PIXEL_PER_METER);
	//	
	//	// weight = Vec2(0.f, body->Mass * 9.8 * PIXEL_PER_METER);
	//	
	//	body->AddForce(weight);

	//	body->Update(deltaTime);

		//if (body->shape->GetType() == CIRCLE)
		//{
		//	CircleShape* circleShape = (CircleShape*)body->shape;
		//	if (body->Position.x + circleShape->radius <= 0)
		//	{
		//		body->Position.x = circleShape->radius;
		//		body->Velocity.x *= -1.f;
		//	}

		//	else if (body->Position.x + circleShape->radius >= Graphics::Width())
		//	{
		//		body->Position.x = Graphics::Width() - circleShape->radius;
		//		body->Velocity.x *= -1.f;
		//	}

		//	if (body->Position.y + circleShape->radius <= 0)
		//	{
		//		body->Position.y = circleShape->radius;
		//		body->Velocity.y *= -1.f;
		//	}

		//	else if (body->Position.y + circleShape->radius >= Graphics::Height())
		//	{
		//		body->Position.y = Graphics::Height() - circleShape->radius;
		//		body->Velocity.y *= -1.f;
		//	}
		//}
	//}
	

	//for (int i = 0; i < bodies.size() - 1; i++)
	//	for (int j = i + 1; j < bodies.size(); j++)
	//	{
	//		bodies[i]->isCollided = false;
	//		bodies[j]->isCollided = false;
	//	}


	//for(int i = 0; i <= bodies.size() - 1; i++)
	//	for (int j = i + 1; j < bodies.size(); j++)
	//	{
	//		Contact contact;

	//		if (CollisionDetection::isColliding(bodies[i], bodies[j], contact))
	//		{
	//			if (isInDebugMode)
	//			{
	//				Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFF00FFFF);
	//				Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFF00FFFF);
	//				Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 20, contact.start.y + contact.normal.y * 20, 0xFF00FFFF);
	//			}

	//			bodies[i]->isCollided = true;
	//			bodies[j]->isCollided = true;
	//			contact.ResolveCollision();
	//		}
	//	}

}


void Application::Render()
{

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


	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	delete world;

	Graphics::CloseWindow();
}
