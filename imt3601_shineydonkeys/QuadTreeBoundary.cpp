#include "QuadTreeBoundary.h"
#include "StringUtilities.h"

#include <sstream>
#include <algorithm>

QuadTreeBoundary::QuadTreeBoundary(double minX, double maxX, double minY, double maxY) : 
_minX(minX),
_maxX(maxX),
_minY(minY),
_maxY(maxY)
{

}

QuadTreeBoundary::~QuadTreeBoundary()
{

}

bool QuadTreeBoundary::contains(double x, double y) const
{
	return x >= _minX && x < _maxX && y >= _minY && y < _maxY;
}

bool QuadTreeBoundary::intersects(const QuadTreeBoundary& other) const
{
	return other._maxX >= _minX && other._minX <= _maxX && other._maxY >= _minY && other._minY <= _maxY;
}

std::string QuadTreeBoundary::insertionPath(double x, double y, int maxDepth)
{
	auto xBinStr = StringUtilities::doubleToBinaryStr((x - _minX) / getWidth());
	xBinStr = xBinStr.substr(2);
	auto padding = std::max(int(maxDepth + 1 - xBinStr.length()), 0);
	xBinStr.append(padding, '0');

	auto yBinStr = StringUtilities::doubleToBinaryStr((y - _minY) / getHeight());
	yBinStr = yBinStr.substr(2);
	padding = std::max(int(maxDepth + 1 - yBinStr.length()), 0);
	yBinStr.append(padding, '0');

	std::stringstream ss;

	for (auto i = 0; i < maxDepth; i++)
	{
		ss << xBinStr[i] << yBinStr[i];
	}

	return ss.str();
}

double QuadTreeBoundary::getWidth() const
{
	return _maxX - _minX;
}

double QuadTreeBoundary::getHeight() const
{
	return _maxY - _minY;
}
