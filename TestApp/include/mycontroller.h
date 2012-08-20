#pragma once

#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H

#include "core/actor/pawn/controller/controller.h"

class MyController	: public loki::LkController
{
public:
	MyController();
	~MyController();

private:
	void _OnEvent( const loki::LkEvent& _Event );

};

#endif