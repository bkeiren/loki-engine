#pragma once

#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

namespace loki
{

class SceneGraph
{
public:
	class BaseNode;
	class Node;

	typedef BaseNode RootNode;

	SceneGraph();
	~SceneGraph();

	RootNode* GetRoot() const;
private:
	RootNode* m_Root;
};

}

#endif