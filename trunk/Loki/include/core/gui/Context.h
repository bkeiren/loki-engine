#pragma once

#ifndef CONTEXT_H
#define CONTEXT_H

namespace Rocket
{

namespace Core
{

class Context;

}

}

namespace loki
{

namespace gui
{

class Document;

class Context
{
	friend class GUI;

	CONTAINER_MACRO_LIST(Document*, Documents)
public:
	Document* LoadDocument( const std::string& _File );
	void UnloadDocument( Document* _Document );

private:
	Context();
	Context( const std::string& _Name, const int2& _Dimensions );
	~Context();

	Rocket::Core::Context* m_RocketContext;
	const std::string m_Name;

	Documents m_Documents;
};

}

}

#endif