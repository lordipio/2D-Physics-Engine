#include "Contact.h"

void Contact::ResolvePeneteration()
{
	if (a->IsStatic() && b->IsStatic())
		return;

	float InvMass1 = a->InverseMass;
	float InvMass2 = b->InverseMass;

	float d1 = depth* InvMass1 / (InvMass1 + InvMass2);
	float d2 = depth* InvMass2 / (InvMass1 + InvMass2);

	a->position -= normal * d1 * 0.8;
	b->position += normal * d2 * 0.8;

	a->shape->UpdateVertices(a->rotation, a->position);
	b->shape->UpdateVertices(b->rotation, b->position);
}

void Contact::ResolveCollision()
{
	if (a->IsStatic() && b->IsStatic())
		return;

	ResolvePeneteration();

	const Vec2 ra = end - a->position;
	const Vec2 rb = start - b->position;
	
	const Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
	const Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);
	
	const Vec2 vRel = va - vb;

	const float restitution = std::min(a->restitution, b->restitution);
	float Jn = ((-restitution - 1) * vRel.Dot(normal)) / ((a->InverseMass + b->InverseMass) + ((pow(ra.Cross(normal), 2) * a->inverseI) + (pow(rb.Cross(normal), 2) * b->inverseI)));


	const Vec2 tangent = normal.Normal();
	const float friction = std::min(a->friction, b->friction);
	float Jt = (friction * (-restitution - 1) * vRel.Dot(tangent)) / ((a->InverseMass + b->InverseMass) + ((pow(ra.Cross(tangent), 2) * a->inverseI) + (pow(rb.Cross(tangent), 2) * b->inverseI)));

	Vec2 JAnongTangent = tangent * Jt;
	Vec2 JAnongNormal = normal * Jn;

	Vec2 J = JAnongTangent + JAnongNormal;

	a->ApplyImpulseAtPoint(J, ra);
	b->ApplyImpulseAtPoint(-J, rb);

}