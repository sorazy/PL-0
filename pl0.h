#ifndef __P_L_ZERO_H
#define __P_L_ZERO_H

#define DEBUG_MODE 1
#define MAX_CODE_LINES 500
#define MAX_ID_LENGTH 11
#define MAX_INT_DIGITS 5
#define MAX_LEXI_LEVEL 3
#define MAX_STACK_HEIGHT 2000

typedef enum {
    NONE = 0,
    NONLETTER_VAR_INITIAL,
    NAME_TOO_LONG,
    NUM_TOO_LONG,
    INV_SYM,
    NO_SOURCE_CODE,
} LexErr;


static char keys[34][16] = 
{
   "XXX",         "nulsym",      "identsym",    "numbersym",   "plussym",     "minussym",
   "multsym",     "slashsym",    "oddsym",      "eqlsym",      "neqsym",      "lessym",
   "leqsym",      "gtrsym",      "geqsym",      "lparetsym",   "rparentsym",  "commasym",
   "semicolonsym","periodsym",   "becomesym",   "beginsym",    "endsym",      "ifsym",
   "thensym",     "whilesym",    "dosym",       "callsym",     "constsym",    "intsym",
   "procsym",     "writesym",    "readsym",     "elsesym"
}; 

// A global variable that will be used by multiple functions
static char opCodes[23][4] =
{
   "XXX", "lit", "rtn", "lod", "sto", "cal", "inc", "jmp", "jpc", "sio", "neg","add",
   "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss", "leq", "gtr", "geq"
};

// This will hold the instruction when generating the assembly code
typedef struct Instruction
{
   int op;
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
   int id;
   char *lexeme;
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




