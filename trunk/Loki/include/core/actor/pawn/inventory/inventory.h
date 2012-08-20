#pragma once

#ifndef INVENTORY_H
#define INVENTORY_H

#include "core/actor/actor.h"

namespace loki
{

class Item;

class LkInventory
{
public:
	LkInventory();
	~LkInventory();

private:
	std::list<Item*> m_Items;
};

}

#endif