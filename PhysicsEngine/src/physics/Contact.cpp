#include "Contact.h"

void Contact::ResolvePeneteration()
{
	if (body1->IsStatic() && body2->IsStatic())
		return;

	float InvMass1 = body1->InverseMass;
	float InvMass2 = body2->InverseMass;

	float d1 = depth* InvMass1 / (InvMass1 + InvMass2);
	float d2 = depth* InvMass2 / (InvMass1 + InvMass2);

	body1->Position -= normal * d1 * 0.8;
	body2->Position += normal * d2 * 0.8;

	body1->shape->UpdateVertices(body1->rotation, body1->Position);
	body2->shape->UpdateVertices(body2->rotation, body2->Position);
}

void Contact::ResolveCollision()
{
	if (body1->IsStatic() && body2->IsStatic())
		return;

	ResolvePeneteration();

	const Vec2 ra = end - body1->Position;
	const Vec2 rb = start - body2->Position;
	
	const Vec2 va = body1->Velocity + Vec2(-body1->angularVelocity * ra.y, body1->angularVelocity * ra.x);
	const Vec2 vb = body2->Velocity + Vec2(-body2->angularVelocity * rb.y, body2->angularVelocity * rb.x);
	
	const Vec2 vRel = va - vb;

	const float restitution = std::min(body1->restitution, body2->restitution);
	float Jn = ((-restitution - 1) * vRel.Dot(normal)) / ((body1->InverseMass + body2->InverseMass) + ((pow(ra.Cross(normal), 2) * body1->inverseI) + (pow(rb.Cross(normal), 2) * body2->inverseI)));


	const Vec2 tangent = normal.Normal();
	const float friction = std::min(body1->friction, body2->friction);
	float Jt = (friction * (-restitution - 1) * vRel.Dot(tangent)) / ((body1->InverseMass + body2->InverseMass) + ((pow(ra.Cross(tangent), 2) * body1->inverseI) + (pow(rb.Cross(tangent), 2) * body2->inverseI)));

	Vec2 JAnongTangent = tangent * Jt;
	Vec2 JAnongNormal = normal * Jn;

	Vec2 J = JAnongTangent + JAnongNormal;

	body1->ApplyImpulse(J, ra);
	body2->ApplyImpulse(-J, rb);

}