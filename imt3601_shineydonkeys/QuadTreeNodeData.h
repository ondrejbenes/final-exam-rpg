#pragma once

class QuadTreeNodeData
{
public:
	virtual double getX() const = 0;
	virtual double getY() const = 0;
	virtual void setX(double x) = 0;
	virtual void setY(double y) = 0;
};