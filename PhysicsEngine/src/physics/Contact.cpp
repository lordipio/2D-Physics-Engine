#include "Contact.h"

void Contact::ResolvePeneteration()
{
	if (body1->IsStatic() && body2->IsStatic())
		return;

	float InvMass1 = body1->InverseMass;
	float InvMass2 = body2->InverseMass;

	float d1 = depth* InvMass1 / (InvMass1 + InvMass2);
	float d2 = depth* InvMass2 / (InvMass1 + InvMass2);

	body1->Position -= normal * d1;
	body2->Position += normal * d2;
}

void Contact::ResolveCollision()
{
	if (body1->IsStatic() && body2->IsStatic())
		return;

	ResolvePeneteration();

	

	float J = ((-std::min(body1->restitution, body2->restitution) - 1) * (body1->Velocity - body2->Velocity).Dot(normal)) / (body1->InverseMass + body2->InverseMass);

	Vec2 Jn = normal * J;

	body1->ApplyImpulse(Jn);
	body2->ApplyImpulse(-Jn);

}