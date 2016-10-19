#pragma once

#include <vector>

#include "QuadTreeBoundary.h"
#include "QuadTreeNode.h"
#include "QuadTreeNodeData.h"

class QuadTree
{
public:
	friend QuadTreeNode;

	static const std::string DIR_NW;
	static const std::string DIR_NE;
	static const std::string DIR_SW;
	static const std::string DIR_SE;
	static const int MAX_DEPTH;

	explicit QuadTree(QuadTreeBoundary* boundary);
	~QuadTree();

	void add(QuadTreeNodeData* data);
	QuadTreeNodeData* get(double x, double y);
	QuadTreeNodeData* remove(double x, double y);
	std::vector<QuadTreeNodeData*> getInInterval(QuadTreeBoundary* boundary);
	std::vector<QuadTreeBoundary*> getBoundaries();
private:
	QuadTreeBoundary* _boundary;
	QuadTreeNode* _root;

	QuadTreeNode* search(QuadTreeNode* root, std::string path, double x, double y);
	// TODO move to quadNode?
	bool hasOneSiblingWithData(QuadTreeNode* current);
	bool hasSiblingWithChildren(QuadTreeNode* current);
	QuadTreeNode* getSiblingWithData(QuadTreeNode* current);
};