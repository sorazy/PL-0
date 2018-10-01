// Soliman Alnaizy, UCFID: 3715450, Summer '18
// COP 3402, Dr. Montagne
// =====================================================
// ASSIGNMENT #4: ~~ T H E   M A I N   F U N C T I O N~~
// =====================================================

#include "systemsoftware.h"

int main(int argc, char **argv)
{
	FILE *ifp = fopen(argv[1], "r");
	FILE *ofp = fopen(argv[2], "w");
	HashTable *tokenTable = createHashTable(), *symbolTable = createHashTable();
	LinkedList *lexemeList = NULL;
	iList *instructionList = NULL;
	int **code = calloc(MAX_CODE_LENGTH, sizeof(int *));
	int *stack = calloc(MAX_STACK_HEIGHT, sizeof(int));
	int *regFile = calloc(16, sizeof(int));
	int lines, lflag = 0, aflag = 0, vflag = 0, i;

	// Check if the a valid file was given
	if (argc < 3 || argc > 6)
	{
		printf("Proper syntax: ./a.out <input_filename.txt> <output_filename.txt>\n");
		printf("The following compiler directives are optional:\n");
		printf("Use '-' instead of <output_filename.txt> to print results to screen.\n");
		printf("-l : Prints out the list of lexemes/tokens to the screen.\n");
		printf("-a : Prints out the generated assembly code to the screen.\n");
		printf("-v : Prints out the VM execution trace to the screen.\n");

		return 0;
	}

	// return 0 is either of the files couldn't be opened
	if (ifp == NULL)
	{
		printf("The file <%s> was not found in directory.\n", argv[1]);
		return 0;
	}

	if (ofp == NULL)
	{
		printf("The file <%s> could not be opened.\n", argv[2]);
		return 0;
	}

	if (argv[2][0] == '-')
	{
		ofp = stdout;
	}

	// See which compiler directives were given by the user
	for (i = 2; i < argc; ++i)
	{
		if (!strcpy(argv[i], '-l'))
			lflag = 1;
		else if (!strcpy(argv[i], '-a'))
			aflag = 1;
		else if (!strcpy(argv[i], '-v'))
			vflag = 1;
	}

	// Make sure they entered the correct compiler directives
	if (argc - lflag - aflag - vflag - 3 != 0)
	{
		printf("Improper compiler directive detected...\n");
		printf("Please select from the following:\n");
		printf("-s : Prints everything to screen instead of")
		printf("-l : Prints out the list of lexemes/tokens to the screen.\n");
		printf("-a : Prints out the generated assembly code to the screen.\n");
		printf("-v : Prints out the VM execution trace to the screen.\n");

		return 0;
	}

	// Create the Token Table to set up for the upcoming functions 
	createPL0Table(tokenTable);

	// Print the original input file if any compiler directive was given
	if (lflag || vflag || aflag)
	{
		printf("=======================================================\n");
		printf("The original input file:\n");
		printf("=======================================================\n");
		printFile(argv[1]);
	}

	// Create the lexeme list (scanner)
	lexemeList = createLexemeList(ifp, tokenTable);
	
	// If '-l' was included in the command line, print out the lexeme list
	if (lflag)
	{
		printf("=======================================================\n");
		printf("The lexeme list for this program:\n");
		printf("=======================================================\n");
		printLexemeListWithKeys(lexemeList);
	}

	// Error detection and intermediate code generation   
	instructionList = analyzeLexemeList(lexemeList, tokenTable, symbolTable);

	// If instructionList is NULL, that means an error was detected. End the program
	if (instructionList == NULL)
		return 0;

	// Print the instructions to a file named "igc.txt"
	exportInstructions(instructionList);

	// If '-a' was included in the command line, print out the generated code
	if (aflag)
	{
		printf("\n=======================================================\n");
		printf("The generated code of the input program:\n");
		printf("=======================================================\n");
		printFile("igc.txt");
	}

	// Convert "code.txt" to a 2D integer array and return the number of lines
	lines = fileToArray(code);

	// If '-v' was included in the command line, print the VM output
	if (vflag)
	{
		printf("=======================================================\n");
		printf("The virtual machine output:\n");
		printf("=======================================================\n");
		vmTrace(code, lines);

		printf("\n=======================================================\n");
		printf("The virtual machine stack trace:\n");
		printf("=======================================================\n");
		printf("    OP  Reg  Lex  Val  [ PC BP SP]   Stack:\n");
		stackTrace(code, stack, regFile, lines);
	}
	else
	{
		stackTraceWithoutPrinting(code, stack, regFile, lines);
	}
	
	return 0;
}
