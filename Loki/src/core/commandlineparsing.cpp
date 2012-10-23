/*
	This file contains code that can be used to parse the command line input of an application.
	The parsing process formats the input and stores it in an std::list<Parameter*>.
	The Parameter structure contains an std::string which represents the command line parameter itself, and integer, f32 and double value
	arrays which represent any (optional) values passed for the associated parameter. Additionally, each value is also stored as a C-string.
	NOTE: It is NOT possible to pass negative values because the minus sign ('-') is parsed as being the beginning of a new parameter.
	NOTE: THe maximum length in characters for any key or value is 32.		
	
	Example:

	Command line parameter: -sz 1024 768
	Parameter structure:	Parameter.m_Key = "sz" (NOTE: std::string object for ease of working with it)
				Parameter.m_NumValues = 2
				Parameter.m_ValuesInt[0] = 1024
				Parameter.m_ValuesFloat[0] = 1024.000000...
				Parameter.m_ValuesDouble[0] = 1024.00000000000...
				Parameter.m_ValuesChar[0] = "1024"
				Parameter.m_ValuesInt[1] = 768
				Parameter.m_ValuesFloat[1] = 768.000000...
				Parameter.m_ValuesDouble[1] = 768.00000000000...
				Parameter.m_ValuesChar[1] = "768"



	To parse command line input, call the function 'ParseCommandLine()', using the argc and argv arguments of the main() function, and
	the address of an std::list<Parameter*> object in which the result must be stored.

	Example:

	int32 main( int32 argc, char** argv )
	{
		// Parse command line arguments and store list with Parameter-pointers.
		std::list<Parameter*> cmdParams;
		ParseCommandLine( argc, argv, cmdParams );

		// cmdParams now contains all passed parameters and associated values.
		
		return 0;
	}
*/

 #pragma warning( disable : 4996 )	// Disable a pesky warning (C4996) about how using std::bas_string<blah>::copy might be unsafe.

#include <stdio.h>	// Required for printf().
#include "core/commandlineparsing.h"

using namespace loki;

// Function that can be called to parse command line input.
// First two arguments match the arguments of the main() function, the third argument is an std::list<> reference in which the
// resulting list must be stored.
void ParseCommandLine( int32 argc, char** argv, CommandLineParameters& paramsList )
{
	// List of std::string which will contain command line arguments and their associated values (all in a single string).
	std::list<std::string*> stringList;
	std::string* previousString = 0;
	
	// Handle the first argument on it's own, as this argument is the absolute path to the executable.
	Parameter* pathparameter = new Parameter();
	pathparameter->m_String = argv[0];
	paramsList["EXEPATH"] = pathparameter;

	// For each command line argument...
	for ( int32 i = 1; i < argc; ++i )
	{
		// If argument starts with a dash ('-')...
		if (argv[i][0] == '-')
		{
			// Add a new string to the list.
			previousString = new std::string(argv[i]);
			stringList.push_back( previousString );
		}
		else
		{
			if (previousString != NULL)
			{
				// Append a space to separate values from themselves and to separate values and keys.
				*previousString += " ";
				
				// Then append the value.
				*previousString += argv[i];
			}
			// If debug build, output some text notifying the user of the fact that a command line argument is being skipped because 
			// there is an incorrect syntax.
#ifdef _DEBUG
			else
			{
				printf("ParseCommandLine: Argument skipped due to an incorrect syntax or because there were no associated parameters: '");
				printf( argv[i] );
				printf("'\n");
			}
#endif
		}
	}

	// For each string in stringList...
	for ( std::list<std::string*>::iterator it = stringList.begin(); it != stringList.end(); ++it )
	{
		std::string* currentString = (*it);

		Parameter* currentParameter = new Parameter();
		
		char cstring[32];

		// Find the first occurrence of a space (" ") and store it's position.
		uint32 stringIteratorIndex = currentString->find(" ", 0);

		// Make sure that if stringIteratorIndex is -1, it is set to the length of the string, minus 1.
		if (stringIteratorIndex == -1) stringIteratorIndex = currentString->size() - 1;
		
		// Copy the key by copying part of currentString.
		// The range of characters that is copied is from position 1 (to skip position 0, which should be a dash)
		// to the first occurrence of a space (which should occur at the end of the key).
		currentString->copy( cstring, stringIteratorIndex, 1 );

		// Indicate where the string ends explicitly. If this is skipped, all the other array-elements will be considered part
		// of the string.
		cstring[stringIteratorIndex] = '\0';

		// Store the key.
		std::string key = std::string(cstring);

		// List for values (which will be represented through char*).
		std::list<char*> valueList;

		// While stringIteratorIndex is less than the length of the string...
		while ( stringIteratorIndex < currentString->length() - 1 )
		{
			// Store the next position of a space (or the end of the string if that is found before any space).
			int32 nextStringIteratorIndex = currentString->find(" ", stringIteratorIndex + 1);

			if (nextStringIteratorIndex == -1) nextStringIteratorIndex = currentString->size() - 1;

			// Get contents from stringIteratorIndex to nextStringIteratorIndex.
			char cstring2[32];	// Additional char array because trying to use the first one proved to be annoying and longwinded...
			currentString->copy( cstring2, nextStringIteratorIndex, stringIteratorIndex );

			// Add the char* to the list of values.
			valueList.push_back(cstring2);

			stringIteratorIndex = nextStringIteratorIndex; // Set stringIteratorIndex for the next iteration.
		}

		int32 NumValues = valueList.size();

		currentParameter->m_ValuesInt = new int32[NumValues];
		currentParameter->m_ValuesFloat = new f32[NumValues];
		currentParameter->m_ValuesDouble = new double[NumValues];
		//currentParameter->m_ValuesChar = new char*[NumValues];

		// Initialize all variables to 0.
		for ( int32 j = 0; j < NumValues; ++j )
		{
			currentParameter->m_ValuesInt[j] = 0;
			currentParameter->m_ValuesFloat[j] = 0.0f;
			currentParameter->m_ValuesDouble[j] = 0;
		}

		int32 ValueIndex = 0;

		// For each value represented through a char* in valueList...
		for ( std::list<char*>::iterator it2 = valueList.begin(); it2 != valueList.end(); ++it2, ++ValueIndex )
		{
			int32 IntegerValue = atoi( (*it2) );
			double DoubleValue = atof( (*it2) );

			currentParameter->m_ValuesInt[ValueIndex] = IntegerValue;
			currentParameter->m_ValuesFloat[ValueIndex] = (f32)DoubleValue;
			currentParameter->m_ValuesDouble[ValueIndex] = DoubleValue;
			//currentParameter->m_ValuesChar[ValueIndex] = (*it2);

			++currentParameter->m_NumValues;
		}

		// Store entire parameter in paramsList.
		paramsList[key] = currentParameter;
	}
}

// This is a simple utility function that takes a parameter list such as the one used by ParseCommandLine(), and prints
// it's contents in a understandable, organized manner.
void PrintParameterList( CommandLineParameters& paramsList )
{
	printf("\nCommand Line Parameter List:\n\nKey:\tkeyname\nValues:\tinteger\tfloat\tdouble\n\n----------------------------------------------------\n");
	for (CommandLineParametersConstIter it = paramsList.begin(); it != paramsList.end(); ++it)
	{
		Parameter* currentParam = (*it).second;
		std::string key = (*it).first;
		printf("\nKey:\t");
		printf(key.c_str());

		if (currentParam->m_NumValues > 0)
		{
			printf("\nValues:");
			for ( uint32 i = 0; i < currentParam->m_NumValues; ++i )
			{
				printf("\t%i\t%f\t%f\t\n", currentParam->m_ValuesInt[i], currentParam->m_ValuesFloat[i], currentParam->m_ValuesDouble[i]);
			}
		}
		else
		{
			printf("\n");
		}
	}
	printf("\n");
}