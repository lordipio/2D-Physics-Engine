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

float CircleShape::GetMomentOfInertia() const
{
	return 0.5f * radius * radius;
}

PolygonShape::PolygonShape(const std::vector<Vec2>& vertices)
{
	this->localVertices = vertices;
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

float PolygonShape::GetMomentOfInertia() const
{
	return 0.0f;
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
