#include "JSONCpp/json.h"
#include "util/json/jsonvalue.h"

namespace loki
{

namespace util
{

namespace
{

Json::Value DummyValue = Json::Value(0);

}

JSONValue::JSONValue( const std::string& _String )	:
	m_Value(new Json::Value(_String)),
	m_ValueIsOwned(true)
{

}

JSONValue::JSONValue( int _Int )	:
	m_Value(new Json::Value(_Int)),
	m_ValueIsOwned(true)
{
	
}

JSONValue::JSONValue( uint32 _UInt )	:
	m_Value(new Json::Value(_UInt)),
	m_ValueIsOwned(true)
{
	
}

JSONValue::JSONValue( double _Double )	:
	m_Value(new Json::Value(_Double)),
	m_ValueIsOwned(true)
{
	
}

JSONValue::JSONValue( bool _Bool )	:
	m_Value(new Json::Value(_Bool)),
	m_ValueIsOwned(true)
{
	
}

JSONValue::JSONValue( Json::Value& _Value )	:
	m_Value(&_Value),
	m_ValueIsOwned(false)
{

}

JSONValue::JSONValue()	:
	m_Value(&DummyValue),
	m_ValueIsOwned(false)
{
	ILLEGAL_CTOR_ERROR("JSONValue");
}

JSONValue::~JSONValue()
{
	if (m_ValueIsOwned)
	{
		delete m_Value;
		m_Value = 0;
	}
}

std::string JSONValue::AsString()
{
	return m_Value->asString();
}

int JSONValue::AsInt() const
{
	return m_Value->asInt();
}

uint32 JSONValue::AsUInt() const
{
	return m_Value->asUInt();
}

double JSONValue::AsDouble() const
{
	return m_Value->asDouble();
}

bool JSONValue::AsBool() const
{
	return m_Value->asBool();
}

bool JSONValue::IsNull() const
{
	return m_Value->isNull();
}

bool JSONValue::IsBool() const
{
	return m_Value->isBool();
}

bool JSONValue::IsInt() const
{
	return m_Value->isInt();
}

bool JSONValue::IsUInt() const
{
	return m_Value->isUInt();
}

bool JSONValue::IsIntegral() const
{
	return m_Value->isIntegral();
}

bool JSONValue::IsDouble() const
{
	return m_Value->isDouble();
}

bool JSONValue::IsNumeric() const
{
	return m_Value->isNumeric();
}

bool JSONValue::IsString() const
{
	return m_Value->isString();
}

bool JSONValue::IsArray() const
{
	return m_Value->isArray();
}

bool JSONValue::IsObject() const
{
	return m_Value->isObject();
}

uint32 JSONValue::Size() const
{
	return m_Value->size();
}

bool JSONValue::IsEmpty() const
{
	return m_Value->empty();
}

JSONValue JSONValue::Get( uint32 _Index ) const
{
	return (m_Value->operator [](_Index));
}

JSONValue JSONValue::Get( const std::string& _Key ) const
{
	return (m_Value->operator [](_Key));
}

JSONValue JSONValue::operator[]( uint32 _Index ) const
{
	return JSONValue(m_Value->operator [](_Index));
}

JSONValue JSONValue::operator[]( const std::string& _Key )
{
	return JSONValue(m_Value->operator [](_Key));
}

const JSONValue JSONValue::operator[]( const std::string& _Key ) const
{
	return JSONValue(m_Value->operator [](_Key));
}

bool JSONValue::operator <( const JSONValue& _Other ) const
{
	return (m_Value->operator < (_Other.m_Value));
}

bool JSONValue::operator <=( const JSONValue& _Other ) const
{
	return (m_Value->operator <= (_Other.m_Value));
}

bool JSONValue::operator >=( const JSONValue& _Other ) const
{
	return (m_Value->operator >= (_Other.m_Value));
}

bool JSONValue::operator >( const JSONValue& _Other ) const
{
	return (m_Value->operator > (_Other.m_Value));
}

bool JSONValue::operator ==( const JSONValue& _Other ) const
{
	return (m_Value->operator == (_Other.m_Value));
}

bool JSONValue::operator !=( const JSONValue& _Other ) const
{
	return (m_Value->operator != (_Other.m_Value));
}

JSONValue& JSONValue::operator =( const JSONValue& _Other )
{
	(m_Value->operator =(_Other.m_Value));
	return *this;
}

JSONValue& JSONValue::operator =( const std::string& _String )
{
	m_Value->operator = (_String);
	return *this;
}

JSONValue& JSONValue::operator =( int _Int )
{
	m_Value->operator = (_Int);	
	return *this;
}

JSONValue& JSONValue::operator =( uint32 _UInt )
{
	m_Value->operator = (_UInt);
	return *this;
}

JSONValue& JSONValue::operator =( double _Double )
{
	m_Value->operator = (_Double);
	return *this;
}

JSONValue& JSONValue::operator =( bool _Bool )
{
	m_Value->operator = (_Bool);
	return *this;
}

}

}