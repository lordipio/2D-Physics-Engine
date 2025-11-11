#ifndef SHAPE_H
#define SHAPE_H

#include "Vec2.h"
#include <vector>

enum ShapeType
{
	CIRCLE,
	POLYGON,
	BOX
};

struct Shape
{
	virtual ~Shape() = default;
	virtual ShapeType GetType() const = 0;
	virtual Shape* Clone() const = 0;
	virtual float GetMomentOfInertia() const = 0;
};

struct CircleShape : public Shape
{
	float radius;
	CircleShape(float radius);
	virtual ~CircleShape();
	virtual ShapeType GetType() const override;
	virtual Shape* Clone() const override;
	virtual float GetMomentOfInertia() const override;

};

struct PolygonShape : public Shape
{
	std::vector<Vec2> localVertices;
	std::vector<Vec2> worldVertices;
	PolygonShape() = default;
	PolygonShape(const std::vector<Vec2>& vertices);
	virtual ~PolygonShape();
	virtual ShapeType GetType() const override;
	virtual Shape* Clone() const override;
	virtual float GetMomentOfInertia() const override;
	virtual void UpdateVertices(float angle, const Vec2& position);

};

struct BoxShape : public PolygonShape
{
	float width, height;
	BoxShape(const float width, const float height);
	virtual ~BoxShape();
	virtual ShapeType GetType() const override;
	virtual Shape* Clone() const override;
	virtual float GetMomentOfInertia() const override;
};


#endif // !SHAPE_H
