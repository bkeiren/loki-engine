digit			([0-9])
alpha			([a-fA-F])
character		([a-zA-Z])
hextail			(({digit}|{alpha}){1,8})
hex				(0[xX]{hextail})
whitespace		([ ])
tab				([\t])
newline			([\n|\r])
float			(-?{digit}+\.{digit}+)
integer			(-?{digit}+)
boolean			(true|false|TRUE|FALSE)
string			(\"(.*)*\")
identifier		(({character}+{digit}*)*)
section			(\[{identifier}\])
value			(.+)
property		({identifier}[\t| ]*={value})
meshdef			((mesh|Mesh|MESH)({whitespace}|{tab})+{string})
materialdef		((material|Material|MATERIAL|mat|Mat|MAT){digit}({whitespace}|{tab})+{string})

%{
typedef void(*ParseFunctor)( loki::renderer::Model*, const char* );
loki::renderer::Model* _Model = NULL;

ParseFunctor _ParseMeshDef = NULL;
ParseFunctor _ParseMaterialDef = NULL;

%}

%%

{meshdef}			{if (_ParseMeshDef) _ParseMeshDef(_Model, yytext);}
{materialdef}		{if (_ParseMaterialDef) _ParseMaterialDef(_Model, yytext);}
.						{}

%%