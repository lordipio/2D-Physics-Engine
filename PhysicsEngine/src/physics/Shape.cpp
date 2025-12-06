#include "Shape.h"

CircleShape::CircleShape(float radius)
{
	this->radius = radius;
}

CircleShape::~CircleShape()
{

}

ShapeType CircleShape::GetType() const
{
	return CIRCLE;
}

Shape* CircleShape::Clone() const
{
	return new CircleShape(this->radius);
}

void CircleShape::UpdateVertices(float angle, const Vec2& position)
{
	
}

float CircleShape::GetMomentOfInertia() const
{
	return 0.5f * radius * radius;
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices)
{
	this->localVertices = vertices;
	this->worldVertices = vertices;
}

PolygonShape::~PolygonShape()
{

}

ShapeType PolygonShape::GetType() const
{
	return POLYGON;
}

Shape* PolygonShape::Clone() const
{
	return new PolygonShape(this->localVertices);
}

Vec2 PolygonShape::EdgeAt(int index) const
{
	return worldVertices[(index + 1) % worldVertices.size()] - worldVertices[index];
}

float PolygonShape::GetMomentOfInertia() const
{
	// Implement Moment of Inertia Here!
	return 5000.f;
}

void PolygonShape::UpdateVertices(float angle, const Vec2& position)
{
	for (int i = 0; i < localVertices.size(); i++)
	{
		worldVertices[i] = localVertices[i].Rotate(angle);
		worldVertices[i] += position;
	}
}

BoxShape::BoxShape(const float width, const float height)
{
	this->width = width;
	this->height = height;

	localVertices.push_back(Vec2(-width / 2, -height / 2));
	localVertices.push_back(Vec2( width / 2, -height / 2));
	localVertices.push_back(Vec2( width / 2,  height / 2));
	localVertices.push_back(Vec2(-width / 2,  height / 2));

	worldVertices.push_back(Vec2(-width / 2, -height / 2));
	worldVertices.push_back(Vec2( width / 2, -height / 2));
	worldVertices.push_back(Vec2( width / 2,  height / 2));
	worldVertices.push_back(Vec2(-width / 2,  height / 2));
}

BoxShape::~BoxShape()
{

}

ShapeType BoxShape::GetType() const
{
	return BOX;
}

Shape* BoxShape::Clone() const
{
	return new BoxShape(this->width, this->height);
}

float BoxShape::GetMomentOfInertia() const
{
	return 0.08333f * ((width * width) + (height * height));
}

float PolygonShape::FindMinSeparation(const PolygonShape* other, Vec2& axis, Vec2& point) const
{
	float separation = std::numeric_limits<float>::lowest();
	for (int i = 0; i < this->worldVertices.size(); i++)
	{
		Vec2 va = this->worldVertices[i];
		Vec2 normal = this->EdgeAt(i).Normal();
		Vec2 minVertex;

		float minStep = std::numeric_limits<float>::max();

		for (int j = 0; j < other->worldVertices.size(); j++)
		{
			Vec2 vb = other->worldVertices[j];
			float project = normal.Dot(vb - va);
			if (minStep > project)
			{
				minStep = project;
				minVertex = vb;
			}
		}

		if (minStep > separation)
		{
			separation = minStep;
			point = minVertex;
			axis = this->EdgeAt(i);
		}
	}

	return separation;
}