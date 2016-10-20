#include "QuadTreeNode.h"
#include "Logger.h"
#include "QuadTree.h"


QuadTreeNode::QuadTreeNode(int depth, QuadTreeBoundary* boundary, QuadTreeNode* parent) :
_depth(depth),
_parent(parent),
_boundary(boundary)
{
	_data = nullptr;
	_children = nullptr;
	_northWest = nullptr;
	_northEast = nullptr;
	_southWest = nullptr;
	_southEast = nullptr;
}


QuadTreeNode::~QuadTreeNode()
{
	if (_boundary != nullptr)
		delete _boundary;
	if(_children != nullptr)
	{
		for (auto it = _children->begin(); it != _children->end(); ++it)
			delete *it;
	}
}

QuadTreeNode* QuadTreeNode::getChildInDirection(std::string direction)
{
	if(direction == QuadTree::DIR_NW)
		return _northWest;
	if (direction == QuadTree::DIR_NE)
		return _northEast;
	if (direction == QuadTree::DIR_SW)
		return _southWest;
	if (direction == QuadTree::DIR_SE)
		return _southEast;

	return nullptr;
}

void QuadTreeNode::insert(QuadTreeNodeData* data, std::string insertPath)
{
	if (_depth >= QuadTree::MAX_DEPTH)
	{
		LOG_E("Max depth reached");
		return;
	}

	if (_data == nullptr && _children == nullptr)
	{
		_data = data;
		return;
	}

	if (_data != nullptr && _children == nullptr)
	{
		if (_data->getX() == data->getX() && _data->getY() == data->getY())
		{
			LOG_E("Node with these coords already exists!");
			return;
		}

		initChildren();
		auto* originalData = _data;
		_data = nullptr;
		auto insertionPath = _boundary->insertionPath(originalData->getX(), originalData->getY(), QuadTree::MAX_DEPTH - _depth);
		
		QuadTreeNode* childForOriginal = nullptr;

		for(auto it = _children->begin(); it != _children->end(); ++it)
		{
			if ((*it)->_boundary->contains(originalData->getX(), originalData->getY()))
				childForOriginal = *it;
		}

		if (childForOriginal == nullptr)
		{
			LOG_E("Out of boundary!");
			return;
		}

		childForOriginal->insert(originalData, insertionPath);
	}

	auto direction = insertPath.substr(0, 2);
	auto newInsertPath = insertPath.substr(2);

	getChildInDirection(direction)->insert(data, newInsertPath);
}

QuadTreeNode* QuadTreeNode::getSiblingWithData()
{
	if (_parent == nullptr)
		return nullptr;
	auto siblings = _parent->_children;
	QuadTreeNode* siblingWithData = nullptr;

	for (auto it = siblings->begin(); it != siblings->end(); ++it)
	{
		if ((*it)->_data != nullptr)
			siblingWithData = *it;
	}

	return siblingWithData;
}

bool QuadTreeNode::hasSiblingWithChildren()
{
	if (_parent == nullptr)
		return false;
	auto siblings = _parent->_children;
	auto siblingsWithChildren = 0;

	for (auto it = siblings->begin(); it != siblings->end(); ++it)
	{
		if ((*it)->_children != nullptr)
			siblingsWithChildren++;
	}

	return siblingsWithChildren > 0;
}

bool QuadTreeNode::hasOneSiblingWithData()
{
	if (_parent == nullptr)
		return false;
	auto siblings = _parent->_children;
	auto siblingsWithDataCount = 0;

	for (auto it = siblings->begin(); it != siblings->end(); ++it)
	{
		if ((*it)->_data != nullptr)
			siblingsWithDataCount++;
	}

	return siblingsWithDataCount == 1;
}

void QuadTreeNode::initChildren()
{
	_northWest = new QuadTreeNode(_depth + 1, getQudrant(QuadTree::DIR_NW), this);
	_northEast = new QuadTreeNode(_depth + 1, getQudrant(QuadTree::DIR_NE), this);
	_southWest = new QuadTreeNode(_depth + 1, getQudrant(QuadTree::DIR_SW), this);
	_southEast = new QuadTreeNode(_depth + 1, getQudrant(QuadTree::DIR_SE), this);

	_children = new std::vector<QuadTreeNode*>();
	_children->push_back(_northWest);
	_children->push_back(_northEast);
	_children->push_back(_southWest);
	_children->push_back(_southEast);
}

QuadTreeBoundary* QuadTreeNode::getQudrant(std::string direction)
{
	if (direction == QuadTree::DIR_NW)
		return new QuadTreeBoundary(_boundary->minX(), _boundary->minX() + _boundary->getWidth() / 2, _boundary->minY(), _boundary->minY() + _boundary->getHeight() / 2);
	if (direction == QuadTree::DIR_NE)
		return new QuadTreeBoundary(_boundary->minX() + _boundary->getWidth() / 2, _boundary->maxX(), _boundary->minY(), _boundary->minY() + _boundary->getHeight() / 2); ;
	if (direction == QuadTree::DIR_SW)
		return  new QuadTreeBoundary(_boundary->minX(), _boundary->minX() + _boundary->getWidth() / 2, _boundary->minY() + _boundary->getHeight() / 2, _boundary->maxY());
	if (direction == QuadTree::DIR_SE)
		return new QuadTreeBoundary(_boundary->minX() + _boundary->getWidth() / 2, _boundary->maxX(), _boundary->minY() + _boundary->getHeight() / 2, _boundary->maxY());

	return nullptr;
}