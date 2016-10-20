#pragma once
#include <string>

class QuadTreeBoundary
{
public:
	QuadTreeBoundary(double min_x, double max_x, double min_y, double max_y);
	~QuadTreeBoundary();

	bool contains(double x, double y) const;
	bool intersects(const QuadTreeBoundary& other) const;
	std::string insertionPath(double x, double y, int maxDepth);
	double getWidth() const;
	double getHeight() const;


	double minX() const { return _minX; }
	double maxX() const { return _maxX; }
	double minY() const { return _minY; }
	double maxY() const { return _maxY; }

private:
	double _minX, _maxX, _minY, _maxY;
};
