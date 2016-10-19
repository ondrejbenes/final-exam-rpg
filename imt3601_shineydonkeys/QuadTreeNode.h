#pragma once

#include "QuadTreeNodeData.h"
#include <vector>

class QuadTreeBoundary;
class QuadTree;

class QuadTreeNode
{
public:
	friend QuadTree;

	QuadTreeNode(int depth, QuadTreeBoundary* boundary, QuadTreeNode* parent);
	~QuadTreeNode();

	QuadTreeNode* GetChildInDirection(std::string direction);
	void Insert(QuadTreeNodeData* data, std::string insertPath);

	// TODO private
	QuadTreeNodeData* Data;
	QuadTreeNode* Parent;
	std::vector<QuadTreeNode*>* Children;
	QuadTreeBoundary* Boundary;
private:
	int Depth;

	QuadTreeNode* NorthWest;
	QuadTreeNode* NorthEast;
	QuadTreeNode* SouthWest;
	QuadTreeNode* SouthEast;

	void initChildren();
	QuadTreeBoundary* getQudrant(std::string direction);
};

