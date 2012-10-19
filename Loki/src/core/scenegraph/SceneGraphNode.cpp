#include "core/scenegraph/SceneGraphNode.h"
#include <algorithm>

namespace loki
{

SceneGraph::BaseNode::BaseNode()
{

}

SceneGraph::BaseNode::~BaseNode()
{
	for (ChildrenConstIter it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->Delete();
	}
}

SceneGraph::Node* SceneGraph::BaseNode::CreateChild()
{
	SceneGraph::Node* node = new SceneGraph::Node();
	AttachChild(node);
	return node;
}

void SceneGraph::BaseNode::AttachChild( SceneGraph::Node* _Child )
{
	_Child->_MakeOrphan();
	m_Children.push_back(_Child);
}

Transform& SceneGraph::BaseNode::GetTransform()
{
	return m_Transform;
}

bool SceneGraph::BaseNode::_FindChildIterator( SceneGraph::Node* _Child, SceneGraph::BaseNode::ChildrenConstIter& _OutputIter )
{
	ChildrenConstIter it = std::find(m_Children.begin(), m_Children.end(), _Child);
	if (it != m_Children.end())
	{
		_OutputIter = it;
		return true;
	}
	return false;
}


SceneGraph::Node::Node()	:
	m_Parent(0)
{
	
}

SceneGraph::Node::~Node()
{

}

void SceneGraph::Node::Delete()
{
	_MakeOrphan();

	for (ChildrenConstIter it = m_Children.begin(); it != m_Children.end(); ++it)
	{
		(*it)->Delete();
	}

	delete this;
}

void SceneGraph::Node::_MakeOrphan()
{
	if (!m_Parent)
	{
		return;
	}

	ChildrenConstIter Iter;
	if (!m_Parent->_FindChildIterator(this, Iter))
	{
		LOG(VL_ERROR, "SceneGraph::Node::_MakeOrphan: Detected graph corruption. Parent does not contain child");
	}
	else
	{
		m_Parent->m_Children.erase(Iter);
	}

	m_Parent = 0;
}

}