// Written by: Soliman Alnaizy
// =====================================
// PL-0: ~~ T H E   H A S H T A B L E ~~
// =====================================

#include "hashtable.h"

// Creates and returns a new HashTable of length 3
HashTable *initHashTable(void)
{
	HashTable *newTable = malloc(sizeof(HashTable));
	newTable->table = calloc(3, sizeof(Token*));
	newTable->size = 0;
	newTable->capacity =  3;
	newTable->prime = 0;

	return newTable;
}

// Using Horner's rule to mutliply the ASCII value by decreasing powers of 31
int hashFunction(char *str)
{
	int i, hash = 0;
	
	for (i = 0; str[i] != '\0'; ++i)
		hash = 31 * hash + str[i];

	// To make sure that the hash value is positive (integer overflow protection)
	if (hash < 0)
		hash *= -1;

	return hash;
}

// We'll use quadratic probing to insert values in the HashTable
void quadraticProbe(Token **table, Token *t, int len)
{
	int i = 0, hash = hashFunction(t->name);
	Token *temp = table[hash % len];

	// Insert whenever you hit a NULL spot or a DRTY bit spot, whichever comes first
	while (temp != NULL || !strcmp(temp->lexeme, DRTY))
	{
		hash += 2 * (i++) + 1;

		temp = table[hash % len];
	}

	table[hash % len] = t;

	return;
}

// Insert an a token that was already created into the HashTable
HashTable *insertToken(HashTable *h, Token *t)
{
	int hash = hashFunction(t->name);

	// Expand the HashTable if it becomes more than half full
	if (++h->size > (h->capacity / 2))
		expandTable(h);

	// Insert into the table using quadratic probing
	quadraticProbe(h->table, t, h->capacity);

	return h;
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

// Returns the token that has the lexeme of str. If token is not in table, return NULL
Token *getToken(HashTable *h, char *str)
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

// Call the get function. If get returns NULL, that means it's not in the table
int containsToken(HashTable *h, char *str)
{
	if (get(h, str) == NULL)
		return 0;
	else
		return 1;
}

// Call the get function, and set the lexeme value to DRTY (defined in hashtable.h)
// Returns 1 for successful deletion, 0 if token not found.
int deleteToken(HashTable *h, char *str)
{
	Token *temp = get(h, str);

	if (temp == NULL)
		return 0;

	strcpy(temp->lexeme, DRTY);
	h->size--;

	return 1;
}
