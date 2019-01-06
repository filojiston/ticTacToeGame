#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

typedef enum symbolStatus {
	LONG_SYMBOL,
	EMPTY_SYMBOL,
	VALID_SYMBOL
}symbolStatus;

typedef enum players {
	PLAYER_1,
	PLAYER_2
}players;

typedef enum nameStatus {
	LONG_NAME,
	IS_PUNCT_NAME,
	IS_DIGIT_NAME,
	EMPTY_NAME,
	VALID_NAME
}nameStatus;

typedef enum coord {
	X_COORD,
	Y_COORD
}coord;

typedef enum coordStatus {
	IS_ALPHA_COORD,
	IS_PUNCT_COORD,
	LONG_COORD,
	EMPTY_COORD,
	NOT_IN_RANGE_COORD,
	VALID_COORD
}coordStatus;

typedef enum gameStatus {
	WIN,
	TIE,
	GAME_CONTINUE
}gameStatus;


void initializeBoard();
void printBoard();
int updateBoard(int x, int y, char userSymbol);
int handleCoord(coord coordinate, char *userName);
char handleUserSymbol(players player, char *userName);
char *handleUserName(players player);
symbolStatus checkUserSymbol(char *userSymbol);
nameStatus checkUserName(char *userName);
coordStatus checkCoordValidity(char *input);
gameStatus setGameStatus();

char board[3][3];

int main(int argc, char const *argv[])
{
	players p1 = PLAYER_1;
	players p2 = PLAYER_2;

	coord xCoord = X_COORD;
	coord yCoord = Y_COORD;

	gameStatus status = GAME_CONTINUE;

	char *p1Name = handleUserName(p1);
	char *p2Name = handleUserName(p2);

	char p1Symbol = handleUserSymbol(p1, p1Name);
	char p2Symbol = handleUserSymbol(p2, p2Name);

	char playAgain = 'E';
	int alreadyMarked = FALSE;
	int x, y;

	initializeBoard();
	char *currentPlayerName = p1Name;
	char currentPlayerSymbol = p1Symbol;
	while (toupper(playAgain) == 'E') {
		do {
			printBoard();
			label:	x = handleCoord(X_COORD, currentPlayerName);
			y = handleCoord(Y_COORD, currentPlayerName);
			alreadyMarked = updateBoard(x, y, currentPlayerSymbol);
			if (!alreadyMarked)
				goto label;

			status = setGameStatus();
			if (status == WIN) {
				printBoard();
				printf("Congratulations, %s! You won!", currentPlayerName);
				break;
			} else if (status == TIE) {
				printBoard();
				printf("There is no winner everyone, tie game!");
				break;
			}

			if (currentPlayerName == p1Name && currentPlayerSymbol == p1Symbol) {
				currentPlayerName = p2Name;
				currentPlayerSymbol = p2Symbol;
			} else {
				currentPlayerName = p1Name;
				currentPlayerSymbol = p1Symbol;
			}

		} while(status == GAME_CONTINUE);

		printf("\n\nDo you wanna play again? (y/n): ");
		scanf(" %c", &playAgain);
		getchar();
		if (toupper(playAgain) == 'Y') {
			initializeBoard();
			currentPlayerName = p1Name;
			currentPlayerSymbol = p1Symbol;
		} else
			break;
	}

	return 0;
}

void initializeBoard() 
{
	int i, j;
	for (i = 0; i < 3; i++) 
		for (j = 0; j < 3; j++)
			board[i][j] = ' ';
}

void printBoard()
{
	printf("\n\n");
	int i, j;
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("|  %c  |\t", board[i][j]);
		}
		printf("\n");
		printf("_______________________\n\n");
	}

	printf("\n\n");
}

int updateBoard(int x, int y, char userSymbol)
{
	if (board[x -1][y - 1] == ' ') {
		board[x - 1][y - 1] = userSymbol;
		return TRUE;
	}
	else {
		printf("A player already marked this location.\n");
		return FALSE;
	}
}

int handleCoord(coord coordinate, char *userName)
{
	char *userInput = (char*)malloc(sizeof(char) * 5);
	coordStatus validCoord = EMPTY_COORD;

	do {
		if (coordinate == X_COORD) 
			printf("Please enter the x coordinate, %s: ", userName);
		else
			printf("Please enter the y coordinate, %s: ", userName);
		fgets(userInput, 5, stdin);
		userInput[strlen(userInput) - 1] = '\0';
		validCoord = checkCoordValidity(userInput);
	} while (validCoord != VALID_COORD);

	int coordToInt = atoi(userInput);
	return coordToInt;
}

char handleUserSymbol(players player, char* userName)
{
	char *userSymbol = (char*)malloc(sizeof(char) * 5);
	symbolStatus validSymbol = EMPTY_SYMBOL;

	do {
		if (player == PLAYER_1)
			printf("Please enter your symbol, %s: ", userName);
		else
			printf("Please enter your symbol, %s: ", userName);
		fgets(userSymbol, 5, stdin);
		userSymbol[strlen(userSymbol) - 1] = '\0';
		validSymbol = checkUserSymbol(userSymbol);
	} while (validSymbol != VALID_SYMBOL);
	
	return userSymbol[0];
}

char *handleUserName(players player)
{
	char *userName = (char*)malloc(sizeof(char) * 25);
	nameStatus validName = EMPTY_NAME;

	do {
		if (player == PLAYER_1)
			printf("Please enter your name, player1: ");
		else
			printf("Please enter your name, player2: ");
		fgets(userName, 25, stdin);
		userName[strlen(userName) - 1] = '\0';
		validName = checkUserName(userName);
	} while (validName != VALID_NAME);

	return userName;
}

symbolStatus checkUserSymbol(char *userSymbol)
{
	int inputLength = strlen(userSymbol);
	char ch = userSymbol[0];

	if (inputLength != 1 && ch != '\0') {
		printf("You should've enter a symbol.\n");
		return LONG_SYMBOL;
	}

	if (ch == '\0') {
		printf("Empty input.\n");
		return EMPTY_SYMBOL;
	}

	return VALID_SYMBOL;
}

nameStatus checkUserName(char *userName)
{
	int inputLength = strlen(userName);

	if (inputLength > 20) {
		printf("The input is too long.\n");
		return LONG_NAME;
	}

	if (userName[0] == '\0') {
		printf("Empty input.\n");
		return EMPTY_NAME;
	}

	int i;
	for (i = 0; i < inputLength; i++)
		if (isdigit(userName[i])) {
			printf("No digits allowed.\n");
			return IS_DIGIT_NAME;
		} else if (ispunct(userName[i])) {
			printf("No punctuations allowed.\n");
			return IS_PUNCT_NAME;
		}

	return VALID_NAME;
}


coordStatus checkCoordValidity(char *input)
{
	int inputLength = strlen(input);
	char ch = input[0];

	if (inputLength != 1 && ch != '\0') {
		printf("Your input is too long.\n");
		return LONG_COORD;
	}

	if (ch == '\0') {
		printf("Empty input.\n");
		return EMPTY_COORD;
	} else if (ispunct(ch)) {
		printf("You should've enter a digit, not punctuation.\n");
		return IS_PUNCT_COORD;
	} else if (isalpha(ch)) {
		printf("You should've enter a digit, not a letter.\n");
		return IS_ALPHA_COORD;
	} 

	int coordToInt = atoi(input);
	if (coordToInt < 1 || coordToInt > 3) {
		printf("The input does not in range.\n");
		return NOT_IN_RANGE_COORD;
	}

	return VALID_COORD;
}

gameStatus setGameStatus() 
{
	int i, j;
	int marked = 0;

	if (board[0][0] == board[0][1] && board[0][0] == board[0][2] && board[0][0] != ' ')
		return WIN;
	if (board[1][0] == board[1][1] && board[1][0] == board[1][2] && board[1][0] != ' ')
		return WIN;
	if (board[2][0] == board[2][1] && board[2][0] == board[2][2] && board[2][0] != ' ')
		return WIN;

	if (board[0][0] == board[1][0] && board[0][0] == board[2][0] && board[0][0] != ' ')
		return WIN;
	if (board[0][1] == board[1][1] && board[0][1] == board[2][1] && board[0][1] != ' ')
		return WIN;
	if (board[0][2] == board[1][2] && board[0][2] == board[2][2] && board[0][2] != ' ')
		return WIN;

	if (board[0][0] == board[1][1] && board[0][0] == board[2][2] && board[0][0] != ' ')
		return WIN;
	if (board[0][2] == board[1][1] && board[0][2] == board[2][0] && board[0][2] != ' ')
		return WIN;

	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			if (board[i][j] != ' ')
				marked++;

	if (marked == 9)
		return TIE;

	return GAME_CONTINUE;
}
