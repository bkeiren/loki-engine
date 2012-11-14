template TEMPLATELINE
Property TEMPLATEARGUMENTLINE ::Property()
{
	
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE ::~Property()
{
	
}

template TEMPLATELINE
_Class& Property TEMPLATEARGUMENTLINE ::Self()
{
	return *reinterpret_cast<_Class *> (reinterpret_cast<char *> (this) - _Offset());
}

template TEMPLATELINE
_Class const& Property TEMPLATEARGUMENTLINE ::Self() const
{
	return *reinterpret_cast<_Class const *> (reinterpret_cast<char const *> (this) - _Offset());
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE& Property TEMPLATEARGUMENTLINE ::operator = (_T const& rhs) 
{
	_Set(Self(), rhs); 
	return *this; 
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE& Property TEMPLATEARGUMENTLINE ::operator += (_T const& rhs) 
{ 
	_Set(Self(), _Get(Self()) + rhs); 
	return *this; 
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE& Property TEMPLATEARGUMENTLINE ::operator -= (_T const& rhs) 
{ 
	_Set(Self(), _Get(Self()) - rhs); 
	return *this; 
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE& Property TEMPLATEARGUMENTLINE ::operator *= (_T const& rhs) 
{ 
	_Set(Self(), _Get(Self()) * rhs); 
	return *this; 
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE& Property TEMPLATEARGUMENTLINE ::operator /= (_T const& rhs) 
{ 
	_Set(Self(), _Get(Self()) / rhs); 
	return *this; 
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE ::operator _T const & () const
{
	return _Get(Self());
}

template TEMPLATELINE
_T* Property TEMPLATEARGUMENTLINE ::operator -> ()
{
	return &const_cast<_T &> (_Get(Self()));
}

template TEMPLATELINE
_T const* Property TEMPLATEARGUMENTLINE ::operator -> () const
{
	return &_Get(Self());
}