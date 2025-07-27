#include "Application.h"

bool Application::IsRunning()
{
	return isRunning;
}

void Application::Setup()
{
	isRunning = Graphics::OpenWindow();

	particles.push_back(new Particle(Vec2(100, 100), 1, 10.f));
	particles.push_back(new Particle(Vec2(200, 200), 2, 20.f));

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
	

	for (Particle* particle : particles)
	{
		particle->AddForce(PushForce);
		particle->AddForce(Vec2(0.f, particle->Mass * 9.8 * PIXEL_PER_METER));

		if (particle->Position.y > Graphics::Height() - FluidRect.h)
		{
			Vec2 dragForce = Force::GenerateDragForce(particle->Velocity, 0.06f);
			particle->AddForce(dragForce);
		}
		else
			particle->AddForce(wind);



		particle->Integrate(deltaTime);

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
	Graphics::DrawFillRect(FluidRect.x + FluidRect.w / 2, FluidRect.y + FluidRect.h / 2, FluidRect.w, FluidRect.h, 0xFFFF0000);

	for (Particle* particle : particles)
		Graphics::DrawFillCircle(particle->Position.x, particle->Position.y, particle->Radius, 0xFFFFFFFF);

	Graphics::RenderFrame();
}


void Application:: Destroy()
{
	for (Particle* particle : particles)
		delete particle;

	Graphics::CloseWindow();
}
