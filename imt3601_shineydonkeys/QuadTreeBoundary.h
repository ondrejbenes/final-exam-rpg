#pragma once
#include <string>

class QuadTreeBoundary
{
public:
	QuadTreeBoundary(double min_x, double max_x, double min_y, double max_y);

	bool contains(double x, double y) const;
	bool intersects(const QuadTreeBoundary& other) const;

	std::string getInsertionPath(double x, double y, int maxDepth);

	double getWidth() const;
	double getHeight() const;

	double getMinX() const { return _minX; }
	double getMaxX() const { return _maxX; }
	double getMinY() const { return _minY; }
	double getMaxY() const { return _maxY; }

private:
	double _minX, _maxX, _minY, _maxY;
};
