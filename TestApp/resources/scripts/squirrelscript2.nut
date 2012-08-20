class SomeActorClass extends Actor
{
	constructor( _name )
	{
		base.constructor( _name );
	}
}

local someactor = SomeActorClass("I_Am_An_Actor");
local someactor2 = SpawnActor("Je Oma Is Lelijk", "ToBeImplemented");
someactor.PrintData();
someactor2.PrintData();