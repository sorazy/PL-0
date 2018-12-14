# PL/0 Virtual Machine
## Inroduction
PL/0 is a programming language that was invented for educational purposes. It is a relatively simple language that's used as a gateway into the world of compilers. There's also a lot to be discussed when it comees to the essence of how programming languages are written and understood by computers.

## EBNF of PL/0
Use the following productions in order to write any program in PL/0
```
program ::= block "." . 
block ::= const-declaration  var-declaration  procedure-declaration statement.	
constdeclaration ::= ["const" ident "=" number {"," ident "=" number} ";"].	
var-declaration  ::= [ "var "ident {"," ident} “;"].
procedure-declaration ::= { "procedure" ident ";" block ";" }
statement   ::= [ ident ":=" expression
                | "call" ident
	      	| "begin" statement { ";" statement } "end" 
	      	| "if" condition "then" statement ["else" statement]
		| "while" condition "do" statement
		| "read" ident
		| "write" expression
	      	| e ] .  
condition  ::= "odd" expression 
               | expression  rel-op  expression.  
rel-op     ::= "="|“!="|"<"|"<="|">"|">=“.
expression ::= [ "+"|"-"] term { ("+"|"-") term}.
term       ::= factor {("*"|"/") factor}. 
factor     ::= ident | number | "(" expression ")“.
number     ::= digit {digit}.
ident      ::= letter {letter | digit}.
digit      ::= "0" | "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9“.
letter     ::= "a" | "b" | … | "y" | "z" | "A" | "B" | ... | "Y" | "Z".
```

## What my program does
My program takes in a program written in Pl/0 in a .txt document as an arguement. It will then run it through a lexical analyzer, parser, and virtual machine (all written in C) and produce an output based on the program that was passed in.

## How to use the program
