#include "Force.h"

Vec2 Force::GenerateDragForce(const Vec2& Velocity, float k)
{
	Vec2 DragForce = Vec2(0, 0);

	if (Velocity.MagnitudeSquared() > 0)
	{
		Vec2 DragDirection = Velocity.UnitVector() * -1;

		float DragMagnitude = Velocity.MagnitudeSquared() * k;

		DragForce = DragDirection * DragMagnitude;
	}

	return DragForce;
}
