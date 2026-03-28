#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define red "\x1b[31m"
#define green "\x1b[32m" 
#define purple "\x1b[35m"
#define resetcolor "\x1b[0m"
#define bold "\x1b[1m"
#define italic "\x1b[3m"
#define underline "\x1b[4m"
#define background "\x1b[45m"

// To print Unicode characters in Windows
#ifdef _WIN32
	#include <windows.h>
	//for fireworks
	#define SLEEP(ms) Sleep(ms)
    #define CLEAR() system("cls")
    #define RED     "\033[31m"
    #define GREEN   "\033[32m"
    #define CYAN    "\033[36m"
    #define PURPLE  "\033[35m"
    #define RESET   "\033[0m"
#else
    #include <unistd.h>
    #define SLEEP(ms) usleep((ms) * 1000)
    #define CLEAR() printf("\033[2J\033[H")
    #define RED     "\033[31m"
    #define GREEN   "\033[32m"
    #define CYAN    "\033[36m"
    #define PURPLE  "\033[35m"
    #define RESET   "\033[0m"	
#endif

#define sub1 "\u2081"
#define sub2 "\u2082"
#define sub3 "\u2083"
#define sub4 "\u2084"
#define sub5 "\u2085"
#define sub6 "\u2086"
#define sub7 "\u2087"
#define sub8 "\u2088"
#define sub9 "\u2089"

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

void print_3rows (int board[9][9], int startrow, int *highlighted); // prints the numbers on board, 3 rows at a time
void print_boardpencil (int board[9][9], int pencil [3][3][3][3][3][3], int *highlighted);
//void print_board (int board[9][9], int *highlighted); // prints the entire sudoku board
void make3x3grid (int board[9][9], int grid [3][3][3][3]); // converts 9x9 grid into 3x3x3x3 grid
int findrow (int board[9][9], int row, int entry);
int findcolumn (int board[9][9], int column, int entry);
int findblock (int grid [3][3][3][3], int row, int column, int entry);
int validmove (int board[9][9], int grid [3][3][3][3], int row, int column, int entry); // checks same row, column or 3x3 grid
int unfinishedboard (int board [9][9]); // checks if the board is incomplete
struct Numbers; // declares a struct that counts tally of each number on board
void numbers_tally (int board[9][9], struct Numbers *tally); // counts how many of each number is on board
int solvepuzzle (int solvedpuzzle[9][9], int solvedpuzzlegrid [3][3][3][3]); // uses recursion to make a solved puzzle to check user's mistakes
void inputoutcome (int isvalid, int board[9][9], int solvedpuzzle[9][9], int row, int column, int entry, int *mistakes, char *message, int pencil [3][3][3][3][3][3], int *highlighted, struct Numbers *tally); // prints if move is valid
int validateinput (int board[9][9], int *row, int *column, int *entry); // ensures the user inputs 0-9
void highlight (void (*print_board)(int board[9][9], int pencil [3][3][3][3][3][3], int *highlighted), int board [9][9], int pencil [3][3][3][3][3][3], int *highlighted); // highlights all the same numbers
void updatepencil (int board[9][9], int pencil [3][3][3][3][3][3], int row, int column, int entry);
int haspencilmarks (int pencil [3][3][3][3][3][3], int i, int j, int k, int l);
void penciloption (int board[9][9], int grid[3][3][3][3], int pencil [3][3][3][3][3][3], int *pencilactive);
int addpencil (int board[9][9], int pencil [3][3][3][3][3][3], char *message);
void generatepencil (int board[9][9], int grid[3][3][3][3], int pencil [3][3][3][3][3][3]);
void clear_screen();
void move_cursor_top();
void fireworks(int board[9][9], int pencil[3][3][3][3][3][3]);
void print_finalboard (int board[9][9]);



int main () {
	// To print Unicode characters in Windows
	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif
	
	FILE *in; // Declare file
	in = fopen ("sudokutest.txt", "r"); // Open file with test values in read mode
	int board [9][9], grid [3][3][3][3], solvedpuzzle [9][9], solvedpuzzlegrid [3][3][3][3], row, column, entry, option;
	char optionchar[10];
	char message[100] = "";
	int mistakes=0;
	int highlighted = -1; // Don't want to highlight any number for default
	int pencil[3][3][3][3][3][3] = {0};
	int pencilactive=0;
	struct Numbers tally = {0,0,0,0,0,0,0,0,0};
	
	// Scan file for values to make the Sudoku board
	for (int i = 0; i<9; i++) {
		for (int j = 0 ; j < 9; j++) {
			fscanf(in, "%d", &board[i][j]);
			solvedpuzzle[i][j] = board[i][j]; // Put a copy of board into solvedpuzzle
		}
	}
	
	make3x3grid(solvedpuzzle, solvedpuzzlegrid); // Make a 3x3x3x3 version of 9x9 solvedpuzzle
	make3x3grid(board, grid);
	solvepuzzle(solvedpuzzle, solvedpuzzlegrid); // Store the solved puzzle in solvedpuzzle
	
	do {
		clear_screen();
		printf("\n");
		print_boardpencil(board, pencil, &highlighted);
		printf("\n");
		printf("%s\n", message);
		message[0] = '\0'; // erase the message after printing
		make3x3grid(board, grid); // Make a 3x3x3x3 version of the 9x9 board
		
		// Keep prompting user to pick an option as long as they don't input 1 or 2
		do {
			printf("Choose an option:\n"green italic bold"(1)"resetcolor" Highlight a number          "green italic bold"(2)"resetcolor" Fill a cell\n");
			printf(green italic bold"(3)"resetcolor" Add/erase a pencil mark     "); 
			if (pencilactive) printf(green bold italic"(4)"resetcolor" Clear all pencil marks\n");
			else printf(green bold italic"(4)"resetcolor" Fast pencil (fill all pencil marks)\n");
			scanf(" %9s", optionchar); // add space before %s in scanf and limit chars to 9
			option = optionchar[0] - '0'; // Convert char to int
		} while (optionchar[1] != '\0' || option <1 || option >4);
		
		if (option ==1) {
			highlight(print_boardpencil, board, pencil, &highlighted); // Option 1: highlight a number
		}

		else if (option == 2) {
			if (validateinput(board, &row, &column, &entry)) {
				int isvalid = validmove(board, grid, row, column, entry); // check if the move is valid
				inputoutcome(isvalid, board, solvedpuzzle, row, column, entry, &mistakes, message, pencil, &highlighted, &tally); // based on isvalid, print invalid or fill cell
				printf("\n");
			}
		} else if (option == 3) {
			addpencil(board, pencil, message);
		} else if (option == 4) {
			penciloption(board, grid, pencil, &pencilactive);
		}
		
		//Skip inputting values to right before finish screen
		/*for (int x = 0; x<9; x++) 
			for (int y = 0; y<8; y++) 
				board[x][y] = solvedpuzzle[x][y];*/
		
	} while (unfinishedboard (board)); // keep repeating until the board no longer has 0
	
	fireworks(board, pencil);
	fclose(in); // Close file
	return 0;
}

void clear_screen() {
	#ifdef _WIN32
		//#include <windows.h> (already included above)
		system("cls");
	#else
		printf("\033[2J\033[H");
	#endif	
}

void move_cursor_top() {
	#ifdef _WIN32
		//#include <windows.h> (already included above)
		COORD coord = {0,0};
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	#else
		printf("\033[H");
	#endif	
}





// this function needs to be seen by print_board function
void numbers_tally (int board[9][9], struct Numbers *tally) {
	// For each value in board, check how many of each number there are
	for (int i = 0; i <9; i++) {
		for (int j  = 0; j<9; j++) {
			if (board[i][j] == 1) tally->ones++;
			if (board[i][j] == 2) tally->twos++;
			if (board[i][j] == 3) tally->threes++;
			if (board[i][j] == 4) tally->fours++;
			if (board[i][j] == 5) tally->fives++;
			if (board[i][j] == 6) tally->sixes++;
			if (board[i][j] == 7) tally->sevens++;
			if (board[i][j] == 8) tally->eights++;
			if (board[i][j] == 9) tally->nines++;	
		}
	}
}

void print_boardpencil (int board[9][9], int pencil [3][3][3][3][3][3], int *highlighted) {
	for (int x=0; x<9; x++) { // board row
		for (int m = 0; m<3; m++) { // pencil mark row
			printf("   ");
			for (int y = 0; y <9; y++) { // board column
				int i = (int)((x+1)/3.0 - 0.01); // convert 9x9 row to 3x3 block row
				int j = (int) ((y+1)/3.0 - 0.01); // convert 9x9 column to 3x3 block column
				int k = x % 3; // convert 9x9 row to cell row within 3x3 block
				int l = y % 3; // convert 9x9 column to cell column within 3x3 block
				for (int n = 0; n<3; n++) { // pencil mark column
					int mark = m*3 + 1 +n; // convert m and n into the value of the pencil mark
					char *subs[10] = {"", sub1, sub2, sub3, sub4, sub5, sub6, sub7, sub8, sub9};
					if (board[x][y] !=0 && board[x][y] == *highlighted && m==1 && n==1) {
						printf(background bold"%d"resetcolor, board[x][y]);  // print board value in centre of mxn matrix
					} else if (board[x][y] !=0 && board[x][y] != *highlighted && m==1 && n==1) {
						printf(bold "%d"resetcolor, board[x][y]);  // print board value in centre of mxn matrix
					} else if (board[x][y] !=0 && board[x][y] == *highlighted) {
						printf(background " " resetcolor);
					} else if (board[x][y]==0 && pencil[i][j][k][l][m][n] && mark == *highlighted) {
						printf(bold background"%s"resetcolor, subs[mark]); // print the pencil mark in its respective coordinates within pencil matrix
					} else if (board[x][y]==0 && pencil[i][j][k][l][m][n] && mark != *highlighted) {
						printf("%s", subs[mark]); // print the pencil mark in its respective coordinates within pencil matrix
					} else if (board [x][y] == 0 && m==1 && n==1 && !haspencilmarks(pencil, i, j, k, l)) {
						printf(".");
					} else {
						printf(" ");
					}	
				}
				if (y==2 || y ==5) printf("    |    "); // vertical lines to separate the 3x3 blocks
				else if (y != 8) printf("     "); // add space between each number
			} 
			printf("\n");	
		}
		printf("                          |                           |\n");
		if (x==2 || x==5) {
				for (int s=0; s<79;s++) printf("_");
				printf("\n\n");
		}
	}
	
	if (unfinishedboard (board)) {
		// Print remaining 1-9 under the sudoku board
		struct Numbers tally = {0,0,0,0,0,0,0,0,0};
		numbers_tally(board, &tally);
		printf(bold"\n                               Remaining to fill:\n\n"resetcolor);
		
		if (tally.ones == 9) printf("             ");
			else printf(bold"            1"resetcolor);
		if (tally.twos == 9) printf("       ");
			else printf(bold"      2"resetcolor);
		if (tally.threes == 9) printf("       ");
			else printf(bold"      3"resetcolor);
		if (tally.fours == 9) printf("       ");
			else printf(bold"      4"resetcolor);
		if (tally.fives == 9) printf("       ");
			else printf(bold"      5"resetcolor);
		if (tally.sixes == 9) printf("       ");
			else printf(bold"      6"resetcolor);
		if (tally.sevens == 9) printf("       ");
			else printf(bold"      7"resetcolor);
		if (tally.eights == 9) printf("       ");
			else printf(bold"      8"resetcolor);
		if (tally.nines == 9) printf("         \n");
			else printf(bold"      9  \n"resetcolor);
	}
}


void highlight (void (*print_boardpencil)(int board[9][9], int pencil [3][3][3][3][3][3], int *highlighted), int board [9][9], int pencil [3][3][3][3][3][3], int *highlighted) {
	char highlightchar[10];
	int temp;
	do {	
			printf("Pick a number to highlight "green italic bold"(1-9)"resetcolor", or "red bold italic"(0)"resetcolor" to go back to menu: ");
			scanf(" %9s", highlightchar); // add space before %s in scanf and limit chars to 9
			temp = highlightchar[0] - '0'; // Convert char to int
	} while (highlightchar[1] != '\0' || highlightchar[0] < '0' || highlightchar[0] > '9');
	if (temp ==0) return;
	else *highlighted = temp;
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

int findrow (int board[9][9], int row, int entry) {
	for (int i = 0; i < 9; i++) {
		if (board[row][i] == entry) return i;
	} return -1;
}

int findcolumn (int board[9][9], int column, int entry) {
	for (int i = 0; i < 9; i++) {
		if (board[i][column] == entry) return i;
	} return -1;
}

int findblock (int grid [3][3][3][3], int row, int column, int entry) {
	int i = (int) ((row+1)/3.0 -0.01);
	int j = (int) ((column +1)/3.0 -0.01);
	for (int k =0; k <3; k++) {
		for (int l = 0; l<3; l++) {
			if (grid [i][j][k][l] == entry) return 3*k + l; // index if the cells in the ixj matrix were numbered 0-8
		}
	} return -1;
}

int validmove (int board[9][9], int grid [3][3][3][3], int row, int column, int entry) {
	if (findrow(board, row, entry) != -1) return 0;
	if (findcolumn(board, column, entry) != -1) return -1;
	if (findblock(grid, row, column, entry) != -1) return -2;
	return 1;
}

void updatepencil (int board[9][9], int pencil [3][3][3][3][3][3], int row, int column, int entry) {
	int m = (int) ((entry -1.0) / 3.0);
	int n = (int) (entry - 1.0) % 3;
	
	for (int y=0; y<9; y++) {
		// Same row
		int i = row/3;
		int j = y/3;
		int k = row % 3;
		int l = y % 3;
		
		if (y != column) pencil[i][j][k][l][m][n] = 0;
		
		// Same column
		i = y/3;
		j = column/3;
		k = y % 3;
		l = column % 3;
		if (y != row) pencil[i][j][k][l][m][n] = 0;
	}
	
	// Check if same block
	int i = row/3;
	int j = column/3;
	for (int k=0; k<3; k++) 
		for (int l=0; l<3; l++) 
			if ((i*3 + k) != row || (j*3 + l) != column) 
				pencil[i][j][k][l][m][n] = 0;
}

void generatepencil (int board[9][9], int grid[3][3][3][3], int pencil [3][3][3][3][3][3]) {
	for (int x = 0; x<9; x++) {
		for (int y = 0; y<9; y++) {
			if (board[x][y] == 0) {
				int i = x/3;
				int j = y/3;
				int k = x % 3;
				int l = y % 3;
				for (int mark = 1; mark <=9; mark++) {
					int m = (mark - 1)/3;
					int n = (mark - 1) % 3;
					if (validmove(board, grid, x, y, mark) == 1) {
						pencil[i][j][k][l][m][n] = 1;
					} else {
						pencil[i][j][k][l][m][n] = 0;
					}
				}
			}
		}
	}
}

void penciloption (int board[9][9], int grid[3][3][3][3], int pencil [3][3][3][3][3][3], int *pencilactive) {
	char surechar[10];
	int sure;
	do {
		printf("Are you sure? Enter "bold italic green"(1)"resetcolor" to "bold green"proceed"resetcolor" or "bold italic red"(0)"resetcolor" to "red bold"cancel"resetcolor": ");
		scanf(" %9s", surechar); // add space before %s in scanf and limit chars to 9
		sure = surechar[0] - '0'; // Convert char to int
	} while (surechar[1] != '\0' || (surechar[0] != '0' && surechar[0] != '1'));
	
	if (sure == 1) {
		if (*pencilactive == 0) {
			generatepencil(board, grid, pencil); 
			*pencilactive = 1;
		} else {
			memset(pencil, 0, sizeof(int)*3*3*3*3*3*3);
			*pencilactive=0;
		}
	 } else return;
}

int validateinput (int board[9][9], int *row, int *column, int *entry) {
	char rowchar[10], columnchar[10], entrychar[10];
	do {
		do {
			printf("Pick a row "green italic bold"(1-9)"resetcolor", or "red bold italic"(0)"resetcolor" to go back to menu: ");
			scanf(" %9s", rowchar); // add space before %s in scanf and limit chars to 9
			*row = rowchar[0] - '0'; // Convert char to int
			(*row)--;
		} while (rowchar[1] != '\0' || rowchar[0] < '0' || rowchar[0] > '9');
		if (*row == -1) return 0;
		
		do {
			printf("Pick a column "green italic bold"(1-9)"resetcolor", or "red bold italic"(0)"resetcolor" to go back to menu: ");
			scanf(" %9s", columnchar);
			*column = columnchar[0] - '0';
			(*column)--;
		} while (columnchar[1] !='\0' || columnchar[0] < '0' || columnchar[0] >'9');
		if (*column ==-1) return 0;
		
		// Check if entry is already in that row
		if (board[*row][*column] !=0) printf(bold italic red"This cell is already filled. Select an empty cell\n"resetcolor);
		
	} while (board[*row][*column]!= 0);
	
	do {
		printf("What number would you like to enter? "green italic bold"(1-9)"resetcolor" or "red bold italic"(0)"resetcolor" to go back to menu: ");
		scanf(" %9s", entrychar);
		*entry = entrychar[0] - '0';
	} while (entrychar[1] != '\0' || entrychar[0] <'0' || entrychar[0] > '9');
	if (*entry == -1) return 0;
	
	return 1;
}

int addpencil (int board[9][9], int pencil [3][3][3][3][3][3], char *message) {
	char rowchar[10], columnchar[10], markchar[10];
	int row, column, mark;
	do {
		do {
			printf("Pick a row "green italic bold"(1-9)"resetcolor", or "red bold italic"(0)"resetcolor" to go back to menu: ");
			scanf(" %9s", rowchar); // add space before %s in scanf and limit chars to 9
			row = rowchar[0] - '0'; // Convert char to int
			row--;
		} while (rowchar[1] != '\0' || rowchar[0] < '0' || rowchar[0] > '9');
		if (row == -1) return 0;
		
		do {
			printf("Pick a column "green italic bold"(1-9)"resetcolor" or "red bold italic"(0)"resetcolor" to go back to menu: ");
			scanf(" %9s", columnchar);
			column = columnchar[0] - '0';
			(column)--;
		} while (columnchar[1] !='\0' || columnchar[0] < '0' || columnchar[0] >'9');
		if (column ==-1) return 0;
		
		// Check if entry is already in that row
		if (board[row][column] !=0) printf(bold italic red"Cannot add pencil marks to a cell that is already filled\n"resetcolor);
		
	} while (board[row][column]!= 0);
	
	do {
		printf("What pencil mark would you like to add/erase? "green italic bold"(1-9)"resetcolor", or "red bold italic"(0)"resetcolor" to go back to menu: ");
		scanf(" %9s", markchar);
		mark = markchar[0] - '0';
	} while (markchar[1] != '\0' || markchar[0] <'0' || markchar[0] > '9');
	
	if (mark == -1) return 0;
	
	int i = row /3;
	int j = column /3;
	int k = row % 3;
	int l = column % 3;
	int m = (int) (mark - 1.0) /3.0;
	int n = (mark-1) % 3;
	
	if (pencil [i][j][k][l][m][n] ==0) {
		// if not filled, then fill	
		pencil [i][j][k][l][m][n] = 1;
		sprintf(message, bold green italic"Pencil mark added successfully\n"resetcolor);
	} else {
		// if filled, then erase
		pencil [i][j][k][l][m][n] = 0;	
		sprintf(message, bold green italic "Pencil mark erased successfully\n" resetcolor);
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

int haspencilmarks (int pencil [3][3][3][3][3][3], int i, int j, int k, int l) {
	for (int m = 0; m<3; m++)
		for (int n=0; n<3; n++)
			if (pencil[i][j][k][l][m][n]) return 1;
	return 0;
}

int gettally (struct Numbers *tally, int entry) {
	switch(entry) {
		case 1: return tally->ones;
		case 2: return tally->twos;
		case 3: return tally->threes;
		case 4: return tally->fours;
		case 5: return tally->fives;
		case 6: return tally->sixes;
		case 7: return tally->sevens;
		case 8: return tally->eights;
		case 9: return tally->nines;
		default: return 0;
	}
}

void autohighlight(int board[9][9], int *highlighted, struct Numbers *tally) {
	if (*highlighted <1 && *highlighted >9) return;
	
	for (int i =1; i<=9; i++) {
		int next = (*highlighted %9) + 1; // when highlighted is 9, next number is 1
		*highlighted = next;
		if(gettally(tally, next) <9) return; // found next incomplete
	}
}

void inputoutcome (int isvalid, int board[9][9], int solvedpuzzle[9][9], int row, int column, int entry, int *mistakes, char *message, int pencil [3][3][3][3][3][3], int *highlighted, struct Numbers *tally) {
	if (isvalid ==0) {
		(entry == 8) ? sprintf(message,bold italic red"There is already an %d in this row. Please try again.\n"resetcolor, entry) : 
		sprintf(message, bold italic red"There is already a %d in this row. Please try again.\n"resetcolor, entry);
	} else if (isvalid == -1) {
		(entry ==8) ? sprintf(message, bold italic red"There is already an %d in this column. Please try again.\n"resetcolor, entry) :
		sprintf(message, bold italic red"There is already a %d in this column. Please try again.\n"resetcolor, entry);
	} else if (isvalid == -2) {
		(entry ==8) ? sprintf(message, bold italic red"There is already an %d in this 3x3 block. Please try again.\n"resetcolor, entry) :
		sprintf(message, bold italic red"There is already a %d in this 3x3 block. Please try again.\n"resetcolor, entry);
	} else if (isvalid==1) {
		if (entry != solvedpuzzle[row][column]) {
			(*mistakes)++;
			sprintf(message, bold italic red"Wrong number inputted. Mistakes: %d\n"resetcolor, *mistakes);
		} else {
			board[row][column] = entry;
			updatepencil (board, pencil, row, column, entry);
			*highlighted = entry;
			memset(tally, 0, sizeof(*tally));
			numbers_tally(board, tally);
			if (gettally(tally, entry) == 9) {
				int oldhighlight = *highlighted;
				autohighlight(board, highlighted, tally);
				if (*highlighted != oldhighlight)
					sprintf(message, bold italic green"Nice! All cells with %d are now filled!\n"resetcolor, entry);
				else
					sprintf(message, bold italic green"Nice! Grid is now updated with your input\n"resetcolor);
			}		
		}
	}
}

void print_finalboard (int board[9][9]) {
	for (int x=0; x<9; x++) { // board row
		for (int m = 0; m<3; m++) { // pencil mark row
			for (int y = 0; y <9; y++) { // board column
				for (int n = 0; n<3; n++) { // pencil mark column
					if (m==1 && n==1) {
						printf(bold"%d"resetcolor, board[x][y]);  // print board value in centre of mxn matrix
					} else {
						printf(" ");
					}	
				}
				if (y==2 || y ==5) printf(" | "); // vertical lines to separate the 3x3 blocks
				else if (y != 8) printf("  "); // add space between each number
			} 
			printf("\n");	
		}
		//printf("              |             |\n");
		if (x==2 || x==5) {
				for (int s=0; s<45;s++) printf("_");
				printf("\n\n");
		}
	}
}

void fireworks(int board[9][9], int pencil[3][3][3][3][3][3]) {
    #define FW_WIDTH  55
    #define FW_HEIGHT 15
    #define FW_MAX    50

    typedef struct { float x, y, vx, vy; int alive, color; } FWParticle;
    FWParticle p[FW_MAX];
    memset(p, 0, sizeof(p));
    const char *colors[] = {RED, GREEN, CYAN, PURPLE};
    int color = 0, frame = 0;

    // hide cursor to reduce flicker
    #ifdef _WIN32
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(console, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(console, &cursorInfo);
    #else
        printf("\033[?25l");
    #endif

    CLEAR();  // clear once at the start only

    while (1) {
        if (frame % 20 == 0) {
            float x = 5.0f + rand() % (FW_WIDTH - 10);
            float y = 2.0f + rand() % (FW_HEIGHT - 4);
            for (int i = 0; i < FW_MAX; i++) {
                if (!p[i].alive) {
                    p[i].x     = x;
                    p[i].y     = y;
                    p[i].vx    = ((float)(rand() % 21) - 10) / 3.0f;
                    p[i].vy    = ((float)(rand() % 21) - 10) / 3.0f;
                    p[i].alive = 6 + rand() % 5;
                    p[i].color = color;
                }
            }
            color = (color + 1) % 4;
        }

        // update particles
        for (int i = 0; i < FW_MAX; i++) {
            if (!p[i].alive) continue;
            p[i].x  += p[i].vx;
            p[i].y  += p[i].vy;
            p[i].vy += 0.35f;
            p[i].alive--;
        }

        // build grid
        int grid[FW_HEIGHT][FW_WIDTH];
        int gcol[FW_HEIGHT][FW_WIDTH];
        memset(grid, 0, sizeof(grid));
        for (int i = 0; i < FW_MAX; i++) {
            if (!p[i].alive) continue;
            int px = (int)p[i].x;
            int py = (int)p[i].y;
            if (px >= 0 && px < FW_WIDTH && py >= 0 && py < FW_HEIGHT) {
                grid[py][px] = 1;
                gcol[py][px] = p[i].color;
            }
        }

        // move to top and overwrite — no cls, no flicker
        move_cursor_top();

        // fireworks
        for (int y = 0; y < FW_HEIGHT; y++) {
            for (int x = 0; x < FW_WIDTH; x++) {
                if (grid[y][x]) printf("%s*%s", colors[gcol[y][x]], RESET);
                else putchar(' ');
            }
            putchar('\n');
        }
        
        // congratulations message
        printf(GREEN bold "\n\n~*~  Congratulations! Puzzle Completed!  ~*~\n\n" RESET);

        // finished board
        printf("\n");
        print_finalboard (board);
        printf("\n");

        #ifdef _WIN32
            Sleep(40);
        #else
            usleep(40 * 1000);
        #endif
        frame++;
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

	// SCAN PENCIL FILE
	/*for (int i = 0; i<3; i++) 
		for (int j = 0; j<3; j++) 
			for (int k = 0; k<3; k++) 
				for (int l = 0; l<3; l++) 
					for (int m = 0; m<3; m++) 
						for (int n =0; n<3; n++) 
							fscanf(pencilfile, "%d", &pencil[i][j][k][l][m][n]);
*/
