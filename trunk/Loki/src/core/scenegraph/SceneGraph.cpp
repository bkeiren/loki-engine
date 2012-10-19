#include "core/scenegraph/SceneGraph.h"
#include "core/scenegraph/SceneGraphNode.h"

namespace loki
{

SceneGraph::SceneGraph()	:
	m_Root(0)
{
	m_Root = new RootNode();
}

SceneGraph::~SceneGraph()
{
	delete m_Root;
}

SceneGraph::RootNode* SceneGraph::GetRoot() const
{
	return m_Root;
}

}