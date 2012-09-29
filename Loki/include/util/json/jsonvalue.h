#pragma once

#ifndef JSONVALUE_H
#define JSONVALUE_H

namespace loki
{

namespace util
{

class JSONValue
{
public:
	JSONValue( const std::string& _String );
	JSONValue( int _Int );
	JSONValue( uint32 _UInt );
	JSONValue( double _Double );
	JSONValue( bool _Bool );
	~JSONValue();

	// Get value functions.
	std::string AsString();
	int AsInt() const;
	uint32 AsUInt() const;
	double AsDouble() const;
	bool AsBool() const;

	// Type-testing functions.
	bool IsNull() const;
	bool IsBool() const;
	bool IsInt() const;
	bool IsUInt() const;
	bool IsIntegral() const;
	bool IsDouble() const;
	bool IsNumeric() const;
	bool IsString() const;
	bool IsArray() const;
	bool IsObject() const;
	
	// Array/Object functions.
	uint32 Size() const;
	bool IsEmpty() const;

	// Indexing functions. Identical to [] operators.
	JSONValue Get( uint32 _Index ) const;
	JSONValue Get( const std::string& _Key ) const;

	// Indexing operators. Identical to Get() function (including overloads).
	JSONValue operator[]( uint32 _Index ) const;
	JSONValue operator[]( const std::string& _Key );
	const JSONValue operator[]( const std::string& _Key ) const;

	// Comparison operators.
	bool operator <( const JSONValue& _Other ) const;
	bool operator <=( const JSONValue& _Other ) const;
	bool operator >=( const JSONValue& _Other ) const;
	bool operator >( const JSONValue& _Other ) const;
	bool operator ==( const JSONValue& _Other ) const;
	bool operator !=( const JSONValue& _Other ) const;

	// Assignment operators.
	JSONValue& operator =( const JSONValue& _Other );
	JSONValue& operator =( const std::string& _String );
	JSONValue& operator =( int _Int );
	JSONValue& operator =( uint32 _UInt );
	JSONValue& operator =( double _Double );
	JSONValue& operator =( bool _Bool );
private:
	friend class JSONDocument;

	JSONValue( Json::Value& _Value );
	JSONValue();

	Json::Value* m_Value;
	bool m_ValueIsOwned;
};

}

}

#endif