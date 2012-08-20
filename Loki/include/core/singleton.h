#pragma once

// --------------------------------------------------
// Singleton class.
// Usage instructions:
// Classes that should implement the singleton pattern
// should inherit from this class, where typename T is
// their own classtype. For example, a class Foo should
// be declares as:
// class Foo : public Singleton<Foo>
// Additionally, inheriting class should not have public
// constructors, and must provide the Singleton class
// with access to their private constructor.
// This can be achieved by declaring the Singleton class
// as friend.
// --------------------------------------------------
template<typename T>
class LkSingleton
{
public:
	LkSingleton();

	/*
		Obtain a pointer to the singleton instance.
		Returns a constant pointer, which means that the pointer address is constant.
	*/
	static T* const GetPtr();

	/*
		Obtain a reference to the singleton instance.
		
		Implementation note: Doesn't return a const reference as GetPtr() does with a pointer, because the const
		keyword is ignored on references.
	*/
	static T& Get();
private:
	~LkSingleton();

	static T* m_Instance;
};

// --------------------------------------------------
// Singleton class implementations.
// --------------------------------------------------
template<typename T>
T* LkSingleton<T>::m_Instance = NULL;

template<typename T>
LkSingleton<T>::LkSingleton()
{

}

template<typename T>
T* const LkSingleton<T>::GetPtr()
{
	if (!m_Instance)
	{
		m_Instance = new T();
		printf("Singleton of type '%s' was instantiated.\n", typeid(m_Instance).name());
	}

	return m_Instance;
}

template<typename T>
T& LkSingleton<T>::Get()
{
	if (!m_Instance)
	{
		m_Instance = new T();
		printf("Singleton of type '%s' was instantiated.\n", typeid(m_Instance).name());
	}	

	return *m_Instance;
}