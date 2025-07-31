#include "Application.h"

bool Application::IsRunning()
{
	return isRunning;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();

	particles.push_back(new Particle(Vec2(100, 100), 2, 6.f));
	particles.push_back(new Particle(Vec2(300, 100), 2, 6.f));
	particles.push_back(new Particle(Vec2(300, 300), 2, 6.f));
	particles.push_back(new Particle(Vec2(100, 300), 2, 6.f));
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
				particles.push_back(new Particle(Vec2(x, y), 1.f, 10));
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
				Vec2 impulseDirection = (particles[3]->Position - mouseCursor).UnitVector();
				float impulseMagnitude = (particles[3]->Position - mouseCursor).Magnitude() * 5.0;
				particles[3]->Velocity = impulseDirection * impulseMagnitude;
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

	// particle->Acceleration = Vec2(0.f, 9.8f * PIXEL_PER_METER);

	//Vec2 wind = Vec2(0.4f * PIXEL_PER_METER, 0);
	
	// Vec2 gravitationalForce = Force::GenerateGravitationalForce(particles[0], particles[1], 1000.f, 10, 50);
	//particles[0]->AddForce(gravitationalForce);
	//particles[1]->AddForce(-gravitationalForce);

	Vec2 springForce = Force::GenerateSpringForce(particles[0], particles[1], restLength, K);
	particles[0]->AddForce(springForce);
	particles[1]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(particles[1], particles[2], restLength, K);
	particles[1]->AddForce(springForce);
	particles[2]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(particles[2], particles[3], restLength, K);
	particles[2]->AddForce(springForce);
	particles[3]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(particles[3], particles[0], restLength, K);
	particles[3]->AddForce(springForce);
	particles[0]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(particles[0], particles[2], diagonalRestLength, K);
	particles[0]->AddForce(springForce);
	particles[2]->AddForce(-springForce);

	springForce = Force::GenerateSpringForce(particles[1], particles[3], diagonalRestLength, K);
	particles[1]->AddForce(springForce);
	particles[3]->AddForce(-springForce);


	for (Particle* particle : particles)
	{
		particle->AddForce(PushForce);
		particle->AddForce(Vec2(0.f, g) * particle->Mass * PIXEL_PER_METER);
		Vec2 friction = Force::GenerateFrictionForce(particle->Velocity, 0.01f);
		particle->AddForce(friction);

		particle->Integrate(deltaTime);


		if (particle->Position.x + particle->Radius <= 0)
		{
			particle->Position.x = particle->Radius;
			particle->Velocity.x *= -0.8f;
		}

		else if (particle->Position.x + particle->Radius >= Graphics::Width())
		{
			particle->Position.x = Graphics::Width() - particle->Radius;
			particle->Velocity.x *= -0.8f;
		}

		if (particle->Position.y + particle->Radius <= 0)
		{
			particle->Position.y = particle->Radius;
			particle->Velocity.y *= -0.8f;
		}

		else if (particle->Position.y + particle->Radius >= Graphics::Height())
		{
			particle->Position.y = Graphics::Height() - particle->Radius;
			particle->Velocity.y *= -0.8f;
		}
	}



}


void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);


	Graphics::DrawLine(particles[0]->Position.x, particles[0]->Position.y, particles[1]->Position.x, particles[1]->Position.y, 0xFFFF0000);
	Graphics::DrawLine(particles[1]->Position.x, particles[1]->Position.y, particles[2]->Position.x, particles[2]->Position.y, 0xFFFF0000);
	Graphics::DrawLine(particles[2]->Position.x, particles[2]->Position.y, particles[3]->Position.x, particles[3]->Position.y, 0xFFFF0000);
	Graphics::DrawLine(particles[3]->Position.x, particles[3]->Position.y, particles[0]->Position.x, particles[0]->Position.y, 0xFFFF0000);
	Graphics::DrawLine(particles[0]->Position.x, particles[0]->Position.y, particles[2]->Position.x, particles[2]->Position.y, 0xFFFF0000);
	Graphics::DrawLine(particles[1]->Position.x, particles[1]->Position.y, particles[3]->Position.x, particles[3]->Position.y, 0xFFFF0000);


	for (Particle* particle : particles)
	{
		Graphics::DrawFillCircle(particle->Position.x, particle->Position.y, particle->Radius, 0xFFFF3333);
	}


	if (rightMouseButtonDown) {
		Graphics::DrawLine(particles[3]->Position.x, particles[3]->Position.y, mouseCursor.x, mouseCursor.y, 0xFFFF00FF);
	}
	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	for (Particle* particle : particles)
		delete particle;

	Graphics::CloseWindow();
}
