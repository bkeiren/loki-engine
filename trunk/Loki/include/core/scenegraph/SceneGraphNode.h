#pragma once

#ifndef SCENEGRAPHNODE_H
#define SCENEGRAPHNODE_H

#include "core/entitysystem/component/default/Transform.h"
#include "core/scenegraph/SceneGraph.h"

namespace loki
{

//////////////////////////////////////////////////////////////////////////
// Scene node base class.
//////////////////////////////////////////////////////////////////////////
class SceneGraph::BaseNode
{
	friend class SceneGraph;

	CONTAINER_MACRO_VECTOR(Node*, Children);
public:
	//////////////////////////////////////////////////////////////////////////
	// Creates a new child node.
	//////////////////////////////////////////////////////////////////////////
	Node* CreateChild();

	//////////////////////////////////////////////////////////////////////////
	// Attaches the node to this node as a child.
	// NOTE: There is no DetachChild function because that would result
	// in a parent-less node, which we do not allow (Save for the scene's root
	// node). If a node should be moved to a different parent node, AttachChild
	// will take care of this completely.
	//////////////////////////////////////////////////////////////////////////
	void AttachChild( Node* _Child );

	Transform& GetTransform();
private:
	BaseNode();
	virtual ~BaseNode();

	bool _FindChildIterator( Node* _Child, ChildrenConstIter& _OutputIter );

	Children m_Children;

	Transform m_Transform;
};


//////////////////////////////////////////////////////////////////////////
// Regular scene node.
//////////////////////////////////////////////////////////////////////////
class SceneGraph::Node	: public SceneGraph::BaseNode
{
	friend class SceneGraph::BaseNode;
public:
	//////////////////////////////////////////////////////////////////////////
	// Removes the node from it's parent and deletes the node and all
	// of its children.
	// After calling this function, any pointers to the object or its children
	// is no longer valid and should NOT be used.
	//////////////////////////////////////////////////////////////////////////
	void Delete();

	Node* GetParent() const;
private:
	Node();
	~Node();

	//////////////////////////////////////////////////////////////////////////
	// Clears the parent pointer and removes the node from it's current parent.
	//////////////////////////////////////////////////////////////////////////
	void _MakeOrphan();

	Node* m_Parent;
};

}

#endif