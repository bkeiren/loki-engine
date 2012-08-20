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

%{
typedef void(*ParseFunctor)( const char* );

ParseFunctor _ParseSection = NULL;
ParseFunctor _ParseProperty = NULL;

%}

%%

{section}				{if (_ParseSection) _ParseSection(yytext);}
{property}				{if (_ParseProperty) _ParseProperty(yytext);}
.						{}

%%