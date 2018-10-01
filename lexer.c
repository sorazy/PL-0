// Soliman Alnaizy, UCFID: 3715450, Summer '18
// COP 3402, Dr. Montagne
// ============================================
// ASSIGNMENT #4:  ~~ T H E   S C A N N E R ~~
// ============================================

#include <ctype.h>
#include <limits.h>
#include "systemsoftware.h"

// Creates and returns a new HashTable of length 3
HashTable *createHashTable(void)
{
   HashTable *newTable = malloc(sizeof(HashTable));
   newTable->table = calloc(3, sizeof(Token*));
   newTable->size = 0;
   newTable->capacity =  3;
   newTable->prime = 0;

   return newTable;
}

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

// My own hash function that multiplies the ASCII value of each letter in the
// Tokens name by a descending power of 3
int hashFunction(char *str)
{
   int i, hash = 0;
   
   for (i = 0; str[i] != '\0'; ++i)
      hash = 31 * hash + str[i];

   // To make sure that the hash value is positive
   if (hash < 0)
      hash *= -1;

   return hash;
}

// Insert the Token t into the table array using quadratic probing. Keep 
// searching untill you find a spot that's not NULL, then insert the Token
void quadraticProbe(Token **table, Token *t, int len)
{
   int i, hash = hashFunction(t->name);

   for (i = 1; table[hash % len] != NULL; ++i)
      hash += 2 * i - 1;

   table[hash % len] = t;

   return;
}

// Expands the table to the next prime in the static int array at the top
void expandTable(HashTable *h)
{
   int i, hash, newLength = primes[h->prime + 1];
   Token **newTable = calloc(newLength, sizeof(Token*));

   // Go through the old table and move them to the new table
   for (i = 0; i < h->capacity; ++i)
   {
      if (h->table[i] == NULL)
         continue;

      quadraticProbe(newTable, h->table[i], newLength);
   }

   // Free the old table and update struct members
   free(h->table);
   h->table = newTable;
   h->capacity = newLength;
   h->prime++;

   return;
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

// Creates a token with the information passed into it, and inserts it into a HashTable
HashTable *insertToken(HashTable *h, int kind, char *name, int value, int level, int address, int mark)
{
   int hash;
   Token* t = NULL;

   if (name == NULL)
      return NULL;

   if (h == NULL)
      h = createHashTable();

   t = createToken(kind, name, value, level, address, mark);

   hash = hashFunction(t->name);

   // Expand the HashTable if this insertion will cause the table to become
   // more than half full
   if (++h->size > (h->capacity / 2))
      expandTable(h);

   // Insert into the table using quadratic probing
   quadraticProbe(h->table, t, h->capacity);

   return h;
}

// Insert an a token that was already created into the HashTable
HashTable *insertFullToken(HashTable *h, Token *t)
{
   int hash = hashFunction(t->name);

   // Expand the HashTable if this insertion will cause the table to become
   // more than half full
   if (++h->size > (h->capacity / 2))
      expandTable(h);

   // Insert into the table using quadratic probing
   quadraticProbe(h->table, t, h->capacity);

   return h;
}

// Returns the token that has the name of str. If token is not in table, the
// function returns NULL
Token *get(HashTable *h, char *str)
{
   int i = 1, hash = hashFunction(str), len = h->capacity;

   if (h == NULL)
      return NULL;

   // Use quadratic probing to look for the token
   while (h->table[hash % len] != NULL)
   {
      if (!strcmp(h->table[hash % len]->name, str))
         return h->table[hash % len];
      else
         hash += 2 * i++ - 1;
   }

   return NULL;
}

// Returns a token with name of str and with a level equal to depth
Token *getTokenWithSameLevel(HashTable *h, Token *t, int depth)
{
   int i = 1, hash = hashFunction(t->name), len = h->capacity;
   Token *temp;

   if (h == NULL)
      return NULL;

   // Use quadratic probing to look for the token, get the variable with the closes depth
   // Looking for a token with name t->name, and depth, then depth - 1, then depth - 2, etc...
      while (h->table[hash % len] != NULL)
      {
         temp = h->table[hash % len];
         if (!strcmp(temp->name, t->name) && (temp->level == depth))
            return h->table[hash % len];
         else
            hash += 2 * i++ - 1;
      }
  
   return NULL;
}

// Returns a token with name of str and with a level equal or less than to depth
Token *getTokenFromTable(HashTable *h, Token *t, int depth)
{
   int i = 1, hash, len = h->capacity;
   Token *temp;

   if (h == NULL)
      return NULL;

   // Use quadratic probing to look for the token, get the variable with the closes depth
   // Looking for a token with name t->name, and depth, then depth - 1, then depth - 2, etc...
   while (depth >= 0)
   {
      hash = hashFunction(t->name);

      while (h->table[hash % len] != NULL)
      {
         temp = h->table[hash % len];
         if (!strcmp(temp->name, t->name) && (temp->level == depth))
            return h->table[hash % len];
         else
            hash += 2 * i++ - 1;
      }
   
      --depth;
   }

   return NULL;
}

// Call the get function. If get returns NULL, that means it's not in the table
int contains(HashTable *h, char *str)
{
   if (get(h, str) == NULL)
      return 0;
   else
      return 1;
}

// Call the get function. If get returns NULL, that means it's not in the table
int containsTokenWithSameLevel(HashTable *h, Token *t, int depth)
{
   if (getTokenWithSameLevel(h, t, depth) == NULL)
      return 0;
   else
      return 1;
}

// Call the get function. If get returns NULL, that means it's not in the table
int containsTokenFromTable(HashTable *h, Token *t, int depth)
{
   if (getTokenFromTable(h, t, depth) == NULL)
      return 0;
   else
      return 1;
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
void createPL0Table(HashTable *tokenTable)
{
   tokenTable = insertToken(tokenTable, 4, "+", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 5, "-", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 6, "*", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 7, "/", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 8, "odd", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 9, "=", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 10, "<>", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 11, "<", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 12, "<=", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 13, ">", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 14, ">=", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 15, "(", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 16, ")", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 17, ",", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 18, ";", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 19, ".", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 20, ":=", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 21, "begin", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 22, "end", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 23, "if", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 24, "then", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 25, "while", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 26, "do", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 27, "call", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 28, "const", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 29, "var", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 30, "procedure", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 31, "write", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 32, "read", 0, 0, 0, 0);
   tokenTable = insertToken(tokenTable, 33, "else", 0, 0, 0, 0);

   return;
}

// Prints the contents of a file to the screen and resets the file pointer
void printFile(char *filename)
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
