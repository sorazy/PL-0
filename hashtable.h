#ifndef __HASHTABLES_H
#define __HASHTABLES_H

#include <stdlib.h>
#include <string.h>

// We'll use this when expanding the HashTable
static int primes[10] = {3, 7, 17, 37, 79, 173, 347, 701, 1409, 2819};

static char DRTY[] = "+=+=+=+=+=+";

// We'll use a HashTable for the symbol table
typedef struct HashTable
{
   Token **table;
   int size;
   int capacity;
   int prime;
} HashTable;

HashTable *initHashTable(void);

int hashFunction(char *str);

void quadraticProbe(Token **table, Token *t, int len);

HashTable *insertToken(HashTable *h, Token *t);

void expandTable(HashTable *h);

Token *getToken(HashTable *h, char *str);

int containsToken(HashTable *h, char *str);

int deleteToken(HashTable *h, char *str);

#endif
