#pragma once

#ifndef OCTREE_SPECIALIZATIONS_H
#define OCTREE_SPECIALIZATIONS_H

#include "core/entitysystem/component/default/MeshRenderer.h"
#include "core/entitysystem/component/default/Transform.h"

namespace loki
{

namespace graphics
{

//////////////////////////////////////////////////////////////////////////
// MeshRenderer component specializations.
//////////////////////////////////////////////////////////////////////////
template<>
inline void GetOctantMemberPosition<components::MeshRenderer*>( components::MeshRenderer* _Member, vec3& _Output )
{
	_Output = _Member->GetTransform().GetPosition();
}

// template<>
// inline void GetOctantMemberBoundingBox<components::MeshRenderer*>( components::MeshRenderer* _Member, BoundingBox& _Output )
// {
// TODO
// }
// 
// template<>
// inline f32 GetOctantMemberBoundingRadius<components::MeshRenderer*>( components::MeshRenderer* _member )
// {
// TODO
// }

template<>
inline bool GetOctantMemberIsDirty<components::MeshRenderer*>( components::MeshRenderer* _Member )
{
	return true;	// NOTE: Implement some way to check here if a transformation is dirty (It has translated
					// or one of it's parents has done that OR rotated).
					// If this function simply returns true, all members in the octree will always be updated. That's not
					// very efficient.
}

}

}

#endif