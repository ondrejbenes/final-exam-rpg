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

	_root->Insert(data, _boundary->insertionPath(data->getX(), data->getY(), MAX_DEPTH));
}

QuadTreeNodeData* QuadTree::get(double x, double y)
{
	auto path = _boundary->insertionPath(x, y, MAX_DEPTH);
	auto node = search(_root, path, x, y);

	return node != nullptr ? node->Data : nullptr;
}

QuadTreeNodeData* QuadTree::remove(double x, double y)
{
	auto path = _boundary->insertionPath(x, y, MAX_DEPTH);
	auto current = search(_root, path, x, y);

	if (current == nullptr) return nullptr;

	if (current == _root)
	{
		_root = nullptr;
		return current->Data;
	}

	auto* ret = current->Data;
	current->Data = nullptr;

	
	
	while (hasOneSiblingWithData(current))
	{
		if (hasSiblingWithChildren(current)) 
			break;

		auto sibling = getSiblingWithData(current);
		current->Parent->Data = sibling->Data;
		current->Parent->Children = nullptr;
		current = current->Parent;
	}
	
	return ret;
}

QuadTreeNode* QuadTree::getSiblingWithData(QuadTreeNode* current)
{
	if (current->Parent == nullptr)
		return nullptr;
	auto siblings = current->Parent->Children;
	QuadTreeNode* siblingWithData = nullptr;

	for (auto it = siblings->begin(); it != siblings->end(); ++it)
	{
		if ((*it)->Data != nullptr)
			siblingWithData = *it;
	}

	return siblingWithData;
}

bool QuadTree::hasSiblingWithChildren(QuadTreeNode* current)
{
	if (current->Parent == nullptr)
		return false;
	auto siblings = current->Parent->Children;
	auto siblingsWithChildren = 0;

	for (auto it = siblings->begin(); it != siblings->end(); ++it)
	{
		if ((*it)->Children != nullptr)
			siblingsWithChildren++;
	}

	return siblingsWithChildren > 0;
}

bool QuadTree::hasOneSiblingWithData(QuadTreeNode* current)
{
	if(current->Parent == nullptr)
		return false;
	auto siblings = current->Parent->Children;
	auto siblingsWithDataCount = 0;

	for (auto it = siblings->begin(); it != siblings->end(); ++it)
	{		
		if ((*it)->Data != nullptr)
			siblingsWithDataCount++;
	}

	return siblingsWithDataCount == 1;
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

		if (node->Data != nullptr && boundary->contains(node->Data->getX(), node->Data->getY()))
			ret.push_back(node->Data);

		if (node->Children == nullptr)
			continue;

		for (auto it = node->Children->begin(); it != node->Children->end(); ++it)
		{
			auto child = *it;
			if (child->Boundary->intersects(*boundary) && (child->Data != nullptr || child->Children != nullptr))
				nodes.push(child);
		}
	}

	return ret;
}

std::vector<QuadTreeBoundary*> QuadTree::getBoundaries()
{
	std::vector<QuadTreeBoundary*> boundaries;
	std::stack<QuadTreeNode*> nodes;

	if (_root == nullptr) 
		return boundaries;

	nodes.push(_root);

	while (nodes.size() > 0)
	{
		auto node = nodes.top();
		nodes.pop();
		if (node->Children == nullptr) 
			continue;
		for(auto it = node->Children->begin(); it != node->Children->end(); ++it)
			nodes.push(*it);
		boundaries.push_back(node->Boundary);
	}

	return boundaries;
}

QuadTreeNode* QuadTree::search(QuadTreeNode* root, std::string path, double x, double y)
{
	auto current = root;
	QuadTreeNode* ret = nullptr;

	while (current != nullptr && path.length() >= 2)
	{
		if (current->Data != nullptr && current->Data->getX() == x && current->Data->getY() == y)
		{
			ret = current;
			break;
		}
		auto direction = path.substr(0, 2);
		path = path.substr(2);
		current = current->GetChildInDirection(direction);
	}

	return ret;
}

const std::string QuadTree::DIR_NW = "00";
const std::string QuadTree::DIR_NE = "10";
const std::string QuadTree::DIR_SW = "01";
const std::string QuadTree::DIR_SE = "11";
const int QuadTree::MAX_DEPTH = 9;