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

	QuadTreeNode* getChildInDirection(std::string direction);
	void insert(QuadTreeNodeData* data, std::string insertPath);

	bool hasOneSiblingWithData();
	bool hasSiblingWithChildren();
	QuadTreeNode* getSiblingWithData();
private:
	int _depth;
	QuadTreeNodeData* _data;
	QuadTreeNode* _parent;
	QuadTreeBoundary* _boundary;
	std::vector<QuadTreeNode*>* _children;
	QuadTreeNode* _northWest;
	QuadTreeNode* _northEast;
	QuadTreeNode* _southWest;
	QuadTreeNode* _southEast;

	void initChildren();
	QuadTreeBoundary* getQudrant(std::string direction);
};

