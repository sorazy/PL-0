
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
