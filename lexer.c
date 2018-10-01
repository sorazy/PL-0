// Soliman Alnaizy, UCFID: 3715450, Summer '18
// COP 3402, Dr. Montagne
// ============================================
// ASSIGNMENT #4:  ~~ T H E   S C A N N E R ~~
// ============================================

#include <ctype.h>
#include <limits.h>
#include "systemsoftware.h"

Token *createToken(int kind, char *name, int value, int level, int address, int mark)
{
   Token *t = malloc(sizeof(Token));
   t->name = malloc(sizeof(char) * (strlen(name) + 1));
   strcpy(t->name, name);
   t->kind = kind;
   t->value = value;
   t->level = level;
   t->address = address;
   t->mark = mark;

   return t;
}





// Preforms a tail insert on a LinkedList
void insertList(LinkedList *list, Token *t)
{
   node *newNode = calloc(1, sizeof(node));
   newNode->t = t;

   if (list->size == 0)
   {
      list->head = newNode;
      list->tail = list->head;
   }
   else
   {
      list->tail->next = newNode;
      newNode->prev = list->tail;
      list->tail = list->tail->next;
      list->tail->next = NULL;
   }
   
   list->size++;
   return;
}

// Checks if a passed string is made entirely of numbers
int isNumber(char *str)
{
   int len = strlen(str), i;
  
   for (i = 0; i < len; ++i)
      if (isalpha(str[i]))
         return 0;

   return 1;
}

// Prints the contents of a HashTable
void printTable(HashTable *h)
{
   int i;
   for (i = 0; i < h->capacity; i++)
      if (h->table[i] != NULL)
         printf("%d- %s\n", i, h->table[i]->name);
      else
         printf("%d- %s\n", i, "NULL");

   return;
}

// Just inserts a bunch of tokens into the HashTable
void insertKeywords(HashTable *h)
{
   h = insertToken(h, 4, "+", 0, 0, 0, 0);
   h = insertToken(h, 5, "-", 0, 0, 0, 0);
   h = insertToken(h, 6, "*", 0, 0, 0, 0);
   h = insertToken(h, 7, "/", 0, 0, 0, 0);
   h = insertToken(h, 8, "odd", 0, 0, 0, 0);
   h = insertToken(h, 9, "=", 0, 0, 0, 0);
   h = insertToken(h, 10, "<>", 0, 0, 0, 0);
   h = insertToken(h, 11, "<", 0, 0, 0, 0);
   h = insertToken(h, 12, "<=", 0, 0, 0, 0);
   h = insertToken(h, 13, ">", 0, 0, 0, 0);
   h = insertToken(h, 14, ">=", 0, 0, 0, 0);
   h = insertToken(h, 15, "(", 0, 0, 0, 0);
   h = insertToken(h, 16, ")", 0, 0, 0, 0);
   h = insertToken(h, 17, ",", 0, 0, 0, 0);
   h = insertToken(h, 18, ";", 0, 0, 0, 0);
   h = insertToken(h, 19, ".", 0, 0, 0, 0);
   h = insertToken(h, 20, ":=", 0, 0, 0, 0);
   h = insertToken(h, 21, "begin", 0, 0, 0, 0);
   h = insertToken(h, 22, "end", 0, 0, 0, 0);
   h = insertToken(h, 23, "if", 0, 0, 0, 0);
   h = insertToken(h, 24, "then", 0, 0, 0, 0);
   h = insertToken(h, 25, "while", 0, 0, 0, 0);
   h = insertToken(h, 26, "do", 0, 0, 0, 0);
   h = insertToken(h, 27, "call", 0, 0, 0, 0);
   h = insertToken(h, 28, "const", 0, 0, 0, 0);
   h = insertToken(h, 29, "var", 0, 0, 0, 0);
   h = insertToken(h, 30, "procedure", 0, 0, 0, 0);
   h = insertToken(h, 31, "write", 0, 0, 0, 0);
   h = insertToken(h, 32, "read", 0, 0, 0, 0);
   h = insertToken(h, 33, "else", 0, 0, 0, 0);

   return;
}

// Prints the contents of a file to the screen and resets the file pointer
void dumpSourceCode(char *filename)
{
   FILE *ifp = fopen(filename, "r");

   char buffer[1024];

   while (fgets(buffer, 1024, ifp) != NULL)
      printf("%s", buffer);

   printf("\n");

   fclose(ifp);

   return;
} 

// Process the passed in ifp and prints out the lexemeTable. It also returns
// a LinkedList that contains the head of the lexeme list that will be created
LinkedList *createLexemeList(FILE *ifp, HashTable *tokens)
{
   LinkedList *list = calloc(1, sizeof(LinkedList));
   int i, j, commentFlag = 0, length, len2, symTableAddress = 5;
   char buffer[1024], str[32];

   // Tokenize each string from the input file and store it into buffer
   while (fscanf(ifp, "%s", buffer) != EOF)
   {
      length = strlen(buffer);

      // We'll start by checking for comments
      // To be able to ignore comments, if the comment flag is asserted, just
      // continue on with the while loop and don't bother w/ commented strings
      if (buffer[0] == '/' && buffer[1] == '*')
      {
         commentFlag = 1;
      }

      if (buffer[length - 1] == '/' && buffer[length - 2] == '*' && commentFlag)
      {
         commentFlag = 0;
         continue;
      }

      if (commentFlag == 1)
      {
         continue;
      }

      // If the table is contained in our "tokens" hashtable, get that token
      // from the HashTable and insert it into the linked list
      if (contains(tokens, buffer))
      {
         insertList(list, get(tokens, buffer));
         continue;
      }
      
      // If the string was not immediately found in the hashtable, we're going
      // to need to tokenize it and seperate the keywords, terminators, identifiers,
      // and/or numbers. We'll copy the characters from buffer to a new string
      // called 'str' and check after each character insertion.
      for (i = 0, j = 0; i < length; ++i)
      {
         str[j++] = buffer[i];
         str[j] = '\0';

         if (contains(tokens, str))
         {
            insertList(list, get(tokens, str));
            str[0] = '\0';
            j = 0;
         }
         else if (ispunct(str[0]))
         {
            continue;
         }
         else if (ispunct(buffer[i+1]) || (!ispunct(buffer[length-1]) && (i == length - 1)))
         {
            if (isNumber(str))
               insertList(list, createToken(3, str, atoi(str), 0, 0, 0));
            else
               insertList(list, createToken(2, str, 0, 0, 0, 0));

            str[0] = '\0';
            j = 0;
         }
      }

      str[0] = '\0';
   }

   return list;
}

void printLexemeTable(LinkedList *list)
{
   node *temp = list->head;

   while(temp != NULL)
   {
      printf("%s\t\t%d\n", temp->t->name, temp->t->kind);
      temp = temp->next;
   }

   return;
}

void printLexemeList(LinkedList *list)
{
   node *temp = list->head;

   if (list == NULL || list->head == NULL)
   {
      printf("yo we got a NULL pointer up in printLexemeList.\n");
      return;
   }

   while(temp != NULL)
   {
      if (temp->t->kind == 2 || temp->t->kind == 3)
         printf("%d %s", temp->t->kind, temp->t->name);
      else
         printf("%d", temp->t->kind);

		printf(" %s", (temp->next == NULL) ? "\n" : "");
   
      temp = temp->next;
   }
   
   return;
}

void printLexemeListWithKeys(LinkedList *list)
{
   node *temp = list->head;
   int i = 0;

   while(temp != NULL)
   {
      if (temp->t->kind == 2 || temp->t->kind == 3)
         printf("%s %s", keys[temp->t->kind], temp->t->name);
      else
         printf("%s", keys[temp->t->kind]);

		printf(" %s", (temp->next == NULL) ? "\n" : "");
      
      i += strlen(keys[temp->t->kind]);

      // print new lines to make it look a little better
      if (i > 60)
      {
         printf("\n");
         i = 0;
      }

      temp = temp->next;
   }
   
   printf("\n");
   return;
}
