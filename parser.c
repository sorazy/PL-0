// Soliman Alnaizy, UCFID: 3715450, Summer '18
// COP 3402, Dr. Montagne
// ===================================================================
// ASSIGNMENT #4: ~~ P A R S E R   &   C O D E   G E N E R A T I O N~~
// ===================================================================

#include <ctype.h>
#include "systemsoftware.h"

// This global variable will keep track of which line are we at the program.
int numberOfLines = 0;

// Global reg variable to keep track of which register we're at
int regPointer = 0;

// Prints a certain error message to screen
void error(int e)
{
   printf("%s\n", errors[e]);
   
   if (e == 0)
      return;
   else
      exit(0);
}

void tailInsert(iList *list, Instruction *i)
{
   iNode *newNode = calloc(1, sizeof(iNode));
   newNode->instruction = i;

   if (list->head == NULL)
   {
      list->head = list->tail = newNode;
      return;
   }

   list->tail->next = newNode;
   list->tail = list->tail->next;

   return;
}

Token *getToken(LinkedList *list, int num)
{
   int i;
   node *temp = list->head;

   for (i = 0; (i < num) && (temp != NULL); ++i)
      temp = temp->next;

   if (temp == NULL)
      return NULL;

   return temp->t;
}

Instruction *getInstruction(iList *list, int bookmark)
{
	int i;
	iNode *temp = list->head;
	
	for (i = 0; i < bookmark && temp != NULL; ++i)
		temp = temp->next;

	if (temp == NULL)
		return NULL;

	return temp->instruction;
}

Token *getLastToken(LinkedList *list)
{
   int i;
   node *temp = list->head;

   for (i = 0; (temp->next != NULL); ++i)
      temp = temp->next;

   if (temp == NULL)
      return NULL;

   return temp->t;
}

Instruction *genInstruction(int psuedoCode, int reg, int l, int m)
{
   Instruction *newInstruction = malloc(sizeof(Instruction));
   newInstruction->opCode = psuedoCode;
   newInstruction->r = reg;
   newInstruction->l = l;
   newInstruction->m = m;

   return newInstruction;
}

void exportInstructions(iList *list)
{
   Instruction *tempi;
   iNode *temp;
   FILE *igc = fopen("igc.txt", "w");

   if (igc == NULL)
   {
      printf("Couldn't find file in exportInstructions().\n");
      return;
   }

   for (temp = list->head; temp != NULL; temp = temp->next)
   {
      tempi = temp->instruction;
      fprintf(igc, "%d %d %d %d\n", tempi->opCode, tempi->r, tempi->l, tempi->m);
   }

   fclose(igc);

   return;
}

int isValidNumber(Token *t)
{
	// Check if it's actually a number to begin with
	if (t->kind != 3)
   {
      error(8);
      return 0;
   }            

	// Check that the number doesn't exceed 99999
   if (t->value > MAX_INT_VALUE)
   {
      error(2);
      return 0;
   }

	return 1;
}

int isValidIdentifier(Token *t)
{
	int i, len = strlen(t->name);

   // Check if the token is an identifier to begin with
   if (t->kind != 2)
   {
      error(6);
      return 0;
   }

   // Check if identifier is exceeds length limit
   if (strlen(t->name) > MAX_ID_LENGTH)
   {
      error(3);
      return 0;
   }
            
   // Check if identifier begins with a number
   if (!isalpha(t->name[0]))
   {
      error(9);
      return 0;
   }

	// Check that there aren't any invalid symbols/characters   
   for (i = 0; i < len; i++)
      if (!isalnum(t->name[i]))
         error(1);

	return 1;
}

int isValidFactor(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth)
{
	int negFlag = 0, endFlag = 0, storeValue, jumpbookmark = *bookmark, sbookmark = *bookmark;
   int stackPointer = 4, numOfVariables = 0;
   Instruction *tempInst = NULL;
   Token *tempToken = getToken(lexemeList, *bookmark);

	// If it's an identifer, check if it's in the symbol table. If it's a number
	// check if it's a valid number
	if (tempToken->kind == 2)
	{
		if (!containsTokenFromTable(symbols, tempToken, depth))
			error(11);
	}
	else if (tempToken->kind == 3)
	{
		isValidNumber(tempToken);
	}
	else if (tempToken->kind == 15)
	{
		tempToken = getToken(lexemeList, ++*bookmark);

		// Check if there's a valid expression
		isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);

		tempToken = getToken(lexemeList, (*bookmark)++);

		// Check for a right parenthesis
		if (tempToken->kind != 16)
			error(20);
	}
	else
	{
		error(29);
	}
	
   // If it's valid, go ahead and load it into a register
	if (tempToken->kind == 3)
   {
		tempInst = genInstruction(1, regPointer++, 0, tempToken->value);
      tailInsert(instructionList, tempInst);
	   ++numberOfLines;
     	tempToken = getToken(lexemeList, ++*bookmark);
   }
	else if (tempToken->kind == 2)
   {
      tempToken = getTokenFromTable(symbols, tempToken, depth);
		tempInst = genInstruction(3, regPointer++, depth, tempToken->address);
      tailInsert(instructionList, tempInst);
	   ++numberOfLines;
		tempToken = getToken(lexemeList, ++*bookmark);
   }

	return 1;
}

int isValidTerm(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth)
{
	int negFlag = 0, multFlag = 0, divFlag = 0, storeValue, jumpbookmark = *bookmark, sbookmark = *bookmark;
   int stackPointer = 4, numOfVariables = 0, reg1, reg2;
   Instruction *tempInst = NULL;
   Token *tempToken = getToken(lexemeList, *bookmark);

	// Right away, check if it's a valid factor, save which register we're at.
	isValidFactor(instructionList, lexemeList, symbols, bookmark, depth);
	reg1 = regPointer - 1;

	tempToken = getToken(lexemeList, *bookmark);

	// Keep looping as long as there are * or / signs
	while (tempToken != NULL && (tempToken->kind == 6 || tempToken->kind == 7))
	{
		// Check whether it's * or / and assert it's respective flag variable
		if (tempToken->kind == 6)
			multFlag = 1;
		else if (tempToken->kind == 7)
			divFlag = 1;

		tempToken = getToken(lexemeList, ++*bookmark);

		// Check if there's a valid factor
		isValidFactor(instructionList, lexemeList, symbols, bookmark, depth);
		reg2 = regPointer - 1;

		tempToken = getToken(lexemeList, *bookmark);
		
		// Go ahead and peform whatever operation was present
		if (multFlag)
			tempInst = genInstruction(13, regPointer++, reg1, reg2);
		else if (divFlag)
			tempInst = genInstruction(14, regPointer++, reg1, reg2);

		tailInsert(instructionList, tempInst);
		++numberOfLines;

		// To avoid segfaults, we're going to need to check if this is a semicolon or not
		if (tempToken->kind == 2 || tempToken->kind == 3)
			tempToken = getToken(lexemeList, ++*bookmark);

		reg1 = regPointer - 1;
      multFlag = divFlag = 0;
	}

	return 1;
}

int isValidExpression(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth)
{
	int negFlag = 0, endFlag = 0, storeValue, jumpbookmark = *bookmark, sbookmark = *bookmark;
   int stackPointer = 4, numOfVariables = 0, reg1, reg2;
   Instruction *tempInst = NULL;
   Token *tempToken = getToken(lexemeList, *bookmark);

	// Start by checking for an optional positive or negative sign
	if (tempToken->kind == 5)
	{
		negFlag = 1;
		tempToken = getToken(lexemeList, ++*bookmark);
	}
	else if (tempToken->kind == 4)
	{
		negFlag = 0;
		tempToken = getToken(lexemeList, ++*bookmark);
	}

	// Check if what's next is a valid term
	isValidTerm(instructionList, lexemeList, symbols, bookmark, depth);
	reg1 = regPointer - 1;

	// If the negFlag was asserted earlier, generate a NEG instruction
	if (negFlag)
	{
		tempInst = genInstruction(10, reg1, reg1, 0);
		tailInsert(instructionList, tempInst);
		++numberOfLines;
	}

	tempToken = getToken(lexemeList, *bookmark);

	while (tempToken != NULL && (tempToken->kind == 4 || tempToken->kind == 5))
	{
		// Start by checking for an optional positive or negative sign

		if (tempToken->kind == 5)
			negFlag = 1;
		else if (tempToken->kind == 4)
			negFlag = 0;

		tempToken = getToken(lexemeList, ++*bookmark);

		// Start by checking for a valid expression
		isValidTerm(instructionList, lexemeList, symbols, bookmark, depth);
		reg2 = regPointer - 1;

		tempToken = getToken(lexemeList, *bookmark);

		// If negFlag was asserted, generate a NEG instruction
		if (negFlag)
		{
			tempInst = genInstruction(10, reg2, reg2, 0);
			tailInsert(instructionList, tempInst);
			++numberOfLines;
		}
		
		// Add the two registers
		tempInst = genInstruction(11, regPointer++, reg1, reg2);
		tailInsert(instructionList, tempInst);
		++numberOfLines;
		
		// To avoid segfaults, we're going to need to check if this is a semicolon or not
		//if (tempToken->kind != 16 && tempToken->kind != 18 && tempToken->kind != 19)
		if (tempToken->kind == 2 || tempToken->kind == 3)
			tempToken = getToken(lexemeList, ++*bookmark);

		reg1 = regPointer - 1;
	}

	return 1;
}

int isValidCondition(iList *instructionList, LinkedList *lexemeList, HashTable *symbols, int *bookmark, int depth)
{
	int les = 0, leq = 0, equ = 0, neq = 0, gtr = 0, geq = 0, reg1 = 0, reg2 = 0;
   Instruction *tempInst = NULL;
   Token *tempToken = getToken(lexemeList, *bookmark);

	// Start by checking to see if we have an expression or 'odd' rel-op
	if (tempToken->kind == 8)
	{
		tempToken = getToken(lexemeList, ++*bookmark);

		isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);
		reg1 = regPointer - 1;

		// Generate and ODD instruction at register reg1;
		tempInst = genInstruction(15, reg1, 0, 0);
      tailInsert(instructionList, tempInst);
		++numberOfLines;
	}
	else
	{
		isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);
		reg1 = regPointer - 1;

		tempToken = getToken(lexemeList, (*bookmark)++);

		// Check which rel-op is present and assert the flag accordingly
		if (tempToken->kind == 9)
			equ = 1;
		else if (tempToken->kind == 10)
			neq = 1;
		else if (tempToken->kind == 11)
			les = 1;
		else if (tempToken->kind == 12)
			leq = 1;
		else if (tempToken->kind == 13)
			gtr = 1;
		else if (tempToken->kind == 14)
			geq = 1;
      else
         error(36);

		// Now check for a second valid expression
		isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);
		reg2 = regPointer - 1;
		
		// Now depending on which flag is asserted, generate an instrctuion
		if (equ)
			tempInst = genInstruction(17, 15, reg1, reg2);
		else if (neq)
			tempInst = genInstruction(18, 15, reg1, reg2);
		else if (les)
			tempInst = genInstruction(19, 15, reg1, reg2);
		else if (leq)
			tempInst = genInstruction(20, 15, reg1, reg2);
		else if (gtr)
			tempInst = genInstruction(21, 15, reg1, reg2);
		else if (geq)
			tempInst = genInstruction(22, 15, reg1, reg2);

		tailInsert(instructionList, tempInst);
		++numberOfLines;
	}

	return 1;
}
		
int isValidStatement(iList *instructionList, LinkedList *lexemeList, HashTable *keywords, HashTable *symbols, int *bookmark, int depth)
{
	int negFlag = 0, endFlag = 0, storeValue, jumpbookmark = *bookmark, sbookmark = *bookmark;
   int stackPointer = 4, numOfVariables = 0, ifbookmark, elsebookmark, endifbookmark, bline, pline;
   int whilebookmark, otherwhilebookmark, writeflag;
   Instruction *tempInst = NULL;
   Token *tempToken = getToken(lexemeList, *bookmark);

   
   switch (tempToken->kind)
   {
   case 2:     
      // If there's a procedure, we first need to look for an identifier
	   // and whether or not it's valid
		isValidIdentifier(tempToken);
		sbookmark = *bookmark;

		// Check if it's in the symbol table or not
		if (!containsTokenFromTable(symbols, tempToken, depth))
			error(11);

		// If the identifier is valid, we need to check if there's an ':=' sign
		tempToken = getToken(lexemeList, ++*bookmark);
		if (tempToken->kind != 20)
			error(28);

		// Next, check for a valid statement
		tempToken = getToken(lexemeList, ++*bookmark);
		isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);

		// Move whatever was in register 0 to the address of the identifier above
		// We need to get the identifier from the symbol table using the get() function
		tempToken = getToken(lexemeList, sbookmark);
		tempToken = getTokenFromTable(symbols, tempToken, depth);

		tempInst = genInstruction(4, regPointer - 1, depth - tempToken->level, tempToken->address);
      tailInsert(instructionList, tempInst);
		++numberOfLines;

		// Check for semicolon at the end
		tempToken = getToken(lexemeList, *bookmark);
		if (tempToken != NULL && tempToken->kind != 18 && tempToken->kind != 19)
			error(27);			

      regPointer = 0;
		break;
	case 27:						// If there's a function call
      tempToken = getToken(lexemeList, ++*bookmark);
   
      // Next should be an identifier, check if it's in the symbol table
		if (tempToken->kind != 2)
			error(30);

		if (!containsTokenFromTable(symbols, tempToken, depth))
			error(31);

      // If it does contain it, make sure it's a procedure
      if (getTokenFromTable(symbols, tempToken, depth)->kind != 99)
         error(35);

		// Get the address from the symbol table so we can generate a CAL instruction
		tempToken = getTokenFromTable(symbols, tempToken, depth);

		tempInst = genInstruction(5, 0, tempToken->level, tempToken->address + 1);
      tailInsert(instructionList, tempInst);
		++numberOfLines;

      tempToken = getToken(lexemeList, ++*bookmark);

		break;
	case 21:						// If there's a 'begin' statement
		tempToken = getToken(lexemeList, (*bookmark)++);

		// Keep checking for valid statements until you hit an 'end' token
		while((tempToken != NULL && tempToken->kind != 22) || tempToken->kind == 21)
		{
			isValidStatement(instructionList, lexemeList, keywords, symbols, bookmark, depth);
			tempToken = getToken(lexemeList, *bookmark);

			// Check for a semicolon, if it is, keep iterating
			if (tempToken != NULL && tempToken->kind != 18)
				error(27);
			else
				tempToken = getToken(lexemeList, ++*bookmark);	
		}

		// Check for an 'end'. If it is an end, then increment tempToken and leave
		if (tempToken == NULL || tempToken->kind != 22)
			error(18);
		else
			tempToken = getToken(lexemeList, ++*bookmark);	

		return 1;
	case 23:						// If there's an if-statement
      tempToken = getToken(lexemeList, ++*bookmark);

      // Check if there's a valid condition
      isValidCondition(instructionList, lexemeList, symbols, bookmark, depth);

		// Generate a JPC instruction. The result of the condition above will be
		// stored in register 15.
		tempInst = genInstruction(8, 15, 0, 369);
      tailInsert(instructionList, tempInst);
		ifbookmark = numberOfLines;
		++numberOfLines;

      tempToken = getToken(lexemeList, *bookmark);

		// Check if there's a 'then'
		if (tempToken->kind != 24)
			error(33);

		tempToken = getToken(lexemeList, ++*bookmark);

		// After that, check for a valid statement
		isValidStatement(instructionList, lexemeList, keywords, symbols, bookmark, depth);
		
		// Check for semicolon
		tempToken = getToken(lexemeList, *bookmark);
		if (tempToken != NULL && tempToken->kind != 18 && tempToken->kind != 19)
			error(27);		
		
      // We only want to increment if there's an else statement
		tempToken = getToken(lexemeList, (*bookmark) + 1);
      if (tempToken->kind == 33)
      	tempToken = getToken(lexemeList, ++*bookmark);
      else
         tempToken = getToken(lexemeList, *bookmark);

		// Check for an 'else' statement or a semi colon, and update the value of the JPC
		// instruction above depending on whether there is an 'else' or not
		if (tempToken->kind == 33)
		{
			// Generate a normal JMP instruction. This JMP will only trigger if
			// the condition was true. i.e. it will skip the 'else' part
			tempInst = genInstruction(7, 0, 0, 369);
      	tailInsert(instructionList, tempInst);
			elsebookmark = numberOfLines;
			++numberOfLines;

			tempToken = getToken(lexemeList, ++*bookmark);
			isValidStatement(instructionList, lexemeList, keywords, symbols, bookmark, depth);

			tempToken = getToken(lexemeList, *bookmark);

			// Update the value of the JMP instruction above
			tempInst = getInstruction(instructionList, ifbookmark);
			tempInst->m = elsebookmark + 1;
			tempInst = getInstruction(instructionList, elsebookmark);
			tempInst->m = numberOfLines;

         // finally, check for a semi colon
		   tempToken = getToken(lexemeList, *bookmark);
		   if (tempToken != NULL && tempToken->kind != 18 && tempToken->kind != 19)
			   error(27);		
		}
		else
		{
			tempInst = getInstruction(instructionList, ifbookmark);
			tempInst->m = numberOfLines;
		}

		regPointer = 0;
		break;
	case 25:						// If there's a while-loop
      tempToken = getToken(lexemeList, ++*bookmark);

      // Save the line number so we could jump back here at the end of the loop
      bline = numberOfLines;
  
      // Check if there's a valid condition
      isValidCondition(instructionList, lexemeList, symbols, bookmark, depth);

		// Generate a JPC instruction. The result of the condition above will be
		// stored in register 15.
		tempInst = genInstruction(8, 15, 0, 369);
      tailInsert(instructionList, tempInst);
		whilebookmark = numberOfLines;
		++numberOfLines;

      tempToken = getToken(lexemeList, *bookmark);

		// Check if there's a 'do'
		if (tempToken->kind != 26)
			error(34);

		tempToken = getToken(lexemeList, ++*bookmark);

		// After that, check for a valid statement
		isValidStatement(instructionList, lexemeList, keywords, symbols, bookmark, depth);

		// Check for semicolon
		tempToken = getToken(lexemeList, *bookmark);
		if (tempToken != NULL && tempToken->kind != 18 && tempToken->kind != 19)
			error(27);		

      // Now generate a normal jump function to go back to the beginning of the loop
		tempInst = genInstruction(7, 0, 0, bline);
      tailInsert(instructionList, tempInst);
		++numberOfLines;

      // Finally update the initial JPC instruction
      tempInst = getInstruction(instructionList, whilebookmark);
      tempInst->m = numberOfLines;

		break;
	case 32:						// If there's a write command
      tempToken = getToken(lexemeList, ++*bookmark);

      // Check for a valid expression and write it into register 14
      isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);

      // Generate an SIO instruction
      tempInst = genInstruction(9, 14, 0, 1);
		tailInsert(instructionList, tempInst);
		++numberOfLines;
         
		break;
	case 31:						// If there's a read command
      tempToken = getToken(lexemeList, ++*bookmark);

      // Check for a valid expression and write it into register 14
      isValidExpression(instructionList, lexemeList, symbols, bookmark, depth);

      // Generate an SIO instruction
      tempInst = genInstruction(9, 14, 0, 2);
		tailInsert(instructionList, tempInst);
		++numberOfLines;

		break;
	}

	return 1;
}

int isValidBlock(iList *instructionList, LinkedList *lexemeList, HashTable *keywords, HashTable *symbols, int *bookmark, int depth)
{
	int negFlag = 0, endFlag = 0, storeValue, jumpbookmark = numberOfLines, begin = numberOfLines, end;
   int stackPointer = 3, numOfVariables = 0, sbookmark = 0, pline = -1, bline, incbookmark = numberOfLines;
   Instruction *tempInst = NULL;
   Token *tempToken = NULL;

   // Our first block instructions, we'll start with a JMP and INC to give the 
   // stack space for the FV, SL, DL, and RA.
   tempInst = genInstruction(6, 0, 0, 4);
   tailInsert(instructionList, tempInst);
	++numberOfLines;

	// Get the next token
	tempToken = getToken(lexemeList, *bookmark);

	// If the program is syntactically correct, we should stop at a '.' symbol.
	// Checking for NULL first to avoid segfaults due to syntactic errors
	while (tempToken != NULL && (tempToken->kind != 19 || tempToken->kind == 18))
	{
		switch(tempToken->kind)
		{
		case 28:						// Check for contstant declarations
			// Since there could be more than 1 constant declaration at a time
			// We'll use a while loop and keep going as long as there are commas
			while (tempToken->kind == 28 || tempToken->kind == 17)
			{
				// If there's constant declaration, we first need to check for an identifier
				// and whether or not it's valid. If valid, insert it into the symbol table
				tempToken = getToken(lexemeList, ++*bookmark);
				isValidIdentifier(tempToken);

				// Check if it's already in the symbol table
				if (containsTokenWithSameLevel(symbols, tempToken, depth))
					error(22);

				// If the identifier is valid, we need to check if there's an '=' sign
				tempToken = getToken(lexemeList, ++*bookmark);
				if (tempToken->kind != 9)
					error(7);
			

				// Check if there's a negative sign
				tempToken = getToken(lexemeList, ++*bookmark);

				if (tempToken->kind == 5)
				{
					negFlag = 1;
					tempToken = getToken(lexemeList, ++*bookmark);
				}

				// After the '=' sign, there has to be a valid number, and store it's value
				isValidNumber(tempToken);
				storeValue = tempToken->value;

				// If everything is valid, store the variable in the symbol table
				tempToken = getToken(lexemeList, (negFlag) ? *bookmark - 3 : *bookmark - 2);

				tempToken->value = (negFlag) ? storeValue * -1 : storeValue;
				tempToken->level = depth;
				tempToken->address = ++stackPointer;
				insertFullToken(symbols, tempToken);

				// Before we move on, let's generate the intermediate code. We'll
            // push this variable to the stack. We're going to generate a LIT.
            // Followed by a STO instruction
            tempInst = genInstruction(1, 0, depth, tempToken->value);
            tailInsert(instructionList, tempInst);
            tempInst = genInstruction(4, 0, depth, tempToken->address);
            tailInsert(instructionList, tempInst);
				tempToken = getToken(lexemeList, ++*bookmark);
				numberOfLines += 2;

				// By this point, it looks like everything went well.
            // Move on to the next Token
            negFlag = 0;
			}

			break;
		case 29:						// Check for variable declaration
			// Similar to the constant declaration above, we'll keep looping as long
			// as there are commas or until we hit a period or semicolon
			while (tempToken->kind == 29 || tempToken->kind == 17)
			{
				// If there's variable declaration, we first need to check for an identifier
				// and whether or not it's valid. If valid, insert it into the symbol table
				tempToken = getToken(lexemeList, ++*bookmark);
				isValidIdentifier(tempToken);

				// Check if it's already in the symbol table
				if (containsTokenWithSameLevel(symbols, tempToken, depth))
					error(22);

				// Update the variables address and store it into the symbol table
				tempToken->level = depth;
				tempToken->address = ++stackPointer;
				insertFullToken(symbols, tempToken);

				// Check if there's a comma after the identifier
				tempToken = getToken(lexemeList, ++*bookmark);
			}
			
			break;
		case 30:						// Check for procedure declarations
			// If there's a procedure declaration, we're going to generate a JMP instruction
			tempInst = genInstruction(7, 0, 0, 369);
   		tailInsert(instructionList, tempInst);
			bline = numberOfLines;
			++numberOfLines;

			// If there's a procedure, we first need to look for an identifier
			// and whether or not it's valid
			tempToken = getToken(lexemeList, ++*bookmark);
			isValidIdentifier(tempToken);

			// Update the variables address and store it into the symbol table
         tempToken->kind = 99;
			tempToken->level = depth;
			tempToken->address = bline;
			insertFullToken(symbols, tempToken);

			// After checking for a valid identifier, check if there's a semicolon
			tempToken = getToken(lexemeList, ++*bookmark);
			if (tempToken->kind != 18)
				error(32);

			// After the identifier, there has to be a valid block of code
			tempToken = getToken(lexemeList, ++*bookmark);
			pline = isValidBlock(instructionList, lexemeList, keywords, symbols, bookmark, depth + 1);

			// Check for a semicolon after checking for a valid block of code
			tempToken = getToken(lexemeList, *bookmark);
			if (tempToken->kind != 18)
				error(27);

			// If all is well, update the jump instruction
			tempInst = getInstruction(instructionList, bline);
			tempInst->m = bline + pline + 1;

			break;
		default:						// All other cases, check for a valid statement
			isValidStatement(instructionList, lexemeList, keywords, symbols, bookmark, depth);
         tempToken = getToken(lexemeList, *bookmark - 1);

			// To avoid infinite recursion, we'll set tempToken to the previous lexeme.
			// If tempToken is 'end', that means we're at the end of a procedure declaration
			// and should return. Also, generate a RTN instruction
			if (tempToken->kind == 22)
			{
				// At the very end, generate a RTN instruction ONLY if depth is > 0
				if (depth > 0)
				{
					tempInst = genInstruction(2, 0, 0, 0);
					tailInsert(instructionList, tempInst);
					++numberOfLines;
				}

				// At the very end, update the value of the INC and JMP to accomodate for the number
				// of variables declared through out the procedure.
   			tempInst = getInstruction(instructionList, incbookmark);
				tempInst->m = stackPointer + 1;

				return numberOfLines - begin;
			}

         break;
		}

		// Check for a semicolon, comma, or period after each statement.
      // If it's not, generate an error
		if (tempToken->kind == 17 || tempToken->kind == 18 || tempToken->kind == 19)
      	tempToken = getToken(lexemeList, ++*bookmark);
		else if (tempToken->kind == 2)
			error(4);
		else
			error(24);

	}

	return 1;
}

// Analyzes a list of tokens and produces a list of intermediate generated code
iList *analyzeLexemeList(LinkedList *lexemeList, HashTable *keywords, HashTable *symbols)
{
	iList *instructionList = calloc(1, sizeof(iList));
	Instruction *tempInst = NULL;
   Token *tempToken = lexemeList->head->t;
	int *bookmark = calloc(1, sizeof(int));

	// Check if the program has a valid block
	isValidBlock(instructionList, lexemeList, keywords, symbols, bookmark, 0);

	// Check if the last token is a '.'
	tempToken = getLastToken(lexemeList);
	if (tempToken->kind != 19)
		error(13);

   // Add an SIO halt instruction at the very end
   tempInst = genInstruction(9, 0, 0, 3);
   tailInsert(instructionList, tempInst);
   ++numberOfLines;

   // error(0) means no errors were detected
   error(0);

	return instructionList;
}
