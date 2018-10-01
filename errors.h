#ifndef __LEXER_ERRORS_H
#define __LEXER ERRORS_H

// A bunch of strings that contain errors
static char errors[340][1024] = 
{
   "No errors, program is syntactically correct.",
   "Error 1: Invalid symbol detected!",
   "Error 2: Number is greater than 99999!",
   "Error 3: Identifier is longer than 11 characters!",
   "Error 4: Missing ',' to seperate identifiers in constant declarations",
   "Error 5: Use of '=' instead of ':='",
   "Error 6: Expected identifier in 'const' or 'var'.",
   "Error 7: Identifer must be followed by '=' in constant declaration",
   "Error 8: '=' must be followed by a number.",
   "Error 9: Identifier begins with a number.",
   "Error 10: Invalid symbol detected.",
   "Error 11: Undeclared identifier is being used.",
   "Error 12: Assignment operator expected.",
   "Error 13: Program must end with '.'", 
   "Error 14: Number is less than -99999!:",
   "Error 15: Invalid Symbol found",
   "Error 16: Missing an identifier or number in a statement.",
   "Error 17: Statement contains no identifiers or numbers after ':='",
   "Error 18: There contains a 'begin' with no 'end'.",
   "Error 19: 'end' was found without a 'begin'",
   "Error 20: Right paranthesis is missing.",
   "Error 21: Cannot use a keyword as an identifier.",
   "Error 22: Cannot declare two identifiers with the same name.",
   "Error 23: Use of ':=' instead of '='",
   "Error 24: Expected semicolon ';' or period '.'",
   "Error 25: Missing ',' to seperate identifiers in variable declarations.",
   "Error 26: Identifier expected after 'procedure'.",
   "Error 27: Expected semicolon ';' at end of procedure.",
   "Error 28: Expected ':=' symbol in statement.",
	"Error 29: Expected identifer or number.",
   "Error 30: Expected identifier after procedure call."
   "Error 31: Calling an undeclared procedure.",
   "Error 32: Expected semicolon after procedure declaration.",
   "Error 33: Expected 'then' keyword after an if-condition.",
   "Error 33: Expected 'then' keyword after an if-condition.",
   "Error 34: 'do' expected after the condition statement for the while loop.",
   "Error 35: Calling an undeclard procedure.",
   "Error 36: Expected relational operator.",
};

#endif