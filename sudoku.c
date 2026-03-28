#include <stdio.h>

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

int main () {
	FILE *in; // Declare file
	in = fopen ("sudokutest.txt", "r"); // Open file with test values in read mode
	
	int board [9][9];
	
	// Scan file for values for each index of board
	for (int i = 0; i<9; i++) {
		for (int j = 0 ; j < 9; j++) {
			fscanf(in, "%d", &board[i][j]);
		}
	}
	
	print_board(board); // Call print_board function
	
	fclose(in); // Close file
	
	return 0;
}
