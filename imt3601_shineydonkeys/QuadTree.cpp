#include "QuadTree.h"
#include "QuadTreeNode.h"
#include "QuadTreeNodeData.h"
#include "Logger.h"
#include <stack>

QuadTree::QuadTree(QuadTreeBoundary* boundary) : _boundary(boundary)
{
	_root = nullptr;
}

QuadTree::~QuadTree()
{
	if (_root != nullptr)
		delete _root;
}

QuadTreeBoundary QuadTree::getBoundaryCpy()
{
	return *_boundary;
}

void QuadTree::add(QuadTreeNodeData* data)
{
	if (!_boundary->contains(data->getX(), data->getY()))
	{
		LOG_W("Coordinates are out of boundary!");
		return;
	}

	if (_root == nullptr)
		_root = new QuadTreeNode(0, _boundary, nullptr);

	_root->insert(data, _boundary->insertionPath(data->getX(), data->getY(), MAX_DEPTH));
}

// TODO come up with a more elegant solution (don't set x and y in quad tree method)
void QuadTree::move(QuadTreeNodeData* data, double newX, double newY)
{
	auto prevX = data->getX();
	auto prevY = data->getY();
	auto path = _boundary->insertionPath(prevX, prevY, MAX_DEPTH);
	auto node = search(_root, path, prevX, prevY);

	if (node == nullptr)
	{
		LOG_W("Attempted to move non-existing element");
		return;
	}

	if(!node->_boundary->contains(newX, newY))
	{
		remove(prevX, prevY);

		data->setX(newX);
		data->setY(newY);

		add(data);
	} 
	else 	
	{
		data->setX(newX);
		data->setY(newY);		
	}
}

QuadTreeNodeData* QuadTree::get(double x, double y)
{
	auto path = _boundary->insertionPath(x, y, MAX_DEPTH);
	auto node = search(_root, path, x, y);

	return node != nullptr ? node->_data : nullptr;
}

QuadTreeNodeData* QuadTree::remove(double x, double y)
{
	auto path = _boundary->insertionPath(x, y, MAX_DEPTH);
	auto current = search(_root, path, x, y);

	if (current == nullptr) return nullptr;

	if (current == _root)
	{
		_root = nullptr;
		return current->_data;
	}

	auto* ret = current->_data;
	current->_data = nullptr;

	
	
	while (current->hasOneSiblingWithData())
	{
		if (current->hasSiblingWithChildren())
			break;

		auto sibling = current->getSiblingWithData();
		current->_parent->_data = sibling->_data;
		current->_parent->_children = nullptr;
		current = current->_parent;
	}
	
	return ret;
}

std::vector<QuadTreeNodeData*> QuadTree::getInInterval(QuadTreeBoundary* boundary)
{
	std::vector<QuadTreeNodeData*> ret;
	std::stack<QuadTreeNode*> nodes;

	if (_root == nullptr || !boundary->intersects(*_boundary)) return ret;

	nodes.push(_root);

	while (nodes.size() > 0)
	{
		auto node = nodes.top();
		nodes.pop();

		if (node->_data != nullptr && boundary->contains(node->_data->getX(), node->_data->getY()))
			ret.push_back(node->_data);

		if (node->_children == nullptr)
			continue;

		for (auto it = node->_children->begin(); it != node->_children->end(); ++it)
		{
			auto child = *it;
			if (child->_boundary->intersects(*boundary) && (child->_data != nullptr || child->_children != nullptr))
				nodes.push(child);
		}
	}

	return ret;
}

QuadTreeNode* QuadTree::search(QuadTreeNode* root, std::string path, double x, double y)
{
	auto current = root;
	QuadTreeNode* ret = nullptr;

	while (current != nullptr && path.length() >= 2)
	{
		if (current->_data != nullptr && current->_data->getX() == x && current->_data->getY() == y)
		{
			ret = current;
			break;
		}
		auto direction = path.substr(0, 2);
		path = path.substr(2);
		current = current->getChildInDirection(direction);
	}

	return ret;
}

const std::string QuadTree::DIR_NW = "00";
const std::string QuadTree::DIR_NE = "10";
const std::string QuadTree::DIR_SW = "01";
const std::string QuadTree::DIR_SE = "11";
const int QuadTree::MAX_DEPTH = 15;