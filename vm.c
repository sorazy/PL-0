// Soliman Alnaizy, UCFID: 3715450, Summer '18
// COP 3402, Dr. Montagne
// ==========================================================
// ASSIGNMENT #4: ~~ T H E   V I R T U A L   M A C H I N E ~~
// ==========================================================

#include "systemsoftware.h"

// Taken from the hw1Resources pdf
void printStack(int sp, int bp, int* stack, int lex)
{

   int i;

   if (bp == 1)
	{
   	if (lex > 0)
			printf("|");
   }	   
   else 
	{
   	//Print the lesser lexical level
     	printStack(bp-1, stack[bp + 2], stack, lex-1);
		printf("|");
   }

   //Print the stack contents - at the current level
   for (i = bp; i <= sp; i++)
   	printf("%2d ", stack[i]);	

}

// Taken from Appendix D of the HW1 pdf
int base(int *stack, int lexi, int bp)
{
	int b1;
	
	for (b1 = bp; lexi > 0; --lexi)
		b1 = stack[b1 + 1];
		
	return b1;
} 

void printRegister(int *regFile)
{
	int i;
	printf("Registers: [");
	for (i = 0; i < 16; ++i)
		printf("%d%c", regFile[i], (i < 15) ? ' ' : ']');

	printf("\n");

	return;
}

// Goes through the code from the parser and makes it neat.
void vmTrace(int **code, int lines)
{
   int i, j;

	for (i = 0; i < lines; ++i)
		for (j = 0; j < 4; ++j)
			if (j == 0)
				printf("%02d  %s ", code[i][4] = i, pseudoCode[code[i][j]]);
			else
				printf(" %02d %c", code[i][j], (j == 3) ? '\n' : ' ');

   return;
}

// Executes the code without printing anything to the screen
void stackTraceWithoutPrinting(int **code, int *stack, int *regFile, int lines)
{
	int reg, lex, m, pc = 0, sp = 0, bp = 1, j = 0, flag = 0, fetch, savepc;

   // Initialize the stack
	stack[0] = 0;
	stack[1] = 0;
	stack[2] = 0;

   while (1)
	{
		// First fetch instruction from the "code" array
		fetch = code[pc][0];
		reg = code[pc][1];
		lex = code[pc][2];
		m = code[pc][3];
      savepc = pc;

      switch(fetch)
		{
			case 1:		// LIT: Loading constant into register
				regFile[reg] = m;
				break;
			case 2:		// RTN: Returns from a subroutine and restore the caller
				sp = bp - 1;
				bp = stack[sp + 3];
				pc = stack[sp + 4];
				--pc;
				break;
			case 3:		// LOD: Store value from stack to register
				regFile[reg] = stack[base(stack, lex, bp) + m];
				break;
			case 4:		// STO: Store value register to stack 
				stack[base(stack, lex, bp) + m] = regFile[reg];
				break;
			case 5:		// CAL: Generate new activation record
				++pc;
				stack[sp + 1] = 0;
				stack[sp + 2] = bp;
				stack[sp + 3] = bp;
				stack[sp + 4] = pc;
				bp = sp + 1;
				pc = m;
				--pc;
				break;
			case 6:		// INC: Increment sp by M to make space for local variables
				sp += m;
				break;
			case 7:		// JMP: Jump the pc to location M
				pc = m;
				--pc;
				break;
			case 8:		// JPC: Jump only if R = 0
				if (regFile[reg] == 0)
					pc = m - 1;
				break;
			case 9:		// Does different things depending on the value of m
				if (m == 1)
				{
					printf("Register[%d] contains: %d\n", reg, regFile[reg]);
				}
				else if (m == 2)
				{
					printf("Input a value to store in Register <%d>: \n", reg);
					scanf(" %d", &regFile[reg]);
				}
				else		// if m == 3, set the halt flag to 1 and reset all the values.
				{	
					flag = 1;
					stack[0] = 0;
					stack[1] = 0;
					stack[2] = 1;
					pc = -1;
					bp = 1;
					sp = 0;
				}
				break;
			case 10:		// NEG
				regFile[reg] = regFile[reg] * -1;
				break;
			case 11: 	// ADD
				regFile[reg] = regFile[lex] + regFile[m];
				break;
			case 12:		// SUB
				regFile[reg] = regFile[lex] - regFile[m];
				break;
			case 13:		// MUL
				regFile[reg] = regFile[lex] * regFile[m];
				break;
			case 14:		// DIV
				regFile[reg] = regFile[lex] / regFile[m];
				break;			
			case 15:		// ODD
				regFile[reg] = regFile[reg] % 2;
				break;
			case 16:		// MOD
				regFile[reg] = regFile[lex] % regFile[m];
				break;
			case 17:		// EQL
				regFile[reg] = regFile[lex] == regFile[m];
				break;
			case 18:		// NEQ
				regFile[reg] = regFile[lex] != regFile[m];
				break;
			case 19:		// LSS
				regFile[reg] = regFile[lex] < regFile[m];
				break;
			case 20:		//	LEQ
				regFile[reg] = regFile[lex] <= regFile[m];
				break;
			case 21:		// GTR
				regFile[reg] = regFile[lex] > regFile[m];
				break;
			case 22:		// GEQ
				regFile[reg] = regFile[lex] >= regFile[m];
				break;
		}

		// Print out values, registers, and increment pc 
      ++pc;
		
      if (flag == 1)
         return;
   }
   
   return;
}

// Goes through a 2D int array and executes the code. Prints everything to screen
void stackTrace(int **code, int *stack, int *regFile, int lines)
{
	int reg, lex, m, pc = 0, sp = 0, bp = 1, j = 0, flag = 0, fetch, savepc;

   // Initialize the stack
	stack[0] = 0;
	stack[1] = 0;
	stack[2] = 0;

   while (1)
	{
		// First fetch instruction from the "code" array
		fetch = code[pc][0];
		reg = code[pc][1];
		lex = code[pc][2];
		m = code[pc][3];
      savepc = pc;

      switch(fetch)
		{
			case 1:		// LIT: Loading constant into register
				regFile[reg] = m;
				break;
			case 2:		// RTN: Returns from a subroutine and restore the caller
				sp = bp - 1;
				bp = stack[sp + 3];
				pc = stack[sp + 4];
				--pc;
				break;
			case 3:		// LOD: Store value from stack to register
				regFile[reg] = stack[base(stack, lex, bp) + m];
				break;
			case 4:		// STO: Store value register to stack 
				stack[base(stack, lex, bp) + m] = regFile[reg];
				break;
			case 5:		// CAL: Generate new activation record
				++pc;
				stack[sp + 1] = 0;
				stack[sp + 2] = bp;
				stack[sp + 3] = bp;
				stack[sp + 4] = pc;
				bp = sp + 1;
				pc = m;
				--pc;
				break;
			case 6:		// INC: Increment sp by M to make space for local variables
				sp += m;
				break;
			case 7:		// JMP: Jump the pc to location M
				pc = m;
				--pc;
				break;
			case 8:		// JPC: Jump only if R = 0
				if (regFile[reg] == 0)
					pc = m - 1;
				break;
			case 9:		// Does different things depending on the value of m
				if (m == 1)
				{
					printf("Register[%d] contains: %d\n", reg, regFile[reg]);
				}
				else if (m == 2)
				{
					printf("Input a value to store in Register <%d>: \n", reg);
					scanf(" %d", &regFile[reg]);
				}
				else		// if m == 3, set the halt flag to 1 and reset all the values.
				{	
					flag = 1;
					stack[0] = 0;
					stack[1] = 0;
					stack[2] = 1;
					pc = -1;
					bp = 1;
					sp = 0;
				}
				break;
			case 10:		// NEG
				regFile[reg] = regFile[reg] * -1;
				break;
			case 11: 	// ADD
				regFile[reg] = regFile[lex] + regFile[m];
				break;
			case 12:		// SUB
				regFile[reg] = regFile[lex] - regFile[m];
				break;
			case 13:		// MUL
				regFile[reg] = regFile[lex] * regFile[m];
				break;
			case 14:		// DIV
				regFile[reg] = regFile[lex] / regFile[m];
				break;			
			case 15:		// ODD
				regFile[reg] = regFile[reg] % 2;
				break;
			case 16:		// MOD
				regFile[reg] = regFile[lex] % regFile[m];
				break;
			case 17:		// EQL
				regFile[reg] = regFile[lex] == regFile[m];
				break;
			case 18:		// NEQ
				regFile[reg] = regFile[lex] != regFile[m];
				break;
			case 19:		// LSS
				regFile[reg] = regFile[lex] < regFile[m];
				break;
			case 20:		//	LEQ
				regFile[reg] = regFile[lex] <= regFile[m];
				break;
			case 21:		// GTR
				regFile[reg] = regFile[lex] > regFile[m];
				break;
			case 22:		// GEQ
				regFile[reg] = regFile[lex] >= regFile[m];
				break;
		}

		// Print out values, registers, and increment pc 
		printf("%-2d  %s %3d  %3d  %3d", code[savepc][4], pseudoCode[fetch], reg, lex, m);
		printf("  [%3d%3d%3d]  ", ++pc, bp, sp);
		printStack(sp, bp, stack, lex);
		printf("\n");
		printRegister(regFile);
		printf("\n");

      if (flag == 1)
         return;
   }
   
   return;
}

// Converts a .txt document to a 2D int array. Returns the number of lines.
int fileToArray(int **code)
{
   int i = 0, j = 0, lines = 0;
   int buffer;

   FILE *ifp = fopen("igc.txt", "r");

   if (ifp == NULL)
   {
      printf("File not found in fileToArray.\n");
      return 0;
   }

   // Initialize the first line of code to avoid segfaults
   code[0] = malloc(sizeof(int) * 4);

   // Storing the code into a 2D integer array, to have easy access of the
   // entire code, and not having to fscanf the same line multiple times
	while (fscanf(ifp, " %d", &buffer) != EOF)
	{
		code[i][j++] = buffer;
		if (j % 4 == 0)
		{
			++i;
			++lines;
			j = 0;
         code[i] = malloc(sizeof(int) * 4);
		}
	}

   return lines;
}
