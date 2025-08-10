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
	this->vertices = vertices;
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
	return new PolygonShape(this->vertices);
}

float PolygonShape::GetMomentOfInertia() const
{
	return 0.0f;
}

BoxShape::BoxShape(const float width, const float height)
{
	this->width = width;
	this->height = height;
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
