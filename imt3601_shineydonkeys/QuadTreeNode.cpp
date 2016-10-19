#include "QuadTreeNode.h"
#include "Logger.h"
#include "QuadTree.h"


QuadTreeNode::QuadTreeNode(int depth, QuadTreeBoundary* boundary, QuadTreeNode* parent) :
Depth(depth),
Boundary(boundary),
Parent(parent)
{
	Data = nullptr;
	Children = nullptr;
	NorthWest = nullptr;
	NorthEast = nullptr;
	SouthWest = nullptr;
	SouthEast = nullptr;
}


QuadTreeNode::~QuadTreeNode()
{
}

QuadTreeNode* QuadTreeNode::GetChildInDirection(std::string direction)
{
	if(direction == QuadTree::DIR_NW)
		return NorthWest;
	if (direction == QuadTree::DIR_NE)
		return NorthEast;
	if (direction == QuadTree::DIR_SW)
		return SouthWest;
	if (direction == QuadTree::DIR_SE)
		return SouthEast;

	return nullptr;
}

void QuadTreeNode::Insert(QuadTreeNodeData* data, std::string insertPath)
{
	if (Depth >= QuadTree::MAX_DEPTH)
	{
		LOG_E("Max depth reached");
		return;
	}

	if (Data == nullptr && Children == nullptr)
	{
		Data = data;
		return;
	}

	if (Data != nullptr && Children == nullptr)
	{
		if (Data->getX() == data->getX() && Data->getY() == data->getY())
		{
			LOG_E("Node with these coords already exists!");
			return;
		}

		initChildren();
		auto* originalData = Data;
		Data = nullptr;
		auto insertionPath = Boundary->insertionPath(originalData->getX(), originalData->getY(), QuadTree::MAX_DEPTH - Depth);
		
		QuadTreeNode* childForOriginal = nullptr;

		for(auto it = Children->begin(); it != Children->end(); ++it)
		{
			if ((*it)->Boundary->contains(originalData->getX(), originalData->getY()))
				childForOriginal = *it;
		}

		childForOriginal->Insert(originalData, insertionPath);
	}

	auto direction = insertPath.substr(0, 2);
	auto newInsertPath = insertPath.substr(2);

	GetChildInDirection(direction)->Insert(data, newInsertPath);
}

void QuadTreeNode::initChildren()
{
	NorthWest = new QuadTreeNode(Depth + 1, getQudrant(QuadTree::DIR_NW), this);
	NorthEast = new QuadTreeNode(Depth + 1, getQudrant(QuadTree::DIR_NE), this);
	SouthWest = new QuadTreeNode(Depth + 1, getQudrant(QuadTree::DIR_SW), this);
	SouthEast = new QuadTreeNode(Depth + 1, getQudrant(QuadTree::DIR_SE), this);

	Children = new std::vector<QuadTreeNode*>();
	Children->push_back(NorthWest);
	Children->push_back(NorthEast);
	Children->push_back(SouthWest);
	Children->push_back(SouthEast);
}

QuadTreeBoundary* QuadTreeNode::getQudrant(std::string direction)
{
	if (direction == QuadTree::DIR_NW)
		return new QuadTreeBoundary(Boundary->minX(), Boundary->minX() + Boundary->getWidth() / 2, Boundary->minY(), Boundary->minY() + Boundary->getHeight() / 2);
	if (direction == QuadTree::DIR_NE)
		return new QuadTreeBoundary(Boundary->minX() + Boundary->getWidth() / 2, Boundary->maxX(), Boundary->minY(), Boundary->minY() + Boundary->getHeight() / 2); ;
	if (direction == QuadTree::DIR_SW)
		return  new QuadTreeBoundary(Boundary->minX(), Boundary->minX() + Boundary->getWidth() / 2, Boundary->minY() + Boundary->getHeight() / 2, Boundary->maxY());
	if (direction == QuadTree::DIR_SE)
		return new QuadTreeBoundary(Boundary->minX() + Boundary->getWidth() / 2, Boundary->maxX(), Boundary->minY() + Boundary->getHeight() / 2, Boundary->maxY());

	return nullptr;
}