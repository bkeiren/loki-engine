#include "core/gui/Context.h"
#include "core/gui/Document.h"
#include <ROcket/Core.h>

namespace loki
{

namespace gui
{

Context::Context()
{
	ILLEGAL_CTOR_ERROR("gui::Context")
}

Context::Context( const std::string& _Name, const int2& _Dimensions )	:
	m_RocketContext(0)
	,m_Name(_Name)
{
	m_RocketContext = Rocket::Core::CreateContext(	Rocket::Core::String(_Name.c_str()), 
													Rocket::Core::Vector2i(_Dimensions.x, _Dimensions.y), 
													0	);
}

Context::~Context()
{
	m_RocketContext->RemoveReference();
}

Document* Context::LoadDocument( const std::string& _File )
{
	Rocket::Core::ElementDocument* rocketdoc = m_RocketContext->LoadDocument(Rocket::Core::String(_File.c_str()));
	Document* doc = new Document();
	doc->m_RocketElementDocument = rocketdoc;
	m_Documents.push_back(doc);
	doc->Show();
	return doc;
}

void Context::UnloadDocument( Document* _Document )
{
	m_Documents.remove(_Document);
	m_RocketContext->UnloadDocument(_Document->m_RocketElementDocument);
	delete _Document;
}

}

}