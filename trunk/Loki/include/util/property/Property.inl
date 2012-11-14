template TEMPLATELINE
Property TEMPLATEARGUMENTLINE ::Property()
{
	m_Class = 0;
	m_SetFunctor = 0;
	m_GetFunctor = 0;
}

template TEMPLATELINE
Property TEMPLATEARGUMENTLINE ::~Property()
{
	
}

template TEMPLATELINE
void Property TEMPLATEARGUMENTLINE ::Init(_ParentClassType* _Class, GetFunctor _GetFunctor, SetFunctor _SetFunctor )
{
	m_Class = _Class;
	m_SetFunctor = _SetFunctor;
	m_GetFunctor = _GetFunctor;
}

template TEMPLATELINE
inline Property TEMPLATEARGUMENTLINE ::operator _PropertyType(void)
{
	return this->Get();
}

template TEMPLATELINE
inline const _PropertyType& Property TEMPLATEARGUMENTLINE ::operator = (const _PropertyType& _Value)
{
	this->Set(_Value);
	return m_Value;
}

template TEMPLATELINE
inline const _PropertyType& Property TEMPLATEARGUMENTLINE ::operator += (const _PropertyType& _Value)
{
	this->Set(this->Get() + _Value);
	return m_Value;
}

template TEMPLATELINE
inline const _PropertyType& Property TEMPLATEARGUMENTLINE ::operator -= (const _PropertyType& _Value)
{
	this->Set(this->Get() - _Value);
	return m_Value;
}

template TEMPLATELINE
inline const _PropertyType& Property TEMPLATEARGUMENTLINE ::operator *= (const _PropertyType& _Value)
{
	this->Set(this->Get() * _Value);
	return m_Value;
}

template TEMPLATELINE
inline const _PropertyType& Property TEMPLATEARGUMENTLINE ::operator /= (const _PropertyType& _Value)
{
	this->Set(this->Get() / _Value);
	return m_Value;
}

template TEMPLATELINE
inline const _PropertyType& Property TEMPLATEARGUMENTLINE ::Get(void)
{
	return (m_Class->*m_GetFunctor)();
}

template TEMPLATELINE
inline void Property TEMPLATEARGUMENTLINE ::Set(const _PropertyType& _Value)
{
	(m_Class->*m_SetFunctor)(_Value);
}