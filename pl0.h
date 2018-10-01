#ifndef __SYSTEMSOFTWARE_H
#define __SYSTEMSOFTWARE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG_MODE 1
#define MAX_CODE_LENGTH 500
#define MAX_ID_LENGTH 11
#define MAX_INT_VALUE 99999
#define MAX_LEXI_LEVEL 3
#define MAX_STACK_HEIGHT 2000
#define MIN_INT_VALUE -99999

// We'll use this when creating our Hash Table
static int primes[10] = {3, 7, 17, 37, 79, 173, 347, 701, 1409, 2819};

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

static char keys[34][16] = 
{
   "XXX", "nulsym", "identsym", "numbersym", "plussym", "minussym", "multsym",
   "slashsym", "oddsym", "eqlsym", "neqsym", "lessym", "leqsym", "gtrsym",
   "geqsym", "lparetsym", "rparentsym", "commasym", "semicolonsym", "periodsym",
   "becomesym", "beginsym", "endsym", "ifsym", "thensym", "whilesym", "dosym",
   "callsym", "constsym", "intsym", "procsym", "writesym", "readsym", "elsesym"
}; 

// A global psuedocode variable that will be used by multiple functions
static char pseudoCode[23][4] =
{
   "X", "lit", "rtn", "lod", "sto", "cal", "inc", "jmp", "jpc", "sio", "neg","add",
   "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss", "leq", "gtr", "geq"
};

// This will hold the instruction when generating the assembly code
typedef struct Instruction
{
   int opCode;
   int r;
   int l;
   int m;
} Instruction;

// To preserve the order of the instructions, we'll put them into a linked list
typedef struct iNode
{
   Instruction *instruction;
   struct iNode *next;
} iNode;

// A typical linked list struct
typedef struct iList
{
   iNode *head, *tail;
   int size;
} iList;

// This will be used for the identifers, keywords, terminators, etc.
typedef struct Token
{
   int kind;
   char *name;
   int value;
   int level;
   int address;
   int mark;
} Token;

// Also inorder to preserve the order of the lexemes, we'll use a linked list
typedef struct node
{
   Token *t;
   struct node *next, *prev;
} node; 

// A generic Linked List
typedef struct LinkedList
{
   node *head, *tail;
   int size;
} LinkedList;

// We'll use a HashTable for the symbol table
typedef struct HashTable
{
   Token **table;
   int size;
   int capacity;
   int prime;
} HashTable;

// ======================================================
// pm0vm FUCNTIONS
// ======================================================

void printStack(int sp, int bp, int* stack, int lex);

int base(int *stack, int lexi, int bp);

void printRegister(int *regFile);

void vmTrace(int **code, int lines);

void stackTraceWithoutPrinting(int **code, int *stack, int *regFile, int lines);

void stackTrace(int **code, int *stack, int *regFile, int lines);

int fileToArray(int **code);

// ======================================================
// scanner functions
// ======================================================

HashTable *createHashTable(void);

Token *createToken(int kind, char *name, int value, int level, int address, int mark);

int hashFunction(char *str);

void quadraticProbe(Token **table, Token *t, int len);

void expandTable(HashTable *h);

void insertList(LinkedList *list, Token *t);

HashTable *insertToken(HashTable *h, int kind, char *name, int value, int level, int address, int mark);

HashTable *insertFullToken(HashTable *h, Token *t);

Token *get(HashTable *h, char *str);

Token *getTokenWithSameLevel(HashTable *h, Token *t, int depth);

Token *getTokenFromTable(HashTable *h, Token *t, int depth);

int contains(HashTable *h, char *str);

int containsTokenWithSameLevel(HashTable *h, Token *t, int depth);

int containsTokenFromTable(HashTable *h, Token *t, int depth);

int isNumber(char *str);

void printTable(HashTable *h);

void createPL0Table(HashTable *tokenTable);

void printFile(char *filename);

LinkedList *createLexemeList(FILE *ifp, HashTable *tokens);

void printLexemeTable(LinkedList *list);

void printLexemeList(LinkedList *list);

void printLexemeListWithKeys(LinkedList *list);

// ======================================================
// Parser functions
// ======================================================

void error(int e);

void tailInsert(iList *list, Instruction *i);

Token *getToken(LinkedList *list, int num);

Instruction *getInstruction(iList *list, int bookmark);

Token *getLastToken(LinkedList *list);

Instruction *genInstruction(int psuedoCode, int reg, int l, int m);

void exportInstructions(iList *list);

int isValidNumber(Token *t);

int isValidIdentifier(Token *t);

int isValidFactor(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth);

int isValidTerm(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth);

int isValidExpression(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth);

int isValidCondition(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth);

int isValidStatement(iList *instructionList, LinkedList *lexemeList, HashTable *keywords, HashTable *symbols, int *bookmark, int depth);

int isValidBlock(iList *instructionList, LinkedList *lexemeList, HashTable *keywords, HashTable *symbols, int *bookmark, int depth);

iList *analyzeLexemeList(LinkedList *lexemeList, HashTable *keywords, HashTable *symbols);

#endif




