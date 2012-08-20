#pragma once

#ifndef PHYSICSCOMPONENT_H
#define PHYSICSCOMPONENT_H

#include <list>
#include "core/actor/components/base/actorcomponent.h"

namespace loki
{

class LkPhysicsComponent	: public LkActorComponent
{
public:
	LkPhysicsComponent();
	~LkPhysicsComponent();

protected:
	void Update();

private:
	
};

}

#endif