#pragma once

#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "core/actor/pawn/controller/controller.h"

namespace loki
{

class LkPlayerController	: public LkController
{
public:
	LkPlayerController();
	virtual ~LkPlayerController() = 0;

private:
};

}

#endif