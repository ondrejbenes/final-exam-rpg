#include "VectorUtilities.h"
#include <cmath>


float VectorUtilities::calculateDistance(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	auto xDiff = pow(abs(vec1.x - vec2.x), 2);
	auto yDiff = pow(abs(vec1.y - vec2.y), 2);
	auto dist = sqrt(xDiff + yDiff);

	return dist;
}
