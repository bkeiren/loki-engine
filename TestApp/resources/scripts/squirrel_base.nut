// This script can be used to define global functionality (such as classes, functions or data)
// that can then be used by other scripts.
// NOTE: This script must be run by the engine before it's functionality can be used by other scripts.


////////////////////////////////////////////////////////////////////////////////////////////
// Data.

ActorList <- {};	// Create an empty array.
local ActorCounter = -1;


////////////////////////////////////////////////////////////////////////////////////////////
// Classes.

class vec3
{
	constructor( _x, _y, _z )
	{
		x = _x;
		y = _y;
		z = _z;
	}

	x = 0;
	y = 0;
	z = 0;
}

class Actor
{
	// C-tor.
	constructor( _name )
	{
		// Inheriting classes can call this c-tor with the following syntax:
		// base.constructor(name);

		ActorCounter = ActorCounter + 1;	// ++ActorCounter; (?)
		id = ActorCounter;
		name = _name;
	}
	
	// Functions.
	function PrintData()
	{
		::print("Actor Data: '" + name + "' (" + id + ") [" + pos.x + ", " + pos.y + ", " + pos.z + "]");
	}


	// Data.
	pos = vec3(0, 0, 0);
	id = 0;		// Squirrel ID. (Used for indexing?)
	cid = 0;	// C++ ID.
	name = "Unknown";
}

////////////////////////////////////////////////////////////////////////////////////////////
// Functions.

function SpawnActor( _name, _type )
{
	ActorList[_name] <- Actor(_name);
	return ActorList[_name];
}
