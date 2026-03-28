#include <stdio.h>

void print_3rows (int board[9][9], int startrow);
void print_board (int board[9][9]);
void make3x3grid (int board[9][9], int grid [3][3][3][3]);
int validmove (int board[9][9], int grid [3][3][3][3], int row, int column, int entry);
int unfinishedboard (int board [9][9]);
struct Numbers;
void numbers_tally (int board[9][9], struct Numbers *tallyptr);
int solvepuzzle (int solvedpuzzle[9][9], int solvedpuzzlegrid [3][3][3][3]);
void print_valid (int isvalid, int board[9][9], int solvedpuzzle[9][9], int row, int column, int entry, int *mistakes);
void validateinput (int board[9][9], char rowchar[10], int *row, char columnchar[10], int *column, char entrychar[10], int *entry);


int main () {
	FILE *in; // Declare file
	in = fopen ("sudokutest2.txt", "r"); // Open file with test values in read mode
	
	int board [9][9], grid [3][3][3][3], solvedpuzzle [9][9], solvedpuzzlegrid [3][3][3][3], row, column, entry;
	char rowchar[10], columnchar[10], entrychar[10];
	int mistakes=0;
	
	// Scan file for values to make the Sudoku board
	for (int i = 0; i<9; i++) {
		for (int j = 0 ; j < 9; j++) {
			fscanf(in, "%d", &board[i][j]);
			solvedpuzzle[i][j] = board[i][j]; // Put a copy of board into solvedpuzzle
		}
	}
	
	do {
		printf("\n");
		print_board(board); // Print the sudoku board
		printf("\n");
		
		make3x3grid(board, grid); // Make a 3x3x3x3 version of the 9x9 board
		make3x3grid(solvedpuzzle, solvedpuzzlegrid); // Make a 3x3x3x3 version of 9x9 solvedpuzzle
		solvepuzzle(solvedpuzzle, solvedpuzzlegrid); // Store the solved puzzle in solvedpuzzle
		validateinput(board, rowchar, &row, columnchar, &column, entrychar, &entry); // ask user for input, validate each input
		
		printf("\n");
		int isvalid = validmove(board, grid, row, column, entry); // Return result of validmove function
		print_valid(isvalid, board, solvedpuzzle, row, column, entry, &mistakes); // based on isvalid, print invalid or fill up board
	} while (unfinishedboard (board)); // keep repeating until the board no longer has 0
	
	print_board(board); // print the final finished board
	printf("CONGRATULATIONS! Sudoku puzzle solved!\n");
	
	fclose(in); // Close file
	return 0;
}

void validateinput (int board[9][9], char rowchar[10], int *row, char columnchar[10], int *column, char entrychar[10], int *entry) {
	do {
		do {
			printf("Pick a row (1-9): ");
			scanf(" %9s", rowchar); // add space before %s in scanf and limit chars to 9
			*row = rowchar[0] - '0'; // Convert char to int
			(*row)--;
		} while (rowchar[1] != '\0' || rowchar[0] < '1' || rowchar[0] > '9');
		
		do {
			printf("Pick a column (1-9): ");
			scanf(" %9s", columnchar);
			*column = columnchar[0] - '0';
			(*column)--;
		} while (columnchar[1] !='\0' || columnchar[0] < '1' || columnchar[0] >'9');
		
		// Check if entry is already in that row
		if (board[*row][*column] !=0) printf("This cell is already filled. Select an empty cell\n");
		
	} while (board[*row][*column]!= 0);
	
	do {
		printf("What number would you like to enter? (1-9): ");
		scanf(" %9s", entrychar);
		*entry = entrychar[0] - '0';
	} while (entrychar[1] != '\0' || entrychar[0] <'1' || entrychar[0] > '9');
}

// print_3rows function prints three rows at a time
void print_3rows (int board [9][9], int startrow) {
	// Parameter startrow allows us to call print_3rows
	// starting from different initial values
	
	for (int i = startrow; i < startrow + 3; i++) {
		for (int j = 0; j < 9; j++) {
			
			// Print value first
			if (board [i][j] == 0) printf(".");
			else printf("%d", board[i][j]);
			
			// Except for last column, separate values by spaces
			// Separate every three values by a separator |
			if (j != 8) {
				((j+1) % 3 ==0) ? printf("  |  ") : printf("   ");
			}
		}
		// Except for rows that are multiples of 3, print separator lines
		// after each row (the ones that separate 3x3 grids)
		printf("\n");
		if ((i+1)%3 != 0){
			printf("           |");
			printf("             |\n");
		}
	}
}

struct Numbers {
	int ones;
	int twos;
	int threes;
	int fours;
	int fives;
	int sixes;
	int sevens;
	int eights;
	int nines;
};

// this function needs to be seen by print_board function
void numbers_tally (int board[9][9], struct Numbers *tallyptr) {
	// For each value in board, check how many of each number there are
	for (int i = 0; i <9; i++) {
		for (int j  = 0; j<9; j++) {
			if (board[i][j] == 1) tallyptr->ones++;
			if (board[i][j] == 2) tallyptr->twos++;
			if (board[i][j] == 3) tallyptr->threes++;
			if (board[i][j] == 4) tallyptr->fours++;
			if (board[i][j] == 5) tallyptr->fives++;
			if (board[i][j] == 6) tallyptr->sixes++;
			if (board[i][j] == 7) tallyptr->sevens++;
			if (board[i][j] == 8) tallyptr->eights++;
			if (board[i][j] == 9) tallyptr->nines++;	
		}
	}
}

void print_board (int board [9][9]) {
	// Use for loop to call print_3rows (for loop increment by 3)
	for (int i =0; i<=6; i+=3) {
		print_3rows(board, i); // print three rows at a time
		
		// If not the last set of triples, print separator rows
		if (i !=6) {
			for (int j = 0; j < 38; j++) {
				printf("_");
			}
			printf("\n\n");
		}
	}
	
	// Print remaining 1-9 under the sudoku board
	struct Numbers tally = {0,0,0,0,0,0,0,0,0};
	numbers_tally(board, &tally);
	
	printf("\nRemaining to fill:\n\n");
	
	if (tally.ones == 9) printf("   ");
		else printf("  1");
	if (tally.twos == 9) printf("    ");
		else printf("   2");
	if (tally.threes == 9) printf("    ");
		else printf("   3");
	if (tally.fours == 9) printf("    ");
		else printf("   4");
	if (tally.fives == 9) printf("    ");
		else printf("   5");
	if (tally.sixes == 9) printf("    ");
		else printf("   6");
	if (tally.sevens == 9) printf("    ");
		else printf("   7");
	if (tally.eights == 9) printf("    ");
		else printf("   8");
	if (tally.nines == 9) printf("      \n");
		else printf("   9  \n");
}

void make3x3grid (int board[9][9], int grid [3][3][3][3]) {
	for (int i =0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			for (int x = 0; x<9; x++) {
				for (int y = 0; y<9; y++) {
					i = (int) ((x+1)/3.0 -0.01);
					j = (int) ((y+1)/3.0 -0.01);
					grid[i][j][x-3*i][y-3*j] = board [x][y];
				}
			}
		}
	}
}


int validmove (int board[9][9], int grid [3][3][3][3], int row, int column, int entry) {
	// Check if entry is already in that row
	for (int i = 0; i < 9; i++) {
		if (board[row][i] == entry) return 0;
	}
	// Check if entry is already in that column
	for (int i = 0; i < 9; i++) {
		if (board[i][column] == entry) return -1;
	}
	// Check if entry is already in that 9x9 box
	int i = (int) ((row+1)/3.0 -0.01);
	int j = (int) ((column+1)/3.0 -0.01);
	for (int k =0; k <3; k++) {
		for (int l = 0; l<3; l++) {
			if (grid [i][j][k][l] == entry) return -2;
		}
	}
	return 1;
}

int unfinishedboard (int board [9][9]) {
	int countzeros = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j <9; j++) {
			if (board[i][j] ==0) countzeros++;
		}
	}
	if (countzeros ==0) return 0;
	else return 1;
}

int solvepuzzle (int solvedpuzzle[9][9], int solvedpuzzlegrid [3][3][3][3]) {
	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {
			if (solvedpuzzle[x][y] ==0) {
				for (int n = 1; n<=9; n++) {
					int isvalid = validmove (solvedpuzzle, solvedpuzzlegrid, x, y, n);
					if (isvalid ==1) {
						solvedpuzzle[x][y] = n;
						make3x3grid (solvedpuzzle, solvedpuzzlegrid);
						// Need to backtrack/use recursion
						if (solvepuzzle(solvedpuzzle, solvedpuzzlegrid)) return 1;
						solvedpuzzle[x][y] = 0;
						make3x3grid (solvedpuzzle, solvedpuzzlegrid);
					}
				}
				return 0;
			}
		}
	}
	return 1;
}

void print_valid (int isvalid, int board[9][9], int solvedpuzzle[9][9], int row, int column, int entry, int *mistakes) {
	if (isvalid ==0) {
		(entry == 8) ? printf("There is already an %d in this row. Please try again.\n", entry) : 
		printf("There is already a %d in this row. Please try again.\n", entry);
	} else if (isvalid == -1) {
		(entry ==8) ? printf("There is already an %d in this column. Please try again.\n", entry) :
		printf("There is already a %d in this column. Please try again.\n", entry);
	} else if (isvalid == -2) {
		(entry ==8) ? printf("There is already an %d in this 3x3 block. Please try again.\n", entry) :
		printf("There is already a %d in this 3x3 block. Please try again.\n", entry);
	} else if (isvalid==1) {
		if (entry != solvedpuzzle[row][column]) {
			(*mistakes)++;
			printf("Wrong number inputted. Mistakes: %d\n", *mistakes);
		} else {
			board[row][column] = entry;
			printf("Nice! Grid is now updated with your input\n");
		}
	}
}

	/* test print 3x3
	 * test print the 3x3 grid
	for (int i =0; i<3; i++) {
		for (int j = 0; j<3; j++) {
			for (int k = 0; k<3; k++) {
				for (int l = 0; l<3; l++) {
					printf("%2d ", grid[i][j][k][l]);
				}
				printf(" | ");
			}
			printf("|");
		} printf("\n");
	}
	*/

	/* Test print solved puzzle
	 * for (int i = 0; i<9; i++) {
		for (int j = 0; j<9; j++) {
			printf(" %d ", solvedpuzzle[i][j]);
		} printf("\n");
	}
	*/
