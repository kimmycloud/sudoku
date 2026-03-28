#include <stdio.h>

void print_3rows (int board[9][9], int startrow);
void print_board (int board[9][9]);
void make3x3grid (int board[9][9], int grid [3][3][3][3]);
int validmove (int board[9][9], int grid [3][3][3][3], int row, int column, int entry);
int unfinishedboard (int board [9][9]);

int main () {
	FILE *in; // Declare file
	in = fopen ("sudokutest2.txt", "r"); // Open file with test values in read mode
	
	int board [9][9];
	int grid [3][3][3][3];
	int row, column, entry;
	
	// Scan file for values for each index of board
	for (int i = 0; i<9; i++) {
		for (int j = 0 ; j < 9; j++) {
			fscanf(in, "%d", &board[i][j]);
		}
	}
	do{
	print_board(board); // Call print_board function
	
	printf("\n");
	
	make3x3grid(board, grid);

	
	printf("Pick a row: ");
	scanf("%d", &row);
	row--;
	
	printf("Pick a column: ");
	scanf("%d", &column);
	column--;
	
	printf("What number would you like to enter? (0-9): ");
	scanf("%d", &entry);
	
	int isvalid = validmove(board, grid, row, column, entry);
	
	if (isvalid) {
		board[row][column] = entry;
		//print_board(board);
		//make3x3grid(board,grid);
	} 
	
	
	} while (unfinishedboard (board));
	
	print_board(board);
	printf("Sudoku puzzle completed!\n");
	
	
	fclose(in); // Close file
	
	return 0;
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
	if (board[row][column] !=0) {
		printf("Select an empty cell\n");
		return -1;
	}
	for (int i = 0; i < 9; i++) {
		if (board[row][i] == entry) return 0;
	}
	
	// Check if entry is already in that column
	for (int i = 0; i < 9; i++) {
		if (board[i][column] == entry) return 0;
	}
	// Check if entry is already in that 9x9 box
	int i = (int) ((row+1)/3.0 -0.01);
	int j = (int) ((column+1)/3.0 -0.01);
	for (int k =0; k <3; k++) {
		for (int l = 0; l<3; l++) {
			if (grid [i][j][k][l] == entry) return 0;
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

	/*
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
