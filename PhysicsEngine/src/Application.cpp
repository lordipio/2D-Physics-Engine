#include "Application.h"

bool Application::IsRunning()
{
	return isRunning;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();

	particles.push_back(new Particle(Vec2(Graphics::Width()/2, 400), 2, 20.f));

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
				Vec2 impulseDirection = (particles[0]->Position - mouseCursor).UnitVector();
				float impulseMagnitude = (particles[0]->Position - mouseCursor).Magnitude() * 5.0;
				particles[0]->Velocity = impulseDirection * impulseMagnitude;
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

	Vec2 wind = Vec2(0.4f * PIXEL_PER_METER, 0);
	
	// Vec2 gravitationalForce = Force::GenerateGravitationalForce(particles[0], particles[1], 1000.f, 10, 50);
	//particles[0]->AddForce(gravitationalForce);
	//particles[1]->AddForce(-gravitationalForce);


	for (Particle* particle : particles)
	{
		particle->AddForce(PushForce);
		Vec2 sptringForce = Force::GenerateSpringForce(particle, Vec2(Graphics::Width() / 2, 10), 10, 10);
		particle->AddForce(sptringForce);
		particle->AddForce(Vec2(0.f, 9.8f) * particle->Mass * PIXEL_PER_METER);
		particle->Integrate(deltaTime);

		Vec2 friction = Force::GenerateFrictionForce(particle->Velocity, 0.002f);
		particle->AddForce(friction);
		//particle->AddForce(Vec2(0.f, particle->Mass * 9.8 * PIXEL_PER_METER));

		//if (particle->Position.y > Graphics::Height() - FluidRect.h)
		//{
		//	Vec2 dragForce = Force::GenerateDragForce(particle->Velocity, 0.06f);
		//	particle->AddForce(dragForce);
		//}
		//else
			//particle->AddForce(wind);




		if (particle->Position.x + particle->Radius <= 0)
		{
			particle->Position.x = particle->Radius;
			particle->Velocity.x *= -1.f;
		}

		else if (particle->Position.x + particle->Radius >= Graphics::Width())
		{
			particle->Position.x = Graphics::Width() - particle->Radius;
			particle->Velocity.x *= -1.f;
		}

		if (particle->Position.y + particle->Radius <= 0)
		{
			particle->Position.y = particle->Radius;
			particle->Velocity.y *= -1.f;
		}

		else if (particle->Position.y + particle->Radius >= Graphics::Height())
		{
			particle->Position.y = Graphics::Height() - particle->Radius;
			particle->Velocity.y *= -1.f;
		}
	}






}


void Application::Render()
{
	Graphics::ClearScreen(0xFF056263);



	// Graphics::DrawFillRect(FluidRect.x + FluidRect.w / 2, FluidRect.y + FluidRect.h / 2, FluidRect.w, FluidRect.h, 0xFFFF0000);

	//for (Particle* particle : particles)
	Graphics::DrawFillCircle(particles[0]->Position.x, particles[0]->Position.y, particles[0]->Radius, 0xFFFF3333);
	Graphics::DrawFillCircle(Graphics::Width() / 2, 10, 5, 0xFF000000);
	Graphics::DrawLine(Graphics::Width() / 2, 10, particles[0]->Position.x, particles[0]->Position.y, 0xFFFF0000);


	if (rightMouseButtonDown) {
		Graphics::DrawLine(particles[0]->Position.x, particles[0]->Position.y, mouseCursor.x, mouseCursor.y, 0xFFFF00FF);
	}
	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	for (Particle* particle : particles)
		delete particle;

	Graphics::CloseWindow();
}
