#pragma once

#ifndef AICONTROLLER_H
#define AICONTROLLER_H

#include "core/actor/pawn/controller/controller.h"

namespace loki
{

class LkAIController	: public LkController
{
public:
	LkAIController();
	virtual ~LkAIController() = 0;

private:
};

}

#endif