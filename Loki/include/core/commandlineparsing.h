#include <list>

/*
	Parameter structure that can hold a key, number of values associated with the key, and the actual values associated with the key.
*/
struct Parameter
{
	Parameter()	:
		m_NumValues(0),
		m_ValuesInt(0),
		m_ValuesFloat(0),
		m_ValuesDouble(0)
		//m_ValuesChar(0)
	{}
	~Parameter()
	{
		delete[m_NumValues] m_ValuesInt;
		delete[m_NumValues] m_ValuesFloat;
		delete[m_NumValues] m_ValuesDouble;
		//delete[m_NumValues] m_ValuesChar;
	}

	loki::uint32			m_NumValues;
	loki::int32*			m_ValuesInt;
	loki::f32*			m_ValuesFloat;
	double*			m_ValuesDouble;
	//char**			m_ValuesChar;
	std::string		m_String;
};

// TODO: Use a multiset instead of a list of Parameter*?
CONTAINER_MACRO_MAP(std::string, Parameter*, CommandLineParameters);
void ParseCommandLine( loki::int32 argc, char** argv, CommandLineParameters& paramsList );
void PrintParameterList( CommandLineParameters& paramsList );